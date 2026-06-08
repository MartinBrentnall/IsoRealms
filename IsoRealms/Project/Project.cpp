/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Project.h"

#include "IsoRealms/Application.h"
#include "IsoRealms/Assets/Type/IScreenListener.h"
#include "IsoRealms/DisplayResolution.h"
#include "IsoRealms/Persistence/JSONArray.h"
#include "IsoRealms/Persistence/JSONValue.h"
#include "IsoRealms/PropertyData.h"

#include "Module.h"

namespace IsoRealms {
  Project::Project(Application& application, std::function<void(bool)> onFinish) :
          cDefModuleChooser(*this),
          cApplication(application),
          cFunctionNotifyComplete(onFinish),
          cAnalogueInputs(*this),
          cBindings(*this),
          cDigitalInputs(*this),
          cScreens(*this),
          cStrings(*this),
          cTextures(*this),
          cFloats(cApplication),
          cIntegers(cApplication),
          cColours(cApplication),
          cDefProjectFileStructure(*this, "", true),
          cDefInputHandler(*this, cDefProjectFileStructure, *this),
          cDefScreen(*this, cDefProjectFileStructure, *this),
          cDefDefaultEditor(*this, cDefProjectFileStructure, *this),
          cDefActionOnStart(*this, cDefProjectFileStructure, *this),
          cDefActionOnCloseRequest(*this, cDefProjectFileStructure, *this),
          cLuaBindingApplication(cLuaState, &application),
          cLuaBindingProject(cLuaState, this),
          cLuaBindingOptions(cLuaState, &cDefOptions),
          cResourcesLoaded(false),
          cLoading(false),
          cProcessingInput(false),
          cRuntimeUpdatingRuntime(false),
          cRuntimeResetPostponed(false),
          cQuitAction(*this) {
    add<IBinding, IBinding>(&cLuaBindingApplication, "Application", "Application");
    add<IBinding, IBinding>(&cLuaBindingProject,     "Project",     "Project");
    add<IBinding, IBinding>(&cLuaBindingOptions,     "Options",     "Options");
    add<IAction,  IAction> (&cQuitAction,            "Quit",        "Terminate the Application");
  }

  Project::Project(Application& application, std::function<void(bool)> onFinish, const std::string& file, bool user) :
            Project(application, onFinish) {
    cLoading = true;
    cProcessingInput = true;
    cDefProjectFileStructure.rename(file, user);

    // Load modules and any resources declared within them
    std::vector<std::unique_ptr<JSONDocument>> mOpenedDocuments = loadResources(cDefProjectFileStructure);
    for (const std::unique_ptr<Module>& mModule : cDefModules) {
      mModule->registerAssets();
    }
    cResourcesLoaded = true;

    // Initialise everything
    for (unsigned int j = 0; j < cInitialisers.size(); j++) {
      // std::cout << "INIT " << j << " OF " << cInitialisers.size() << std::endl;
      // if (j == 96) {
      //   std::cout << "DEBUG!" << std::endl;
      // }
      cInitialisers[j]();
    }
    cInitialisers.clear();

    // Screen listeners cannot notified of screens before initialisation, so we need to do it now.
    cScreens.notifyAllScreensAdded();

    cProcessingInput = false;
    cLoading = false;
  }

  std::vector<std::unique_ptr<JSONDocument>> Project::loadResources(ProjectFile& file) {
    std::unique_ptr<JSONDocument> mProjectDocument = std::make_unique<JSONDocument>(file.cFile);
    JSONObject mProjectObject = mProjectDocument->getObject(JSON_PROJECT);
    std::vector<std::unique_ptr<JSONDocument>> mOpenedDocuments;
    mOpenedDocuments.emplace_back(std::move(mProjectDocument));

    cDefScreen.init(mProjectObject, JSON_SCREEN, file);
    cDefInputHandler.init(mProjectObject, JSON_INPUT, file);
    cDefActionOnStart.init(mProjectObject, JSON_ON_START, file);
    cDefActionOnCloseRequest.init(mProjectObject, JSON_ON_CLOSE_REQUEST, file);
    cDefDefaultEditor.init(mProjectObject, JSON_EDITOR, file);

    for (JSONThing mModuleThing : mProjectObject.getObject(JSON_MODULES)) {
      std::string mModuleName = mModuleThing.getName();
      getModule(mModuleName);
    }

    if (mProjectObject.hasMember(JSON_LAUNCH_CONFIGURATIONS)) {
      for (JSONThing mLaunchConfigurationThing : mProjectObject.getObject(JSON_LAUNCH_CONFIGURATIONS)) {
        cDefTestLaunchConfigurations.emplace_back(std::make_unique<ProjectLaunchConfiguration>(*this, file, mLaunchConfigurationThing));
      }
    }

    for (JSONThing mModuleThing : mProjectObject.getObject(JSON_MODULES)) {
      JSONObject mModuleObject = mModuleThing.getValue();
      std::string mModuleName = mModuleThing.getName();
      Module* mModule = getModule(mModuleName);
      mModule->loadResources(mModuleObject, &file);
    }

    file.setDescription(mProjectObject);
    for (JSONValue mIncludeValue : mProjectObject.getArray(JSON_INCLUDE)) {
      JSONObject mIncludeObject = mIncludeValue.getObject();
      ProjectFile* mIncludedProject = file.cInclusions.emplace_back(std::make_unique<ProjectFile>(*this, mIncludeObject)).get();
      std::vector<std::unique_ptr<JSONDocument>> mMoreOpenedDocuments = loadResources(*mIncludedProject);
      mOpenedDocuments.insert(mOpenedDocuments.end(), std::make_move_iterator(mMoreOpenedDocuments.begin()), std::make_move_iterator(mMoreOpenedDocuments.end()));
    }
    return mOpenedDocuments;
  }

  Module* Project::getModule(const std::string& name) {
    for (const std::unique_ptr<Module>& mModule : cDefModules) {
      if (mModule->getName() == name) {
        return mModule.get();
      }
    }
    return loadModule(name);
  }

  Project::~Project() {
    // remove(&cLuaBindingApplication);
    // remove(&cLuaBindingProject);
    // remove(&cLuaBindingOptions);
    // remove(&cQuitAction);
  }
  
  void Project::reset() {
    if (cRuntimeUpdatingRuntime) {
      cRuntimeResetPostponed = true;
    } else {
      for (const std::unique_ptr<Module>& mModule : cDefModules) {
        mModule->reset();
      }
    }
  }
  
  void Project::reset(Options& options) {
    cDefOptions = options;
    reset();
    cPostponedActions.emplace_back(***cDefActionOnStart);
  }

  void Project::reset(const ProjectLaunchConfiguration* configuration) {
    Options mOptions = configuration->getOptions();
    reset(mOptions);
  }

  bool Project::input(sf::Event& event) {
    cProcessingInput = true;
    bool mProcessed = (**cDefInputHandler)->input(event);
    cProcessingInput = false;
    return mProcessed;
  }

  void Project::updateRuntime(unsigned int milliseconds) {
    cRuntimeUpdatingRuntime = true;
    for (IAction* mPostponedAction : cPostponedActions) {
      mPostponedAction->execute();
    }
    cPostponedActions.clear();
    
    for (const std::unique_ptr<Module>& mModule : cDefModules) {
      mModule->updateInputs(milliseconds);
    }
    for (const std::unique_ptr<Module>& mModule : cDefModules) {
      mModule->updateRuntime(milliseconds);
    }
    updateTasks();
    cRuntimeUpdatingRuntime = false;
  }

  void Project::updateTasks() {
    while (!cUpdateTasks.empty()) {
      cUpdateTasks.front()();
      cUpdateTasks.pop();
    }
  }

  void Project::updateRuntimeComplete() {
    if (cRuntimeResetPostponed) {
      cRuntimeResetPostponed = false;
      reset();
    }
  }

  void Project::render(float aspectRatio) {
    (**cDefScreen)->renderScreen(1.0f, aspectRatio);
  }

  void Project::requestQuit() {
    (*cDefActionOnCloseRequest)->execute();
  }

  void Project::updateEditing(unsigned int milliseconds) {
    for (const std::unique_ptr<Module>& mModule : cDefModules) {
      mModule->updateEditing(milliseconds);
    }
    updateTasks();
  }

  void Project::save() const {
    saveRecursive(cDefProjectFileStructure);
  }

  void Project::save(const std::string& filename) {
    cDefProjectFileStructure.cFile.setPath(filename, true);
    save();
  }

  void Project::save(const ProjectFile& file) const {
    if (file.cFile.isSet() && file.isModifiable()) {
      JSONDocument mJSONDocument;
      JSONObject mProjectObject = mJSONDocument.addObject(JSON_PROJECT);
      file.save(mProjectObject);

      // Save project used assets
      cDefScreen.save(mProjectObject, JSON_SCREEN, file);
      cDefInputHandler.save(mProjectObject, JSON_INPUT, file);
      cDefDefaultEditor.save(mProjectObject, JSON_EDITOR, file);
      cDefActionOnStart.save(mProjectObject, JSON_ON_START, file);
      cDefActionOnCloseRequest.save(mProjectObject, JSON_ON_CLOSE_REQUEST, file);

      // Save launch configurations.
      bool mLaunchConfigurationsNeedSaving = false;
      for (const std::unique_ptr<ProjectLaunchConfiguration>& mLaunchConfiguration : cDefTestLaunchConfigurations) {
        if (mLaunchConfiguration->isOwnedBy(file)) {
          mLaunchConfigurationsNeedSaving = true;
          break;
        }
      }
      if (mLaunchConfigurationsNeedSaving) {
        JSONObject mLaunchConfigurationsObject = mProjectObject.addObject(JSON_LAUNCH_CONFIGURATIONS);
        for (const std::unique_ptr<ProjectLaunchConfiguration>& mLaunchConfiguration : cDefTestLaunchConfigurations) {
          mLaunchConfiguration->save(mLaunchConfigurationsObject, file);
        }
      }

      // Save modules
      JSONObject mModulesObject = mProjectObject.addObject(JSON_MODULES);
      for (const std::unique_ptr<Module>& mModule : cDefModules) {
        if (mModule->needsSaving(&file)) {
          mModule->save(mModulesObject, &file);
        }
      }

      mJSONDocument.save(file.cFile.getRelativePath());
    }
  }

  void Project::saveRecursive(const ProjectFile& file) const {
    save(file);
    for (const std::unique_ptr<ProjectFile>& mIncludedProject : file.cInclusions) {
      saveRecursive(*mIncludedProject.get());
    }
  }
  
  bool Project::isUser() {
    return cDefProjectFileStructure.cFile.isUser();
  }

  void Project::getProperties(IPropertyMaker& propertyMaker) {
    const Metadata& mMetadata = cApplication.getMetadata("Application");
    propertyMaker.createPropertyStruct("FileStructure", "Edit...", [this, &mMetadata](IPropertyMaker& propertyMaker) {
      cDefProjectFileStructure.getProperties(propertyMaker, mMetadata, *this, false);
    });
    propertyMaker.createPropertyStruct("LaunchConfigurations", "Edit...", [this, &mMetadata](IPropertyMaker& propertyMaker) {
      propertyMaker.createPropertyArray("LaunchConfigurationAdd", cDefTestLaunchConfigurations, [](const std::unique_ptr<ProjectLaunchConfiguration>& i)->ProjectLaunchConfiguration& {return *i;}, [this, &propertyMaker, &mMetadata](ProjectLaunchConfiguration& launchConfiguration) {
        propertyMaker.createPropertyStruct("LaunchConfiguration", launchConfiguration.getName(), [this, &mMetadata, &launchConfiguration](IPropertyMaker& propertyMaker) {
          launchConfiguration.getProperties(propertyMaker, mMetadata, *this);
        }, [this, &launchConfiguration]() {
          Utils::removeElementUnique(cDefTestLaunchConfigurations, &launchConfiguration);
        });
      }, [this]()->ProjectLaunchConfiguration& {
        return *cDefTestLaunchConfigurations.emplace_back(std::make_unique<ProjectLaunchConfiguration>(*this, cDefProjectFileStructure));
      });
    });
    cDefActionOnStart.getProperty(       propertyMaker, mMetadata, "OnStart");
    cDefActionOnCloseRequest.getProperty(propertyMaker, mMetadata, "OnCloseRequest");
    cDefInputHandler.getProperty(        propertyMaker, mMetadata, "InputHandler");
    cDefScreen.getProperty(              propertyMaker, mMetadata, "Display");
    cDefDefaultEditor.getProperty(       propertyMaker, mMetadata, "DefaultEditor");
  }
  
  IEditable* Project::getDefaultEditable() {
    return (*cDefDefaultEditor)->getTreeItemInfo().cID == "None" ? nullptr : ***cDefDefaultEditor;
  }

  IScreen* Project::getScreenProxy(IScreen* screen) {
    return cScreens.getProxy(screen);
  }

  Module* Project::loadModule(const std::string& moduleName) {
    Module* mModule = cDefModules.emplace_back(std::make_unique<Module>(moduleName, *this, &cLuaState)).get();
    mModule->registerAssets();
    return mModule;
  }

  void Project::unloadModule(const std::string& moduleName) {
    for (const std::unique_ptr<Module>& mModule : cDefModules) {
      if (mModule->getName() == moduleName) {
        cDefModules.erase(std::remove_if(cDefModules.begin(), cDefModules.end(), [moduleName](const std::unique_ptr<Module>& mModule) {
          return mModule->getName() == moduleName;
        }));
        break;
      }
    }
  }

  const std::vector<std::unique_ptr<Module>>& Project::getModules() const {
    return cDefModules;
  }
  
  std::vector<std::string> Project::getUnusedModuleNames() const {
    std::vector<std::string> mModuleFilenames = System::getFileList("./", true);
    std::vector<std::string> mModuleNames;
    for (std::string& mFilename : mModuleFilenames) {
      if (mFilename.substr(0, 10) == "IsoRealms-" && mFilename.substr(mFilename.length() - System::MODULE_EXTENSION.length()) == System::MODULE_EXTENSION) {
        std::string mModuleName = mFilename.substr(10, mFilename.length() - (10 + System::MODULE_EXTENSION.length()));
        bool mIsModuleLoaded = false;
        for (const std::unique_ptr<Module>& mModule : cDefModules) {
          if (mModule->getName() == mModuleName) {
            mIsModuleLoaded = true;
            break;
          }
        }
        if (!mIsModuleLoaded) {
          mModuleNames.emplace_back(mModuleName);
        }
      }
    }
    return mModuleNames;
  }

  bool Project::isLaunchConfigurationNameUsed(const std::string& name, ProjectLaunchConfiguration* configuration) const {
    for (const std::unique_ptr<ProjectLaunchConfiguration>& mLaunchConfiguration : cDefTestLaunchConfigurations) {
      if (configuration != mLaunchConfiguration.get() && mLaunchConfiguration->getName() == name) {
        return true;
      }
    }
    return false;
  }

  std::string Project::makeLaunchConfigurationName() const {
    std::string mDesiredName = "Launch Configuration";
    std::string mProposedName = mDesiredName;
    int mCount = 1;
    while (isLaunchConfigurationNameUsed(mProposedName, nullptr)) {
      mProposedName = mDesiredName + " " + Utils::toString(mCount++);
    }
    return mProposedName;
  }

  int Project::getLaunchConfigurationCount() const {
    return cDefTestLaunchConfigurations.size();
  }

  const ProjectLaunchConfiguration* Project::getLaunchConfiguration(int index) {
    return cDefTestLaunchConfigurations[index].get();
  }
  
  std::vector<std::string> Project::getWritableProjectFileNames() const {
    return cDefProjectFileStructure.getNames();
  }

  ProjectFile* Project::getProjectFile() {
    return &cDefProjectFileStructure;
  }
  
  ProjectFile* Project::getProjectFile(const std::string& id) {
    return cDefProjectFileStructure.getFile(id);
  }

  Application& Project::getApplication() {
    return cApplication;
  }

  const Application& Project::getApplication() const {
    return cApplication;
  }

  LuaState& Project::getLuaState() {
    return cLuaState;
  }
  
  void Project::init(std::function<void()> initialiser) {
    // std::cout << "ADDING INIT " << cInitialisers.size() << std::endl;
//     if (cInitialisers.size() == 241) {
//       std::cout << "DEBUG!" << std::endl;
//     }

    if (cResourcesLoaded) {
      throw ArgumentException("ERROR: Project::init: Resource initialisation is not allowed at this stage");
    }
    cInitialisers.push_back(initialiser);
  }
  
  // TODO: Rename this!  It's only used for rendering to texture tasks, so should be done during rendering phase.
  void Project::updateLater(std::function<void()> task) {
    cUpdateTasks.push(task);
  }
  
  std::string Project::getProjectPathPrefix(bool user) {
    return user ? (System::USER_DATA_DIRECTORY + (cDefProjectFileStructure.cFile.isUser() ? "" : "/Program/"))
                : (""                                                                            );
  }

  std::filesystem::file_time_type Project::getLastWriteTime() {
    std::string mPath = cDefProjectFileStructure.cFile.getPath();
    return std::filesystem::last_write_time(mPath);
  }

  void Project::makeUserDataDirectory(const std::string& path) {
    System::makeUserDataDirectory(getUserDataPath() + "/" + path);
  }

  void Project::renameUserDataDirectory(const std::string& path, const std::string& oldName, const std::string& newName) {
    System::renameUserDataDirectory(getUserDataPath() + "/" + path + "/" + oldName, getUserDataPath() + "/" + path + "/" + newName);
  }
  
  void Project::addScreenListener(IScreenListener* listener) {
    cScreens.addScreenListener(listener);
  }

  void Project::removeScreenListener(IScreenListener* listener) {
    cScreens.removeScreenListener(listener);
  }

  bool Project::isLoading() const {
    return cLoading;
  }
  
  void Project::execute(IAction& action) {
    if (cProcessingInput || cLoading) {
      cPostponedActions.emplace_back(&action);
    } else {
      action.execute();
    }
  }

  IEventBindings* Project::getEventBindings() const {
    return cLuaState.getCurrentEventBindings();
  }

  void Project::setEventBindings(IEventBindings* eventBindings) {
    cLuaState.setCurrentEventBindings(eventBindings);
  }

  IBoolean* Project::createLiteralBoolean(IAssetUser<IBoolean>* user, IResourceData& owner, bool value) {
    return cBooleans.literal(user, owner, value);
  }
  
  IColour* Project::createLiteralColour(IAssetUser<IColour>* user, IResourceData& owner, float red, float green, float blue, float alpha) {
    return cColours.literal(user, owner, red, green, blue, alpha);
  }
  
  IFloat* Project::createLiteralFloat(IAssetUser<IFloat>* user, IResourceData& owner, float value) {
    return cFloats.literal(user, owner, value);
  }
  
  IInteger* Project::createLiteralInteger(IAssetUser<IInteger>* user, IResourceData& owner, int value) {
    return cIntegers.literal(user, owner, value);
  }
  
  IString* Project::createLiteralString(IAssetUser<IString>* user, IResourceData& owner, const std::string& value) {
    return cStrings.literal(user, owner, value);
  }
  
  IVertex*  Project::createLiteralVertex(IAssetUser<IVertex>* user, IResourceData& owner, float x, float y, float z) {
    return cVertices.literal(user, owner, x, y, z);
  }

  void Project::finish(bool finishedByQuitRequest) {
    cFunctionNotifyComplete(finishedByQuitRequest);
  }

  std::string Project::getUserDataPath() {
    std::string mDataPath = cDefProjectFileStructure.cFile.getRelativePath();
    mDataPath = mDataPath.substr(0, mDataPath.find_last_of('.'));
    return (cDefProjectFileStructure.cFile.isUser() ? "" : "Program/") + mDataPath;
  }

  std::string Project::getDataPath(bool user) {
    std::string mDataPath = cDefProjectFileStructure.cFile.getRelativePath();
    mDataPath = mDataPath.substr(0, mDataPath.find_last_of('.'));
    return getProjectPathPrefix(user) + mDataPath;
  }

  std::string Project::getResourceID() const {
    return ""; // TODO: Implement this.
  }

  std::string Project::getResourceName() const {
    return ""; // TODO: Implement this.
  }

  std::string Project::getPath(const std::string& file, bool user) const {
    return ""; // TODO: Implement this.
  }

  void Project::makeUserDataDirectory() {
    System::makeUserDataDirectory(getUserDataPath());
  }

  bool Project::isIncluded() const {
    return false; // TODO: Implement this.
  }

  bool Project::isReadOnly() const {
    return false;
  }
  
  void Project::setOwner(ProjectFile* owner) {
    // Should never be called.
  }

  Project& Project::getProject() {
    return *this;
  }
  
  const Project& Project::getProject() const {
    return *this;
  }

  Project& Project::getAssetManager() {
    return *this;
  }

  IActionContext& Project::getDummyActionContext() {
    return *this;
  }

  const Metadata& Project::getMetadata() const {
    return cApplication.getMetadata("Application");
  }

  IResourceData& Project::getResourceData() {
    return *this;
  }

  IEventBindings* Project::getBindingRegistry() {
    return nullptr;
  }

  Project::QuitAction::QuitAction(Project& parent) :
            cParent(parent) {
  }

  void Project::QuitAction::execute() {
    cParent.finish(true);
  }

  bool Project::QuitAction::renderAssetIcon() const {
    return false;
  }

  void Project::QuitAction::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Project::QuitAction::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }
  
  bool Project::QuitAction::isDefaultConfiguration() const {
    return true;
  }
}

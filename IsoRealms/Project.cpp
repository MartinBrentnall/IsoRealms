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

#include "Application.h"
#include "Assets/Type/IScreenListener.h"
#include "DisplayResolution.h"
#include "Module.h"
#include "PropertyData.h"

namespace IsoRealms {
  Project::Project(Application& application, std::function<void(bool)> onFinish) :
          cApplication(application),
          cFunctionNotifyComplete(onFinish),
          cActions(*this),
          cBindings(*this),
          cBindingTypes(*this),
          cBooleans(*this),
          cColours(*this),
          cEditables(*this),
          cFloats(*this),
          cFonts(*this),
          cInputHandlers(*this),
          cIntegers(*this),
          cModels(*this),
          cScreens(*this),
          cStrings(*this),
          cTextures(*this),
          cVertices(*this),
          cDefProjectFileStructure(*this, "", true),
          cDefInputHandler(*this, cDefProjectFileStructure, *this),
          cDefScreen(*this, cDefProjectFileStructure, *this),
          cDefDefaultEditor(*this, cDefProjectFileStructure, *this),
          cDefInitAction(*this, cDefProjectFileStructure, *this),
          cDefResetAction(*this, cDefProjectFileStructure, *this),
          cDefStartAction(*this, cDefProjectFileStructure, *this),
          cDefQuitAction(*this, cDefProjectFileStructure, *this),
          cLuaBindingApplication(cLuaState, &application),
          cLuaBindingProject(cLuaState, this),
          cOptionsBinding(cLuaState, nullptr, this),
          cResourcesLoaded(false),
          cLoading(false),
          cProcessingInput(false),
          cRuntimeUpdatingRuntime(false),
          cRuntimeResetPostponed(false),
          cQuitAction(*this) {
    add<IBinding, IBinding>(&cLuaBindingApplication, "Application", "System");
    add<IBinding, IBinding>(&cLuaBindingProject,     "Project",     "System");
    add<IAction,  IAction> (&cQuitAction,            "Quit",        "System");
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

    cApplication.mainThreadInit([this]() {
      (*cDefInitAction)->execute();
    });
    cLoading = false;
  }

  std::vector<std::unique_ptr<JSONDocument>> Project::loadResources(ProjectFile& file) {
    std::unique_ptr<JSONDocument> mProjectDocument = std::make_unique<JSONDocument>(file.cFile);
    JSONObject mProjectObject = mProjectDocument->getObject(JSON_PROJECT);
    std::vector<std::unique_ptr<JSONDocument>> mOpenedDocuments;
    mOpenedDocuments.emplace_back(std::move(mProjectDocument));

    cDefScreen.init(mProjectObject, JSON_SCREEN, file);
    cDefInputHandler.init(mProjectObject, JSON_INPUT, file);
    cDefInitAction.init(mProjectObject, JSON_INITIALISATION, file);
    cDefResetAction.init(mProjectObject, JSON_RESET, file);
    cDefStartAction.init(mProjectObject, JSON_START, file);
    cDefQuitAction.init(mProjectObject, JSON_QUIT, file);
    cDefDefaultEditor.init(mProjectObject, JSON_EDITOR, file);

    for (JSONValue mModuleValue : mProjectObject.getArray(JSON_MODULES)) {
      JSONObject mModuleObject = mModuleValue.getObject();
      std::string mModuleName = mModuleObject.getString(JSON_NAME);
      getModule(mModuleName);
    }

    if (mProjectObject.hasMember(JSON_LAUNCH_CONFIGURATIONS)) {
      for (JSONThing mLaunchConfigurationThing : mProjectObject.getObject(JSON_LAUNCH_CONFIGURATIONS)) {
        cDefTestLaunchConfigurations.emplace_back(std::make_unique<ProjectLaunchConfiguration>(*this, file, mLaunchConfigurationThing));
      }
    }

    for (JSONValue mModuleValue : mProjectObject.getArray(JSON_MODULES)) {
      JSONObject mModuleObject = mModuleValue.getObject();
      std::string mModuleName = mModuleObject.getString(JSON_NAME);
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
    // remove(&cQuitAction);
  }
  
  void Project::reset() {
    if (cRuntimeUpdatingRuntime) {
      cRuntimeResetPostponed = true;
    } else {
      for (const std::unique_ptr<Module>& mModule : cDefModules) {
        mModule->reset();
      }
      (*cDefResetAction)->execute();
      cPostponedActions.emplace_back(***cDefStartAction);
    }
  }
  
  void Project::reset(Options& options) {
    // TODO: Implement this.
    reset();
  }

  void Project::reset(ProjectLaunchConfiguration* configuration) {
    // TODO: Implement this.
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
    (*cDefQuitAction)->execute();
  }

  void Project::updateEditing(unsigned int milliseconds) {
    for (const std::unique_ptr<Module>& mModule : cDefModules) {
      mModule->updateEditing(milliseconds);
    }
    updateTasks();
  }

  void Project::save() {
    save(cDefProjectFileStructure);
  }

  void Project::save(const std::string& filename) {
    cDefProjectFileStructure.cFile.setPath(filename, true);
    save();
  }

  void Project::save(ProjectFile& file) {
    if (file.cFile.isSet() && file.cFile.isUser()) {
      JSONDocument mJSONDocument;
      JSONObject mProjectObject = mJSONDocument.addObject(JSON_PROJECT);
      file.save(mProjectObject);

      // Save project used assets
      cDefScreen.save(mProjectObject, JSON_SCREEN, file);
      cDefInputHandler.save(mProjectObject, JSON_INPUT, file);
      cDefDefaultEditor.save(mProjectObject, JSON_EDITOR, file);
      cDefInitAction.save(mProjectObject, JSON_INITIALISATION, file);
      cDefResetAction.save(mProjectObject, JSON_RESET, file);
      cDefStartAction.save(mProjectObject, JSON_START, file);
      cDefQuitAction.save(mProjectObject, JSON_QUIT, file);

      // Save launch configurations.
      bool mLaunchConfigurationsNeedSaving = false;
      for (std::unique_ptr<ProjectLaunchConfiguration>& mLaunchConfiguration : cDefTestLaunchConfigurations) {
        if (mLaunchConfiguration->isOwnedBy(file)) {
          mLaunchConfigurationsNeedSaving = true;
          break;
        }
      }
      if (mLaunchConfigurationsNeedSaving) {
        JSONObject mLaunchConfigurationsObject = mProjectObject.addObject(JSON_LAUNCH_CONFIGURATIONS);
        for (std::unique_ptr<ProjectLaunchConfiguration>& mLaunchConfiguration : cDefTestLaunchConfigurations) {
          mLaunchConfiguration->save(mLaunchConfigurationsObject, file);
        }
      }

      // Save modules
      JSONArray mModulesArray = mProjectObject.addArray(JSON_MODULES);
      for (const std::unique_ptr<Module>& mModule : cDefModules) {
        if (mModule->needsSaving(&file)) {
          JSONObject mModuleObject = mModulesArray.addObject();
          mModule->save(mModuleObject, &file);
        }
      }

      mJSONDocument.save(file.cFile.getRelativePath());
    }

    for (std::unique_ptr<ProjectFile>& mIncludedProject : file.cInclusions) {
      save(*mIncludedProject.get());
    }
  }
  
  bool Project::isUser() {
    return cDefProjectFileStructure.cFile.isUser();
  }

  void Project::getProperties(PropertyMaker& propertyMaker) {
    const Metadata& mMetadata = cApplication.getMetadata("Application");
    propertyMaker.createPropertyStruct(mMetadata.getPropertyData("AppModules"), "Edit...", [this, &mMetadata](PropertyMaker& propertyMaker) {
      unsigned int mIndex = 1;
      for (const std::unique_ptr<Module>& mModule : cDefModules) {
        propertyMaker.createPropertyStruct(mMetadata.getPropertyData("Module"), mModule->getName(), [this, &mModule](PropertyMaker& propertyMaker) {
          // Nothing to do.
        }, [this, &mModule]() {
          Utils::removeElementUnique(cDefModules, mModule.get());
        });
        mIndex++;
      }
      propertyMaker.createPropertyOptional<ModuleChooser>(mMetadata.getPropertyData("Module"), [this](const std::string& value) {
        loadModule(value);
      });
    });
    propertyMaker.createPropertyStruct(mMetadata.getPropertyData("AppFileStructure"), "Edit...", [this, &mMetadata](PropertyMaker& propertyMaker) {
      cDefProjectFileStructure.getProperties(propertyMaker, mMetadata, *this, false);
    });
    propertyMaker.createPropertyStruct(mMetadata.getPropertyData("LaunchConfigurations"), "Edit...", [this, &mMetadata](PropertyMaker& propertyMaker) {
      propertyMaker.createPropertyArray(mMetadata.getPropertyData("LaunchConfigurationAdd"), cDefTestLaunchConfigurations, [](const std::unique_ptr<ProjectLaunchConfiguration>& i)->ProjectLaunchConfiguration& {return *i;}, [this, &propertyMaker, &mMetadata](ProjectLaunchConfiguration& launchConfiguration) {
        propertyMaker.createPropertyStruct(mMetadata.getPropertyData("LaunchConfiguration"), launchConfiguration.getName(), [this, &mMetadata, &launchConfiguration](PropertyMaker& propertyMaker) {
          launchConfiguration.getProperties(propertyMaker, mMetadata, *this);
        }, [this, &launchConfiguration]() {
          Utils::removeElementUnique(cDefTestLaunchConfigurations, &launchConfiguration);
        });
      }, [this]()->ProjectLaunchConfiguration& {
        return *cDefTestLaunchConfigurations.emplace_back(std::make_unique<ProjectLaunchConfiguration>(*this, cDefProjectFileStructure));
      });
    });
    cDefInitAction.getProperty(   propertyMaker, mMetadata, "OnInitialisation");
    cDefResetAction.getProperty(  propertyMaker, mMetadata, "OnReset");
    cDefStartAction.getProperty(  propertyMaker, mMetadata, "OnStart");
    cDefQuitAction.getProperty(   propertyMaker, mMetadata, "OnQuit");
    cDefInputHandler.getProperty( propertyMaker, mMetadata, "InputHandler");
    cDefScreen.getProperty(       propertyMaker, mMetadata, "Display");
    cDefDefaultEditor.getProperty(propertyMaker, mMetadata, "DefaultEditor");
  }
  
  IEditable* Project::getDefaultEditable() {
    return (*cDefDefaultEditor)->getID() == "None" ? nullptr : ***cDefDefaultEditor;
  }

  IScreen* Project::getScreenProxy(IScreen* screen) {
    return cScreens.getProxy(screen);
  }

  Module* Project::loadModule(const std::string& moduleName) {
    return cDefModules.emplace_back(std::make_unique<Module>(moduleName, *this, &cLuaState)).get();
  }

  void Project::unloadModule(const std::string& moduleName) {
    // TODO: Implement this
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
  
  std::vector<std::string> Project::getProjectFileNames() const {
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

  void Project::addStateChangeListener(const IFloat* asset, IStateListener<IFloat*>* listener) {
    cFloats.addStateChangeListener(asset, listener);
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

  IBinding* Project::getBinding(const std::string& id) {
    return id == "options" ? &cOptionsBinding
         :                   nullptr;
  }

  void Project::saveBinding(JSONObject object, const IBinding* binding) const {
    if (binding == &cOptionsBinding) {
      object.addString(JSON_LOCAL, "options");
    }
  }

  void Project::releaseBinding(const IBinding* asset) {
    // Nothing to do.
  }

  std::string Project::getPath(const std::string& file, bool user) const {
    return ""; // TODO: Implement this.
  }

  void Project::makeUserDataDirectory() {
    // TODO: Implement this.
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

  IActionClient& Project::getDummyActionClient() {
    return *this;
  }

  IResourceData& Project::getResourceData() {
    return *this;
  }

  IBindingRegistry* Project::getBindingRegistry() {
    return this;
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

  void Project::QuitAction::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }
  
  bool Project::QuitAction::isDefaultConfiguration() const {
    return true;
  }
}

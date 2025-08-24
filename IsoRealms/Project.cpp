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
  Project::Project(IApplication& application, std::function<void(bool)> onFinish) :
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
          cDefInputHandler(*this, *this),
          cDefScreen(*this, *this),
          cDefDefaultEditor(*this, *this),
          cDefInitAction(*this, *this),
          cDefResetAction(*this, *this),
          cDefStartAction(*this, *this),
          cDefQuitAction(*this, *this),
          cLuaBinding(cLuaState, this),
          cOptionsBinding(cLuaState, nullptr, this),
          cResourcesLoaded(false),
          cLoading(false),
          cProcessingInput(false),
          cRuntimeUpdatingRuntime(false),
          cRuntimeResetPostponed(false),
          cFilenameString(*this),
          cFileUserBoolean(*this),
          cQuitAction(*this) {

    // Project singletons.
    add<IBinding, IBinding>(&cLuaBinding,      "Project",         "System");
    add<IString,  IString> (&cFilenameString,  "ProjectFilename", "System");
    add<IBoolean, IBoolean>(&cFileUserBoolean, "ProjectUser",     "System");
    add<IAction,  IAction> (&cQuitAction,      "Quit",            "System");
  }

  Project::Project(IApplication& application, std::function<void(bool)> onFinish, const std::string& file, bool user) :
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

    mainThreadInit([this]() {
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

  Project::~Project() {
    // remove(&cLuaBinding);
    // remove(&cFilenameString);
    // remove(&cFileUserBoolean);
    // remove(&cQuitAction);

//     cActions.checkClean("Action Types");
//     cBindings.checkClean("Bindings");
//     cBooleans.checkClean("Booleans");
//     cColours.checkClean("Colours");
//     cEditables.checkClean("Editables");
//     cFloats.checkClean("Floats");
//     cFonts.checkClean("Fonts");
//     cInputHandlers.checkClean("Input Handlers");
//     cIntegers.checkClean("Integers");
//     cModels.checkClean("Models");
//     cScreens.checkClean("Screens");
//     cStrings.checkClean("Strings");
//     cTextures.checkClean("Textures");
//     cVertices.checkClean("Vertices");
  }































  bool Project::isLoading() const {
    return cLoading;
  }

  Module* Project::getModule(const std::string& name) {
    for (const std::unique_ptr<Module>& mModule : cDefModules) {
      if (mModule->getName() == name) {
        return mModule.get();
      }
    }
    return loadModule(name);
  }

  bool Project::isModuleLoaded(const std::string& name) const {
    for (const std::unique_ptr<Module>& mModule : cDefModules) {
      if (mModule->getName() == name) {
        return true;
      }
    }
    return false;
  }
  
  void Project::render(float aspectRatio) {
    (**cDefScreen)->renderScreen(1.0f, aspectRatio);
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

    while (!cUpdateTasks.empty()) {
      std::function<void()> mTask = cUpdateTasks.front();
      mTask();
      cUpdateTasks.pop();
    }
    cRuntimeUpdatingRuntime = false;
  }

  void Project::updateRuntimeComplete() {
    if (cRuntimeResetPostponed) {
      cRuntimeResetPostponed = false;
      reset();
    }
  }

  void Project::updateEditing(unsigned int milliseconds) {
    /*
     * This call is needed when a new editor view is added causing
     * more view dependent Textures to be created in the C64 Terrain
     * Texture Set.  TODO: I think there is a more elegant way of
     * handling this situation.
     */
    initMainThread();

    for (const std::unique_ptr<Module>& mModule : cDefModules) {
      mModule->updateEditing(milliseconds);
    }

    while (!cUpdateTasks.empty()) {
      std::function<void()> mTask = cUpdateTasks.front();
      mTask();
      cUpdateTasks.pop();
    }
  }

  void Project::finish(bool finishedByQuitRequest) {
    cFunctionNotifyComplete(finishedByQuitRequest);
  }

  bool Project::isFullScreen() {
    return cApplication.isFullScreen();
  }
    
  DisplayResolution Project::getDisplayResolution() {
    return cApplication.getDisplayResolution();
  }
    
  void Project::setDisplayResolution(DisplayResolution mode, bool fullScreen) {
    cApplication.setDisplayResolution(mode, fullScreen);
  }

  void Project::reset() {
    if (cRuntimeUpdatingRuntime) {
      cRuntimeResetPostponed = true;
    } else {
      for (const std::unique_ptr<Module>& mModule : cDefModules) {
        mModule->reset();
      }
      (*cDefResetAction)->execute();
      postponeAction(***cDefStartAction);
    }
  }
  
  void Project::reset(Options& options) {
    // TODO: Implement this.
    reset();
  }

  void Project::reset(ProjectLaunchConfiguration* configuration) {
    // TODO: Implement this.
  }

  void Project::requestQuit() {
    (*cDefQuitAction)->execute();
  }

  bool Project::input(sf::Event& event) {
    cProcessingInput = true;
    bool mProcessed = (**cDefInputHandler)->input(event);
    cProcessingInput = false;
    return mProcessed;
  }

  std::filesystem::file_time_type Project::getLastWriteTime() {
    std::string mPath = cDefProjectFileStructure.cFile.getPath();
    return std::filesystem::last_write_time(mPath);
  }

  void Project::initMainThread() {
    while (!cMainThreadAllocTasks.empty()) {
      std::function<void()> mTask = cMainThreadAllocTasks.front();
      mTask();
      cMainThreadAllocTasks.pop();
    }
    while (!cMainThreadInitTasks.empty()) {
      std::function<void()> mTask = cMainThreadInitTasks.front();
      mTask();
      cMainThreadInitTasks.pop();
    }
  }

  bool Project::canSave() {
    return cDefProjectFileStructure.cFile.isUser();
  }

  void Project::saveFile(ProjectFile& file) {
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
  }

  void Project::save(ProjectFile& file) {
    saveFile(file);
    for (std::unique_ptr<ProjectFile>& mIncludedProject : file.cInclusions) {
      save(*mIncludedProject.get());
    }
  }

  void Project::save() {
    save(cDefProjectFileStructure);
  }

  void Project::save(const std::string& filename) {
    cDefProjectFileStructure.cFile.setPath(filename, true);
    save();
  }

  bool Project::isUserProject() {
    return cDefProjectFileStructure.cFile.isUser();
  }

  std::string Project::getFilename() {
    return cDefProjectFileStructure.cFile.getPath();
  }
  
  Project& Project::getAssetManager() {
    return *this;
  }

  IActionClient& Project::getDummyActionClient() {
    return *this;
  }

  bool Project::isReadOnly() const {
    return false;
  }
  
  void Project::setOwner(ProjectFile* owner) {
    // Should never be called.
  }

  IResourceData& Project::getResourceData() {
    return *this;
  }

  IBindingRegistry* Project::getBindingRegistry() {
    return this;
  }

  ProjectFile* Project::getFile() {
    return &cDefProjectFileStructure;
  }

  Module* Project::loadModule(const std::string& moduleName) {
    return cDefModules.emplace_back(std::make_unique<Module>(moduleName, *this, &cLuaState)).get();
  }

  void Project::unloadModule(const std::string& moduleName) {
    // TODO: Implement this
  }

  std::set<IModule*> Project::getModules() {
    std::set<IModule*> mModules;
    for (const std::unique_ptr<Module>& mModule : cDefModules) {
      mModules.insert(mModule.get());
    }
    return mModules;
  }
  
  std::vector<std::string> Project::getUnusedModuleNames() const {
    std::vector<std::string> mModuleFilenames = System::getFileList("./", true);
    std::vector<std::string> mModuleNames;
    for (std::string& mFilename : mModuleFilenames) {
      if (mFilename.substr(0, 10) == "IsoRealms-" && mFilename.substr(mFilename.length() - System::MODULE_EXTENSION.length()) == System::MODULE_EXTENSION) {
        std::string mModuleName = mFilename.substr(10, mFilename.length() - (10 + System::MODULE_EXTENSION.length()));
        if (!isModuleLoaded(mModuleName)) {
          mModuleNames.emplace_back(mModuleName);
        }
      }
    }
    return mModuleNames;
  }

  JSONDocument Project::createDocument() {
    return JSONDocument();
  }

  JSONDocument Project::openDocument(const std::string& name) {
    return JSONDocument(name, true);
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

  void Project::makeUserDataDirectory(const std::string& path) {
    System::makeUserDataDirectory(getUserDataPath() + "/" + path);
  }

  void Project::renameUserDataDirectory(const std::string& path, const std::string& oldName, const std::string& newName) {
    System::renameUserDataDirectory(getUserDataPath() + "/" + path + "/" + oldName, getUserDataPath() + "/" + path + "/" + newName);
  }
  
  std::string Project::getProjectPathPrefix(bool user) {
    return user ? (System::USER_DATA_DIRECTORY + (cDefProjectFileStructure.cFile.isUser() ? "" : "/Program/"))
                : (""                                                                            );
  }

  IEditable* Project::getDefaultEditable() {
    return (*cDefDefaultEditor)->getID() == "None" ? nullptr : ***cDefDefaultEditor;
  }

  LuaState& Project::getLuaState() {
    return cLuaState;
  }
  
  IBoolean* Project::createLiteralBoolean(IAssetUser<IBoolean>* user, IResourceData& owner, bool value)                                      {return cBooleans.literal(user, owner, value ? std::string("true") : std::string("false"));}
  IColour*  Project::createLiteralColour( IAssetUser<IColour>*  user, IResourceData& owner, float red, float green, float blue, float alpha) {return cColours.literal( user, owner, Utils::toString(red) + " " + Utils::toString(green) + " " + Utils::toString(blue) + " " + Utils::toString(alpha));}
  IFloat*   Project::createLiteralFloat(  IAssetUser<IFloat>*   user, IResourceData& owner, float value)                                     {return cFloats.literal(  user, owner, Utils::toString(value));}
  IInteger* Project::createLiteralInteger(IAssetUser<IInteger>* user, IResourceData& owner, int value)                                       {return cIntegers.literal(user, owner, Utils::toString(value));}
  IString*  Project::createLiteralString( IAssetUser<IString>*  user, IResourceData& owner, const std::string& value)                        {return cStrings.literal( user, owner, value);}
  IVertex*  Project::createLiteralVertex( IAssetUser<IVertex>*  user, IResourceData& owner, float x, float y, float z)                       {return cVertices.literal(user, owner, Utils::toString(x) + " " + Utils::toString(y) + " " + Utils::toString(z));}

  Project& Project::getProject() {
    return *this;
  }
  
  const Project& Project::getProject() const {
    return *this;
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
  
  void Project::mainThreadAlloc(std::function<void()> task) {
    cMainThreadAllocTasks.push(task);
  }

  void Project::mainThreadInit(std::function<void()> task) {
    cMainThreadInitTasks.push(task);
  }

  void Project::mainThreadCleanUp(std::function<void()> function) {
    cApplication.mainThreadCleanUp(function);
  }

  IScreen* Project::getScreenProxy(IScreen* screen) {
    return cScreens.getProxy(screen);
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

  IApplication& Project::getApplication() {
    return cApplication;
  }

  const IApplication& Project::getApplication() const {
    return cApplication;
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

  bool Project::isProcessingInput() {
    return cProcessingInput;
  }

  void Project::postponeAction(IAction* action) {
    cPostponedActions.emplace_back(action);
  }

  std::vector<std::string> Project::getProjectFileNames() const {
    std::vector<std::string> mNames;
    cDefProjectFileStructure.getNames(mNames);
    return mNames;
  }

  ProjectFile* Project::getProjectFile(const std::string& id) {
    return cDefProjectFileStructure.getFile(id);
  }

  Project::Filename::Filename(Project& parent) :
            cParent(parent) {
  }

  std::string Project::Filename::getValue() const {
    return cParent.cDefProjectFileStructure.cFile.getRelativePath();
  }

  bool Project::Filename::renderAssetIcon() const {
    return false;
  }

  void Project::Filename::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Project::Filename::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool Project::Filename::isDefaultConfiguration() const {
    return true;
  }

  Project::FileUser::FileUser(Project& parent) :
            cParent(parent) {
  }

  bool Project::FileUser::getValue() const {
    return cParent.cDefProjectFileStructure.cFile.isUser();
  }

  bool Project::FileUser::renderAssetIcon() const {
    return false;
  }

  void Project::FileUser::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Project::FileUser::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool Project::FileUser::isDefaultConfiguration() const {
    return true;
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
  
  const std::string Project::JSON_EDITOR                = "editor";
  const std::string Project::JSON_INCLUDE               = "include";
  const std::string Project::JSON_INITIALISATION        = "initialisation";
  const std::string Project::JSON_INPUT                 = "input";
  const std::string Project::JSON_LAUNCH_CONFIGURATIONS = "launchConfigurations";
  const std::string Project::JSON_LOCAL                 = "local";
  const std::string Project::JSON_MODULES               = "modules";
  const std::string Project::JSON_NAME                  = "name";
  const std::string Project::JSON_PROJECT               = "project";
  const std::string Project::JSON_QUIT                  = "quit";
  const std::string Project::JSON_RESET                 = "reset";
  const std::string Project::JSON_SCREEN                = "screen";
  const std::string Project::JSON_START                 = "start";
}

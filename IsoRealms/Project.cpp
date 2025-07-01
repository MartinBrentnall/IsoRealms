/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Project.h"

#include "Application.h"

namespace IsoRealms {
  const std::string Project::JSON_ACTION         = "action";
  const std::string Project::JSON_EDITOR         = "editor";
  const std::string Project::JSON_FILENAME       = "filename";
  const std::string Project::JSON_ID             = "id";
  const std::string Project::JSON_INCLUDE        = "include";
  const std::string Project::JSON_INITIALISATION = "initialisation";
  const std::string Project::JSON_INPUT          = "input";
  const std::string Project::JSON_LOCAL          = "local";
  const std::string Project::JSON_MODULES        = "modules";
  const std::string Project::JSON_NAME           = "name";
  const std::string Project::JSON_PROJECT        = "project";
  const std::string Project::JSON_PROPERTIES     = "properties";
  const std::string Project::JSON_QUIT           = "quit";
  const std::string Project::JSON_RESET          = "reset";
  const std::string Project::JSON_SCREEN         = "screen";
  const std::string Project::JSON_START          = "start";
  const std::string Project::JSON_USER           = "user";

  const std::string Project::CATEGORY_CONVERSIONS = "Conversions";
  const std::string Project::CATEGORY_FIXED       = "Fixed";
  const std::string Project::CATEGORY_LOCAL       = "Local";

  Project::Project(IApplication& application, std::function<void(bool)> onFinish, IAssetOverride* override) :
          cApplication(application),
          cAssetOverride(override),
          cLuaBinding(*this, this),
          cResourcesLoaded(false),
          cLoading(false),
          cProcessingInput(false),
          cRuntimeUpdatingRuntime(false),
          cRuntimeResetPostponed(false),
          cActionTypes(&cLiteralProviderActionType),
          cBindings(&cLiteralProviderBinding),
          cBindingTypes(&cLiteralProviderBindingType, "Any"),
          cBooleans(&cLiteralProviderBoolean, "Literal"),
          cColours(&cLiteralProviderColour, "Literal"),
          cEditables(&cLiteralProviderEditable),
          cFloats(&cLiteralProviderFloat, "Literal"),
          cFonts(&cLiteralProviderFont),
          cInputHandlers(&cLiteralProviderInputHandler),
          cIntegers(&cLiteralProviderInteger, "Literal"),
          cModels(&cLiteralProviderModel),
          cScreens(&cLiteralProviderScreen),
          cProjectOptions(&cLiteralProviderProjectOptions),
          cAssets(&cLiteralProviderAssets),
          cStrings(&cLiteralProviderString, "Literal"),
          cTextures(&cLiteralProviderTexture),
          cVertices(&cLiteralProviderVertex, "Literal"),
          cLiteralProviderBoolean(*this),
          cBindingTypeAction(":Action"),
          cBindingTypeBoolean(":Boolean"),
          cBindingTypeColour(":Colour"),
          cBindingTypeFloat(":Float"),
          cBindingTypeFont(":Font"),
          cBindingTypeInputHandler(":InputHandler"),
          cBindingTypeInteger(":Integer"),
          cBindingTypeProject(":Project"),
          cBindingTypeProjectOptions(":ProjectOptions"),
          cBindingTypeScreen(":Screen"),
          cBindingTypeString(":String"),
          cBindingTypeVertex(":Vertex"),
          cConversionProviderActionToBinding(*this),
          cConversionProviderBooleanToBinding(*this),
          cConversionProviderColourToBinding(*this),
          cConversionProviderFloatToBinding(*this),
          cConversionProviderFontToBinding(*this),
          cConversionProviderInputHandlerToBinding(*this),
          cConversionProviderIntegerToBinding(*this),
          cConversionProviderProjectOptionsToBinding(*this),
          cConversionProviderProjectToBinding(*this),
          cConversionProviderScreenToBinding(*this),
          cConversionProviderStringToBinding(*this),
          cConversionProviderVertexToBinding(*this),
          cConversionProviderIntegerToString(*this),
          cConversionProviderFloatToString(*this),
          cConversionProviderProjectToString(*this),
          cConversionProviderProjectToInteger(*this),
          cFunctionNotifyComplete(onFinish),
          cProjectFile(*this, "", true),
          cFilenameString(*this),
          cFileUserBoolean(*this),
          cQuitAction(*this),
          cDefInputHandler(*this),
          cDefScreen(*this),
          cDefDefaultEditor(*this),
          cDefInitAction(*this),
          cDefResetAction(*this),
          cDefStartAction(*this),
          cDefQuitAction(*this),
          cPropertyValue(""),
          cPropertyValueBinding(*this, &cPropertyValue, this) {

    // Support conversions.
    cBindings.add(&cConversionProviderActionToBinding,         ":Action",         CATEGORY_CONVERSIONS);
    cBindings.add(&cConversionProviderBooleanToBinding,        ":Boolean",        CATEGORY_CONVERSIONS);
    cBindings.add(&cConversionProviderColourToBinding,         ":Colour",         CATEGORY_CONVERSIONS);
    cBindings.add(&cConversionProviderFloatToBinding,          ":Float",          CATEGORY_CONVERSIONS);
    cBindings.add(&cConversionProviderFontToBinding,           ":Font",           CATEGORY_CONVERSIONS);
    cBindings.add(&cConversionProviderInputHandlerToBinding,   ":InputHandler",   CATEGORY_CONVERSIONS);
    cBindings.add(&cConversionProviderIntegerToBinding,        ":Integer",        CATEGORY_CONVERSIONS);
    cBindings.add(&cConversionProviderProjectOptionsToBinding, ":ProjectOptions", CATEGORY_CONVERSIONS);
    cBindings.add(&cConversionProviderProjectToBinding,        ":Project",        CATEGORY_CONVERSIONS);
    cBindings.add(&cConversionProviderScreenToBinding,         ":Screen",         CATEGORY_CONVERSIONS);
    cBindings.add(&cConversionProviderStringToBinding,         ":String",         CATEGORY_CONVERSIONS);
    cBindings.add(&cConversionProviderVertexToBinding,         ":Vertex",         CATEGORY_CONVERSIONS);

    cBindingTypes.add(&cBindingTypeAction,         "Action",          CATEGORY_CONVERSIONS);
    cBindingTypes.add(&cBindingTypeBoolean,        "Boolean",         CATEGORY_CONVERSIONS);
    cBindingTypes.add(&cBindingTypeColour,         "Colour",          CATEGORY_CONVERSIONS);
    cBindingTypes.add(&cBindingTypeFloat,          "Float",           CATEGORY_CONVERSIONS);
    cBindingTypes.add(&cBindingTypeFont,           "Font",            CATEGORY_CONVERSIONS);
    cBindingTypes.add(&cBindingTypeInputHandler,   "Input Handler",   CATEGORY_CONVERSIONS);
    cBindingTypes.add(&cBindingTypeInteger,        "Integer",         CATEGORY_CONVERSIONS);
    cBindingTypes.add(&cBindingTypeProject,        "Project",         CATEGORY_CONVERSIONS);
    cBindingTypes.add(&cBindingTypeProjectOptions, "Project Options", CATEGORY_CONVERSIONS);
    cBindingTypes.add(&cBindingTypeScreen,         "Screen",          CATEGORY_CONVERSIONS);
    cBindingTypes.add(&cBindingTypeString,         "String",          CATEGORY_CONVERSIONS);
    cBindingTypes.add(&cBindingTypeVertex,         "Vertex",          CATEGORY_CONVERSIONS);

    cStrings.add(&cConversionProviderIntegerToString, ":Integer", CATEGORY_CONVERSIONS);
    cStrings.add(&cConversionProviderFloatToString,   ":Float",   CATEGORY_CONVERSIONS);
    cStrings.add(&cConversionProviderProjectToString, ":Project", CATEGORY_CONVERSIONS);

    cIntegers.add(&cConversionProviderProjectToInteger, ":Project", CATEGORY_CONVERSIONS);
    

    // Support locals
    cBindings.add(&cLocalProviderBinding, "~", CATEGORY_LOCAL);

    // Project singletons.
    add(&cLuaBinding,      "Project",         "System");
    add(&cFilenameString,  "ProjectFilename", "System");
    add(&cFileUserBoolean, "ProjectUser",     "System");
    add(&cQuitAction,      "Quit",            "System");
  }

  Project::Project(IApplication& application, IOptions& options, std::function<void(bool)> onFinish, IAssetOverride* override) :
            Project(application, onFinish, override) {
    cLoading = true;
    std::string mFile = options.getOption("file");
    if (!mFile.empty()) {
      cProcessingInput = true;

//       std::cout << "TYPE: " << options.getOption("type") << std::endl;
      bool mUser = options.getOption("type") == "user";
      cProjectFile.cFile.setPath(mFile, mUser);

      // Open Project File and Node
      JSONDocument mProjectDocument(mFile, mUser);
      JSONObject mProjectObject = mProjectDocument.getObject(JSON_PROJECT);

      // Load modules and any resources declared within them
      loadModules(mProjectObject);
      std::vector<std::unique_ptr<JSONDocument>> mOpenedDocuments = loadResources(mProjectObject, options, cProjectFile);
      for (const std::unique_ptr<Module>& mModule : cModules) {
        mModule->registerAssets();
      }
      cResourcesLoaded = true;

      // Initialise everything
      for (unsigned int j = 0; j < cInitialisers.size(); j++) {
//        std::cout << "INIT " << j << " OF " << cInitialisers.size() << std::endl;
//         if (j == 833) {
//           std::cout << "DEBUG!" << std::endl;
//         }
        cInitialisers[j](*this);
      }
      cInitialisers.clear();

      // Screen listeners cannot notified of screens before initialisation, so we need to do it now.
      cScreens.notifyAllScreensAdded(*this);

      cProcessingInput = false;

      mainThreadInit([this]() {
        (*cDefInitAction)->execute();
      });
    } else {
      cResourcesLoaded = true;
    }
    cLoading = false;
  }

  bool Project::isLoading() const {
    return cLoading;
  }

  Module* Project::getModule(const std::string& name) {
    for (const std::unique_ptr<Module>& mModule : cModules) {
      if (mModule->getName() == name) {
        return mModule.get();
      }
    }
    return loadModule(name);
  }

  void Project::loadModules(JSONObject object) {
    for (JSONObject mModuleObject : object.getArray(JSON_MODULES)) {
      std::string mModuleName = mModuleObject.getString(JSON_NAME);
      getModule(mModuleName);
    }

    for (JSONObject mIncludeObject : object.getArray(JSON_INCLUDE)) {
      std::string mName = mIncludeObject.getString(JSON_FILENAME);
      bool mUser = mIncludeObject.getBoolean(JSON_USER);
      std::unique_ptr<JSONDocument> mProjectDocument = std::make_unique<JSONDocument>(mName, mUser);
      JSONObject mProjectObject = mProjectDocument->getObject(JSON_PROJECT);
      loadModules(mProjectObject);
    }
  }

  bool Project::isModuleLoaded(const std::string& name) const {
    for (const std::unique_ptr<Module>& mModule : cModules) {
      if (mModule->getName() == name) {
        return true;
      }
    }
    return false;
  }
  
  std::vector<std::unique_ptr<JSONDocument>> Project::loadResources(JSONObject object, IOptions& options, ProjectFile& file) {
    cDefScreen.init(object, JSON_SCREEN, &file.cFile);
    cDefInputHandler.init(object, JSON_INPUT, &file.cFile);
    cDefInitAction.init(object, JSON_INITIALISATION, &file.cFile);
    cDefResetAction.init(object, JSON_RESET, &file.cFile);
    cDefStartAction.init(object, JSON_START, &file.cFile);
    cDefQuitAction.init(object, JSON_QUIT, &file.cFile);
    cDefDefaultEditor.init(object, JSON_EDITOR, &file.cFile);

    if (object.hasMember(JSON_PROPERTIES)) {
      for (JSONObject mPropertyObject : object.getArray(JSON_PROPERTIES)) {
        std::string mPropertyID = mPropertyObject.getString(JSON_ID);
        if (cProperties.find(mPropertyID) == cProperties.end()) {
          cProperties.emplace(mPropertyID, std::make_unique<ProjectProperty>(*this, mPropertyObject, &file.cFile));
        }
      }
    }

    for (JSONObject mModuleObject : object.getArray(JSON_MODULES)) {
      std::string mModuleName = mModuleObject.getString(JSON_NAME);
      Module* mModule = getModule(mModuleName);
      LocalOptions mModuleOptions(mModuleName, options);
      mModule->loadResources(mModuleObject, mModuleOptions, &file.cFile);
    }

    std::vector<std::unique_ptr<JSONDocument>> mOpenedDocuments;
    for (JSONObject mIncludeObject : object.getArray(JSON_INCLUDE)) {
      std::string mName = mIncludeObject.getString(JSON_FILENAME);
      bool mUser = mIncludeObject.getBoolean(JSON_USER);
      ProjectFile* mIncludedProject = file.cInclusions.emplace_back(std::make_unique<ProjectFile>(*this, mName, mUser)).get();
      
      std::unique_ptr<JSONDocument> mProjectDocument = std::make_unique<JSONDocument>(mName, mUser);
      JSONObject mProjectObject = mProjectDocument->getObject(JSON_PROJECT);
      std::vector<std::unique_ptr<JSONDocument>> mMoreOpenedDocuments = loadResources(mProjectObject, options, *mIncludedProject);
      mOpenedDocuments.insert(mOpenedDocuments.end(), std::make_move_iterator(mMoreOpenedDocuments.begin()), std::make_move_iterator(mMoreOpenedDocuments.end()));
      mOpenedDocuments.emplace_back(std::move(mProjectDocument));
    }
    return mOpenedDocuments;
  }

  void Project::render(float aspectRatio) {
    (**cDefScreen)->renderScreen(1.0f, aspectRatio);
  }

  void Project::setTime(int time) {
    cTime = time;
  }

  int Project::getTime() const {
    return cTime;
  }

  void Project::updateRuntime(unsigned int milliseconds) {
    cRuntimeUpdatingRuntime = true;
    for (IAction* mPostponedAction : cPostponedActions) {
      mPostponedAction->execute();
    }
    cPostponedActions.clear();
    
    for (std::unique_ptr<UpdateCallbackHandle>& mRuntimeDynamic : cRuntimeDynamics) {
      mRuntimeDynamic->call(milliseconds);
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

    for (std::unique_ptr<UpdateCallbackHandle>& mEditingDynamic : cEditingDynamics) {
      mEditingDynamic->call(milliseconds);
    }

    while (!cUpdateTasks.empty()) {
      std::function<void()> mTask = cUpdateTasks.front();
      mTask();
      cUpdateTasks.pop();
    }
  }

  void Project::executeCommand(const std::string& command) {
    system(command.c_str());
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
      for (std::unique_ptr<ResetCallbackHandle>& mResetter : cResetters) {
        mResetter->call();
      }
      (*cDefResetAction)->execute();
      (**cDefStartAction)->postpone();
    }
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
    std::string mPath = cProjectFile.cFile.getPath();
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
    return cProjectFile.cFile.isUser();
  }

  void Project::saveFile(ProjectFile& file) {
    if (cProjectFile.cFile.isSet() && cProjectFile.cFile.isUser()) {
      JSONDocument mJSONDocument;
      JSONObject mProjectObject = mJSONDocument.addObject(JSON_PROJECT);
      JSONArray mIncludeArray = mProjectObject.addArray(JSON_INCLUDE);
      for (std::unique_ptr<ProjectFile>& mInclusion : file.cInclusions) {
        JSONObject mIncludeObject = mIncludeArray.addObject();
        mInclusion->cFile.save(JSON_FILENAME, mIncludeObject);
      }

      // Save project used assets
      cDefScreen.save(mProjectObject, JSON_SCREEN, &file.cFile);
      cDefInputHandler.save(mProjectObject, JSON_INPUT, &file.cFile);
      cDefDefaultEditor.save(mProjectObject, JSON_EDITOR, &file.cFile);
      cDefInitAction.save(mProjectObject, JSON_INITIALISATION, &file.cFile);
      cDefResetAction.save(mProjectObject, JSON_RESET, &file.cFile);
      cDefStartAction.save(mProjectObject, JSON_START, &file.cFile);
      cDefQuitAction.save(mProjectObject, JSON_QUIT, &file.cFile);

      // Save properties
      bool mPropertiesNeedSaving = false;
      for (std::pair<const std::string, std::unique_ptr<ProjectProperty>>& mPair : cProperties) {
        if (mPair.second->isOwnedBy(&file.cFile)) {
          mPropertiesNeedSaving = true;
          break;
        }
      }
      if (mPropertiesNeedSaving) {
        JSONArray mPropertiesArray = mProjectObject.addArray(JSON_PROPERTIES);
        for (std::pair<const std::string, std::unique_ptr<ProjectProperty>>& mPair : cProperties) {
          mPair.second->save(mPropertiesArray, &file.cFile, mPair.first);
        }
      }

      // Save modules
      JSONArray mModulesArray = mProjectObject.addArray(JSON_MODULES);
      for (const std::unique_ptr<Module>& mModule : cModules) {
        if (mModule->needsSaving(&file.cFile)) {
          JSONObject mModuleObject = mModulesArray.addObject();
          mModule->save(mModuleObject, &file.cFile);
        }
      }

      mJSONDocument.save(cProjectFile.cFile.getRelativePath());
    }
  }

  void Project::save(ProjectFile& file) {
    saveFile(file);
    for (std::unique_ptr<ProjectFile>& mIncludedProject : file.cInclusions) {
      saveFile(*mIncludedProject.get());
    }
  }

  void Project::save() {
    save(cProjectFile);
  }

  void Project::save(const std::string& filename) {
    cProjectFile.cFile.setPath(filename, true);
    save();
  }

  bool Project::isUserProject() {
    return cProjectFile.cFile.isUser();
  }

  IAssetBrowser& Project::getResourceManager() {
    return *this;
  }

  std::string Project::getFilename() {
    return cProjectFile.cFile.getPath();
  }
  
  IProject& Project::getAssetManager() {
    return *this;
  }
  
  bool Project::isReadOnly() const {
    return false;
  }
  
  void Project::setOwner(File* owner) {
    // Should never be called.
  }

  File* Project::getFile() {
    return &cProjectFile.cFile;
  }

  Module* Project::loadModule(const std::string& moduleName) {
    return cModules.emplace_back(std::make_unique<Module>(moduleName, *this, &cLuaState)).get();
  }

  void Project::unloadModule(const std::string& moduleName) {
    // TODO: Implement this
  }

  std::set<IModule*> Project::getModules() {
    std::set<IModule*> mModules;
    for (const std::unique_ptr<Module>& mModule : cModules) {
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
    std::string mDataPath = cProjectFile.cFile.getRelativePath();
    mDataPath = mDataPath.substr(0, mDataPath.find_last_of('.'));
    return (cProjectFile.cFile.isUser() ? "" : "Program/") + mDataPath;
  }

  std::string Project::getDataPath(bool user) {
    std::string mDataPath = cProjectFile.cFile.getRelativePath();
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
    return user ? (System::USER_DATA_DIRECTORY + (cProjectFile.cFile.isUser() ? "" : "/Program/"))
                : (""                                                                            );
  }

  IEditable* Project::getDefaultEditable() {
    return (*cDefDefaultEditor)->getID() == "None" ? nullptr : ***cDefDefaultEditor;
  }

  LuaState* const Project::getLuaState() {
    return &cLuaState;
  }
  
  std::string Project::getProjectResourceDataPath(const std::string& file) {
    std::string mDataPath = cProjectFile.cFile.getRelativePath();
    return mDataPath.substr(0, mDataPath.find_last_of('.')) + "/" + file;
  }

  ActionExecutor* Project::getAction(IAssetUser<ActionExecutor>* user, JSONObject object, IResourceData& owner, const std::string& tag, IBindingRegistry* localArgs) {
    if (object.hasMember(tag)) {
      JSONObject mActionObject = object.getObject(tag);
      std::unique_ptr<ActionExecutor> mActionExecutor = std::make_unique<ActionExecutor>(*this, user, mActionObject, localArgs);
      ActionExecutor* mAction = mActionExecutor.get();
      cActionExecutors[mAction] = std::move(mActionExecutor);
      return mAction;
    }
    return nullptr;
  }
  IActionType*     Project::getActionType(      IAssetUser<IActionType>*     user, JSONObject object, IResourceData& owner,                                      bool required) {return cActionTypes.get(      user, owner, object, nullptr,  required, [this](JSONObject object, IStateListener<IActionType*>*     listener) -> IActionType*     {return cAssetOverride != nullptr ? cAssetOverride->getActionType(      object, listener) : nullptr;});}
  IAssets*         Project::getAssets(          IAssetUser<IAssets>*         user, JSONObject object, IResourceData& owner,                                      bool required) {return cAssets.get(           user, owner, object, nullptr,  required, [this](JSONObject object, IStateListener<IAssets*>*         listener) -> IAssets*         {return cAssetOverride != nullptr ? cAssetOverride->getAssets(          object, listener) : nullptr;});}
  IBinding*        Project::getBinding(         IAssetUser<IBinding>*        user, JSONObject object, IResourceData& owner, IBindingRegistry* locals,            bool required) {
    cLocalProviderBinding.setLocalBindings(locals);
    IBinding* mBinding = cBindings.get(user, owner, object, nullptr, required, [this](JSONObject object, IStateListener<IBinding*>* listener) -> IBinding*    {return cAssetOverride != nullptr ? cAssetOverride->getBinding(       object, listener) : nullptr;});
    cLocalProviderBinding.setLocalBindings(nullptr);
    return mBinding;
  }
  IBindingType*    Project::getBindingType(     IAssetUser<IBindingType>*    user, JSONObject object, IResourceData& owner,                                      bool required) {return cBindingTypes.get(     user, owner, object, nullptr,  required, [this](JSONObject object, IStateListener<IBindingType*>*    listener) -> IBindingType*    {return cAssetOverride != nullptr ? cAssetOverride->getBindingType(     object, listener) : nullptr;});}
  IBoolean*        Project::getBoolean(         IAssetUser<IBoolean>*        user, JSONObject object, IResourceData& owner, IStateListener<IBoolean*>* listener, bool required) {return cBooleans.get(         user, owner, object, listener, required, [this](JSONObject object, IStateListener<IBoolean*>*        listener) -> IBoolean*        {return cAssetOverride != nullptr ? cAssetOverride->getBoolean(         object, listener) : nullptr;});}
  IColour*         Project::getColour(          IAssetUser<IColour>*         user, JSONObject object, IResourceData& owner, IStateListener<IColour*>*  listener, bool required) {return cColours.get(          user, owner, object, listener, required, [this](JSONObject object, IStateListener<IColour*>*         listener) -> IColour*         {return cAssetOverride != nullptr ? cAssetOverride->getColour(          object, listener) : nullptr;});}
  IEditable*       Project::getEditable(        IAssetUser<IEditable>*       user, JSONObject object, IResourceData& owner,                                      bool required) {return cEditables.get(        user, owner, object, nullptr,  required, [this](JSONObject object, IStateListener<IEditable*>*       listener) -> IEditable*       {return cAssetOverride != nullptr ? cAssetOverride->getEditable(        object, listener) : nullptr;});}
  IFloat*          Project::getFloat(           IAssetUser<IFloat>*          user, JSONObject object, IResourceData& owner, IStateListener<IFloat*>*   listener, bool required) {return cFloats.get(           user, owner, object, listener, required, [this](JSONObject object, IStateListener<IFloat*>*          listener) -> IFloat*          {return cAssetOverride != nullptr ? cAssetOverride->getFloat(           object, listener) : nullptr;});}
  IFont*           Project::getFont(            IAssetUser<IFont>*           user, JSONObject object, IResourceData& owner,                                      bool required) {return cFonts.get(            user, owner, object, nullptr,  required, [this](JSONObject object, IStateListener<IFont*>*           listener) -> IFont*           {return cAssetOverride != nullptr ? cAssetOverride->getFont(            object, listener) : nullptr;});}
  IInputHandler*   Project::getInputHandler(    IAssetUser<IInputHandler>*   user, JSONObject object, IResourceData& owner,                                      bool required) {return cInputHandlers.get(    user, owner, object, nullptr,  required, [this](JSONObject object, IStateListener<IInputHandler*>*   listener) -> IInputHandler*   {return cAssetOverride != nullptr ? cAssetOverride->getInputHandler(    object, listener) : nullptr;});}
  IInteger*        Project::getInteger(         IAssetUser<IInteger>*        user, JSONObject object, IResourceData& owner, IStateListener<IInteger*>* listener, bool required) {return cIntegers.get(         user, owner, object, listener, required, [this](JSONObject object, IStateListener<IInteger*>*        listener) -> IInteger*        {return cAssetOverride != nullptr ? cAssetOverride->getInteger(         object, listener) : nullptr;});}
  IModel*          Project::getModel(           IAssetUser<IModel>*          user, JSONObject object, IResourceData& owner,                                      bool required) {return cModels.get(           user, owner, object, nullptr,  required, [this](JSONObject object, IStateListener<IModel*>*          listener) -> IModel*          {return cAssetOverride != nullptr ? cAssetOverride->getModel(           object, listener) : nullptr;});}
  IProjectOptions* Project::getProjectOptions(  IAssetUser<IProjectOptions>* user, JSONObject object, IResourceData& owner,                                      bool required) {return cProjectOptions.get(   user, owner, object, nullptr,  required, [this](JSONObject object, IStateListener<IProjectOptions*>* listener) -> IProjectOptions* {return cAssetOverride != nullptr ? cAssetOverride->getProjectOptions(  object, listener) : nullptr;});}
  IScreen*         Project::getScreen(          IAssetUser<IScreen>*         user, JSONObject object, IResourceData& owner,                                      bool required) {return cScreens.get(          user, owner, object, nullptr,  required, [this](JSONObject object, IStateListener<IScreen*>*         listener) -> IScreen*         {return cAssetOverride != nullptr ? cAssetOverride->getScreen(          object, listener) : nullptr;});}
  IString*         Project::getString(          IAssetUser<IString>*         user, JSONObject object, IResourceData& owner, IStateListener<IString*>*  listener, bool required) {return cStrings.get(          user, owner, object, listener, required, [this](JSONObject object, IStateListener<IString*>*         listener) -> IString*         {return cAssetOverride != nullptr ? cAssetOverride->getString(          object, listener) : nullptr;});}
  ITexture*        Project::getTexture(         IAssetUser<ITexture>*        user, JSONObject object, IResourceData& owner, IStateListener<ITexture*>* listener, bool required) {return cTextures.get(         user, owner, object, listener, required, [this](JSONObject object, IStateListener<ITexture*>*        listener) -> ITexture*        {return cAssetOverride != nullptr ? cAssetOverride->getTexture(         object, listener) : nullptr;});}
  IVertex*         Project::getVertex(          IAssetUser<IVertex>*         user, JSONObject object, IResourceData& owner,                                      bool required) {return cVertices.get(         user, owner, object, nullptr,  required, [this](JSONObject object, IStateListener<IVertex*>*         listener) -> IVertex*         {return cAssetOverride != nullptr ? cAssetOverride->getVertex(          object, listener) : nullptr;});}

  ActionExecutor*  Project::getAction(        IAssetUser<ActionExecutor>*  user, const std::string& id, IResourceData& owner)                                      {
    std::unique_ptr<ActionExecutor> mActionExecutor = std::make_unique<ActionExecutor>(*this, user, id);
    ActionExecutor* mAction = mActionExecutor.get();
    cActionExecutors[mAction] = std::move(mActionExecutor);
    return mAction;
  }
  IActionType*     Project::getActionType(    IAssetUser<IActionType>*     user, const std::string& id, IResourceData& owner)                                      {return cActionTypes.get(   user, owner, id, nullptr);}
  IAssets*         Project::getAssets(        IAssetUser<IAssets>*         user, const std::string& id, IResourceData& owner)                                      {return cAssets.get(        user, owner, id, nullptr);}
  IBinding*        Project::getBinding(       IAssetUser<IBinding>*        user, const std::string& id, IResourceData& owner)                                      {return cBindings.get(      user, owner, id, nullptr);}
  IBindingType*    Project::getBindingType(   IAssetUser<IBindingType>*    user, const std::string& id, IResourceData& owner)                                      {return cBindingTypes.get(  user, owner, id, nullptr);}
  IBoolean*        Project::getBoolean(       IAssetUser<IBoolean>*        user, const std::string& id, IResourceData& owner, IStateListener<IBoolean*>* listener) {return cBooleans.get(      user, owner, id, listener);}
  IColour*         Project::getColour(        IAssetUser<IColour>*         user, const std::string& id, IResourceData& owner, IStateListener<IColour*>*  listener) {return cColours.get(       user, owner, id, listener);}
  IEditable*       Project::getEditable(      IAssetUser<IEditable>*       user, const std::string& id, IResourceData& owner)                                      {return cEditables.get(     user, owner, id, nullptr);}
  IFloat*          Project::getFloat(         IAssetUser<IFloat>*          user, const std::string& id, IResourceData& owner, IStateListener<IFloat*>*   listener) {return cFloats.get(        user, owner, id, listener);}
  IFont*           Project::getFont(          IAssetUser<IFont>*           user, const std::string& id, IResourceData& owner)                                      {return cFonts.get(         user, owner, id, nullptr);}
  IInputHandler*   Project::getInputHandler(  IAssetUser<IInputHandler>*   user, const std::string& id, IResourceData& owner)                                      {return cInputHandlers.get( user, owner, id, nullptr);}
  IInteger*        Project::getInteger(       IAssetUser<IInteger>*        user, const std::string& id, IResourceData& owner, IStateListener<IInteger*>* listener) {return cIntegers.get(      user, owner, id, listener);}
  IModel*          Project::getModel(         IAssetUser<IModel>*          user, const std::string& id, IResourceData& owner)                                      {return cModels.get(        user, owner, id, nullptr);}
  IProjectOptions* Project::getProjectOptions(IAssetUser<IProjectOptions>* user, const std::string& id, IResourceData& owner)                                      {return cProjectOptions.get(user, owner, id, nullptr);}
  IScreen*         Project::getScreen(        IAssetUser<IScreen>*         user, const std::string& id, IResourceData& owner)                                      {return cScreens.get(       user, owner, id, nullptr);}
  IString*         Project::getString(        IAssetUser<IString>*         user, const std::string& id, IResourceData& owner, IStateListener<IString*>*  listener) {return cStrings.get(       user, owner, id, listener);}
  ITexture*        Project::getTexture(       IAssetUser<ITexture>*        user, const std::string& id, IResourceData& owner, IStateListener<ITexture*>* listener) {return cTextures.get(      user, owner, id, listener);}
  IVertex*         Project::getVertex(        IAssetUser<IVertex>*         user, const std::string& id, IResourceData& owner)                                      {return cVertices.get(      user, owner, id, nullptr);}

  ActionExecutor* Project::createLiteralAction(       IAssetUser<ActionExecutor>*  user, IResourceData& owner) {
    std::unique_ptr<ActionExecutor> mActionExecutor = std::make_unique<ActionExecutor>(*this, user);
    ActionExecutor* mAction = mActionExecutor.get();
    cActionExecutors[mAction] = std::move(mActionExecutor);
    return mAction;
  }
  IActionType*     Project::createLiteralActionType(    IAssetUser<IActionType>*     user, IResourceData& owner)                                                                          {return cActionTypes.literal(   user, owner, "");}
  IAssets*         Project::createLiteralAssets(        IAssetUser<IAssets>*         user, IResourceData& owner)                                                                          {return cAssets.literal(        user, owner, "");}
  IBinding*        Project::createLiteralBinding(       IAssetUser<IBinding>*        user, IResourceData& owner)                                                                          {return cBindings.literal(      user, owner, "");}
  IBindingType*    Project::createLiteralBindingType(   IAssetUser<IBindingType>*    user, IResourceData& owner)                                                                          {return cBindingTypes.literal(  user, owner, "");}
  IBoolean*        Project::createLiteralBoolean(       IAssetUser<IBoolean>*        user, IResourceData& owner, const bool value)                                                        {return cBooleans.literal(      user, owner, value ? std::string("true") : std::string("false"));}
  IColour*         Project::createLiteralColour(        IAssetUser<IColour>*         user, IResourceData& owner, const float red, const float green, const float blue, const float alpha) {return cColours.literal(       user, owner, Utils::toString(red) + " " + Utils::toString(green) + " " + Utils::toString(blue) + " " + Utils::toString(alpha));}
  IEditable*       Project::createLiteralEditable(      IAssetUser<IEditable>*       user, IResourceData& owner)                                                                          {return cEditables.literal(     user, owner, "");}
  IFloat*          Project::createLiteralFloat(         IAssetUser<IFloat>*          user, IResourceData& owner, const float value)                                                       {return cFloats.literal(        user, owner, Utils::toString(value));}
  IFont*           Project::createLiteralFont(          IAssetUser<IFont>*           user, IResourceData& owner)                                                                          {return cFonts.literal(         user, owner, "");}
  IInputHandler*   Project::createLiteralInputHandler(  IAssetUser<IInputHandler>*   user, IResourceData& owner)                                                                          {return cInputHandlers.literal( user, owner, "");}
  IInteger*        Project::createLiteralInteger(       IAssetUser<IInteger>*        user, IResourceData& owner, const int value)                                                         {return cIntegers.literal(      user, owner, Utils::toString(value));}
  IModel*          Project::createLiteralModel(         IAssetUser<IModel>*          user, IResourceData& owner)                                                                          {return cModels.literal(        user, owner, "");}
  IProjectOptions* Project::createLiteralProjectOptions(IAssetUser<IProjectOptions>* user, IResourceData& owner)                                                                          {return cProjectOptions.literal(user, owner, "");}
  IScreen*         Project::createLiteralScreen(        IAssetUser<IScreen>*         user, IResourceData& owner)                                                                          {return cScreens.literal(       user, owner, "");}
  IString*         Project::createLiteralString(        IAssetUser<IString>*         user, IResourceData& owner, const std::string& value)                                                {return cStrings.literal(       user, owner, value);}
  ITexture*        Project::createLiteralTexture(       IAssetUser<ITexture>*        user, IResourceData& owner)                                                                          {return cTextures.literal(      user, owner, "");}
  IVertex*         Project::createLiteralVertex(        IAssetUser<IVertex>*         user, IResourceData& owner, const float x, const float y, const float z)                             {return cVertices.literal(      user, owner, Utils::toString(x) + " " + Utils::toString(y) + " " + Utils::toString(z));}
    
  void Project::release(IAssetUser<ActionExecutor>*  user, ActionExecutor*  asset) {
    std::map<ActionExecutor*, std::unique_ptr<ActionExecutor>>::iterator mActionExecutor = cActionExecutors.find(asset);
    if (mActionExecutor == cActionExecutors.end()) {
      std::cout << "WARNING: No client attached to the specified action" << std::endl;
      return;
    }
    cActionExecutors.erase(asset);
  }
  void Project::release(IAssetUser<IActionType>*     user, IActionType*     asset) {cActionTypes.release(   user, asset);}
  void Project::release(IAssetUser<IAssets>*         user, IAssets*         asset) {cAssets.release(        user, asset);}
  void Project::release(IAssetUser<IBinding>*        user, IBinding*        asset) {cBindings.release(      user, asset);}
  void Project::release(IAssetUser<IBindingType>*    user, IBindingType*    asset) {cBindingTypes.release(  user, asset);}
  void Project::release(IAssetUser<IBoolean>*        user, IBoolean*        asset) {cBooleans.release(      user, asset);}
  void Project::release(IAssetUser<IColour>*         user, IColour*         asset) {cColours.release(       user, asset);}
  void Project::release(IAssetUser<IEditable>*       user, IEditable*       asset) {cEditables.release(     user, asset);}
  void Project::release(IAssetUser<IFloat>*          user, IFloat*          asset) {cFloats.release(        user, asset);}
  void Project::release(IAssetUser<IFont>*           user, IFont*           asset) {cFonts.release(         user, asset);}
  void Project::release(IAssetUser<IInputHandler>*   user, IInputHandler*   asset) {cInputHandlers.release( user, asset);}
  void Project::release(IAssetUser<IInteger>*        user, IInteger*        asset) {cIntegers.release(      user, asset);}
  void Project::release(IAssetUser<IModel>*          user, IModel*          asset) {cModels.release(        user, asset);}
  void Project::release(IAssetUser<IProjectOptions>* user, IProjectOptions* asset) {cProjectOptions.release(user, asset);}
  void Project::release(IAssetUser<IScreen>*         user, IScreen*         asset) {cScreens.release(       user, asset);}
  void Project::release(IAssetUser<IString>*         user, IString*         asset) {cStrings.release(       user, asset);}
  void Project::release(IAssetUser<ITexture>*        user, ITexture*        asset) {cTextures.release(      user, asset);}
  void Project::release(IAssetUser<IVertex>*         user, IVertex*         asset) {cVertices.release(      user, asset);}
    
  bool Project::renderAssetIcon() const {
    return false;
  }

  void Project::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Project::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Project::isDefaultConfiguration() const {
    return true;
  }

  std::vector<std::string> Project::getAllActionTypes()   {return cActionTypes.getAll();}
  std::vector<std::string> Project::getAllBindings()      {return cBindings.getAll();}
  std::vector<std::string> Project::getAllBindingTypes()  {return cBindingTypes.getAll();}
  std::vector<std::string> Project::getAllBooleans()      {return cBooleans.getAll();}
  std::vector<std::string> Project::getAllColours()       {return cColours.getAll();}
  std::vector<std::string> Project::getAllEditables()     {return cEditables.getAll();}
  std::vector<std::string> Project::getAllFloats()        {return cFloats.getAll();}
  std::vector<std::string> Project::getAllFonts()         {return cFonts.getAll();}
  std::vector<std::string> Project::getAllInputHandlers() {return cInputHandlers.getAll();}
  std::vector<std::string> Project::getAllIntegers()      {return cIntegers.getAll();}
  std::vector<std::string> Project::getAllModels()        {return cModels.getAll();}
  std::vector<std::string> Project::getAllScreens()       {return cScreens.getAll();}
  std::vector<std::string> Project::getAllStrings()       {return cStrings.getAll();}
  std::vector<std::string> Project::getAllTextures()      {return cTextures.getAll();}
  std::vector<std::string> Project::getAllVertices()      {return cVertices.getAll();}

  void Project::addListener(IAssetListener<IResourceData, IBoolean>*  listener) {cBooleans.addAssetListener( listener);}
  void Project::addListener(IAssetListener<IResourceData, IColour>*   listener) {cColours.addAssetListener(  listener);}
  void Project::addListener(IAssetListener<IResourceData, IEditable>* listener) {cEditables.addAssetListener(listener);}
  void Project::addListener(IAssetListener<IResourceData, IFloat>*    listener) {cFloats.addAssetListener(   listener);}
  void Project::addListener(IAssetListener<IResourceData, IFont>*     listener) {cFonts.addAssetListener(    listener);}
  void Project::addListener(IAssetListener<IResourceData, IInteger>*  listener) {cIntegers.addAssetListener( listener);}
  void Project::addListener(IAssetListener<IResourceData, IModel>*    listener) {cModels.addAssetListener(   listener);}
  void Project::addListener(IAssetListener<IResourceData, IScreen>*   listener) {cScreens.addAssetListener(  listener);}
  void Project::addListener(IAssetListener<IResourceData, IString>*   listener) {cStrings.addAssetListener(  listener);}
  void Project::addListener(IAssetListener<IResourceData, ITexture>*  listener) {cTextures.addAssetListener( listener);}
  void Project::addListener(IAssetListener<IResourceData, IVertex>*   listener) {cVertices.addAssetListener( listener);}

  bool Project::renderActionIcon(      const std::string& id) const {return cActionTypes.renderIcon(id);}
  bool Project::renderBindingIcon(     const std::string& id) const {return cBindings.renderIcon(id);}
  bool Project::renderBindingTypeIcon( const std::string& id) const {return cBindingTypes.renderIcon(id);}
  bool Project::renderBooleanIcon(     const std::string& id) const {return cBooleans.renderIcon(id);}
  bool Project::renderColourIcon(      const std::string& id) const {return cColours.renderIcon(id);}
  bool Project::renderFloatIcon(       const std::string& id) const {return cFloats.renderIcon(id);}
  bool Project::renderFontIcon(        const std::string& id) const {return cFonts.renderIcon(id);}
  bool Project::renderInputHandlerIcon(const std::string& id) const {return cInputHandlers.renderIcon(id);}
  bool Project::renderIntegerIcon(     const std::string& id) const {return cIntegers.renderIcon(id);}
  bool Project::renderModelIcon(       const std::string& id) const {return cModels.renderIcon(id);}
  bool Project::renderScreenIcon(      const std::string& id) const {return cScreens.renderIcon(id);}
  bool Project::renderStringIcon(      const std::string& id) const {return cStrings.renderIcon(id);}
  bool Project::renderTextureIcon(     const std::string& id) const {return cTextures.renderIcon(id);}
  bool Project::renderVertexIcon(      const std::string& id) const {return cVertices.renderIcon(id);}
    
  bool Project::isActionConfigurable(      const std::string& id) const {return cActionTypes.hasConfiguration(id);}
  bool Project::isBindingConfigurable(     const std::string& id) const {return cBindings.hasConfiguration(id);}
  bool Project::isBindingTypeConfigurable( const std::string& id) const {return cBindingTypes.hasConfiguration(id);}
  bool Project::isBooleanConfigurable(     const std::string& id) const {return cBooleans.hasConfiguration(id);}
  bool Project::isColourConfigurable(      const std::string& id) const {return cColours.hasConfiguration(id);}
  bool Project::isFloatConfigurable(       const std::string& id) const {return cFloats.hasConfiguration(id);}
  bool Project::isFontConfigurable(        const std::string& id) const {return cFonts.hasConfiguration(id);}
  bool Project::isInputHandlerConfigurable(const std::string& id) const {return cInputHandlers.hasConfiguration(id);}
  bool Project::isIntegerConfigurable(     const std::string& id) const {return cIntegers.hasConfiguration(id);}
  bool Project::isModelConfigurable(       const std::string& id) const {return cModels.hasConfiguration(id);}
  bool Project::isScreenConfigurable(      const std::string& id) const {return cScreens.hasConfiguration(id);}
  bool Project::isStringConfigurable(      const std::string& id) const {return cStrings.hasConfiguration(id);}
  bool Project::isTextureConfigurable(     const std::string& id) const {return cTextures.hasConfiguration(id);}
  bool Project::isVertexConfigurable(      const std::string& id) const {return cVertices.hasConfiguration(id);}

  IProject& Project::getProject() {
    return *this;
  }
  
  void Project::add(IAssetProvider<IResourceData, IScreen>* provider, const std::string& id, const std::string& category) {cScreens.add(provider, id, category);}
  void Project::add(IAssetProvider<IResourceData, IString>* provider, const std::string& id, const std::string& category) {cStrings.add(provider, id, category);}

  void                      Project::add(IActionType*     asset, const std::string& id, const std::string& category) {       cActionTypes.add(   asset, id, category      );}
  void                      Project::add(IBinding*        asset, const std::string& id, const std::string& category) {       cBindings.add(      asset, id, category      );}
  void                      Project::add(IBindingType*    asset, const std::string& id, const std::string& category) {       cBindingTypes.add(  asset, id, category      );}
  IStateNotifier<IBoolean>* Project::add(IBoolean*        asset, const std::string& id, const std::string& category) {return cBooleans.add(      asset, id, category, true);}
  IStateNotifier<IColour>*  Project::add(IColour*         asset, const std::string& id, const std::string& category) {return cColours.add(       asset, id, category, true);}
  void                      Project::add(IEditable*       asset, const std::string& id, const std::string& category) {       cEditables.add(     asset, id, category      );}
  IStateNotifier<IFloat>*   Project::add(IFloat*          asset, const std::string& id, const std::string& category) {return cFloats.add(        asset, id, category, true);}
  void                      Project::add(IFont*           asset, const std::string& id, const std::string& category) {       cFonts.add(         asset, id, category      );}
  void                      Project::add(IInputHandler*   asset, const std::string& id, const std::string& category) {       cInputHandlers.add( asset, id, category      );}
  IStateNotifier<IInteger>* Project::add(IInteger*        asset, const std::string& id, const std::string& category) {return cIntegers.add(      asset, id, category, true);}
  void                      Project::add(IModel*          asset, const std::string& id, const std::string& category) {       cModels.add(        asset, id, category      );}
  IScreen*                  Project::add(IScreen*         asset, const std::string& id, const std::string& category) {return cScreens.add(       asset, id, category, false, *this);}
  void                      Project::add(IProjectOptions* asset, const std::string& id, const std::string& category) {       cProjectOptions.add(asset, id, category      );}
  void                      Project::add(IAssets*         asset, const std::string& id, const std::string& category) {       cAssets.add(        asset, id, category      );}
  IStateNotifier<IString>*  Project::add(IString*         asset, const std::string& id, const std::string& category) {return cStrings.add(       asset, id, category, true);}
  IStateNotifier<ITexture>* Project::add(ITexture*        asset, const std::string& id, const std::string& category) {return cTextures.add(      asset, id, category, true);}
  IStateNotifier<IVertex>*  Project::add(IVertex*         asset, const std::string& id, const std::string& category) {return cVertices.add(      asset, id, category, true);}

  void Project::remove(IAssetProvider<IResourceData, IActionType>*     provider) {cActionTypes.remove(   provider);}
  void Project::remove(IAssetProvider<IResourceData, IBinding>*        provider) {cBindings.remove(      provider);}
  void Project::remove(IAssetProvider<IResourceData, IBindingType>*    provider) {cBindingTypes.remove(  provider);}
  void Project::remove(IAssetProvider<IResourceData, IBoolean>*        provider) {cBooleans.remove(      provider);}
  void Project::remove(IAssetProvider<IResourceData, IColour>*         provider) {cColours.remove(       provider);}
  void Project::remove(IAssetProvider<IResourceData, IEditable>*       provider) {cEditables.remove(     provider);}
  void Project::remove(IAssetProvider<IResourceData, IFloat>*          provider) {cFloats.remove(        provider);}
  void Project::remove(IAssetProvider<IResourceData, IFont>*           provider) {cFonts.remove(         provider);}
  void Project::remove(IAssetProvider<IResourceData, IInputHandler>*   provider) {cInputHandlers.remove( provider);}
  void Project::remove(IAssetProvider<IResourceData, IInteger>*        provider) {cIntegers.remove(      provider);}
  void Project::remove(IAssetProvider<IResourceData, IModel>*          provider) {cModels.remove(        provider);}
  void Project::remove(IAssetProvider<IResourceData, IScreen>*         provider) {cScreens.remove(       provider);}
  void Project::remove(IAssetProvider<IResourceData, IString>*         provider) {cStrings.remove(       provider);}
  void Project::remove(IAssetProvider<IResourceData, IProjectOptions>* provider) {cProjectOptions.remove(provider);}
  void Project::remove(IAssetProvider<IResourceData, IAssets>*         provider) {cAssets.remove(        provider);}
  void Project::remove(IAssetProvider<IResourceData, ITexture>*        provider) {cTextures.remove(      provider);}
  void Project::remove(IAssetProvider<IResourceData, IVertex>*         provider) {cVertices.remove(      provider);}

  void Project::remove(IActionType*     asset) {cActionTypes.remove(   asset);}
  void Project::remove(IBinding*        asset) {cBindings.remove(      asset);}
  void Project::remove(IBindingType*    asset) {cBindingTypes.remove(  asset);}
  void Project::remove(IBoolean*        asset) {cBooleans.remove(      asset);}
  void Project::remove(IColour*         asset) {cColours.remove(       asset);}
  void Project::remove(IEditable*       asset) {cEditables.remove(     asset);}
  void Project::remove(IFloat*          asset) {cFloats.remove(        asset);}
  void Project::remove(IFont*           asset) {cFonts.remove(         asset);}
  void Project::remove(IInputHandler*   asset) {cInputHandlers.remove( asset);}
  void Project::remove(IInteger*        asset) {cIntegers.remove(      asset);}
  void Project::remove(IModel*          asset) {cModels.remove(        asset);}
  void Project::remove(IScreen*         asset) {cScreens.remove(       asset);}
  void Project::remove(IString*         asset) {cStrings.remove(       asset);}
  void Project::remove(IProjectOptions* asset) {cProjectOptions.remove(asset);}
  void Project::remove(IAssets*         asset) {cAssets.remove(        asset);}
  void Project::remove(ITexture*        asset) {cTextures.remove(      asset);}
  void Project::remove(IVertex*         asset) {cVertices.remove(      asset);}
  
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IActionType>*     provider) {return cActionTypes.hasReadOnlyReferences(   provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IBinding>*        provider) {return cBindings.hasReadOnlyReferences(      provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IBindingType>*    provider) {return cBindingTypes.hasReadOnlyReferences(  provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IBoolean>*        provider) {return cBooleans.hasReadOnlyReferences(      provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IColour>*         provider) {return cColours.hasReadOnlyReferences(       provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IEditable>*       provider) {return cEditables.hasReadOnlyReferences(     provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IFloat>*          provider) {return cFloats.hasReadOnlyReferences(        provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IFont>*           provider) {return cFonts.hasReadOnlyReferences(         provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IInputHandler>*   provider) {return cInputHandlers.hasReadOnlyReferences( provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IInteger>*        provider) {return cIntegers.hasReadOnlyReferences(      provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IModel>*          provider) {return cModels.hasReadOnlyReferences(        provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IScreen>*         provider) {return cScreens.hasReadOnlyReferences(       provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IString>*         provider) {return cStrings.hasReadOnlyReferences(       provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IProjectOptions>* provider) {return cProjectOptions.hasReadOnlyReferences(provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IAssets>*         provider) {return cAssets.hasReadOnlyReferences(        provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, ITexture>*        provider) {return cTextures.hasReadOnlyReferences(      provider);}
  bool Project::hasReadOnlyReferences(IAssetProvider<IResourceData, IVertex>*         provider) {return cVertices.hasReadOnlyReferences(      provider);}

  bool Project::hasReadOnlyReferences(IActionType*     asset) {return cActionTypes.hasReadOnlyReferences(   asset);}
  bool Project::hasReadOnlyReferences(IBinding*        asset) {return cBindings.hasReadOnlyReferences(      asset);}
  bool Project::hasReadOnlyReferences(IBindingType*    asset) {return cBindingTypes.hasReadOnlyReferences(  asset);}
  bool Project::hasReadOnlyReferences(IBoolean*        asset) {return cBooleans.hasReadOnlyReferences(      asset);}
  bool Project::hasReadOnlyReferences(IColour*         asset) {return cColours.hasReadOnlyReferences(       asset);}
  bool Project::hasReadOnlyReferences(IEditable*       asset) {return cEditables.hasReadOnlyReferences(     asset);}
  bool Project::hasReadOnlyReferences(IFloat*          asset) {return cFloats.hasReadOnlyReferences(        asset);}
  bool Project::hasReadOnlyReferences(IFont*           asset) {return cFonts.hasReadOnlyReferences(         asset);}
  bool Project::hasReadOnlyReferences(IInputHandler*   asset) {return cInputHandlers.hasReadOnlyReferences( asset);}
  bool Project::hasReadOnlyReferences(IInteger*        asset) {return cIntegers.hasReadOnlyReferences      (asset);}
  bool Project::hasReadOnlyReferences(IModel*          asset) {return cModels.hasReadOnlyReferences(        asset);}
  bool Project::hasReadOnlyReferences(IScreen*         asset) {return cScreens.hasReadOnlyReferences(       asset);}
  bool Project::hasReadOnlyReferences(IProjectOptions* asset) {return cProjectOptions.hasReadOnlyReferences(asset);}
  bool Project::hasReadOnlyReferences(IAssets*         asset) {return cAssets.hasReadOnlyReferences(        asset);}
  bool Project::hasReadOnlyReferences(IString*         asset) {return cStrings.hasReadOnlyReferences(       asset);}
  bool Project::hasReadOnlyReferences(ITexture*        asset) {return cTextures.hasReadOnlyReferences(      asset);}
  bool Project::hasReadOnlyReferences(IVertex*         asset) {return cVertices.hasReadOnlyReferences(      asset);}
  
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IActionType>*     provider) {return cActionTypes.overrideReadOnlyReferences(   provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IBinding>*        provider) {return cBindings.overrideReadOnlyReferences(      provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IBindingType>*    provider) {return cBindingTypes.overrideReadOnlyReferences(  provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IBoolean>*        provider) {return cBooleans.overrideReadOnlyReferences(      provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IColour>*         provider) {return cColours.overrideReadOnlyReferences(       provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IEditable>*       provider) {return cEditables.overrideReadOnlyReferences(     provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IFloat>*          provider) {return cFloats.overrideReadOnlyReferences(        provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IFont>*           provider) {return cFonts.overrideReadOnlyReferences(         provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IInputHandler>*   provider) {return cInputHandlers.overrideReadOnlyReferences( provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IInteger>*        provider) {return cIntegers.overrideReadOnlyReferences(      provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IModel>*          provider) {return cModels.overrideReadOnlyReferences(        provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IScreen>*         provider) {return cScreens.overrideReadOnlyReferences(       provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IString>*         provider) {return cStrings.overrideReadOnlyReferences(       provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IProjectOptions>* provider) {return cProjectOptions.overrideReadOnlyReferences(provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IAssets>*         provider) {return cAssets.overrideReadOnlyReferences(        provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, ITexture>*        provider) {return cTextures.overrideReadOnlyReferences(      provider, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssetProvider<IResourceData, IVertex>*         provider) {return cVertices.overrideReadOnlyReferences(      provider, &cProjectFile.cFile);}

  void Project::overrideReadOnlyReferences(IActionType*     asset) {cActionTypes.overrideReadOnlyReferences(   asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IBinding*        asset) {cBindings.overrideReadOnlyReferences(      asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IBindingType*    asset) {cBindingTypes.overrideReadOnlyReferences(  asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IBoolean*        asset) {cBooleans.overrideReadOnlyReferences(      asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IColour*         asset) {cColours.overrideReadOnlyReferences(       asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IEditable*       asset) {cEditables.overrideReadOnlyReferences(     asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IFloat*          asset) {cFloats.overrideReadOnlyReferences(        asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IFont*           asset) {cFonts.overrideReadOnlyReferences(         asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IInputHandler*   asset) {cInputHandlers.overrideReadOnlyReferences( asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IInteger*        asset) {cIntegers.overrideReadOnlyReferences      (asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IModel*          asset) {cModels.overrideReadOnlyReferences(        asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IScreen*         asset) {cScreens.overrideReadOnlyReferences(       asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IProjectOptions* asset) {cProjectOptions.overrideReadOnlyReferences(asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IAssets*         asset) {cAssets.overrideReadOnlyReferences(        asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IString*         asset) {cStrings.overrideReadOnlyReferences(       asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(ITexture*        asset) {cTextures.overrideReadOnlyReferences(      asset, &cProjectFile.cFile);}
  void Project::overrideReadOnlyReferences(IVertex*         asset) {cVertices.overrideReadOnlyReferences(      asset, &cProjectFile.cFile);}
  
  void Project::init(std::function<void(IAssets&)> initialiser) {
//    std::cout << "ADDING INIT " << cInitialisers.size() << std::endl;
//     if (cInitialisers.size() == 833) {
//       std::cout << "DEBUG!" << std::endl;
//     }

    if (cResourcesLoaded) {
      throw ArgumentException("ERROR: Project::init: Resource initialisation is not allowed at this stage");
    }
    cInitialisers.push_back(initialiser);
  }

  ICallbackHandle* Project::reset(ProjectCallbackManager& manager, std::function<void()> resetter) {
    return cResetters.emplace_back(std::make_unique<ResetCallbackHandle>(resetter)).get();
  }

  ICallbackHandle* Project::updateRuntime(ProjectCallbackManager& manager, std::function<void(unsigned int)> dynamic) {
    return cRuntimeDynamics.emplace_back(std::make_unique<UpdateCallbackHandle>(dynamic)).get();
  }
  
  ICallbackHandle* Project::updateEditing(ProjectCallbackManager& manager, std::function<void(unsigned int)> dynamic) {
    return cEditingDynamics.emplace_back(std::make_unique<UpdateCallbackHandle>(dynamic)).get();
  }

  // TODO: Rename this!  It's only used for rendering to texture tasks, so should be done during rendering phase.
  void Project::updateLater(std::function<void()> task) {
    cUpdateTasks.push(task);
  }
  
  void Project::removeCallback(ICallbackHandle* callbackHandle) {
    for (unsigned int i = 0; i < cResetters.size(); i++) {
      if (cResetters[i].get() == callbackHandle) {
        cResetters.erase(cResetters.begin() + i);
        return;
      }
    }
 
    for (unsigned int i = 0; i < cRuntimeDynamics.size(); i++) {
      if (cRuntimeDynamics[i].get() == callbackHandle) {
        cRuntimeDynamics.erase(cRuntimeDynamics.begin() + i);
        return;
      }
    }
    
    for (unsigned int i = 0; i < cEditingDynamics.size(); i++) {
      if (cEditingDynamics[i].get() == callbackHandle) {
        cEditingDynamics.erase(cEditingDynamics.begin() + i);
        return;
      }
    }
    std::cout << "WARNING: Project::remove: Specified callback handle not found." << std::endl;
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

  std::string Project::getID(const IActionType*     asset) const {return cActionTypes.getID(   asset);}
  std::string Project::getID(const IAssets*         asset) const {return cAssets.getID(        asset);}
  std::string Project::getID(const IBinding*        asset) const {return cBindings.getID(      asset);}
  std::string Project::getID(const IBindingType*    asset) const {return cBindingTypes.getID(  asset);}
  std::string Project::getID(const IBoolean*        asset) const {return cBooleans.getID(      asset);}
  std::string Project::getID(const IColour*         asset) const {return cColours.getID(       asset);}
  std::string Project::getID(const IEditable*       asset) const {return cEditables.getID(     asset);}
  std::string Project::getID(const IFloat*          asset) const {return cFloats.getID(        asset);}
  std::string Project::getID(const IFont*           asset) const {return cFonts.getID(         asset);}
  std::string Project::getID(const IInputHandler*   asset) const {return cInputHandlers.getID( asset);}
  std::string Project::getID(const IInteger*        asset) const {return cIntegers.getID(      asset);}
  std::string Project::getID(const IModel*          asset) const {return cModels.getID(        asset);}
  std::string Project::getID(const IScreen*         asset) const {return cScreens.getID(       asset);}
  std::string Project::getID(const IProjectOptions* asset) const {return cProjectOptions.getID(asset);}
  std::string Project::getID(const IString*         asset) const {return cStrings.getID(       asset);}
  std::string Project::getID(const ITexture*        asset) const {return cTextures.getID(      asset);}
  std::string Project::getID(const IVertex*         asset) const {return cVertices.getID(      asset);}

  void Project::save(JSONObject object, const IActionType*     asset) const {cActionTypes.save(   object, asset);}
  void Project::save(JSONObject object, const IAssets*         asset) const {cAssets.save(        object, asset);}
  void Project::save(JSONObject object, const IBinding*        asset) const {cBindings.save(      object, asset);}
  void Project::save(JSONObject object, const IBindingType*    asset) const {cBindingTypes.save(  object, asset);}
  void Project::save(JSONObject object, const IBoolean*        asset) const {cBooleans.save(      object, asset);}
  void Project::save(JSONObject object, const IColour*         asset) const {cColours.save(       object, asset);}
  void Project::save(JSONObject object, const IEditable*       asset) const {cEditables.save(     object, asset);}
  void Project::save(JSONObject object, const IFloat*          asset) const {cFloats.save(        object, asset);}
  void Project::save(JSONObject object, const IFont*           asset) const {cFonts.save(         object, asset);}
  void Project::save(JSONObject object, const IInputHandler*   asset) const {cInputHandlers.save( object, asset);}
  void Project::save(JSONObject object, const IInteger*        asset) const {cIntegers.save(      object, asset);}
  void Project::save(JSONObject object, const IModel*          asset) const {cModels.save(        object, asset);}
  void Project::save(JSONObject object, const IScreen*         asset) const {cScreens.save(       object, asset);}
  void Project::save(JSONObject object, const IProjectOptions* asset) const {cProjectOptions.save(object, asset);}
  void Project::save(JSONObject object, const IString*         asset) const {cStrings.save(       object, asset);}
  void Project::save(JSONObject object, const ITexture*        asset) const {cTextures.save(      object, asset);}
  void Project::save(JSONObject object, const IVertex*         asset) const {cVertices.save(      object, asset);}

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

  Project::ProjectProperty::ProjectProperty(Project& parent, JSONObject object, File* ownerProject) :
            cChangeAction(parent),
            cOwnerProject(ownerProject) {
    cChangeAction.init(object, JSON_ACTION, &parent);
  }

  void Project::ProjectProperty::setValue(const std::string& value) {
    cChangeAction.execute();
  }

  void Project::ProjectProperty::save(JSONArray& array, File* savingProject, const std::string& id) const {
    if (savingProject == cOwnerProject) {
      JSONObject mPropertyObject = array.addObject();
      mPropertyObject.addString(JSON_ID, id);
      cChangeAction.save(mPropertyObject, JSON_ACTION);
    }
  }

  bool Project::ProjectProperty::isOwnedBy(File* project) {
    return project == cOwnerProject;
  }

  std::vector<std::unique_ptr<IProperty>> Project::getProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyStruct>("App Modules", "View or change the modules used by this app", "Edit...", [this]() {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      unsigned int mIndex = 1;
      for (const std::unique_ptr<Module>& mModule : cModules) {
        mProperties.emplace_back(std::make_unique<PropertyStruct>("Module \"" + mModule->getName() + "\"", "TODO", "Edit...", [this, &mModule]() {
          return mModule->getProperties();
        }, [this, &mModule]() {
          Utils::removeElementUnique(cModules, mModule.get());
        }));
        mIndex++;
      }
      mProperties.emplace_back(std::make_unique<PropertyOptional<ModuleChooser>>("Module " + Utils::toString(static_cast<int>(cModules.size() + 1)), "TODO", [this](const std::string& value) {
        loadModule(value);
      }, *this, cApplication));
      return mProperties;
    }));
    mProperties.emplace_back(std::make_unique<PropertyStruct>("App File Structure", "View or configure the files that make up this app", "Edit...", [this]() {
      return cProjectFile.getProperties(*this);
    }));
    mProperties.emplace_back(cDefInitAction.getProperty("On Initialisation"));
    mProperties.emplace_back(cDefResetAction.getProperty("On Reset"));
    mProperties.emplace_back(cDefStartAction.getProperty("On Start"));
    mProperties.emplace_back(cDefQuitAction.getProperty("On Quit"));
    mProperties.emplace_back(cDefInputHandler.getProperty("Input Handler"));
    mProperties.emplace_back(cDefScreen.getProperty("Display"));
    mProperties.emplace_back(cDefDefaultEditor.getProperty("Default Editor"));
    return mProperties;
  }
  
  void Project::setProperty(const std::string& property, const std::string& value) {
    std::map<std::string, std::unique_ptr<ProjectProperty>>::iterator mResult = cProperties.find(property);
    if (mResult != cProperties.end()) {
      cPropertyValue.setValue(value);
      mResult->second->setValue(value);
    }
  }

  IBinding* Project::getBinding(const std::string& id) {
    return id == "value" ? &cPropertyValueBinding : nullptr;
  }

  void Project::saveBinding(JSONObject object, const IBinding* binding) const {
    if (binding == &cPropertyValueBinding) {
      object.addString(JSON_LOCAL, "value");
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

  Project::Filename::Filename(Project& parent) :
            cParent(parent) {
  }

  std::string Project::Filename::getValue() const {
    return cParent.cProjectFile.cFile.getRelativePath();
  }

  bool Project::Filename::renderAssetIcon() const {
    return false;
  }

  void Project::Filename::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Project::Filename::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Project::Filename::isDefaultConfiguration() const {
    return true;
  }

  Project::FileUser::FileUser(Project& parent) :
            cParent(parent) {
  }

  bool Project::FileUser::getValue() const {
    return cParent.cProjectFile.cFile.isUser();
  }

  bool Project::FileUser::renderAssetIcon() const {
    return false;
  }

  void Project::FileUser::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Project::FileUser::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Project::FileUser::isDefaultConfiguration() const {
    return true;
  }

  Project::QuitActionType::QuitActionType(Project& parent) :
            cParent(parent),
            cQuitAction(*this) {
  }

  IAction* Project::QuitActionType::createAction(JSONObject object, IResourceData& owner, IBindingRegistry* localObjects) {
    return &cQuitAction;
  }

  IAction* Project::QuitActionType::createAction(IResourceData& owner, IBindingRegistry* localObjects) {
    return &cQuitAction;
  }

  void Project::QuitActionType::destroyAction(IAction* action, IAssets& assets) {
    // Nothing to do.
  }

  bool Project::QuitActionType::renderAssetIcon() const {
    return false;
  }

  void Project::QuitActionType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Project::QuitActionType::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Project::QuitActionType::isDefaultConfiguration() const {
    return true;
  }

  Project::QuitActionType::QuitAction::QuitAction(QuitActionType& parent) :
            cParent(parent) {
  }

  void Project::QuitActionType::QuitAction::save(JSONObject object) const {
    // Nothing to do.
  }

  void Project::QuitActionType::QuitAction::execute() {
    cParent.cParent.finish(true);
  }

  bool Project::QuitActionType::QuitAction::hasConfiguration() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> Project::QuitActionType::QuitAction::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    return mProperties;
  }
  
  bool Project::QuitActionType::QuitAction::isDefaultConfiguration() const {
    return true;
  }

  Project::~Project() {
    remove(&cLuaBinding);
    remove(&cFilenameString);
    remove(&cFileUserBoolean);
    remove(&cQuitAction);

    cBindings.remove(&cConversionProviderActionToBinding);
    cBindings.remove(&cConversionProviderBooleanToBinding);
    cBindings.remove(&cConversionProviderColourToBinding);
    cBindings.remove(&cConversionProviderFloatToBinding);
    cBindings.remove(&cConversionProviderFontToBinding);
    cBindings.remove(&cConversionProviderInputHandlerToBinding);
    cBindings.remove(&cConversionProviderIntegerToBinding);
    cBindings.remove(&cConversionProviderProjectOptionsToBinding);
    cBindings.remove(&cConversionProviderProjectToBinding);
    cBindings.remove(&cConversionProviderScreenToBinding);
    cBindings.remove(&cConversionProviderStringToBinding);
    cBindings.remove(&cConversionProviderVertexToBinding);

    cStrings.remove(&cConversionProviderIntegerToString);
    cStrings.remove(&cConversionProviderFloatToString);
    cStrings.remove(&cConversionProviderProjectToString);

    cIntegers.remove(&cConversionProviderProjectToInteger);

    cBindings.remove(&cLocalProviderBinding);


//     cModels.checkClean("Models");
//     if (!cActionExecutors.empty()) {
//       std::cout << "WARNING: " << cActionExecutors.size() << " Actions not released.  This is a bug." << std::endl;
//     }
//     cActionTypes.checkClean("Action Types");
//     cBindings.checkClean("Bindings");
//     cBooleans.checkClean("Booleans");
//     cColours.checkClean("Colours");
//     cEditables.checkClean("Editables");
//     cFloats.checkClean("Floats");
//     cFonts.checkClean("Fonts");
//     cInputHandlers.checkClean("Input Handlers");
//     cIntegers.checkClean("Integers");
//     cScreens.checkClean("Screens");
//     cProjectOptions.checkClean("Project Options");
//     cAssets.checkClean("Asset Collections");
//     cStrings.checkClean("Strings");
//     cTextures.checkClean("Textures");
//     cVertices.checkClean("Vertices");
//     if (!cResetters.empty()) {
//       std::cout << "WARNING: " << cResetters.size() << " reset actions not released.  This is a bug." << std::endl;
//     }
//     if (!cRuntimeDynamics.empty()) {
//       std::cout << "WARNING: " << cRuntimeDynamics.size() << " runtime dynamics not released.  This is a bug." << std::endl;
//     }
//     if (!cEditingDynamics.empty()) {
//       std::cout << "WARNING: " << cEditingDynamics.size() << " editor dynamics not released.  This is a bug." << std::endl;
//     }
  }
}

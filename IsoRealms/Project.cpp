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
#include "IAssetOverride.h"
#include "Module.h"
#include "PropertyData.h"
#include "PropertyMaker.h"

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
          cActions(&cLiteralProviderAction),
          cBindings(&cLiteralProviderBinding),
          cBindingTypes(&cLiteralProviderBindingType, "Any"),
          cBooleans(&cLiteralProviderBoolean, "Literal", "false"),
          cColours(&cLiteralProviderColour, "Literal", "0.0 0.0 0.0 1.0"),
          cEditables(&cLiteralProviderEditable),
          cFloats(&cLiteralProviderFloat, "Literal", "0.0"),
          cFonts(&cLiteralProviderFont),
          cInputHandlers(&cLiteralProviderInputHandler),
          cIntegers(&cLiteralProviderInteger, "Literal", "0"),
          cModels(&cLiteralProviderModel),
          cScreens(&cLiteralProviderScreen, *this),
          cProjectOptions(&cLiteralProviderProjectOptions),
          cAssets(&cLiteralProviderAssets),
          cStrings(&cLiteralProviderString, "Literal"),
          cTextures(&cLiteralProviderTexture),
          cVertices(&cLiteralProviderVertex, "Literal", "0.0 0.0 0.0"),
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
          cConversionProviderActionToBinding(*this, *this),
          cConversionProviderBooleanToBinding(*this, *this),
          cConversionProviderColourToBinding(*this, *this),
          cConversionProviderFloatToBinding(*this, *this),
          cConversionProviderFontToBinding(*this, *this),
          cConversionProviderInputHandlerToBinding(*this, *this),
          cConversionProviderIntegerToBinding(*this, *this),
          cConversionProviderProjectOptionsToBinding(*this, *this),
          cConversionProviderProjectToBinding(*this, *this),
          cConversionProviderScreenToBinding(*this, *this),
          cConversionProviderStringToBinding(*this, *this),
          cConversionProviderVertexToBinding(*this, *this),
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
          cPropertyValueBinding(*this, &cPropertyValue, this),
          cMissingData("TODO: Name", "TODO: Description") {

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
    add<IBinding, IBinding>(&cLuaBinding,      "Project",         "System");
    add<IString,  IString> (&cFilenameString,  "ProjectFilename", "System");
    add<IBoolean, IBoolean>(&cFileUserBoolean, "ProjectUser",     "System");
    add<IAction,  IAction> (&cQuitAction,      "Quit",            "System");
  }

  Project::Project(IApplication& application, IOptions& options, std::function<void(bool)> onFinish, IAssetOverride* override) :
            Project(application, onFinish, override) {
    cLoading = true;
    std::string mFile = options.getOption("file");
    if (!mFile.empty()) {
      cProcessingInput = true;

//       std::cout << "TYPE: " << options.getOption("type") << std::endl;
      bool mUser = options.getOption("type") == "user";
      cProjectFile.rename(mFile, mUser);

      // Load modules and any resources declared within them
      std::vector<std::unique_ptr<JSONDocument>> mOpenedDocuments = loadResources(options, cProjectFile);
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
      // for (std::pair<IScreen* const, std::unique_ptr<ScreenProxy>>& mPair : cScreenProxyMapping) {
      //   for (IScreenListener* mListener : cScreenListeners) {
      //     mListener->screenAdded(project, mPair.second.get());
      //   }
      // }

      cProcessingInput = false;

      mainThreadInit([this]() {
        (*cDefInitAction)->execute();
      });
    } else {
      cResourcesLoaded = true;
    }
    cLoading = false;
  }

  Project::~Project() {
    // remove(&cLuaBinding);
    // remove(&cFilenameString);
    // remove(&cFileUserBoolean);
    // remove(&cQuitAction);
    //
    // cBindings.remove(&cConversionProviderActionToBinding);
    // cBindings.remove(&cConversionProviderBooleanToBinding);
    // cBindings.remove(&cConversionProviderColourToBinding);
    // cBindings.remove(&cConversionProviderFloatToBinding);
    // cBindings.remove(&cConversionProviderFontToBinding);
    // cBindings.remove(&cConversionProviderInputHandlerToBinding);
    // cBindings.remove(&cConversionProviderIntegerToBinding);
    // cBindings.remove(&cConversionProviderProjectOptionsToBinding);
    // cBindings.remove(&cConversionProviderProjectToBinding);
    // cBindings.remove(&cConversionProviderScreenToBinding);
    // cBindings.remove(&cConversionProviderStringToBinding);
    // cBindings.remove(&cConversionProviderVertexToBinding);
    //
    // cStrings.remove(&cConversionProviderIntegerToString);
    // cStrings.remove(&cConversionProviderFloatToString);
    // cStrings.remove(&cConversionProviderProjectToString);
    //
    // cIntegers.remove(&cConversionProviderProjectToInteger);
    //
    // cBindings.remove(&cLocalProviderBinding);


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
//     cProjectOptions.checkClean("Project Options");
//     cAssets.checkClean("Asset Collections");
//     cStrings.checkClean("Strings");
//     cTextures.checkClean("Textures");
//     cVertices.checkClean("Vertices");
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

  bool Project::isModuleLoaded(const std::string& name) const {
    for (const std::unique_ptr<Module>& mModule : cModules) {
      if (mModule->getName() == name) {
        return true;
      }
    }
    return false;
  }
  
  std::vector<std::unique_ptr<JSONDocument>> Project::loadResources(IOptions& options, ProjectFile& file) {
    std::unique_ptr<JSONDocument> mProjectDocument = std::make_unique<JSONDocument>(file.cFile);
    JSONObject mProjectObject = mProjectDocument->getObject(JSON_PROJECT);
    std::vector<std::unique_ptr<JSONDocument>> mOpenedDocuments;
    mOpenedDocuments.emplace_back(std::move(mProjectDocument));

    cDefScreen.init(mProjectObject, JSON_SCREEN, &file.cFile);
    cDefInputHandler.init(mProjectObject, JSON_INPUT, &file.cFile);
    cDefInitAction.init(mProjectObject, JSON_INITIALISATION, &file.cFile);
    cDefResetAction.init(mProjectObject, JSON_RESET, &file.cFile);
    cDefStartAction.init(mProjectObject, JSON_START, &file.cFile);
    cDefQuitAction.init(mProjectObject, JSON_QUIT, &file.cFile);
    cDefDefaultEditor.init(mProjectObject, JSON_EDITOR, &file.cFile);

    for (JSONObject mModuleObject : mProjectObject.getArray(JSON_MODULES)) {
      std::string mModuleName = mModuleObject.getString(JSON_NAME);
      getModule(mModuleName);
    }

    if (mProjectObject.hasMember(JSON_PROPERTIES)) {
      for (JSONObject mPropertyObject : mProjectObject.getArray(JSON_PROPERTIES)) {
        std::string mPropertyID = mPropertyObject.getString(JSON_ID);
        if (cProperties.find(mPropertyID) == cProperties.end()) {
          cProperties.emplace(mPropertyID, std::make_unique<ProjectProperty>(*this, mPropertyObject, &file.cFile));
        }
      }
    }

    for (JSONObject mModuleObject : mProjectObject.getArray(JSON_MODULES)) {
      std::string mModuleName = mModuleObject.getString(JSON_NAME);
      Module* mModule = getModule(mModuleName);
      LocalOptions mModuleOptions(mModuleName, options);
      mModule->loadResources(mModuleObject, mModuleOptions, &file);
    }

    for (JSONObject mIncludeObject : mProjectObject.getArray(JSON_INCLUDE)) {
      ProjectFile* mIncludedProject = file.cInclusions.emplace_back(std::make_unique<ProjectFile>(*this, mIncludeObject)).get();
      std::vector<std::unique_ptr<JSONDocument>> mMoreOpenedDocuments = loadResources(options, *mIncludedProject);
      mOpenedDocuments.insert(mOpenedDocuments.end(), std::make_move_iterator(mMoreOpenedDocuments.begin()), std::make_move_iterator(mMoreOpenedDocuments.end()));
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
    
    for (const std::unique_ptr<Module>& mModule : cModules) {
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

    for (const std::unique_ptr<Module>& mModule : cModules) {
      mModule->updateEditing(milliseconds);
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
      for (const std::unique_ptr<Module>& mModule : cModules) {
        mModule->reset();
      }
      (*cDefResetAction)->execute();
      postponeAction(***cDefStartAction);
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
    if (file.cFile.isSet() && file.cFile.isUser()) {
      JSONDocument mJSONDocument;
      JSONObject mProjectObject = mJSONDocument.addObject(JSON_PROJECT);
      JSONArray mIncludeArray = mProjectObject.addArray(JSON_INCLUDE);
      file.save(mIncludeArray);

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
    save(cProjectFile);
  }

  void Project::save(const std::string& filename) {
    cProjectFile.cFile.setPath(filename, true);
    save();
  }

  bool Project::isUserProject() {
    return cProjectFile.cFile.isUser();
  }

  std::string Project::getFilename() {
    return cProjectFile.cFile.getPath();
  }
  
  Project& Project::getAssetManager() {
    return *this;
  }

  IActionClient& Project::getDummyActionClient() {
    return *this;
  }

  const PropertyData& Project::getPropertyData(const std::string& key) const {
    return cMissingData;
  }

  std::string Project::getPropertyName(const std::string& key) const {
    return cMissingData.getName();
  }

  std::string Project::getPropertyDescription(const std::string& key) const {
    return cMissingData.getTooltip();
  }

  std::unique_ptr<IProperty> Project::createPropertyNativeFloat(const std::string& metadataKey, std::function<float()> getter, std::function<bool(float)> setter, std::function<void()> removeFunction) {
    return nullptr; // TODO
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
    return &cProjectFile;
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

  IBoolean* Project::createLiteralBoolean(IAssetUser<IBoolean>* user, IResourceData& owner, const bool value)                                                        {return cBooleans.literal(user, owner, value ? std::string("true") : std::string("false"));}
  IColour*  Project::createLiteralColour( IAssetUser<IColour>*  user, IResourceData& owner, const float red, const float green, const float blue, const float alpha) {return cColours.literal( user, owner, Utils::toString(red) + " " + Utils::toString(green) + " " + Utils::toString(blue) + " " + Utils::toString(alpha));}
  IFloat*   Project::createLiteralFloat(  IAssetUser<IFloat>*   user, IResourceData& owner, const float value)                                                       {return cFloats.literal(  user, owner, Utils::toString(value));}
  IInteger* Project::createLiteralInteger(IAssetUser<IInteger>* user, IResourceData& owner, const int value)                                                         {return cIntegers.literal(user, owner, Utils::toString(value));}
  IString*  Project::createLiteralString( IAssetUser<IString>*  user, IResourceData& owner, const std::string& value)                                                {return cStrings.literal( user, owner, value);}
  IVertex*  Project::createLiteralVertex( IAssetUser<IVertex>*  user, IResourceData& owner, const float x, const float y, const float z)                             {return cVertices.literal(user, owner, Utils::toString(x) + " " + Utils::toString(y) + " " + Utils::toString(z));}

  IAction*         Project::getAction(          IAssetUser<IAction>*         user, JSONObject object, IActionClient& owner,                                      bool required) {return cActions.get(          user, owner, object, nullptr,  required, [this](JSONObject object, IStateListener<IAction*>*         listener) -> IAction*         {return cAssetOverride != nullptr ? cAssetOverride->getAction(          object, listener) : nullptr;});}
  IAssets*         Project::getAssets(          IAssetUser<IAssets>*         user, JSONObject object, IResourceData& owner,                                      bool required) {return cAssets.get(           user, owner, object, nullptr,  required, [this](JSONObject object, IStateListener<IAssets*>*         listener) -> IAssets*         {return cAssetOverride != nullptr ? cAssetOverride->getAssets(          object, listener) : nullptr;});}
  IBinding*        Project::getBinding(         IAssetUser<IBinding>*        user, JSONObject object, IActionClient& owner,                                      bool required) {
    cLocalProviderBinding.setLocalBindings(owner.getBindingRegistry());
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

  IAction*         Project::getAction(        IAssetUser<IAction>*         user, const std::string& id, IActionClient& owner)                                      {return cActions.get(       user, owner, id, nullptr);}
  IAssets*         Project::getAssets(        IAssetUser<IAssets>*         user, const std::string& id, IResourceData& owner)                                      {return cAssets.get(        user, owner, id, nullptr);}
  IBinding*        Project::getBinding(       IAssetUser<IBinding>*        user, const std::string& id, IActionClient& owner)                                      {return cBindings.get(      user, owner, id, nullptr);}
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

  bool Project::renderAssetIcon() const {
    return false;
  }

  void Project::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Project::getAssetProperties(IPropertyOwner& owner) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Project::isDefaultConfiguration() const {
    return true;
  }

  Project& Project::getProject() {
    return *this;
  }
  
  void Project::init(std::function<void(IAssets&)> initialiser) {
//    std::cout << "ADDING INIT " << cInitialisers.size() << std::endl;
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

  Project::ProjectProperty::ProjectProperty(Project& parent, JSONObject object, File* ownerProject) :
            cChangeAction(parent),
            cOwnerProject(ownerProject) {
    cChangeAction.init(object, JSON_ACTION);
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
    PropertyMaker mPropertyMaker(*this, *this);
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyStruct>(mPropertyMaker, PropertyData("TODO: App Modules", "View or change the modules used by this app"), "Edit...", [this, &mPropertyMaker]() {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      unsigned int mIndex = 1;
      for (const std::unique_ptr<Module>& mModule : cModules) {
        mProperties.emplace_back(std::make_unique<PropertyStruct>(mPropertyMaker, PropertyData("Module \"" + mModule->getName() + "\"", "TODO"), "Edit...", [this, &mModule]() {
          return mModule->getProperties();
        }, [this, &mModule]() {
          Utils::removeElementUnique(cModules, mModule.get());
        }));
        mIndex++;
      }
      mProperties.emplace_back(std::make_unique<PropertyOptional<ModuleChooser>>(mPropertyMaker, PropertyData("Module " + Utils::toString(static_cast<int>(cModules.size() + 1)), "TODO"), [this](const std::string& value) {
        loadModule(value);
      }, *this, cApplication));
      return mProperties;
    }));
    mProperties.emplace_back(std::make_unique<PropertyStruct>(mPropertyMaker, PropertyData("TODO: App File Structure", "View or configure the files that make up this app"), "Edit...", [this, &mPropertyMaker]() {
      return cProjectFile.getProperties(mPropertyMaker, *this, false);
    }));
    mProperties.emplace_back(cDefInitAction.getProperty(mPropertyMaker, "On Initialisation"));
    mProperties.emplace_back(cDefResetAction.getProperty(mPropertyMaker, "On Reset"));
    mProperties.emplace_back(cDefStartAction.getProperty(mPropertyMaker, "On Start"));
    mProperties.emplace_back(cDefQuitAction.getProperty(mPropertyMaker, "On Quit"));
    mProperties.emplace_back(cDefInputHandler.getProperty(mPropertyMaker, "Input Handler"));
    mProperties.emplace_back(cDefScreen.getProperty(mPropertyMaker, "Display"));
    mProperties.emplace_back(cDefDefaultEditor.getProperty(mPropertyMaker, "Default Editor"));
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

  PropertyMaker Project::getPropertyMaker() {
    return PropertyMaker(*this, *this);
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

  std::vector<std::unique_ptr<IProperty>> Project::Filename::getAssetProperties(IPropertyOwner& owner) {
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

  std::vector<std::unique_ptr<IProperty>> Project::FileUser::getAssetProperties(IPropertyOwner& owner) {
    return std::vector<std::unique_ptr<IProperty>>();
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

  std::vector<std::unique_ptr<IProperty>> Project::QuitAction::getAssetProperties(IPropertyOwner& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    return mProperties;
  }
  
  bool Project::QuitAction::isDefaultConfiguration() const {
    return true;
  }
}

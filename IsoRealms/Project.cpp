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
  const std::string Project::TAG_ACTION       = "Action";
  const std::string Project::TAG_EDITOR       = "Editor";
  const std::string Project::TAG_INCLUDE      = "Include";
  const std::string Project::TAG_INIT_ACTION  = "InitAction";
  const std::string Project::TAG_INPUT        = "Input";
  const std::string Project::TAG_MODULE       = "Module";
  const std::string Project::TAG_MODULES      = "Modules";
  const std::string Project::TAG_PROJECT      = "Project";
  const std::string Project::TAG_PROPERTIES   = "Properties";
  const std::string Project::TAG_PROPERTY     = "Property";
  const std::string Project::TAG_QUIT_ACTION  = "QuitAction";
  const std::string Project::TAG_RESET_ACTION = "ResetAction";
  const std::string Project::TAG_SCREEN       = "Screen";

  const std::string Project::ATTRIBUTE_ID     = "id";
  const std::string Project::ATTRIBUTE_NAME   = "name";
  const std::string Project::ATTRIBUTE_USER   = "user";
  
  const std::string Project::CATEGORY_CONVERSIONS = "Conversions";
  const std::string Project::CATEGORY_FIXED       = "Fixed";
  const std::string Project::CATEGORY_LOCAL       = "Local";

  Project::Project(IApplication* application, std::function<void(bool)> onFinish, IAssetOverride* override) :
          cApplication(application),
          cAssetOverride(override),
          cLuaBinding(this, this),
          cProcessingInput(false),
          cRuntimeUpdatingRuntime(false),
          cRuntimeResetPostponed(false),
          c3DModelTypes(&cLiteralProviderModel),
          cActionTypes(&cLiteralProviderActionType),
          cBindings(&cLiteralProviderBinding),
          cBooleans(&cLiteralProviderBoolean),
          cColours(&cLiteralProviderColour),
          cEditables(&cLiteralProviderEditable),
          cFloats(&cLiteralProviderFloat),
          cFonts(&cLiteralProviderFont),
          cInputHandlers(&cLiteralProviderInputHandler),
          cIntegers(&cLiteralProviderInteger),
          cScreens(&cLiteralProviderScreen),
          cProjectOptions(&cLiteralProviderProjectOptions),
          cAssets(&cLiteralProviderAssets),
          cStrings(&cLiteralProviderString),
          cTextures(&cLiteralProviderTexture),
          cVertices(&cLiteralProviderVertex),
          cConversionProviderActionToBinding(this),
          cConversionProviderBooleanToBinding(this),
          cConversionProviderColourToBinding(this),
          cConversionProviderFloatToBinding(this),
          cConversionProviderFontToBinding(this),
          cConversionProviderInputHandlerToBinding(this),
          cConversionProviderIntegerToBinding(this),
          cConversionProviderProjectOptionsToBinding(this),
          cConversionProviderProjectToBinding(this),
          cConversionProviderScreenToBinding(this),
          cConversionProviderStringToBinding(this),
          cConversionProviderVertexToBinding(this),
          cConversionProviderString(this),
          cConversionProviderProjectToInteger(this),
          cConversionProviderProjectToString(this),
          cFunctionNotifyComplete(onFinish),
          cFilenameString(this),
          cFileUserBoolean(this),
          cQuitAction(this),
          cDefInputHandler(this),
          cDefScreen(this),
          cDefDefaultEditor(this),
          cDefInitAction(this),
          cDefResetAction(this),
          cDefQuitAction(this),
          cPropertyValue(""),
          cPropertyValueBinding(this, &cPropertyValue, this) {

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

    cStrings.add( &cConversionProviderString, ":Integer", CATEGORY_CONVERSIONS);

    cIntegers.add(&cConversionProviderProjectToInteger, ":Project", CATEGORY_CONVERSIONS);
    cStrings.add(&cConversionProviderProjectToString, ":Project", CATEGORY_CONVERSIONS);

    // Support locals
    cBindings.add(&cLocalProviderBinding, "~", CATEGORY_LOCAL);

    // Project singletons.    
    add(&cLuaBinding,      "Project",         "System");
    add(&cFilenameString,  "ProjectFilename", "System");
    add(&cFileUserBoolean, "ProjectUser",     "System");
    add(&cQuitAction,      "Quit",            "System");
  }

  Project::Project(IApplication* application, IOptions* options, std::function<void(bool)> onFinish, IAssetOverride* override) :
            Project(application, onFinish, override) {
    std::string mFile = options->getOption("file");
    if (!mFile.empty()) {
      cProcessingInput = true;

//       std::cout << "TYPE: " << options->getOption("type") << std::endl;
      bool mUser = options->getOption("type") == "user";

      std::size_t mExtensionPosition = mFile.find_last_of('.');
      cProjectDataPath               = mFile.substr(0, mExtensionPosition);
      cFilename                      = mFile;
      cCanSave                       = mUser;

      // Open Project File and Node
      DOMNode mConfigurationRootNode(mFile, mUser ? DOMNode::Type::USER : DOMNode::Type::PROGRAM);
      DOMNode& mProjectNode = mConfigurationRootNode.getNode("Project");

      // Load modules and any resources declared within them
      std::vector<std::unique_ptr<DOMNode>> mIncludeNodes = loadResources(mProjectNode, options, cProjectDataPath);
      for (const std::unique_ptr<Module>& mModule : cModules) {
        mModule->registerAssets();
      }

      // Initialise everything
      // Copy the vector because more initialisers might be added from calling the current ones!
      while (!cInitialisers.empty()) {
        std::vector<std::function<void(IAssets*)>> mCurrentInitialisers = cInitialisers;
        cInitialisers.clear();
        for (unsigned int j = 0; j < mCurrentInitialisers.size(); j++) {
//           std::cout << "INIT " << j << " OF " << mCurrentInitialisers.size() << std::endl;
//           if (j == 360) {
//             std::cout << "DEBUG!" << std::endl;
//           }
          mCurrentInitialisers[j](this);
        }
      }

      // Screen listeners cannot notified of screens before initialisation, so we need to do it now.
      for (std::pair<IScreen* const, std::unique_ptr<ScreenProxy>>& mPair : cScreenProxyMapping) {
        for (IScreenListener* mListener : cListeners) {
          mListener->screenAdded(this, mPair.second.get());
        }
      }

      cProcessingInput = false;

      mainThreadInit([this]() {
        (*cDefInitAction)->execute();
      });
    }
  }

  Module* Project::getModule(const std::string& name) {
    for (const std::unique_ptr<Module>& mModule : cModules) {
      if (mModule->getName() == name) {
        return mModule.get();
      }
    }
    return nullptr;
  }

  std::vector<std::unique_ptr<DOMNode>> Project::loadResources(DOMNode& node, IOptions* options, const std::string& resourceDataPath) {

    // Set screen and input handler
    std::cout << "Loading things with resource path: " << resourceDataPath << std::endl;
    cDefScreen.init(node, TAG_SCREEN, resourceDataPath);
    cDefInputHandler.init(node, TAG_INPUT, resourceDataPath);
    cDefInitAction.init(node, TAG_INIT_ACTION, resourceDataPath);
    cDefResetAction.init(node, TAG_RESET_ACTION, resourceDataPath);
    cDefQuitAction.init(node, TAG_QUIT_ACTION, resourceDataPath);
    cDefDefaultEditor.init(node, TAG_EDITOR, resourceDataPath);
    std::cout << "Finished loading things with resource path: " << resourceDataPath << std::endl;

    if (node.containsNode(TAG_PROPERTIES)) {
      for (DOMNode& mPropertyNode : node.getNode(TAG_PROPERTIES)) {
        std::string mChildName = mPropertyNode.getName();
        if (mChildName == TAG_PROPERTY) {
          std::string mPropertyID = mPropertyNode.getAttribute(ATTRIBUTE_ID);
          if (cProperties.find(mPropertyID) == cProperties.end()) {
            cProperties.emplace(mPropertyID, std::make_unique<ProjectProperty>(this, mPropertyNode, resourceDataPath));
          }
        } else {
          throw ParseException("Unknown tag for Project: " + mChildName);
        }
      }
    }

    for (DOMNode& mModuleNode : node.getNode(TAG_MODULES)) {
      std::string mChildName = mModuleNode.getName();
      if (mChildName == TAG_MODULE) {
        std::string mModuleName = mModuleNode.getAttribute(ATTRIBUTE_NAME);
        Module* mModule = getModule(mModuleName);
        if (mModule == nullptr) {
          loadModule(mModuleName);
          mModule = getModule(mModuleName);
        }
        LocalOptions mModuleOptions(mModuleName, options);
        mModule->loadResources(mModuleNode, &mModuleOptions, resourceDataPath);
      } else {
        throw ParseException("Unknown tag for Project: " + mChildName);
      }
    }
    
    std::vector<std::unique_ptr<DOMNode>> mIncludeNodes;
    if (node.containsNode(TAG_INCLUDE)) {
      for (DOMNode& mNode : node.getNode(TAG_INCLUDE)) {
        std::string mChildName = mNode.getName();
        if (mChildName == TAG_PROJECT) {
          std::string mName = mNode.getAttribute(ATTRIBUTE_NAME);
          bool mUser = mNode.getBooleanAttribute(ATTRIBUTE_USER);

          std::size_t mExtensionPosition = mName.find_last_of('.');
          std::string mIncludeDataPath   = mName.substr(0, mExtensionPosition);

          std::unique_ptr<DOMNode> mConfigurationRootNode = std::make_unique<DOMNode>(mName, mUser ? DOMNode::Type::USER : DOMNode::Type::PROGRAM);
          DOMNode& mProjectNode = mConfigurationRootNode->getNode("Project");
          std::vector<std::unique_ptr<DOMNode>> mSubIncludeNodes = loadResources(mProjectNode, options, mIncludeDataPath);
          mIncludeNodes.insert(mIncludeNodes.end(), std::make_move_iterator(mSubIncludeNodes.begin()), std::make_move_iterator(mSubIncludeNodes.end()));
          mIncludeNodes.emplace_back(std::move(mConfigurationRootNode));

          if (resourceDataPath == cProjectDataPath) {
            cInclusions.emplace_back(std::make_unique<Include>());
            cInclusions.back()->cProject = mName;
            cInclusions.back()->cUser = mUser;
          }
        } else {
          throw ParseException("Unknown tag for Project: " + mChildName);
        }
      }
    }
    return mIncludeNodes;
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
    return cApplication->isFullScreen();
  }
    
  DisplayResolution Project::getDisplayResolution() {
    return cApplication->getDisplayResolution();
  }
    
  void Project::setDisplayResolution(DisplayResolution mode, bool fullScreen) {
    cApplication->setDisplayResolution(mode, fullScreen);
  }

  void Project::reset() {
    if (cRuntimeUpdatingRuntime) {
      cRuntimeResetPostponed = true;
    } else {
      for (std::unique_ptr<ResetCallbackHandle>& mResetter : cResetters) {
        mResetter->call();
      }
      (*cDefResetAction)->execute();
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
    std::string mPath = System::getPath(cFilename, cCanSave);
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
    return cCanSave;
  }

  void Project::save() {
    if (!cFilename.empty() && cCanSave) {
      DOMNodeWriter mProjectNode(TAG_PROJECT);

      // Save return values and projects
      if (!cInclusions.empty()) {
        DOMNodeWriter mInclusionNode = mProjectNode.addBranch(TAG_INCLUDE);
        for (std::unique_ptr<Include>& mInclusion : cInclusions) {
          DOMNodeWriter mIncludedProjectNode = mInclusionNode.addBranch(TAG_PROJECT);
          mIncludedProjectNode.addAttribute(ATTRIBUTE_NAME,  mInclusion->cProject);
          mIncludedProjectNode.addAttribute(ATTRIBUTE_USER,  mInclusion->cUser);
        }
      }

      // Save project used assets
      cDefScreen.save(&mProjectNode, TAG_SCREEN, cProjectDataPath);
      cDefInputHandler.save(&mProjectNode, TAG_INPUT, cProjectDataPath);
      cDefDefaultEditor.save(&mProjectNode, TAG_EDITOR, cProjectDataPath);
      cDefInitAction.save(&mProjectNode, TAG_INIT_ACTION, cProjectDataPath);
      cDefResetAction.save(&mProjectNode, TAG_RESET_ACTION, cProjectDataPath);
      cDefQuitAction.save(&mProjectNode, TAG_QUIT_ACTION, cProjectDataPath);

      // Save properties
      if (!cProperties.empty()) {
        bool mNeedsSaving = false;
        for (std::pair<const std::string, std::unique_ptr<ProjectProperty>>& mPair : cProperties) {
          if (mPair.second->isThisProject(cProjectDataPath)) {
            mNeedsSaving = true;
            break;
          }
        }

        if (mNeedsSaving) {
          DOMNodeWriter mPropertiesNode = mProjectNode.addBranch(TAG_PROPERTIES);
          for (std::pair<const std::string, std::unique_ptr<ProjectProperty>>& mPair : cProperties) {
            mPair.second->save(mPropertiesNode, cProjectDataPath, mPair.first);
          }
        }
      }
      
      // Save modules
      DOMNodeWriter mModulesNode = mProjectNode.addBranch(TAG_MODULES);
      for (const std::unique_ptr<Module>& mModule : cModules) {
        if (mModule->needsSaving()) {
          DOMNodeWriter mModuleNode = mModulesNode.addBranch(TAG_MODULE);
          mModule->save(&mModuleNode, this, cProjectDataPath);
        }
      }
      mProjectNode.save(cFilename);
    }
  }

  void Project::save(const std::string& filename) {
    cFilename = filename;
    cCanSave = true;
    save();
  }

  bool Project::isUserProject() {
    return cCanSave;
  }

  IAssetBrowser* Project::getResourceManager() {
    return this;
  }

  std::string Project::getFilename() {
    return cFilename;
  }

  void Project::loadModule(const std::string& moduleName) {
    cModules.insert(std::make_unique<Module>(moduleName, this, &cLuaState));
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

  DOMNodeWriter Project::createNode(const std::string& name) {
    return DOMNodeWriter(name);
  }

  DOMNode Project::readNode(const std::string& name) {
    return DOMNode(name, DOMNode::Type::USER);
  }

  std::string Project::getUserDataPath() {
    return (cCanSave ? "User/" : "Program/") + cProjectDataPath;
  }

  std::string Project::getDataPath(bool user) {
    return getProjectPathPrefix(user) + cProjectDataPath;
  }

  void Project::makeUserDataDirectory(const std::string& path) {
    System::makeUserDataDirectory(getUserDataPath() + "/" + path);
  }

  std::string Project::getProjectPathPrefix(bool user) {
    return user ? (System::USER_DATA_DIRECTORY + (cCanSave ? "/User/" : "/Program/"))
                : (System::PROGRAM_DATA_DIRECTORY                                   );
  }

  IEditable* Project::getDefaultEditable() {
    return ***cDefDefaultEditor;
  }

  IAction* Project::getAction(IAssetUser<IAction>* user, DOMNode& node, const std::string& tag, IBindingRegistry* localArgs, const std::string& id) {
    for (DOMNode& mNode : node) {
      std::string mChildName = mNode.getName();
      if (mChildName == tag) {
        std::unique_ptr<ActionExecutor> mActionExecutor = std::make_unique<ActionExecutor>(this, mNode, id, user, localArgs);
        IAction* mAction = mActionExecutor.get();
        cActionExecutors[mAction] = std::move(mActionExecutor);
        return mAction;
      }
    }
    return createLiteralAction(user);
  }

  LuaState* const Project::getLuaState() {
    return &cLuaState;
  }
  
  std::string Project::getProjectResourceDataPath(const std::string& file) {
    return cProjectDataPath + "/" + file;
  }

  I3DModelType*    Project::getModelType(       IAssetUser<I3DModelType>*    user, DOMNode& node,                                      bool required) {return c3DModelTypes.get(     user, *this, node, nullptr,  required, [this](DOMNode& node, IStateListener<I3DModelType*>*    listener) -> I3DModelType*    {return cAssetOverride != nullptr ? cAssetOverride->getModelType(       node, listener) : nullptr;});}
  IActionType*     Project::getActionType(      IAssetUser<IActionType>*     user, DOMNode& node,                                      bool required) {return cActionTypes.get(      user, *this, node, nullptr,  required, [this](DOMNode& node, IStateListener<IActionType*>*     listener) -> IActionType*     {return cAssetOverride != nullptr ? cAssetOverride->getActionType(      node, listener) : nullptr;});}
  IAssets*         Project::getAssets(          IAssetUser<IAssets>*         user, DOMNode& node,                                      bool required) {return cAssets.get(           user, *this, node, nullptr,  required, [this](DOMNode& node, IStateListener<IAssets*>*         listener) -> IAssets*         {return cAssetOverride != nullptr ? cAssetOverride->getAssets(          node, listener) : nullptr;});}
  IBinding*        Project::getBinding(         IAssetUser<IBinding>*        user, DOMNode& node, IBindingRegistry* locals,            bool required) {
    cLocalProviderBinding.setLocalBindings(locals);
    IBinding* mBinding = cBindings.get(user, *this, node, nullptr, required, [this](DOMNode& node, IStateListener<IBinding*>* listener) -> IBinding*    {return cAssetOverride != nullptr ? cAssetOverride->getBinding(       node, listener) : nullptr;});
    cLocalProviderBinding.setLocalBindings(nullptr);
    return mBinding;
  }
  IBoolean*        Project::getBoolean(         IAssetUser<IBoolean>*        user, DOMNode& node, IStateListener<IBoolean*>* listener, bool required) {return cBooleans.get(         user, *this, node, listener, required, [this](DOMNode& node, IStateListener<IBoolean*>*        listener) -> IBoolean*        {return cAssetOverride != nullptr ? cAssetOverride->getBoolean(         node, listener) : nullptr;});}
  IColour*         Project::getColour(          IAssetUser<IColour>*         user, DOMNode& node, IStateListener<IColour*>*  listener, bool required) {return cColours.get(          user, *this, node, listener, required, [this](DOMNode& node, IStateListener<IColour*>*         listener) -> IColour*         {return cAssetOverride != nullptr ? cAssetOverride->getColour(          node, listener) : nullptr;});}
  IEditable*       Project::getEditable(        IAssetUser<IEditable>*       user, DOMNode& node,                                      bool required) {return cEditables.get(        user, *this, node, nullptr,  required, [this](DOMNode& node, IStateListener<IEditable*>*       listener) -> IEditable*       {return cAssetOverride != nullptr ? cAssetOverride->getEditable(        node, listener) : nullptr;});}
  IFloat*          Project::getFloat(           IAssetUser<IFloat>*          user, DOMNode& node, IStateListener<IFloat*>*   listener, bool required) {return cFloats.get(           user, *this, node, listener, required, [this](DOMNode& node, IStateListener<IFloat*>*          listener) -> IFloat*          {return cAssetOverride != nullptr ? cAssetOverride->getFloat(           node, listener) : nullptr;});}
  IFont*           Project::getFont(            IAssetUser<IFont>*           user, DOMNode& node,                                      bool required) {return cFonts.get(            user, *this, node, nullptr,  required, [this](DOMNode& node, IStateListener<IFont*>*           listener) -> IFont*           {return cAssetOverride != nullptr ? cAssetOverride->getFont(            node, listener) : nullptr;});}
  IInputHandler*   Project::getInputHandler(    IAssetUser<IInputHandler>*   user, DOMNode& node,                                      bool required) {return cInputHandlers.get(    user, *this, node, nullptr,  required, [this](DOMNode& node, IStateListener<IInputHandler*>*   listener) -> IInputHandler*   {return cAssetOverride != nullptr ? cAssetOverride->getInputHandler(    node, listener) : nullptr;});}
  IInteger*        Project::getInteger(         IAssetUser<IInteger>*        user, DOMNode& node, IStateListener<IInteger*>* listener, bool required) {return cIntegers.get(         user, *this, node, listener, required, [this](DOMNode& node, IStateListener<IInteger*>*        listener) -> IInteger*        {return cAssetOverride != nullptr ? cAssetOverride->getInteger(         node, listener) : nullptr;});}
  IProjectOptions* Project::getProjectOptions(  IAssetUser<IProjectOptions>* user, DOMNode& node,                                      bool required) {return cProjectOptions.get(   user, *this, node, nullptr,  required, [this](DOMNode& node, IStateListener<IProjectOptions*>* listener) -> IProjectOptions* {return cAssetOverride != nullptr ? cAssetOverride->getProjectOptions(  node, listener) : nullptr;});}
  IScreen*         Project::getScreen(          IAssetUser<IScreen>*         user, DOMNode& node,                                      bool required) {return cScreens.get(          user, *this, node, nullptr,  required, [this](DOMNode& node, IStateListener<IScreen*>*         listener) -> IScreen*         {return cAssetOverride != nullptr ? cAssetOverride->getScreen(          node, listener) : nullptr;});}
  IString*         Project::getString(          IAssetUser<IString>*         user, DOMNode& node, IStateListener<IString*>*  listener, bool required) {return cStrings.get(          user, *this, node, listener, required, [this](DOMNode& node, IStateListener<IString*>*         listener) -> IString*         {return cAssetOverride != nullptr ? cAssetOverride->getString(          node, listener) : nullptr;});}
  ITexture*        Project::getTexture(         IAssetUser<ITexture>*        user, DOMNode& node, IStateListener<ITexture*>* listener, bool required) {return cTextures.get(         user, *this, node, listener, required, [this](DOMNode& node, IStateListener<ITexture*>*        listener) -> ITexture*        {return cAssetOverride != nullptr ? cAssetOverride->getTexture(         node, listener) : nullptr;});}
  IVertex*         Project::getVertex(          IAssetUser<IVertex>*         user, DOMNode& node,                                      bool required) {return cVertices.get(         user, *this, node, nullptr,  required, [this](DOMNode& node, IStateListener<IVertex*>*         listener) -> IVertex*         {return cAssetOverride != nullptr ? cAssetOverride->getVertex(          node, listener) : nullptr;});}

  I3DModelType*   Project::createLiteral3DModel(      IAssetUser<I3DModelType>*   user)                                                                          {return c3DModelTypes.literal(user, "");}
  IAction*        Project::createLiteralAction(       IAssetUser<IAction>*        user) {
    std::unique_ptr<ActionExecutor> mActionExecutor = std::make_unique<ActionExecutor>(this, user);
    IAction* mAction = mActionExecutor.get();
    cActionExecutors[mAction] = std::move(mActionExecutor);
    return mAction;
  }
  IActionType*     Project::createLiteralActionType(    IAssetUser<IActionType>*     user)                                                                          {return cActionTypes.literal(   user, "");}
  IAssets*         Project::createLiteralAssets(        IAssetUser<IAssets>*         user)                                                                          {return cAssets.literal(        user, "");}
  IBinding*        Project::createLiteralBinding(       IAssetUser<IBinding>*        user)                                                                          {return cBindings.literal(      user, "");}
  IBoolean*        Project::createLiteralBoolean(       IAssetUser<IBoolean>*        user, const bool value)                                                        {return cBooleans.literal(      user, value ? std::string("true") : std::string("false"));}
  IColour*         Project::createLiteralColour(        IAssetUser<IColour>*         user, const float red, const float green, const float blue, const float alpha) {return cColours.literal(       user, Utils::toString(red) + " " + Utils::toString(green) + " " + Utils::toString(blue) + " " + Utils::toString(alpha));}
  IEditable*       Project::createLiteralEditable(      IAssetUser<IEditable>*       user)                                                                          {return cEditables.literal(     user, "");}
  IFloat*          Project::createLiteralFloat(         IAssetUser<IFloat>*          user, const float value)                                                       {return cFloats.literal(        user, Utils::toString(value));}
  IFont*           Project::createLiteralFont(          IAssetUser<IFont>*           user)                                                                          {return cFonts.literal(         user, "");}
  IInputHandler*   Project::createLiteralInputHandler(  IAssetUser<IInputHandler>*   user)                                                                          {return cInputHandlers.literal( user, "");}
  IInteger*        Project::createLiteralInteger(       IAssetUser<IInteger>*        user, const int value)                                                         {return cIntegers.literal(      user, Utils::toString(value));}
  IProjectOptions* Project::createLiteralProjectOptions(IAssetUser<IProjectOptions>* user)                                                                          {return cProjectOptions.literal(user, "");}
  IScreen*         Project::createLiteralScreen(        IAssetUser<IScreen>*         user)                                                                          {return cScreens.literal(       user, "");}
  IString*         Project::createLiteralString(        IAssetUser<IString>*         user, const std::string& value)                                                {return cStrings.literal(       user, value);}
  ITexture*        Project::createLiteralTexture(       IAssetUser<ITexture>*        user)                                                                          {return cTextures.literal(      user, "");}
  IVertex*         Project::createLiteralVertex(        IAssetUser<IVertex>*         user, const float x, const float y, const float z)                             {return cVertices.literal(      user, Utils::toString(x) + " " + Utils::toString(y) + " " + Utils::toString(z));}
    
  void Project::release(IAssetUser<I3DModelType>*    user, I3DModelType*    asset) {c3DModelTypes.release(  user, asset);}
  void Project::release(IAssetUser<IAction>*         user, IAction*         asset) {
    std::map<IAction*, std::unique_ptr<ActionExecutor>>::iterator mActionExecutor = cActionExecutors.find(asset);
    if (mActionExecutor == cActionExecutors.end()) {
      std::cout << "WARNING: No client attached to the specified action" << std::endl;
      return;
    }
    cActionExecutors.erase(asset);
  }
  void Project::release(IAssetUser<IActionType>*     user, IActionType*     asset) {cActionTypes.release(   user, asset);}
  void Project::release(IAssetUser<IAssets>*         user, IAssets*         asset) {cAssets.release(        user, asset);}
  void Project::release(IAssetUser<IBinding>*        user, IBinding*        asset) {cBindings.release(      user, asset);}
  void Project::release(IAssetUser<IBoolean>*        user, IBoolean*        asset) {cBooleans.release(      user, asset);}
  void Project::release(IAssetUser<IColour>*         user, IColour*         asset) {cColours.release(       user, asset);}
  void Project::release(IAssetUser<IEditable>*       user, IEditable*       asset) {cEditables.release(     user, asset);}
  void Project::release(IAssetUser<IFloat>*          user, IFloat*          asset) {cFloats.release(        user, asset);}
  void Project::release(IAssetUser<IFont>*           user, IFont*           asset) {cFonts.release(         user, asset);}
  void Project::release(IAssetUser<IInputHandler>*   user, IInputHandler*   asset) {cInputHandlers.release( user, asset);}
  void Project::release(IAssetUser<IInteger>*        user, IInteger*        asset) {cIntegers.release(      user, asset);}
  void Project::release(IAssetUser<IProjectOptions>* user, IProjectOptions* asset) {cProjectOptions.release(user, asset);}
  void Project::release(IAssetUser<IScreen>*         user, IScreen*         asset) {cScreens.release(       user, asset);}
  void Project::release(IAssetUser<IString>*         user, IString*         asset) {cStrings.release(       user, asset);}
  void Project::release(IAssetUser<ITexture>*        user, ITexture*        asset) {cTextures.release(      user, asset);}
  void Project::release(IAssetUser<IVertex>*         user, IVertex*         asset) {cVertices.release(      user, asset);}
    
  bool Project::renderAssetIcon() const {
    return false;
  }

  std::vector<std::pair<std::string, std::string>> Project::getAllModelTypes()    {return c3DModelTypes.getAll();}
  std::vector<std::pair<std::string, std::string>> Project::getAllActionTypes()   {return cActionTypes.getAll();}
  std::vector<std::pair<std::string, std::string>> Project::getAllBooleans()      {return cBooleans.getAll();}
  std::vector<std::pair<std::string, std::string>> Project::getAllColours()       {return cColours.getAll();}
  std::vector<std::pair<std::string, std::string>> Project::getAllEditables()     {return cEditables.getAll();}
  std::vector<std::pair<std::string, std::string>> Project::getAllFloats()        {return cFloats.getAll();}
  std::vector<std::pair<std::string, std::string>> Project::getAllFonts()         {return cFonts.getAll();}
  std::vector<std::pair<std::string, std::string>> Project::getAllInputHandlers() {return cInputHandlers.getAll();}
  std::vector<std::pair<std::string, std::string>> Project::getAllIntegers()      {return cIntegers.getAll();}
  std::vector<std::pair<std::string, std::string>> Project::getAllScreens()       {return cScreens.getAll();}
  std::vector<std::pair<std::string, std::string>> Project::getAllStrings()       {return cStrings.getAll();}
  std::vector<std::pair<std::string, std::string>> Project::getAllTextures()      {return cTextures.getAll();}
  std::vector<std::pair<std::string, std::string>> Project::getAllVertices()      {return cVertices.getAll();}

  void Project::addListener(IAssetListener<Project, I3DModelType>* listener) {c3DModelTypes.addAssetListener(listener);}
  void Project::addListener(IAssetListener<Project, IBoolean>* listener)     {cBooleans.addAssetListener(listener);}
  void Project::addListener(IAssetListener<Project, IColour>* listener)      {cColours.addAssetListener(listener);}
  void Project::addListener(IAssetListener<Project, IEditable>* listener)    {cEditables.addAssetListener(listener);}
  void Project::addListener(IAssetListener<Project, IFloat>* listener)       {cFloats.addAssetListener(listener);}
  void Project::addListener(IAssetListener<Project, IFont>* listener)        {cFonts.addAssetListener(listener);}
  void Project::addListener(IAssetListener<Project, IInteger>* listener)     {cIntegers.addAssetListener(listener);}
  void Project::addListener(IAssetListener<Project, IScreen>* listener)      {cScreens.addAssetListener(listener);}
  void Project::addListener(IAssetListener<Project, IString>* listener)      {cStrings.addAssetListener(listener);}
  void Project::addListener(IAssetListener<Project, ITexture>* listener)     {cTextures.addAssetListener(listener);}
  void Project::addListener(IAssetListener<Project, IVertex>* listener)      {cVertices.addAssetListener(listener);}

  bool Project::renderActionIcon(      const std::string& id) const {return cActionTypes.renderIcon(id);}
  bool Project::renderBooleanIcon(     const std::string& id) const {return cBooleans.renderIcon(id);}
  bool Project::renderColourIcon(      const std::string& id) const {return cColours.renderIcon(id);}
  bool Project::renderFloatIcon(       const std::string& id) const {return cFloats.renderIcon(id);}
  bool Project::renderInputHandlerIcon(const std::string& id) const {return cInputHandlers.renderIcon(id);}
  bool Project::renderIntegerIcon(     const std::string& id) const {return cIntegers.renderIcon(id);}
  bool Project::renderModelIcon(       const std::string& id) const {return c3DModelTypes.renderIcon(id);}
  bool Project::renderScreenIcon(      const std::string& id) const {return cScreens.renderIcon(id);}
  bool Project::renderTextureIcon(     const std::string& id) const {return cTextures.renderIcon(id);}
  bool Project::renderVertexIcon(      const std::string& id) const {return cVertices.renderIcon(id);}
    
  IProject* Project::getProject() {
    return this;
  }
  
  void                      Project::add(I3DModelType*    asset, const std::string& id, const std::string& category) {       c3DModelTypes.add(  asset, id, category      );}
  void                      Project::add(IActionType*     asset, const std::string& id, const std::string& category) {       cActionTypes.add(   asset, id, category      );}
  void                      Project::add(IBinding*        asset, const std::string& id, const std::string& category) {       cBindings.add(      asset, id, category      );}
  IStateNotifier<IBoolean>* Project::add(IBoolean*        asset, const std::string& id, const std::string& category) {return cBooleans.add(      asset, id, category, true);}
  IStateNotifier<IColour>*  Project::add(IColour*         asset, const std::string& id, const std::string& category) {return cColours.add(       asset, id, category, true);}
  void                      Project::add(IEditable*       asset, const std::string& id, const std::string& category) {       cEditables.add(     asset, id, category      );}
  IStateNotifier<IFloat>*   Project::add(IFloat*          asset, const std::string& id, const std::string& category) {return cFloats.add(        asset, id, category, true);}
  void                      Project::add(IFont*           asset, const std::string& id, const std::string& category) {       cFonts.add(         asset, id, category      );}
  void                      Project::add(IInputHandler*   asset, const std::string& id, const std::string& category) {       cInputHandlers.add( asset, id, category      );}
  IStateNotifier<IInteger>* Project::add(IInteger*        asset, const std::string& id, const std::string& category) {return cIntegers.add(      asset, id, category, true);}
  IScreen*                  Project::add(IScreen*         asset, const std::string& id, const std::string& category) {
    std::map<IScreen*, std::unique_ptr<ScreenProxy>>::iterator mExistingProxy = cScreenProxyMapping.find(asset);
    if (mExistingProxy == cScreenProxyMapping.end()) {
      std::unique_ptr<ScreenProxy> mNewProxy = std::make_unique<ScreenProxy>(this, asset);
      cScreens.add(mNewProxy.get(), id, category);
      if (!cProcessingInput) {
        for (IScreenListener* mListener : cListeners) {
          mListener->screenAdded(this, mNewProxy.get());
        }
      }
      return cScreenProxyMapping.emplace(asset, std::move(mNewProxy)).first->second.get();
    }
    cScreens.add(mExistingProxy->second.get(), id, category);
    return mExistingProxy->second.get();
  }
  void                      Project::add(IProjectOptions* asset, const std::string& id, const std::string& category) {       cProjectOptions.add(asset, id, category      );}
  void                      Project::add(IAssets*         asset, const std::string& id, const std::string& category) {       cAssets.add(        asset, id, category      );}
  IStateNotifier<IString>*  Project::add(IString*         asset, const std::string& id, const std::string& category) {return cStrings.add(       asset, id, category, true);}
  IStateNotifier<ITexture>* Project::add(ITexture*        asset, const std::string& id, const std::string& category) {return cTextures.add(      asset, id, category, true);}
  IStateNotifier<IVertex>*  Project::add(IVertex*         asset, const std::string& id, const std::string& category) {return cVertices.add(      asset, id, category, true);}

  void Project::remove(I3DModelType*    asset) {c3DModelTypes.remove(  asset);}
  void Project::remove(IActionType*     asset) {cActionTypes.remove(   asset);}
  void Project::remove(IBinding*        asset) {cBindings.remove(      asset);}
  void Project::remove(IBoolean*        asset) {cBooleans.remove(      asset);}
  void Project::remove(IColour*         asset) {cColours.remove(       asset);}
  void Project::remove(IEditable*       asset) {cEditables.remove(     asset);}
  void Project::remove(IFloat*          asset) {cFloats.remove(        asset);}
  void Project::remove(IFont*           asset) {cFonts.remove(         asset);}
  void Project::remove(IInputHandler*   asset) {cInputHandlers.remove( asset);}
  void Project::remove(IInteger*        asset) {cIntegers.remove(      asset);}
  void Project::remove(IScreen*         asset) {
    std::map<IScreen*, std::unique_ptr<ScreenProxy>>::iterator mProxy = cScreenProxyMapping.find(asset);
    if (mProxy == cScreenProxyMapping.end()) {
      throw ArgumentException("ERROR: Project::remove: Proxy for specified screen asset not found.");
    }
    cScreens.remove(mProxy->second.get());
  }
  void Project::remove(IProjectOptions* asset) {cProjectOptions.remove(asset);}
  void Project::remove(IAssets*         asset) {cAssets.remove(        asset);}
  void Project::remove(IString*         asset) {cStrings.remove(       asset);}
  void Project::remove(ITexture*        asset) {cTextures.remove(      asset);}
  void Project::remove(IVertex*         asset) {cVertices.remove(      asset);}
  
  void Project::init(std::function<void(IAssets*)> initialiser) {
//     std::cout << "ADDING INIT " << cInitialisers.size() << std::endl;
//     if (cInitialisers.size() == 360) {
//       std::cout << "DEBUG!" << std::endl;
//     }
    cInitialisers.push_back(initialiser);
  }

  ICallbackHandle* Project::reset(std::function<void()> resetter) {
    return cResetters.emplace_back(std::make_unique<ResetCallbackHandle>(resetter)).get();
  }

  ICallbackHandle* Project::updateRuntime(std::function<void(unsigned int)> dynamic) {
    return cRuntimeDynamics.emplace_back(std::make_unique<UpdateCallbackHandle>(dynamic)).get();
  }
  
  ICallbackHandle* Project::updateEditing(std::function<void(unsigned int)> dynamic) {
    return cEditingDynamics.emplace_back(std::make_unique<UpdateCallbackHandle>(dynamic)).get();
  }

  // TODO: Rename this!  It's only used for rendering to texture tasks, so should be done during rendering phase.
  void Project::updateLater(std::function<void()> task) {
    cUpdateTasks.push(task);
  }
  
  void Project::remove(ICallbackHandle* callbackHandle) {
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

  void Project::save(DOMNodeWriter* node, const I3DModelType*    asset) const {c3DModelTypes.save(  node, asset);}
  void Project::save(DOMNodeWriter* node, const IAssets*         asset) const {cAssets.save(        node, asset);}
  void Project::save(DOMNodeWriter* node, const IActionType*     asset) const {cActionTypes.save(   node, asset);}
  void Project::save(DOMNodeWriter* node, const IBinding*        asset) const {cBindings.save(      node, asset);}
  void Project::save(DOMNodeWriter* node, const IBoolean*        asset) const {cBooleans.save(      node, asset);}
  void Project::save(DOMNodeWriter* node, const IColour*         asset) const {cColours.save(       node, asset);}
  void Project::save(DOMNodeWriter* node, const IEditable*       asset) const {cEditables.save(     node, asset);}
  void Project::save(DOMNodeWriter* node, const IFloat*          asset) const {cFloats.save(        node, asset);}
  void Project::save(DOMNodeWriter* node, const IFont*           asset) const {cFonts.save(         node, asset);}
  void Project::save(DOMNodeWriter* node, const IInputHandler*   asset) const {cInputHandlers.save( node, asset);}
  void Project::save(DOMNodeWriter* node, const IInteger*        asset) const {cIntegers.save(      node, asset);}
  void Project::save(DOMNodeWriter* node, const IScreen*         asset) const {cScreens.save(       node, asset);}
  void Project::save(DOMNodeWriter* node, const IProjectOptions* asset) const {cProjectOptions.save(node, asset);}
  void Project::save(DOMNodeWriter* node, const IString*         asset) const {cStrings.save(       node, asset);}
  void Project::save(DOMNodeWriter* node, const ITexture*        asset) const {cTextures.save(      node, asset);}
  void Project::save(DOMNodeWriter* node, const IVertex*         asset) const {cVertices.save(      node, asset);}

  Project::ActionExecutor::Action::Action(ActionExecutor* parent, DOMNode& node, const std::string& id, IBindingRegistry* localArgs) :
            cParent(parent),
            cActionType(parent->cParent, [this]() {
              cParent->remove(this);
            }, node),
            cAction(cActionType->createAction(node, parent->cParent, localArgs)) {
  }

  void Project::ActionExecutor::Action::execute() {
    if (cParent->cParent->cProcessingInput) {
      cParent->cParent->cPostponedActions.push_back(cAction);
    } else {
      cAction->execute();
    }
  }

  void Project::ActionExecutor::Action::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    identifier->save(node, *cActionType);
    cAction->save(node, identifier);
  }

  Project::ActionExecutor::ActionExecutor::Action::~Action() {
    cActionType->destroyAction(cAction, cParent->cParent);
  }

  Project::ActionExecutor::ActionExecutor(Project* parent, IAssetUser<IAction>* user) :
            cParent(parent),
            cUser(user) {
  }

  Project::ActionExecutor::ActionExecutor(Project* parent, DOMNode& node, const std::string& id, IAssetUser<IAction>* user, IBindingRegistry* localArgs) :
            ActionExecutor(parent, user) {
    for (DOMNode& mNode : node) {
      std::string mChildName = mNode.getName();
      if (mChildName == "Action") {
        cActions.emplace_back(std::make_unique<Action>(this, mNode, id, localArgs));
      }
    }
  }

  void Project::ActionExecutor::remove(Action* action) {
    Utils::removeElementUnique(cActions, action);
  }

  void Project::ActionExecutor::execute() {
    for (std::unique_ptr<Action>& mAction : cActions) {
      mAction->execute();
    }
  }

  IActionType* Project::ActionExecutor::getActionType() const {
    std::cout << "WARNING: getActionType() called, but not implemented!" << std::endl;
    return nullptr;
  }

  void Project::ActionExecutor::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    for (const std::unique_ptr<Action>& mAction : cActions) {
      DOMNodeWriter mActionNode = node->addBranch(TAG_ACTION);
      mAction->save(&mActionNode, identifier);
    }
  }

  bool Project::ActionExecutor::hasConfiguration() const {
    std::cout << "WARNING: hasConfiguration() called, but not implemented!" << std::endl;
    return false; // cAction->hasConfiguration();
  }

  Project::ScreenProxy::ScreenProxy(Project* parent, IScreen* screen) :
            cParent(parent) {
    if (screen == nullptr) {
      throw std::invalid_argument("Screen cannot be nullptr");
    }
    cScreen   = screen;
  }

  void Project::renderScreen(IScreen* screen, float scale, float aspectRatio) {
    for (IScreenListener* mListener : cListeners) {
      mListener->screenPreRender(screen);
    }
    screen->renderScreen(scale, aspectRatio);
    for (IScreenListener* mListener : cListeners) {
      mListener->screenPostRender(screen);
    }
  }

  void Project::addScreenListener(IScreenListener* listener) {
    cListeners.push_back(listener);
  }

  void Project::removeScreenListener(IScreenListener* listener) {
    Utils::removeElement(cListeners, listener);
  }
  
  void Project::addStateChangeListener(const IFloat* asset, IStateListener<IFloat*>* listener) {
    cFloats.addStateChangeListener(asset, listener);
  }

  IApplication* Project::getApplication() {
    return cApplication;
  }

  Project::ProjectProperty::ProjectProperty(Project* parent, DOMNode& node, const std::string& resourcePath) :
            cChangeAction(parent),
            cResourcePath(resourcePath) {
    cChangeAction.init(node, TAG_ACTION, parent);
  }

  void Project::ProjectProperty::setValue(const std::string& value) {
    cChangeAction.execute();
  }

  void Project::ProjectProperty::save(DOMNodeWriter& node, const std::string& resourcePath, const std::string& id) const {
    if (resourcePath == cResourcePath) {
      DOMNodeWriter mPropertyNode = node.addBranch(TAG_PROPERTY);
      mPropertyNode.addAttribute(ATTRIBUTE_ID, id);
      cChangeAction.save(&mPropertyNode, TAG_ACTION);
    }
  }

  bool Project::ProjectProperty::isThisProject(const std::string& resourcePath) {
    return resourcePath == cResourcePath;
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

  void Project::saveBinding(DOMNodeWriter* node, const IBinding* binding) const {
    if (binding == &cPropertyValueBinding) {
      node->addAttribute("local", std::string("value"));
    }
  }

  void Project::releaseBinding(const IBinding* asset) {
    // Nothing to do.
  }

  void Project::ScreenProxy::renderScreen(float scale, float aspectRatio) const {
    cParent->renderScreen(cScreen, scale, aspectRatio);
  }
  
  bool Project::ScreenProxy::renderAssetIcon() const {
    return cScreen->renderAssetIcon();
  }

  const IFloat* Project::ScreenProxy::getYaw() const {
    return cScreen->getYaw();
  }

  const IFloat* Project::ScreenProxy::getPitch() const {
    return cScreen->getPitch();
  }

  Project::Filename::Filename(Project* parent) :
            cParent(parent) {
  }

  std::string Project::Filename::getValue() const {
    return cParent->cFilename;
  }

  bool Project::Filename::renderAssetIcon() const {
    return false;
  }

  Project::FileUser::FileUser(Project* parent) :
            cParent(parent) {
  }

  bool Project::FileUser::getValue() const {
    return cParent->cCanSave;
  }

  bool Project::FileUser::renderAssetIcon() const {
    return false;
  }

  Project::QuitActionType::QuitActionType(Project* parent) :
            cParent(parent),
            cQuitAction(this) {
  }

  IAction* Project::QuitActionType::createAction(DOMNode& node, IProject* project, IBindingRegistry* localObjects) {
    return &cQuitAction;
  }

  IAction* Project::QuitActionType::createAction(IProject* project, IBindingRegistry* localObjects) {
    return &cQuitAction;
  }

  void Project::QuitActionType::destroyAction(IAction* action, IAssets* assets) {
    // Nothing to do.
  }

  bool Project::QuitActionType::renderAssetIcon() const {
    return false;
  }

  Project::QuitActionType::QuitAction::QuitAction(QuitActionType* parent) :
            cParent(parent) {
  }

  IActionType* Project::QuitActionType::QuitAction::getActionType() const {
    return cParent;
  }

  void Project::QuitActionType::QuitAction::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    identifier->save(node, cParent);
  }

  bool Project::QuitActionType::QuitAction::hasConfiguration() const {
    return false;
  }

  void Project::QuitActionType::QuitAction::execute() {
    cParent->cParent->finish(true);
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

    cStrings.remove(&cConversionProviderString);

    cIntegers.remove(&cConversionProviderProjectToInteger);
    cStrings.remove(&cConversionProviderProjectToString);

    cBindings.remove(&cLocalProviderBinding);


//     c3DModelTypes.checkClean("Models");
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

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
#pragma once

#include <functional>
#include <queue>
#include <set>
#include <string>

#include "Assets/Fixed/LiteralActionType.h"
#include "Assets/Fixed/LiteralAssets.h"
#include "Assets/Fixed/LiteralBinding.h"
#include "Assets/Fixed/LiteralEditable.h"
#include "Assets/Fixed/LiteralFont.h"
#include "Assets/Fixed/LiteralInputHandler.h"
#include "Assets/Fixed/LiteralModel.h"
#include "Assets/Fixed/LiteralProjectOptions.h"
#include "Assets/Fixed/LiteralScreen.h"
#include "Assets/Fixed/LiteralTexture.h"
#include "Assets/Providers/AssetConvertedActionToBinding.h"
#include "Assets/Providers/AssetConvertedBooleanToBinding.h"
#include "Assets/Providers/AssetConvertedColourToBinding.h"
#include "Assets/Providers/AssetConvertedFloatToBinding.h"
#include "Assets/Providers/AssetConvertedFontToBinding.h"
#include "Assets/Providers/AssetConvertedInputHandlerToBinding.h"
#include "Assets/Providers/AssetConvertedIntegerToBinding.h"
#include "Assets/Providers/AssetConvertedProjectOptionsToBinding.h"
#include "Assets/Providers/AssetConvertedProjectToBinding.h"
#include "Assets/Providers/AssetConvertedScreenToBinding.h"
#include "Assets/Providers/AssetConvertedStringToBinding.h"
#include "Assets/Providers/AssetConvertedVertexToBinding.h"
#include "Assets/Providers/AssetConvertedString.h"
#include "Assets/Providers/AssetConvertedProjectToInteger.h"
#include "Assets/Providers/AssetConvertedProjectToString.h"
#include "Assets/Providers/AssetLiteralBoolean.h"
#include "Assets/Providers/AssetLiteralColour.h"
#include "Assets/Providers/AssetLiteralDummy.h"
#include "Assets/Providers/AssetLiteralFloat.h"
#include "Assets/Providers/AssetLiteralInteger.h"
#include "Assets/Providers/AssetLiteralString.h"
#include "Assets/Providers/AssetLiteralVertex.h"
#include "Assets/Providers/AssetLocalBinding.h"
#include "Assets/Registry/AssetClientManager.h"
#include "Assets/Type/IScreenListener.h"
#include "DisplayResolution.h"
#include "IAssetBrowser.h"
#include "IAssetOverride.h"
#include "IAssetRemover.h"
#include "ICallbackHandle.h"
#include "Lua.h"
#include "Options/LocalOptions.h"
#include "Options/Options.h"
#include "Persistence/DOMNode.h"
#include "Module.h"
#include "Types.h"

namespace IsoRealms {

  /**
   * The Project is the root object of an IsoRealms game or application.  The
   * Project acts as a container for the assets of the application, and
   * provides control over execution of the application.
   */
  class Project : public IProject,
                  public IAssetRegistry,
                  public IBindingRegistry {
    private:
    static const std::string TAG_ACTION;
    static const std::string TAG_EDITOR;
    static const std::string TAG_INCLUDE;
    static const std::string TAG_INIT_ACTION;
    static const std::string TAG_INPUT;
    static const std::string TAG_MODULE;
    static const std::string TAG_MODULES;
    static const std::string TAG_PROJECT;
    static const std::string TAG_PROPERTIES;
    static const std::string TAG_PROPERTY;
    static const std::string TAG_QUIT_ACTION;
    static const std::string TAG_RESET_ACTION;
    static const std::string TAG_SCREEN;

    static const std::string ATTRIBUTE_ID;
    static const std::string ATTRIBUTE_NAME;
    static const std::string ATTRIBUTE_USER;

    static const std::string CATEGORY_CONVERSIONS;
    static const std::string CATEGORY_FIXED;
    static const std::string CATEGORY_LOCAL;

    struct Include {
      std::string cProject; /// File name of included project.
      bool cUser;           /// User or program project.
    };

    class Filename : public IString {
      public:
      Filename(Project* parent);

      private:
      Project* cParent;

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
    };

    class FileUser : public IBoolean {
      public:
      FileUser(Project* parent);

      private:
      Project* cParent;

      /***********************\
       * Implements IBoolean *
      \***********************/
      bool getValue() const override;
      bool renderAssetIcon() const override;
    };

    class ResetCallbackHandle : public ICallbackHandle {
      private:
      std::function<void()> cFunction;      
        
      public:
      ResetCallbackHandle(std::function<void()> function) :
                cFunction(function) {
      }
        
      void call() {
        cFunction();
      }
    };
    
    class UpdateCallbackHandle : public ICallbackHandle {
      private:
      std::function<void(unsigned int)> cFunction;      
        
      public:
      UpdateCallbackHandle(std::function<void(unsigned int)> function) :
                cFunction(function) {
      }
        
      void call(unsigned int milliseconds) {
        cFunction(milliseconds);
      }
    };
    
    /**
     * A wrapper that controls execution of an action.  Specifically, if the
     * Project is currently handling input, the action executor postpones
     * execution of the action until the beginning of the next update cycle.
     */
    class ActionExecutor : public IAction {
      private:
      class Action {
        private:
        ActionExecutor* cParent;      /// Parent Action Executor.
        ActionType cActionType; /// Type from which the action is derived.
        IAction* cAction;             /// Action to be executed.

        public:
        Action(ActionExecutor* parent, DOMNode& node, const std::string& id, IBindingRegistry* localArgs);

        /**********************\
         * Implements IAction *
        \**********************/
        void execute();
        void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;

        ~Action();
      };

      std::vector<std::unique_ptr<Action>> cActions; /// Actions
      Project* cParent;                              /// Parent Project.
      IAssetUser<IAction>* cUser;                    /// Client of this action.
      
      public:
        
      /**
       * Construct the ActionExecutor.
       * 
       * @param project The parent Project.
       * @param action The action to execute/postpone.
       */
      ActionExecutor(Project* project, IAssetUser<IAction>* user);
      ActionExecutor(Project* project, DOMNode& node, const std::string& id, IAssetUser<IAction>* user, IBindingRegistry* localArgs);

      void remove(Action* action);

      /**********************\
       * Implements IAction *
      \**********************/
      void execute() override;
      IActionType* getActionType() const override;
      void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const override;
      bool hasConfiguration() const override;
    };

    class QuitActionType : public IActionType {
      private:

      class QuitAction : public IAction {
        private:
        QuitActionType* cParent;

        public:
        QuitAction(QuitActionType* parent);

        /**********************\
         * Implements IAction *
        \**********************/
        IActionType* getActionType() const override;
        void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const override;
        void execute() override;
        bool hasConfiguration() const override;
      };
      Project* cParent;
      QuitAction cQuitAction;

      public:
      QuitActionType(Project* parent);

      /**************************\
       * Implements IActionType *
      \**************************/
      IAction* createAction(DOMNode& node, IProject* project, IBindingRegistry* localObjects) override;
      IAction* createAction(IProject* project, IBindingRegistry* localArgs) override;
      void destroyAction(IAction* action, IAssets* assets) override;
      bool renderAssetIcon() const override;
    };

    std::vector<IScreenListener*> cListeners;

    class ScreenProxy : public IScreen {
      private:
      Project* cParent;
      IScreen* cScreen;
      
      public:
      ScreenProxy(Project* parent, IScreen* screen);
      
      /**********************\
       * Implements IScreen *
      \**********************/
      void renderScreen(float scale, float aspectRatio) const override;
      bool renderAssetIcon() const override;
      const IFloat* getYaw() const override;
      const IFloat* getPitch() const override;
    };

    class ProjectProperty {
      public:
      ProjectProperty(Project* parent, DOMNode& node);
      void setValue(const std::string& value);
      void save(DOMNodeWriter& node) const;

      private:
      Action cChangeAction;
    };

    IApplication* cApplication; /// Host application of this project.
    
    const IAssetOverride* const cAssetOverride; /// External asset override for this project.
    LuaState cLuaState;                         /// Lua State for this project.
    std::vector<std::unique_ptr<Include>> cInclusions;          /// List of other project files included within this project.
    LuaBinding<Project> cLuaBinding;          /// Project interface for actions and scripting.


    // Callbacks
    std::queue<std::function<void()>> cMainThreadAllocTasks;         /// Allocation tasks to be perfomed on the main thread during initialisation.
    std::queue<std::function<void()>> cMainThreadInitTasks;          /// initialisation tasks to be performed on the main thread.
    std::vector<std::function<void(IAssets*)>> cInitialisers;        /// Asset initialisation tasks.  Called after assets have been registered.
    std::vector<std::unique_ptr<ResetCallbackHandle>> cResetters;                     /// Reset functions.  Called at the end of a project.
    std::vector<std::unique_ptr<UpdateCallbackHandle>> cRuntimeDynamics;              /// Runtime update functions.  Called at each runtime update cycle.
    std::vector<std::unique_ptr<UpdateCallbackHandle>> cEditingDynamics;              /// Editor update functions.  Called at each editor update cycle.
    std::queue<std::function<void()>> cUpdateTasks;                  /// Postponed update functions.  Called after other update functions.

    // Action execution control
    bool cProcessingInput;                   /// Flag is set when processing input to indicate that actions should be postponed.
    std::vector<IAction*> cPostponedActions; /// List of postponed actions to be executed at next update cycle.
    bool cRuntimeUpdatingRuntime;            /// Flag is set when update cycle is being performed.
    bool cRuntimeResetPostponed;             /// Falg is set when a reset is postponed to be performed upon completion of the update cycle.



























    std::map<std::string, std::vector<I3DModel*>> cInstantiatedModels;

    // Asset registries.
    AssetClientManager<Project, I3DModelType>    c3DModelTypes;
    AssetClientManager<Project, IActionType>     cActionTypes;
    AssetClientManager<Project, IBinding>        cBindings;
    AssetClientManager<Project, IBoolean>        cBooleans;
    AssetClientManager<Project, IColour>         cColours;
    AssetClientManager<Project, IEditable>       cEditables;
    AssetClientManager<Project, IFloat>          cFloats;
    AssetClientManager<Project, IFont>           cFonts;
    AssetClientManager<Project, IInputHandler>   cInputHandlers;
    AssetClientManager<Project, IInteger>        cIntegers;
    AssetClientManager<Project, IScreen>         cScreens;
    AssetClientManager<Project, IProjectOptions> cProjectOptions;
    AssetClientManager<Project, IAssets>         cAssets;
    AssetClientManager<Project, IString>         cStrings;
    AssetClientManager<Project, ITexture>        cTextures;
    AssetClientManager<Project, IVertex>         cVertices;

    // Unsorted
    std::map<IAction*, std::unique_ptr<ActionExecutor>> cActionExecutors;
    std::map<IScreen*, std::unique_ptr<ScreenProxy>> cScreenProxyMapping;

    // Literal and dummy asset providers.
    AssetLiteralDummy<Project, I3DModelType,    LiteralModel>          cLiteralProviderModel;
    AssetLiteralDummy<Project, IActionType,     LiteralActionType>     cLiteralProviderActionType;
    AssetLiteralDummy<Project, IBinding,        LiteralBinding>        cLiteralProviderBinding;
    AssetLiteralBoolean                                                cLiteralProviderBoolean;
    AssetLiteralColour                                                 cLiteralProviderColour;
    AssetLiteralDummy<Project, IEditable,       LiteralEditable>       cLiteralProviderEditable;
    AssetLiteralFloat                                                  cLiteralProviderFloat;
    AssetLiteralDummy<Project, IFont,           LiteralFont>           cLiteralProviderFont;
    AssetLiteralDummy<Project, IInputHandler,   LiteralInputHandler>   cLiteralProviderInputHandler;
    AssetLiteralInteger                                                cLiteralProviderInteger;
    AssetLiteralDummy<Project, IScreen,         LiteralScreen>         cLiteralProviderScreen;
    AssetLiteralDummy<Project, IProjectOptions, LiteralProjectOptions> cLiteralProviderProjectOptions;
    AssetLiteralDummy<Project, IAssets,         LiteralAssets>         cLiteralProviderAssets;
    AssetLiteralString                                                 cLiteralProviderString;
    AssetLiteralDummy<Project, ITexture,        LiteralTexture>        cLiteralProviderTexture;
    AssetLiteralVertex                                                 cLiteralProviderVertex;

    // Conversion asset providers.
    AssetConvertedActionToBinding cConversionProviderActionToBinding;
    AssetConvertedBooleanToBinding cConversionProviderBooleanToBinding;
    AssetConvertedColourToBinding cConversionProviderColourToBinding;
    AssetConvertedFloatToBinding cConversionProviderFloatToBinding;
    AssetConvertedFontToBinding cConversionProviderFontToBinding;
    AssetConvertedInputHandlerToBinding cConversionProviderInputHandlerToBinding;
    AssetConvertedIntegerToBinding cConversionProviderIntegerToBinding;
    AssetConvertedProjectOptionsToBinding cConversionProviderProjectOptionsToBinding;
    AssetConvertedProjectToBinding cConversionProviderProjectToBinding;
    AssetConvertedScreenToBinding cConversionProviderScreenToBinding;
    AssetConvertedStringToBinding cConversionProviderStringToBinding;
    AssetConvertedVertexToBinding cConversionProviderVertexToBinding;
    AssetConvertedString cConversionProviderString;
    AssetConvertedProjectToInteger cConversionProviderProjectToInteger;
    AssetConvertedProjectToString cConversionProviderProjectToString;

    // Local binding support.
    AssetLocalBinding cLocalProviderBinding;

    std::function<void(bool)> cFunctionNotifyComplete;
    bool cCanSave;
    std::string cFilename;
    std::string cProjectDataPath;
    Filename cFilenameString;
    FileUser cFileUserBoolean;
    QuitActionType cQuitAction;

    std::map<std::string, std::unique_ptr<ProjectProperty>> cProperties;

    int cTime;

    // Project definition
    InputHandler cDefInputHandler;            /// Input handler of this project.
    Screen cDefScreen;                        /// Screen of this project.
    Editable cDefDefaultEditor;               /// Default editor of this project.
    Action cDefInitAction;
    Action cDefResetAction;
    Action cDefQuitAction;

    std::set<std::unique_ptr<Module>> cModules;                 /// Modules within this project.

    std::vector<std::unique_ptr<DOMNode>> loadResources(DOMNode& node, IOptions* options, const std::string& resourceDataPath);
    Module* getModule(const std::string& name);
    
    public:

    // TODO: Constructor for creating new projects
    Project(IApplication* application, std::function<void(bool)> onFinish, IAssetOverride* override = nullptr);

    /**
     * Constructor for loading an IsoRealms Project from an IsoRealms Project
     * file.  This is the most common Constructor used for loading games,
     * menus, tools, and other IsoRealms applications.
     * 
     * @param options The options passed to this Project.
     * @param onFinish Function to callback when the Project finishes.
     * @param override Specify to override assets of a Project with external ones.
     */
    Project(IApplication* application, IOptions* options, std::function<void(bool)> onFinish, IAssetOverride* override = nullptr);

    /***********************\
     * Implements IProject *
    \***********************/
    void mainThreadAlloc(std::function<void()> function) override;
    void mainThreadInit(std::function<void()> function) override;
    void init(std::function<void(IAssets*)> initialiser) override;
    ICallbackHandle* reset(std::function<void()> resetter) override;
    ICallbackHandle* updateRuntime(std::function<void(unsigned int)> dynamic) override;
    ICallbackHandle* updateEditing(std::function<void(unsigned int)> dynamic) override;
    void updateLater(std::function<void()> task) override;
    void remove(ICallbackHandle* callbackHandle) override;
    LuaState* const getLuaState() override;

    void setTime(int time);
    int getTime() const;
    
    /*****************************\
     * Implements IAssetRegistry *
    \*****************************/
    void                      add(I3DModelType*    asset, const std::string& id, const std::string& category) override;
    void                      add(IActionType*     asset, const std::string& id, const std::string& category) override;
    void                      add(IBinding*        asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IBoolean>* add(IBoolean*        asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IColour>*  add(IColour*         asset, const std::string& id, const std::string& category) override;
    void                      add(IEditable*       asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IFloat>*   add(IFloat*          asset, const std::string& id, const std::string& category) override;
    void                      add(IFont*           asset, const std::string& id, const std::string& category) override;
    void                      add(IInputHandler*   asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IInteger>* add(IInteger*        asset, const std::string& id, const std::string& category) override;
    IScreen*                  add(IScreen*         asset, const std::string& id, const std::string& category) override;
    void                      add(IProjectOptions* asset, const std::string& id, const std::string& category) override;
    void                      add(IAssets*         asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IString>*  add(IString*         asset, const std::string& id, const std::string& category) override;
    IStateNotifier<ITexture>* add(ITexture*        asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IVertex>*  add(IVertex*         asset, const std::string& id, const std::string& category) override;

    /****************************\
     * Implements IAssetRemover * 
    \****************************/   
    void remove(I3DModelType*    asset) override;
    void remove(IActionType*     asset) override;
    void remove(IAssets*         asset) override;
    void remove(IBinding*        asset) override;
    void remove(IBoolean*        asset) override;
    void remove(IColour*         asset) override;
    void remove(IEditable*       asset) override;
    void remove(IFloat*          asset) override;
    void remove(IFont*           asset) override;
    void remove(IInputHandler*   asset) override;
    void remove(IInteger*        asset) override;
    void remove(IScreen*         asset) override;
    void remove(IProjectOptions* asset) override;
    void remove(IString*         asset) override;
    void remove(ITexture*        asset) override;
    void remove(IVertex*         asset) override;
    
   
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    std::filesystem::file_time_type getLastWriteTime() override;
    void initMainThread() override;
    void requestQuit();
    bool input(sf::Event&);
    void render(float aspectRatio);
    void updateRuntime(unsigned int milliseconds);
    void updateRuntimeComplete();
    void updateEditing(unsigned int milliseconds);
    void executeCommand(const std::string& command);
    void finish(bool finishedByQuitRequest) override;
    bool isApplicationClosing();
    bool isFullScreen();
    DisplayResolution getDisplayResolution();
    void setDisplayResolution(DisplayResolution resolution, bool fullScreen);
    void reset();
    
    bool canSave();
    void save();
    void save(const std::string& file);
    bool isUserProject() override;

  //   void removeElement(IElement*);
    IAssetBrowser* getResourceManager();

    std::string getFilename();
    
    void loadModule(const std::string&);
    void unloadModule(const std::string&);
    std::set<IModule*> getModules();

    // Persistence scripting.  TODO: Maybe shouldn't be in the project
    DOMNodeWriter createNode(const std::string& name);
    DOMNode readNode(const std::string& name);
    std::string getUserDataPath();
    std::string getDataPath(bool user) override;
    void makeUserDataDirectory(const std::string& path);
    std::string getProjectPathPrefix(bool user);
    IEditable* getDefaultEditable() const override;
    
    std::string getProjectResourceDataPath(const std::string& file);


    /**********************\
     * Implements IAssets *
    \**********************/
    I3DModelType*    getModelType(     IAssetUser<I3DModelType>*    user, DOMNode& node,                                      bool required = true) override;
    IActionType*     getActionType(    IAssetUser<IActionType>*     user, DOMNode& node,                                      bool required = true) override;
    IAssets*         getAssets(        IAssetUser<IAssets>*         user, DOMNode& node,                                      bool required = true) override;
    IBinding*        getBinding(       IAssetUser<IBinding>*        user, DOMNode& node, IBindingRegistry* locals,            bool required = true) override;
    IBoolean*        getBoolean(       IAssetUser<IBoolean>*        user, DOMNode& node, IStateListener<IBoolean*>* listener, bool required = true) override;
    IColour*         getColour(        IAssetUser<IColour>*         user, DOMNode& node, IStateListener<IColour*>*  listener, bool required = true) override;
    IEditable*       getEditable(      IAssetUser<IEditable>*       user, DOMNode& node,                                      bool required = true) override;
    IFloat*          getFloat(         IAssetUser<IFloat>*          user, DOMNode& node, IStateListener<IFloat*>*   listener, bool required = true) override;
    IFont*           getFont(          IAssetUser<IFont>*           user, DOMNode& node,                                      bool required = true) override;
    IInputHandler*   getInputHandler(  IAssetUser<IInputHandler>*   user, DOMNode& node,                                      bool required = true) override;
    IInteger*        getInteger(       IAssetUser<IInteger>*        user, DOMNode& node, IStateListener<IInteger*>* listener, bool required = true) override;
    IScreen*         getScreen(        IAssetUser<IScreen>*         user, DOMNode& node,                                      bool required = true) override;
    IProjectOptions* getProjectOptions(IAssetUser<IProjectOptions>* user, DOMNode& node,                                      bool required = true) override;
    IString*         getString(        IAssetUser<IString>*         user, DOMNode& node, IStateListener<IString*>*  listener, bool required = true) override;
    ITexture*        getTexture(       IAssetUser<ITexture>*        user, DOMNode& node, IStateListener<ITexture*>* listener, bool required = true) override;
    IVertex*         getVertex(        IAssetUser<IVertex>*         user, DOMNode& node,                                      bool required = true) override;

    I3DModelType*    createLiteral3DModel(       IAssetUser<I3DModelType>*    user) override;
    IAction*         createLiteralAction(        IAssetUser<IAction>*         user) override;
    IActionType*     createLiteralActionType(    IAssetUser<IActionType>*     user) override;
    IAssets*         createLiteralAssets(        IAssetUser<IAssets>*         user) override;
    IBinding*        createLiteralBinding(       IAssetUser<IBinding>*        user) override;
    IBoolean*        createLiteralBoolean(       IAssetUser<IBoolean>*        user, const bool value) override;
    IColour*         createLiteralColour(        IAssetUser<IColour>*         user, const float red, const float green, const float blue, const float alpha) override;
    IEditable*       createLiteralEditable(      IAssetUser<IEditable>*       user) override;
    IFloat*          createLiteralFloat(         IAssetUser<IFloat>*          user, const float value) override;
    IFont*           createLiteralFont(          IAssetUser<IFont>*           user) override;
    IInputHandler*   createLiteralInputHandler(  IAssetUser<IInputHandler>*   user) override;
    IInteger*        createLiteralInteger(       IAssetUser<IInteger>*        user, const int value) override;
    IProjectOptions* createLiteralProjectOptions(IAssetUser<IProjectOptions>* user) override;
    IScreen*         createLiteralScreen(        IAssetUser<IScreen>*         user) override;
    IString*         createLiteralString(        IAssetUser<IString>*         user, const std::string& value) override;
    ITexture*        createLiteralTexture(       IAssetUser<ITexture>*        user) override;
    IVertex*         createLiteralVertex(        IAssetUser<IVertex>*         user, const float x, const float y, const float z) override;

    void release(IAssetUser<I3DModelType>*        user, I3DModelType*    asset) override;
    void release(IAssetUser<IAction>*             user, IAction*         asset) override;
    void release(IAssetUser<IActionType>*         user, IActionType*     asset) override;
    void release(IAssetUser<IAssets>*             user, IAssets*         asset) override;
    void release(IAssetUser<IBinding>*            user, IBinding*        asset) override;
    void release(IAssetUser<IBoolean>*            user, IBoolean*        asset) override;
    void release(IAssetUser<IColour>*             user, IColour*         asset) override;
    void release(IAssetUser<IEditable>*           user, IEditable*       asset) override;
    void release(IAssetUser<IFloat>*              user, IFloat*          asset) override;
    void release(IAssetUser<IFont>*               user, IFont*           asset) override;
    void release(IAssetUser<IInputHandler>*       user, IInputHandler*   asset) override;
    void release(IAssetUser<IInteger>*            user, IInteger*        asset) override;
    void release(IAssetUser<IProjectOptions>*     user, IProjectOptions* asset) override;
    void release(IAssetUser<IScreen>*             user, IScreen*         asset) override;
    void release(IAssetUser<IString>*             user, IString*         asset) override;
    void release(IAssetUser<ITexture>*            user, ITexture*        asset) override;
    void release(IAssetUser<IVertex>*             user, IVertex*         asset) override;

    bool renderAssetIcon() const override;

    IAction* getAction(IAssetUser<IAction>* user, DOMNode& node, const std::string& tag, IBindingRegistry* localAssetRegistry, const std::string& id) override;

    /*******************************\
     * Implements IAssetIdentifier *
    \*******************************/
    void save(DOMNodeWriter* node, const IActionType*     asset) const override;
    void save(DOMNodeWriter* node, const IAssets*         asset) const override;
    void save(DOMNodeWriter* node, const I3DModelType*    asset) const override;
    void save(DOMNodeWriter* node, const IBinding*        asset) const override;
    void save(DOMNodeWriter* node, const IBoolean*        asset) const override;
    void save(DOMNodeWriter* node, const IColour*         asset) const override;
    void save(DOMNodeWriter* node, const IEditable*       asset) const override;
    void save(DOMNodeWriter* node, const IFloat*          asset) const override;
    void save(DOMNodeWriter* node, const IFont*           asset) const override;
    void save(DOMNodeWriter* node, const IInputHandler*   asset) const override;
    void save(DOMNodeWriter* node, const IInteger*        asset) const override;
    void save(DOMNodeWriter* node, const IScreen*         asset) const override;
    void save(DOMNodeWriter* node, const IProjectOptions* asset) const override;
    void save(DOMNodeWriter* node, const IString*         asset) const override;
    void save(DOMNodeWriter* node, const ITexture*        asset) const override;
    void save(DOMNodeWriter* node, const IVertex*         asset) const override;

    /****************************\
     * Implements IAssetBrowser *
    \****************************/
    std::vector<std::pair<std::string, std::string>> getAllModelTypes() override;
    std::vector<std::pair<std::string, std::string>> getAllActionTypes() override;
    std::vector<std::pair<std::string, std::string>> getAllBooleans() override;
    std::vector<std::pair<std::string, std::string>> getAllColours() override;
    std::vector<std::pair<std::string, std::string>> getAllEditables() override;
    std::vector<std::pair<std::string, std::string>> getAllFloats() override;
    std::vector<std::pair<std::string, std::string>> getAllFonts() override;
    std::vector<std::pair<std::string, std::string>> getAllInputHandlers() override;
    std::vector<std::pair<std::string, std::string>> getAllIntegers() override;
    std::vector<std::pair<std::string, std::string>> getAllScreens() override;
    std::vector<std::pair<std::string, std::string>> getAllStrings() override;
    std::vector<std::pair<std::string, std::string>> getAllTextures() override;
    std::vector<std::pair<std::string, std::string>> getAllVertices() override;
    
    void addListener(IAssetListener<Project, I3DModelType>* listener) override;
    void addListener(IAssetListener<Project, IBoolean>*     listener) override;
    void addListener(IAssetListener<Project, IColour>*      listener) override;
    void addListener(IAssetListener<Project, IEditable>*    listener) override;
    void addListener(IAssetListener<Project, IFloat>*       listener) override;
    void addListener(IAssetListener<Project, IFont>*        listener) override;
    void addListener(IAssetListener<Project, IInteger>*     listener) override;
    void addListener(IAssetListener<Project, IScreen>*      listener) override;
    void addListener(IAssetListener<Project, IString>*      listener) override;
    void addListener(IAssetListener<Project, ITexture>*     listener) override;
    void addListener(IAssetListener<Project, IVertex>*      listener) override;
    
    bool renderActionIcon(      const std::string& id) const override;
    bool renderBooleanIcon(     const std::string& id) const override;
    bool renderColourIcon(      const std::string& id) const override;
    bool renderFloatIcon(       const std::string& id) const override;
    bool renderInputHandlerIcon(const std::string& id) const override;
    bool renderIntegerIcon(     const std::string& id) const override;
    bool renderModelIcon(       const std::string& id) const override;
    bool renderScreenIcon(      const std::string& id) const override;
    bool renderTextureIcon(     const std::string& id) const override;
    bool renderVertexIcon(      const std::string& id) const override;
    
    IProject* getProject() override;
    
    void renderScreen(IScreen* screen, float scale, float aspectRatio);
    void addScreenListener(IScreenListener* listener) override;
    void removeScreenListener(IScreenListener* listener) override;

    void addStateChangeListener(const IFloat* asset, IStateListener<IFloat*>* listener) override;

    IApplication* getApplication() override;

  //  std::vector<IDialogGenerator*> getDialogGenerators();
    
  //  void editResource(IResource*, IAssets*, IEditingContext*);

  //  void removeResource(IResource*, IAssets*);

    void setProperty(const std::string& property, const std::string& value) override;

    /*******************************\
     * Implements IBindingRegistry *
    \*******************************/
    IBinding* getBinding(const std::string& id) override;
    void saveBinding(DOMNodeWriter* node, const IBinding* binding) const override;
    void releaseBinding(const IBinding* asset) override;

    LiteralString cPropertyValue;
    LocalLuaBinding<IString> cPropertyValueBinding;

    virtual ~Project();
  };
}

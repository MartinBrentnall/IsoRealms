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
#pragma once

#include <functional>
#include <queue>
#include <set>
#include <string>

#include "Assets/Literal/LiteralBindingType.h"
#include "Assets/Registry/AssetClientManager.h"
#include "Assets/Registry/ScreenClientManager.h"
#include "Assets/Dummy/DummyAction.h"
#include "Assets/Dummy/DummyAssets.h"
#include "Assets/Dummy/DummyBinding.h"
#include "Assets/Dummy/DummyBindingType.h"
#include "Assets/Dummy/DummyEditable.h"
#include "Assets/Dummy/DummyFont.h"
#include "Assets/Dummy/DummyInputHandler.h"
#include "Assets/Dummy/DummyModel.h"
#include "Assets/Dummy/DummyProjectOptions.h"
#include "Assets/Dummy/DummyScreen.h"
#include "Assets/Dummy/DummyTexture.h"
#include "Assets/Providers/AssetConvertedBinding.h"
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
#include "Editing.h"
#include "IPropertyOwner.h"
#include "IResourceData.h"
#include "Lua.h"
#include "Options/LocalOptions.h"
#include "Options/Options.h"
#include "ProjectFile.h"
#include "Types.h"

namespace IsoRealms {
  template<class TYPE> struct AssetContainerTraits;
  
  class IAssetOverride;
  class IModule;
  class IScreenListener;
  class Module;

  /**
   * The Project is the root object of an IsoRealms game or application.  The
   * Project acts as a container for the assets of the application, and
   * provides control over execution of the application.
   */
  class Project : public IProject,
                  public IBindingRegistry,
                  public IResourceData,
                  public IPropertyOwner,
                  public IActionClient {
    public:

    // TODO: Constructor for creating new projects
    Project(IApplication& application, std::function<void(bool)> onFinish, IAssetOverride* override = nullptr);

    /**
     * Constructor for loading an IsoRealms Project from an IsoRealms Project
     * file.  This is the most common Constructor used for loading games,
     * menus, tools, and other IsoRealms applications.
     * 
     * @param options The options passed to this Project.
     * @param onFinish Function to callback when the Project finishes.
     * @param override Specify to override assets of a Project with external ones.
     */
    Project(IApplication& application, IOptions& options, std::function<void(bool)> onFinish, IAssetOverride* override = nullptr);

    /***********************\
     * Implements IProject *
    \***********************/
    void mainThreadAlloc(std::function<void()> function) override;
    void mainThreadInit(std::function<void()> function) override;
    void mainThreadCleanUp(std::function<void()> function) override;
    void init(std::function<void(IAssets&)> initialiser) override;
    void updateLater(std::function<void()> task) override;
    LuaState* const getLuaState() override;

    void setTime(int time);
    int getTime() const;

    template <typename TYPE, typename THING> IStateNotifier<TYPE>* add(THING* asset, const std::string& id, const std::string& category) {
      return AssetContainerTraits<TYPE>::get(*this).add(asset, id, category, true);
    }

    template <typename TYPE, typename THING> void remove(THING* asset) {
      AssetContainerTraits<TYPE>::get(*this).hasReadOnlyReferences(asset);
    }
    
    template <typename TYPE, typename THING> bool hasReadOnlyReferences(THING* asset) const {
      return AssetContainerTraits<TYPE>::get(*this).hasReadOnlyReferences(asset);
    }
   
    template <typename TYPE, typename THING> void overrideReadOnlyReferences(THING* asset) {
      AssetContainerTraits<TYPE>::get(*this).overrideReadOnlyReferences(asset, &cProjectFile);
    }
    
    template <typename TYPE> void release(IAssetUser<TYPE>* user, TYPE* asset) {
      AssetContainerTraits<TYPE>::get(*this).release(user, asset);
    }

    template <typename TYPE> std::string getID(const TYPE* asset) const {
      return AssetContainerTraits<TYPE>::get(*this).getID(asset);
    }

    template <typename TYPE> void save(JSONObject object, const TYPE* asset) const {
      AssetContainerTraits<TYPE>::get(*this).save(object, asset);
    }

    template <typename TYPE> std::vector<std::string> getAll() const {
      return AssetContainerTraits<TYPE>::get(*this).getAll();
    }
    
    template <typename TYPE> bool renderIcon(const std::string& id) const {
      return AssetContainerTraits<TYPE>::get(*this).renderIcon(id);
    }

    template <typename TYPE> bool isConfigurable(const std::string& id) const {
      return AssetContainerTraits<TYPE>::get(*this).hasConfiguration(id);
    }
    
    template <typename TYPE, typename OWNER> TYPE* createDefault(IAssetUser<TYPE>* user, OWNER& owner) {
      return AssetContainerTraits<TYPE>::get(*this).getDefault(user, owner);
    }

    IBoolean* createLiteralBoolean(IAssetUser<IBoolean>* user, IResourceData& owner, const bool value);
    IColour*  createLiteralColour( IAssetUser<IColour>*  user, IResourceData& owner, const float red, const float green, const float blue, const float alpha);
    IFloat*   createLiteralFloat(  IAssetUser<IFloat>*   user, IResourceData& owner, const float value);
    IInteger* createLiteralInteger(IAssetUser<IInteger>* user, IResourceData& owner, const int value);
    IString*  createLiteralString( IAssetUser<IString>*  user, IResourceData& owner, const std::string& value);
    IVertex*  createLiteralVertex( IAssetUser<IVertex>*  user, IResourceData& owner, const float x, const float y, const float z);


    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    std::filesystem::file_time_type getLastWriteTime() override;
    void initMainThread() override;
    void requestQuit();
    bool input(sf::Event& event);
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
    void save(ProjectFile& file);
    void save();
    void save(const std::string& file);
    bool isLoading() const;
    bool isUserProject() override;

    std::string getFilename();
    Project& getAssetManager() override;
    IActionClient& getDummyActionClient() override;
    const PropertyData& getPropertyData(const std::string& key) const override;
    std::string getPropertyName(const std::string& key) const override;
    std::string getPropertyDescription(const std::string& key) const override;
    std::unique_ptr<IProperty> createPropertyNativeFloat(const std::string& metadataKey, std::function<float()> getter, std::function<bool(float)> setter, std::function<void()> removeFunction = nullptr) override;
    bool isReadOnly() const override;
    void setOwner(ProjectFile* file) override;
    IResourceData& getResourceData() override;
    IBindingRegistry* getBindingRegistry() override;
    ProjectFile* getFile() override;
    
    Module* loadModule(const std::string& name);
    void unloadModule(const std::string& name);
    std::set<IModule*> getModules();
    std::vector<std::string> getUnusedModuleNames() const;

    // Persistence scripting.  TODO: Maybe shouldn't be in the project
    JSONDocument createDocument();
    JSONDocument openDocument(const std::string& name);
    std::string getUserDataPath();
    std::string getDataPath(bool user) override;
    void makeUserDataDirectory(const std::string& path);
    void renameUserDataDirectory(const std::string& path, const std::string& oldName, const std::string& newName);
  
    std::string getProjectPathPrefix(bool user);
    IEditable* getDefaultEditable() override;
    
    std::string getProjectResourceDataPath(const std::string& file);


    /**********************\
     * Implements IAssets *
    \**********************/
    IAction*         getAction(        IAssetUser<IAction>*         user, JSONObject object, IActionClient& owner,                                      bool required = true) override;
    IAssets*         getAssets(        IAssetUser<IAssets>*         user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IBinding*        getBinding(       IAssetUser<IBinding>*        user, JSONObject object, IActionClient& owner,                                      bool required = true) override;
    IBindingType*    getBindingType(   IAssetUser<IBindingType>*    user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IBoolean*        getBoolean(       IAssetUser<IBoolean>*        user, JSONObject object, IResourceData& owner, IStateListener<IBoolean*>* listener, bool required = true) override;
    IColour*         getColour(        IAssetUser<IColour>*         user, JSONObject object, IResourceData& owner, IStateListener<IColour*>*  listener, bool required = true) override;
    IEditable*       getEditable(      IAssetUser<IEditable>*       user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IFloat*          getFloat(         IAssetUser<IFloat>*          user, JSONObject object, IResourceData& owner, IStateListener<IFloat*>*   listener, bool required = true) override;
    IFont*           getFont(          IAssetUser<IFont>*           user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IInputHandler*   getInputHandler(  IAssetUser<IInputHandler>*   user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IInteger*        getInteger(       IAssetUser<IInteger>*        user, JSONObject object, IResourceData& owner, IStateListener<IInteger*>* listener, bool required = true) override;
    IModel*          getModel(         IAssetUser<IModel>*          user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IScreen*         getScreen(        IAssetUser<IScreen>*         user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IProjectOptions* getProjectOptions(IAssetUser<IProjectOptions>* user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IString*         getString(        IAssetUser<IString>*         user, JSONObject object, IResourceData& owner, IStateListener<IString*>*  listener, bool required = true) override;
    ITexture*        getTexture(       IAssetUser<ITexture>*        user, JSONObject object, IResourceData& owner, IStateListener<ITexture*>* listener, bool required = true) override;
    IVertex*         getVertex(        IAssetUser<IVertex>*         user, JSONObject object, IResourceData& owner,                                      bool required = true) override;

    IAction*         getAction(        IAssetUser<IAction>*         user, const std::string& id, IActionClient& owner) override;
    IAssets*         getAssets(        IAssetUser<IAssets>*         user, const std::string& id, IResourceData& owner) override;
    IBinding*        getBinding(       IAssetUser<IBinding>*        user, const std::string& id, IActionClient& owner) override;
    IBindingType*    getBindingType(   IAssetUser<IBindingType>*    user, const std::string& id, IResourceData& owner) override;
    IBoolean*        getBoolean(       IAssetUser<IBoolean>*        user, const std::string& id, IResourceData& owner, IStateListener<IBoolean*>* listener) override;
    IColour*         getColour(        IAssetUser<IColour>*         user, const std::string& id, IResourceData& owner, IStateListener<IColour*>*  listener) override;
    IEditable*       getEditable(      IAssetUser<IEditable>*       user, const std::string& id, IResourceData& owner) override;
    IFloat*          getFloat(         IAssetUser<IFloat>*          user, const std::string& id, IResourceData& owner, IStateListener<IFloat*>*   listener) override;
    IFont*           getFont(          IAssetUser<IFont>*           user, const std::string& id, IResourceData& owner) override;
    IInputHandler*   getInputHandler(  IAssetUser<IInputHandler>*   user, const std::string& id, IResourceData& owner) override;
    IInteger*        getInteger(       IAssetUser<IInteger>*        user, const std::string& id, IResourceData& owner, IStateListener<IInteger*>* listener) override;
    IModel*          getModel(         IAssetUser<IModel>*          user, const std::string& id, IResourceData& owner) override;
    IProjectOptions* getProjectOptions(IAssetUser<IProjectOptions>* user, const std::string& id, IResourceData& owner) override;
    IScreen*         getScreen(        IAssetUser<IScreen>*         user, const std::string& id, IResourceData& owner) override;
    IString*         getString(        IAssetUser<IString>*         user, const std::string& id, IResourceData& owner, IStateListener<IString*>*  listener) override;
    ITexture*        getTexture(       IAssetUser<ITexture>*        user, const std::string& id, IResourceData& owner, IStateListener<ITexture*>* listener) override;
    IVertex*         getVertex(        IAssetUser<IVertex>*         user, const std::string& id, IResourceData& owner) override;

    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    Project& getProject() override;

    IScreen* getScreenProxy(IScreen* screen);
    void addScreenListener(IScreenListener* listener) override;
    void removeScreenListener(IScreenListener* listener) override;

    void addStateChangeListener(const IFloat* asset, IStateListener<IFloat*>* listener) override;

    IApplication& getApplication() override;

    std::vector<std::unique_ptr<IProperty>> getProperties();

    void setProperty(const std::string& property, const std::string& value) override;

    /*******************************\
     * Implements IBindingRegistry *
    \*******************************/
    IBinding* getBinding(const std::string& id) override;
    void saveBinding(JSONObject object, const IBinding* binding) const override;
    void releaseBinding(const IBinding* asset) override;

    /****************************\
     * Implements IResourceData * TODO: Should these be here???
    \****************************/
    std::string getPath(const std::string& file, bool user) const override;
    void makeUserDataDirectory() override;
    bool isIncluded() const override;

    bool isProcessingInput() override;
    void postponeAction(IAction* action) override;

    virtual ~Project();
    
    template <class TYPE> friend struct AssetContainerTraits;

    private:
    static const std::string JSON_ACTION;
    static const std::string JSON_EDITOR;
    static const std::string JSON_FILENAME;
    static const std::string JSON_ID;
    static const std::string JSON_INCLUDE;
    static const std::string JSON_INITIALISATION;
    static const std::string JSON_INPUT;
    static const std::string JSON_LOCAL;
    static const std::string JSON_MODULES;
    static const std::string JSON_NAME;
    static const std::string JSON_PROJECT;
    static const std::string JSON_PROPERTIES;
    static const std::string JSON_QUIT;
    static const std::string JSON_RESET;
    static const std::string JSON_SCREEN;
    static const std::string JSON_START;
    static const std::string JSON_USER;

    static const std::string CATEGORY_CONVERSIONS;
    static const std::string CATEGORY_FIXED;
    static const std::string CATEGORY_LOCAL;

    class Filename : public IString {
      public:
      Filename(Project& parent);

      private:
      Project& cParent;

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;
    };

    class FileUser : public IBoolean {
      public:
      FileUser(Project& parent);

      private:
      Project& cParent;

      /***********************\
       * Implements IBoolean *
      \***********************/
      bool getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;
    };

    class QuitAction : public IAction {
      public:
      QuitAction(Project& parent);

      /**********************\
       * Implements IAction *
      \**********************/
      void execute() override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      Project& cParent;
    };

    class ProjectProperty {
      public:
      ProjectProperty(Project& parent, JSONObject object, File* ownerProject);
      void setValue(const std::string& value);
      void save(JSONArray& object, File* savingProject, const std::string& id) const;
      bool isOwnedBy(File* project);

      private:
      Action cChangeAction;
      File* cOwnerProject;
    };

    template <class OWNER, class TYPE> class ProjectAsset {
      public:
      ProjectAsset(OWNER& owner) :
                cAsset(owner),
                cOwnerProject(nullptr) {
      }

      void init(JSONObject object, const std::string& tag, File* ownerProject) {
        if (object.hasMember(tag) && cOwnerProject == nullptr) {
          cAsset.init(object, tag);
          cOwnerProject = ownerProject;
        }
      }

      TYPE* operator*() {
        return &cAsset;
      }

      void save(JSONObject object, const std::string& tag, File* savingProject) {
        if (cOwnerProject == savingProject) {
          cAsset.save(object, tag);
        }
      }

      std::unique_ptr<IProperty> getProperty(const std::string& name) {
        return std::make_unique<PropertyAsset<TYPE>>(PropertyData(name, "TODO"), cAsset);
      }

      private:
      TYPE cAsset;
      File* cOwnerProject;
    };

    IApplication& cApplication; /// Host application of this project.

    const IAssetOverride* const cAssetOverride; /// External asset override for this project.
    LuaState cLuaState;                         /// Lua State for this project.
    LuaBinding<Project> cLuaBinding;          /// Project interface for actions and scripting.
    bool cResourcesLoaded;                    /// Indicates that resource loading from modules has completed.
    bool cLoading;

    // Callbacks
    std::queue<std::function<void()>> cMainThreadAllocTasks;         /// Allocation tasks to be perfomed on the main thread during initialisation.
    std::queue<std::function<void()>> cMainThreadInitTasks;          /// initialisation tasks to be performed on the main thread.
    std::vector<std::function<void(IAssets&)>> cInitialisers;        /// Asset initialisation tasks.  Called after assets have been registered.
    std::queue<std::function<void()>> cUpdateTasks;                  /// Postponed update functions.  Called after other update functions.

    // Action execution control
    bool cProcessingInput;                   /// Flag is set when processing input to indicate that actions should be postponed.
    std::vector<IAction*> cPostponedActions; /// List of postponed actions to be executed at next update cycle.
    bool cRuntimeUpdatingRuntime;            /// Flag is set when update cycle is being performed.
    bool cRuntimeResetPostponed;             /// Falg is set when a reset is postponed to be performed upon completion of the update cycle.























    std::map<std::string, std::vector<IModelInstance*>> cInstantiatedModels;

    // Asset registries.
    AssetClientManager<IActionClient, IAction>         cActions;
    AssetClientManager<IActionClient, IBinding>        cBindings;
    AssetClientManager<IResourceData, IBindingType>    cBindingTypes; // Note: Contents of this module is set by modules (i.e. NOT configurable!)
    AssetClientManager<IResourceData, IBoolean>        cBooleans;
    AssetClientManager<IResourceData, IColour>         cColours;
    AssetClientManager<IResourceData, IEditable>       cEditables;
    AssetClientManager<IResourceData, IFloat>          cFloats;
    AssetClientManager<IResourceData, IFont>           cFonts;
    AssetClientManager<IResourceData, IInputHandler>   cInputHandlers;
    AssetClientManager<IResourceData, IInteger>        cIntegers;
    AssetClientManager<IResourceData, IModel>          cModels;
    ScreenClientManager                                cScreens;
    AssetClientManager<IResourceData, IProjectOptions> cProjectOptions;
    AssetClientManager<IResourceData, IAssets>         cAssets;
    AssetClientManager<IResourceData, IString>         cStrings;
    AssetClientManager<IResourceData, ITexture>        cTextures;
    AssetClientManager<IResourceData, IVertex>         cVertices;

    // Literal and dummy asset providers.
    AssetLiteralDummy<IActionClient, IAction,         DummyAction>         cLiteralProviderAction;
    AssetLiteralDummy<IActionClient, IBinding,        DummyBinding>        cLiteralProviderBinding;
    AssetLiteralDummy<IResourceData, IBindingType,    DummyBindingType>    cLiteralProviderBindingType;
    AssetLiteralBoolean                                                    cLiteralProviderBoolean;
    AssetLiteralColour                                                     cLiteralProviderColour;
    AssetLiteralDummy<IResourceData, IEditable,       DummyEditable>       cLiteralProviderEditable;
    AssetLiteralFloat                                                      cLiteralProviderFloat;
    AssetLiteralDummy<IResourceData, IFont,           DummyFont>           cLiteralProviderFont;
    AssetLiteralDummy<IResourceData, IInputHandler,   DummyInputHandler>   cLiteralProviderInputHandler;
    AssetLiteralInteger                                                    cLiteralProviderInteger;
    AssetLiteralDummy<IResourceData, IModel,          DummyModel>          cLiteralProviderModel;
    AssetLiteralDummy<IResourceData, IScreen,         DummyScreen>         cLiteralProviderScreen;
    AssetLiteralDummy<IResourceData, IProjectOptions, DummyProjectOptions> cLiteralProviderProjectOptions;
    AssetLiteralDummy<IResourceData, IAssets,         DummyAssets>         cLiteralProviderAssets;
    AssetLiteralString                                                     cLiteralProviderString;
    AssetLiteralDummy<IResourceData, ITexture,        DummyTexture>        cLiteralProviderTexture;
    AssetLiteralVertex                                                     cLiteralProviderVertex;

    // Built-in binding types.
    LiteralBindingType cBindingTypeAction;
    LiteralBindingType cBindingTypeBoolean;
    LiteralBindingType cBindingTypeColour;
    LiteralBindingType cBindingTypeFloat;
    LiteralBindingType cBindingTypeFont;
    LiteralBindingType cBindingTypeInputHandler;
    LiteralBindingType cBindingTypeInteger;
    LiteralBindingType cBindingTypeProject;
    LiteralBindingType cBindingTypeProjectOptions;
    LiteralBindingType cBindingTypeScreen;
    LiteralBindingType cBindingTypeString;
    LiteralBindingType cBindingTypeVertex;

    // Conversion asset providers.
    AssetConvertedBinding<IActionClient, Action>         cConversionProviderActionToBinding;
    AssetConvertedBinding<IResourceData, Boolean>        cConversionProviderBooleanToBinding;
    AssetConvertedBinding<IResourceData, Colour>         cConversionProviderColourToBinding;
    AssetConvertedBinding<IResourceData, Float>          cConversionProviderFloatToBinding;
    AssetConvertedBinding<IResourceData, Font>           cConversionProviderFontToBinding;
    AssetConvertedBinding<IResourceData, InputHandler>   cConversionProviderInputHandlerToBinding;
    AssetConvertedBinding<IResourceData, Integer>        cConversionProviderIntegerToBinding;
    AssetConvertedBinding<IResourceData, ProjectOptions> cConversionProviderProjectOptionsToBinding;
    AssetConvertedBinding<IResourceData, Assets>         cConversionProviderProjectToBinding;
    AssetConvertedBinding<IResourceData, Screen>         cConversionProviderScreenToBinding;
    AssetConvertedBinding<IResourceData, String>         cConversionProviderStringToBinding;
    AssetConvertedBinding<IResourceData, Vertex>         cConversionProviderVertexToBinding;

    AssetConvertedString<Integer>         cConversionProviderIntegerToString;
    AssetConvertedString<Float>           cConversionProviderFloatToString;
    AssetConvertedProjectToString         cConversionProviderProjectToString;

    AssetConvertedProjectToInteger        cConversionProviderProjectToInteger;

    // Local binding support.
    AssetLocalBinding cLocalProviderBinding;

    std::function<void(bool)> cFunctionNotifyComplete;
    ProjectFile cProjectFile;
    Filename cFilenameString;
    FileUser cFileUserBoolean;
    QuitAction cQuitAction;

    std::map<std::string, std::unique_ptr<ProjectProperty>> cProperties;

    int cTime;

    // Project definition
    ProjectAsset<IResourceData, InputHandler> cDefInputHandler;  /// Input handler of this project.
    ProjectAsset<IResourceData, Screen>       cDefScreen;        /// Screen of this project.
    ProjectAsset<IResourceData, Editable>     cDefDefaultEditor; /// Default editor of this project.
    ProjectAsset<IActionClient, Action>       cDefInitAction;    /// Action to execute after project has loaded.
    ProjectAsset<IActionClient, Action>       cDefResetAction;   /// Action to execute after project has completed.
    ProjectAsset<IActionClient, Action>       cDefStartAction;   /// Action to execute upon starting project (following reset)
    ProjectAsset<IActionClient, Action>       cDefQuitAction;    /// Action to execute upon project quit.

    LiteralString cPropertyValue;
    LocalLuaBinding<IString> cPropertyValueBinding;

    std::vector<std::unique_ptr<Module>> cModules;                 /// Modules within this project.

    PropertyData cMissingData;

    bool isModuleLoaded(const std::string& name) const;
    std::vector<std::unique_ptr<JSONDocument>> loadResources(IOptions& options, ProjectFile& file);
    Module* getModule(const std::string& name);
    void saveFile(ProjectFile& file);
  };

  template<> struct AssetContainerTraits<IAction>         {template<class PROJECT> static auto& get(PROJECT& project) {return project.cActions;       }};
  template<> struct AssetContainerTraits<IBinding>        {template<class PROJECT> static auto& get(PROJECT& project) {return project.cBindings;      }};
  template<> struct AssetContainerTraits<IBindingType>    {template<class PROJECT> static auto& get(PROJECT& project) {return project.cBindingTypes;  }};
  template<> struct AssetContainerTraits<IBoolean>        {template<class PROJECT> static auto& get(PROJECT& project) {return project.cBooleans;      }};
  template<> struct AssetContainerTraits<IColour>         {template<class PROJECT> static auto& get(PROJECT& project) {return project.cColours;       }};
  template<> struct AssetContainerTraits<IEditable>       {template<class PROJECT> static auto& get(PROJECT& project) {return project.cEditables;     }};
  template<> struct AssetContainerTraits<IFloat>          {template<class PROJECT> static auto& get(PROJECT& project) {return project.cFloats;        }};
  template<> struct AssetContainerTraits<IFont>           {template<class PROJECT> static auto& get(PROJECT& project) {return project.cFonts;         }};
  template<> struct AssetContainerTraits<IInputHandler>   {template<class PROJECT> static auto& get(PROJECT& project) {return project.cInputHandlers; }};
  template<> struct AssetContainerTraits<IInteger>        {template<class PROJECT> static auto& get(PROJECT& project) {return project.cIntegers;      }};
  template<> struct AssetContainerTraits<IModel>          {template<class PROJECT> static auto& get(PROJECT& project) {return project.cModels;        }};
  template<> struct AssetContainerTraits<IScreen>         {template<class PROJECT> static auto& get(PROJECT& project) {return project.cScreens;       }};
  template<> struct AssetContainerTraits<IProjectOptions> {template<class PROJECT> static auto& get(PROJECT& project) {return project.cProjectOptions;}};
  template<> struct AssetContainerTraits<IAssets>         {template<class PROJECT> static auto& get(PROJECT& project) {return project.cAssets;        }};
  template<> struct AssetContainerTraits<IString>         {template<class PROJECT> static auto& get(PROJECT& project) {return project.cStrings;       }};
  template<> struct AssetContainerTraits<ITexture>        {template<class PROJECT> static auto& get(PROJECT& project) {return project.cTextures;      }};
  template<> struct AssetContainerTraits<IVertex>         {template<class PROJECT> static auto& get(PROJECT& project) {return project.cVertices;      }};
}

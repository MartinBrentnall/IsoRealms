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

#include <filesystem>
#include <functional>
#include <queue>
#include <set>
#include <string>

#include "PropertyMaker.h"

#include "Assets/Client/ResourceOwner.h"
#include "Assets/Registry/AssetClientManager.h"
#include "Assets/Registry/ActionRegistry.h"
#include "Assets/Registry/BooleanRegistry.h"
#include "Assets/Registry/BindingRegistry.h"
#include "Assets/Registry/BindingTypeRegistry.h"
#include "Assets/Registry/ColourRegistry.h"
#include "Assets/Registry/EditableRegistry.h"
#include "Assets/Registry/FloatRegistry.h"
#include "Assets/Registry/FontRegistry.h"
#include "Assets/Registry/InputHandlerRegistry.h"
#include "Assets/Registry/IntegerRegistry.h"
#include "Assets/Registry/ModelRegistry.h"
#include "Assets/Registry/ScreenRegistry.h"
#include "Assets/Registry/StringRegistry.h"
#include "Assets/Registry/TextureRegistry.h"
#include "Assets/Registry/VertexRegistry.h"
#include "Assets/Providers/AssetLiteralDummy.h"
#include "Editing.h"
#include "IResourceData.h"
#include "IStateListener.h"
#include "Lua.h"
#include "Options/Options.h"
#include "OwnedAsset.h"
#include "ProjectFile.h"
#include "ProjectLaunchConfiguration.h"
#include "Types.h"

namespace IsoRealms {
  template<class TYPE> struct AssetContainerTraits;
  
  class IModule;
  class IScreenListener;
  class Module;
  class PropertyMaker;

  /**
   * The Project is the root object of an IsoRealms game or application.  The
   * Project acts as a container for the assets of the application, and
   * provides control over execution of the application.
   */
  class Project : public IBindingRegistry,
                  public IResourceData,
                  public IActionClient {
    public:
    
    // Functions used by a runtime host.
    Project(Application& application, std::function<void(bool)> onFinish);
    Project(Application& application, std::function<void(bool)> onFinish, const std::string& file, bool user);
    virtual ~Project();
    void reset();
    void reset(Options& options);
    void reset(ProjectLaunchConfiguration* configuration);
    bool input(sf::Event& event);
    void updateRuntime(unsigned int milliseconds);
    void updateRuntimeComplete();
    void render(float aspectRatio);
    void requestQuit();

    // Functions used by an editor host.
    void updateEditing(unsigned int milliseconds);
    void save();
    void save(const std::string& file);
    bool isUser();
    void getProperties(PropertyMaker& propertyMaker);
    IEditable* getDefaultEditable();
    IScreen* getScreenProxy(IScreen* screen);
    
    // Functions used by project members.
    Module* loadModule(const std::string& name);
    void unloadModule(const std::string& name);
    std::set<IModule*> getModules();
    std::vector<std::string> getUnusedModuleNames() const;
    bool isLaunchConfigurationNameUsed(const std::string& name, ProjectLaunchConfiguration* launchConfiguration) const;
    std::string makeLaunchConfigurationName() const;
    int getLaunchConfigurationCount() const;
    const ProjectLaunchConfiguration* getLaunchConfiguration(int index);
    
    // Functions used for resource ownership.
    std::vector<std::string> getProjectFileNames() const;
    ProjectFile* getProjectFile();
    ProjectFile* getProjectFile(const std::string& id);
    
    // Function used by modules.
    Application& getApplication();
    const Application& getApplication() const;
    LuaState& getLuaState();
    void init(std::function<void()> initialiser);
    void updateLater(std::function<void()> task);
    std::string getProjectPathPrefix(bool user);
    std::filesystem::file_time_type getLastWriteTime();
    void makeUserDataDirectory(const std::string& path);
    void renameUserDataDirectory(const std::string& path, const std::string& oldName, const std::string& newName);
    void addScreenListener(IScreenListener* listener);
    void removeScreenListener(IScreenListener* listener);
    void addStateChangeListener(const IFloat* asset, IStateListener<IFloat*>* listener);
    
    // Functions used by module client assets.
    bool isLoading() const;
    void execute(IAction& action);
    
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
      AssetContainerTraits<TYPE>::get(*this).overrideReadOnlyReferences(asset, &cDefProjectFileStructure);
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

    template <typename TYPE, typename OWNER> TYPE* getAsset(IAssetUser<TYPE>* user, const std::string& id, OWNER& owner, IStateListener<TYPE*>* listener = nullptr) {
      return AssetContainerTraits<TYPE>::get(*this).get(user, owner, id, listener);
    }

    template <typename TYPE, typename OWNER> TYPE* getAsset(IAssetUser<TYPE>* user, JSONObject object, OWNER& owner, IStateListener<TYPE*>* listener = nullptr, bool required = true) {
      return AssetContainerTraits<TYPE>::get(*this).get(user, owner, object, listener, required);
    }

    IBoolean* createLiteralBoolean(IAssetUser<IBoolean>* user, IResourceData& owner, bool value);
    IColour*  createLiteralColour( IAssetUser<IColour>*  user, IResourceData& owner, float red, float green, float blue, float alpha);
    IFloat*   createLiteralFloat(  IAssetUser<IFloat>*   user, IResourceData& owner, float value);
    IInteger* createLiteralInteger(IAssetUser<IInteger>* user, IResourceData& owner, int value);
    IString*  createLiteralString( IAssetUser<IString>*  user, IResourceData& owner, const std::string& value);
    IVertex*  createLiteralVertex( IAssetUser<IVertex>*  user, IResourceData& owner, float x, float y, float z);

    /***********************\
     * Scripting interface *
    \***********************/
    void finish(bool finishedByQuitRequest);
    std::string getUserDataPath();
    std::string getDataPath(bool user);

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
    bool isReadOnly() const override;
    void setOwner(ProjectFile* file) override;
    Project& getProject() override;
    const Project& getProject() const override;
    Project& getAssetManager() override;
    IActionClient& getDummyActionClient() override;
    
    /****************************\
     * Implements IActionClient * TODO: Should these be here???
    \****************************/
    IResourceData& getResourceData() override;
    IBindingRegistry* getBindingRegistry() override;
    
    template <class TYPE> friend struct AssetContainerTraits;

    private:
    static const std::string JSON_EDITOR;
    static const std::string JSON_INCLUDE;
    static const std::string JSON_INITIALISATION;
    static const std::string JSON_INPUT;
    static const std::string JSON_LAUNCH_CONFIGURATIONS;
    static const std::string JSON_LOCAL;
    static const std::string JSON_MODULES;
    static const std::string JSON_NAME;
    static const std::string JSON_PROJECT;
    static const std::string JSON_QUIT;
    static const std::string JSON_RESET;
    static const std::string JSON_SCREEN;
    static const std::string JSON_START;

    class QuitAction : public IAction {
      public:
      QuitAction(Project& parent);

      /**********************\
       * Implements IAction *
      \**********************/
      void execute() override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;

      private:
      Project& cParent;
    };

    // External interfaces.
    Application& cApplication;                        /// Host application of this Project.
    std::function<void(bool)> cFunctionNotifyComplete; /// Function to be notified of Project completion.

    // Asset registries.
    ActionRegistry       cActions;
    BindingRegistry      cBindings;
    BindingTypeRegistry  cBindingTypes; // Note: Contents of this module is set by modules (i.e. NOT configurable!)
    BooleanRegistry      cBooleans;
    ColourRegistry       cColours;
    EditableRegistry     cEditables;
    FloatRegistry        cFloats;
    FontRegistry         cFonts;
    InputHandlerRegistry cInputHandlers;
    IntegerRegistry      cIntegers;
    ModelRegistry        cModels;
    ScreenRegistry       cScreens;
    StringRegistry       cStrings;
    TextureRegistry      cTextures;
    VertexRegistry       cVertices;

    // Definition data.
    std::vector<std::unique_ptr<Module>>                     cDefModules;
    ProjectFile                                              cDefProjectFileStructure;
    std::vector<std::unique_ptr<ProjectLaunchConfiguration>> cDefTestLaunchConfigurations;
    OwnedAsset<IResourceData, InputHandler>                  cDefInputHandler;
    OwnedAsset<IResourceData, Screen>                        cDefScreen;
    OwnedAsset<IResourceData, Editable>                      cDefDefaultEditor;
    OwnedAsset<IActionClient, Action>                        cDefInitAction;
    OwnedAsset<IActionClient, Action>                        cDefResetAction;
    OwnedAsset<IActionClient, Action>                        cDefStartAction;
    OwnedAsset<IActionClient, Action>                        cDefQuitAction;

    // Scripting support.
    LuaState cLuaState;                       /// Lua State for this project.
    LuaBinding<Application> cLuaBindingApplication;
    LuaBinding<Project> cLuaBindingProject;   /// Project interface for actions and scripting.
    LocalLuaBinding<Options> cOptionsBinding;

    // Callbacks
    std::vector<std::function<void()>> cInitialisers;        /// Asset initialisation tasks.  Called after assets have been registered.
    std::queue<std::function<void()>> cUpdateTasks;          /// Postponed update functions.  Called after other update functions.

    // Runtime state of the Project.
    bool cResourcesLoaded;                   /// Indicates that resource loading from modules has completed.
    bool cLoading;                           /// Indicates that loading is in progress.
    bool cProcessingInput;                   /// Flag is set when processing input to indicate that actions should be postponed.
    std::vector<IAction*> cPostponedActions; /// List of postponed actions to be executed at next update cycle.
    bool cRuntimeUpdatingRuntime;            /// Flag is set when update cycle is being performed.
    bool cRuntimeResetPostponed;             /// Falg is set when a reset is postponed to be performed upon completion of the update cycle.

    // Assets to expose Project data.
    QuitAction cQuitAction;

    // Private functions.
    std::vector<std::unique_ptr<JSONDocument>> loadResources(ProjectFile& file);
    Module* getModule(const std::string& name);
    void updateTasks();
    void save(ProjectFile& file);
  };

  template<> struct AssetContainerTraits<IAction>       {template<class PROJECT> static auto& get(PROJECT& project) {return project.cActions;      }};
  template<> struct AssetContainerTraits<IBinding>      {template<class PROJECT> static auto& get(PROJECT& project) {return project.cBindings;     }};
  template<> struct AssetContainerTraits<IBindingType>  {template<class PROJECT> static auto& get(PROJECT& project) {return project.cBindingTypes; }};
  template<> struct AssetContainerTraits<IBoolean>      {template<class PROJECT> static auto& get(PROJECT& project) {return project.cBooleans;     }};
  template<> struct AssetContainerTraits<IColour>       {template<class PROJECT> static auto& get(PROJECT& project) {return project.cColours;      }};
  template<> struct AssetContainerTraits<IEditable>     {template<class PROJECT> static auto& get(PROJECT& project) {return project.cEditables;    }};
  template<> struct AssetContainerTraits<IFloat>        {template<class PROJECT> static auto& get(PROJECT& project) {return project.cFloats;       }};
  template<> struct AssetContainerTraits<IFont>         {template<class PROJECT> static auto& get(PROJECT& project) {return project.cFonts;        }};
  template<> struct AssetContainerTraits<IInputHandler> {template<class PROJECT> static auto& get(PROJECT& project) {return project.cInputHandlers;}};
  template<> struct AssetContainerTraits<IInteger>      {template<class PROJECT> static auto& get(PROJECT& project) {return project.cIntegers;     }};
  template<> struct AssetContainerTraits<IModel>        {template<class PROJECT> static auto& get(PROJECT& project) {return project.cModels;       }};
  template<> struct AssetContainerTraits<IScreen>       {template<class PROJECT> static auto& get(PROJECT& project) {return project.cScreens;      }};
  template<> struct AssetContainerTraits<IString>       {template<class PROJECT> static auto& get(PROJECT& project) {return project.cStrings;      }};
  template<> struct AssetContainerTraits<ITexture>      {template<class PROJECT> static auto& get(PROJECT& project) {return project.cTextures;     }};
  template<> struct AssetContainerTraits<IVertex>       {template<class PROJECT> static auto& get(PROJECT& project) {return project.cVertices;     }};
}

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

#include "IsoRealms.h"

#include "Assets/Registry/BoundaryTypeRegistry.h"
#include "Assets/Registry/CameraRegistry.h"
#include "Assets/Registry/PhysicalObjectTypeRegistry.h"
#include "Assets/Registry/SurfacePatternRegistry.h"
#include "Assets/Registry/WallPatternRegistry.h"
#include "Assets/Registry/WorldEditorToolRegistry.h"
#include "Assets/Registry/ZoneObjectTypeTraitRegistry.h"
#include "Assets/Registry/ZoneViewTypeRegistry.h"

#include "AlienType/AlienType.h"
#include "BoundaryHandler/BoundaryHandler.h"
#include "CollisionHandler/CollisionHandler.h"
#include "IsoRealms/Assets/IEventBindings.h"
#include "LiftType/LiftType.h"
#include "ModelCycler/ModelCycler.h"
#include "PickUpType/PickUpType.h"
#include "PlayerType/PlayerType.h"
#include "TerrainState/TerrainState.h"
#include "TerrainType/TerrainType.h"
#include "ThemeSet/ThemeSet.h"
#include "World/Editor/DeleteTool.h"
#include "World/Editor/PropertiesTool.h"
#include "World/Editor/ZoneTool.h"
#include "World/World.h"
#include "WorldView/WorldView.h"
#include "ZoneObjectType/ZoneObjectType.h"
#include "ZoneType/ZoneType.h"

namespace IsoRealms::Equilibria {
  template <typename TYPE> struct AssetContainerTraits;
  template <typename TYPE> struct ComponentContainerTraits;

  class Equilibria : public IModuleHandle {
    public:
    
    // Module constants.
    inline static const double DEFAULT_VIEW_ANGLE_PITCH = -90.0 + std::atan(1.0 / std::sqrt(2.0)) * (180.0 / M_PI);
    inline static const double DEFAULT_VIEW_ANGLE_YAW   = -45.0;

    // Module utility functions.
    static int getTerrainSlope(int startA, int startB, int endA, int endB) {
      return endA - startA == endB - startB ? endA - startA : 0;
    }
  
    static float getTerrainSlope(float startA, float startB, float endA, float endB) {
      return endA - startA == endB - startB ? endA - startA : 0.0f;
    }

    // Module constructor.
    Equilibria(Project& project, IComponentTypeRegistry& registry);
    const Metadata& getMetadata(const std::string& key) const;

    void init(std::function<void()> initialiser);
  
    // Interface access (used by all).
    Project& getProject() const;
    Equilibria& getAssetManager();

    bool isReadOnly() const; // TODO: Probably shouldn't be here.
    void setOwner(ProjectFile* owner); // TODO: Probably shouldn't be here.

    std::vector<IBoundaryType*>       getAllBoundaryTypeObjects();
    std::vector<IPhysicalObjectType*> getAllPhysicalObjectTypeObjects();

    template <typename TYPE, typename THING> IStateNotifier* add(THING* asset, const std::string& id, const std::string& category = "") {
      return AssetContainerTraits<TYPE>::get(*this).add(asset, id, category, true);
    }

    template <typename TYPE, typename THING> void remove(THING* asset) {
      AssetContainerTraits<TYPE>::get(*this).remove(asset);
    }

    template <typename TYPE> TYPE* get(IComponentUser<TYPE>* user, const std::string& id) {
      return ComponentContainerTraits<TYPE>::get(*this).getComponentForClient(user, id);
    }

    template <typename TYPE> void release(IComponentUser<TYPE>* user, TYPE* component) {
      ComponentContainerTraits<TYPE>::get(*this).release(user, component);
    }

    template <typename TYPE> std::string getComponentID(const TYPE* component) const {
      return ComponentContainerTraits<TYPE>::get(*this).getID(component);
    }

    template <typename TYPE> std::vector<std::string> getAvailableComponents() const {
      return ComponentContainerTraits<TYPE>::get(*this).getAvailableComponents();
    }

    template <typename TYPE> bool isReadOnly(const TYPE* component) const {
      return ComponentContainerTraits<TYPE>::get(*this).isReadOnly(component);
    }

    template <typename TYPE> void setOwner(const TYPE* component, ProjectFile* owner) {
      ComponentContainerTraits<TYPE>::get(*this).setOwner(component, owner);
    }

    template <typename TYPE> bool hasReadOnlyComponentReferences(const TYPE* component) const {
      return ComponentContainerTraits<TYPE>::get(*this).hasReadOnlyReferences(component);
    }

    template <typename TYPE> void overrideReadOnlyComponentReferences(const TYPE* component) {
      ComponentContainerTraits<TYPE>::get(*this).overrideReadOnlyReferences(component);
    }

    template <typename TYPE> bool isUsedInReadOnlyWorld(const TYPE& component) const {
      for (World* mWorld : cComponentWorld) {
        if (cComponentWorld.isReadOnly(mWorld) && mWorld->isUsed(component)) {
          std::cout << "isUsedInReadOnlyWorld: " << cComponentWorld.getID(mWorld) << std::endl;
          return true;
        }
      }
      return false;
    }

    template <typename TYPE> void overrideReadOnlyWorlds(const TYPE& component) {
      for (World* mWorld : cComponentWorld) {
        if (cComponentWorld.isReadOnly(mWorld) && mWorld->isUsed(component)) {
          std::cout << "overrideReadOnlyWorlds: " << cComponentWorld.getID(mWorld) << std::endl;
          cComponentWorld.setOwner(mWorld, cProject.getProjectFile());
        }
      }
    }

    std::string getComponentID() const;
    std::string getComponentName() const;
    
    // Component removal.
    void removeAll(AlienType*      type);
    void removeAll(LiftType*       type);
    void removeAll(PickUpType*     type);
    void removeAll(PlayerType*     type);
    void removeAll(TerrainType*    type);
    void removeAll(ZoneObjectType* type);
    void removeAll(ZoneType*       type);

    // Module type removal.
    template <typename TYPE> void release(IAssetUser<TYPE>* user, TYPE* asset) {
      AssetContainerTraits<TYPE>::get(*this).release(user, asset);
    }

    template <typename TYPE> TreeItemInfo getTreeItemInfo(const TYPE* asset) const {
      return AssetContainerTraits<TYPE>::get(*this).getTreeItemInfo(asset);
    }

    template <typename TYPE> const Metadata& getPropertyMetadata(const TYPE* asset) const {
      return AssetContainerTraits<TYPE>::get(*this).getPropertyMetadata(asset);
    }

    template <typename TYPE> void save(JSONObject object, const TYPE* asset) const {
      AssetContainerTraits<TYPE>::get(*this).save(object, asset);
    }

    template <typename TYPE> void forEachEntry(const std::function<void(const TreeItemInfo&)>& f) const {
      AssetContainerTraits<TYPE>::get(*this).forEachEntry(f);
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

    template <typename TYPE, typename OWNER> TYPE* getAsset(IAssetUser<TYPE>* user, const std::string& id, OWNER& owner, IStateListener* listener = nullptr) {
      return AssetContainerTraits<TYPE>::get(*this).get(user, owner, id, listener);
    }

    template <typename TYPE, typename OWNER> TYPE* getAsset(IAssetUser<TYPE>* user, JSONObject object, OWNER& owner, IStateListener* listener = nullptr, bool required = true) {
      return AssetContainerTraits<TYPE>::get(*this).get(user, owner, object, listener, required);
    }

    template <typename TYPE, typename THING> bool hasReadOnlyReferences(THING* asset) const {
      return AssetContainerTraits<TYPE>::get(*this).hasReadOnlyReferences(asset);
    }
   
    template <typename TYPE, typename THING> void overrideReadOnlyReferences(THING* asset) {
      AssetContainerTraits<TYPE>::get(*this).overrideReadOnlyReferences(asset, cProject.getProjectFile());
    }
    

    void physicalObjectTypeChanged(CollisionHandler* handler, const IPhysicalObjectType* oldPhysicalObjectType, const IPhysicalObjectType* newPhysicalObjectType);
    
    // Event handling.
    void added(BoundaryHandler* handler);
    void added(CollisionHandler* handler);
    void added(IBoundaryType* boundaryType);
    void added(IPhysicalObjectType* physicalObjectType);
    void added(Zone* zone);
    void removed(BoundaryHandler* handler);
    void removed(CollisionHandler* handler);
    void removed(IBoundaryType* boundaryType);
    void removed(IPhysicalObjectType* physicalObjectType);
    void removed(Zone* zone);

    // Misc functions.
    std::vector<ConditionElement*> getTerrainStateConditionElements();
    TerrainState* getTerrainState(IBoolean* input);
    
    // Editing functions.
    void setAllThemesInUse(bool inUse);
    void setNextTheme();
    void setPreviousTheme();
    void applyDefaultThemes();

    // TODO: Replace with local bindings.
    void bind(Alien* alien);
    void bind(Player* player);
    void bind(Wall* wall);
    void bind1(Zone* zone);
    void bind2(Zone* zone);

    /***********************\
     * Scripting Interface *
    \***********************/
    void setPause(bool pause);
    bool isPaused();

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void registerAssets(ComponentAssetRegistry& assets) override;
    void updateInputs(unsigned int milliseconds) override;
    void updateRuntime(unsigned int milliseconds) override;
    void updateEditing(unsigned int milliseconds) override;
    void reset() override;

    void remove(IWorldEditorTool* asset);
    
    void remove(WorldEditorTool& asset);

    void addZoneBinding(IBinding* binding1, IBinding* binding2, const std::string& id);

    IBinding* getZonePropertyBinding(std::map<std::string, IBinding*>& bindings, const std::string& id);

    IBinding* getBindingPlayer(const std::string& id);
    std::string getBindingIDPlayer(const IBinding* binding) const;
    void getTreeItemsPlayer(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const;

    IBinding* getBindingZone(const std::string& id);
    std::string getBindingIDZone(const IBinding* binding) const;
    void getTreeItemsZone(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const;

    IBinding* getBindingPickUp(const std::string& id);
    std::string getBindingIDPickUp(const IBinding* binding) const;
    void getTreeItemsPickUp(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const;
    
    IBinding* getBindingWallBounce(const std::string& id);
    std::string getBindingIDWallBounce(const IBinding* binding) const;
    void getTreeItemsWallBounce(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const;

    /****************************************\
     * Implements IStateListener<ITexture*> *
    \****************************************/
    void stateChanged(ITexture* asset);

    private:

    // Fixed tool names.
    inline static const std::string TOOL_DELETE      = "DeleteTool";
    inline static const std::string TOOL_PROPERTIES  = "PropertiesTool";
    inline static const std::string TOOL_COPY_ZONE   = "ZoneCopyTool";
    inline static const std::string TOOL_MOVE_ZONE   = "ZoneMoveTool";
    inline static const std::string TOOL_DELETE_ZONE = "ZoneDeleteTool";

    inline static const std::string BIND_TO_PLAYER  = "Player";
    inline static const std::string BIND_TO_TERRAIN = "Terrain";
    inline static const std::string BIND_TO_WALL    = "Wall";
    inline static const std::string BIND_TO_ZONE    = "Zone";

    // External interfaces.
    Project& cProject;
    IComponentTypeRegistry& cModule;

    // Basic world editing tools.
    DeleteTool     cToolDelete;
    PropertiesTool cToolProperties;
    ZoneTool       cToolCopyZone;
    ZoneTool       cToolMoveZone;
    ZoneTool       cToolDeleteZone;
    
    // Equilibria Assets.
    BoundaryTypeRegistry        cBoundaryTypes;
    CameraRegistry              cCameras;
    PhysicalObjectTypeRegistry  cPhysicalObjectTypes;
    SurfacePatternRegistry      cSurfacePatterns;
    WallPatternRegistry         cWallPatterns;
    WorldEditorToolRegistry     cWorldEditorTools;
    ZoneObjectTypeTraitRegistry cZoneObjectTypeTraits;
    ZoneViewTypeRegistry        cZoneViewTypes;

    // Component type definitions.
    ComponentTypeDefinition<Equilibria, AlienType>        cComponentAlien;
    ComponentTypeDefinition<Equilibria, BoundaryHandler>  cComponentBoundaryHandler;
    ComponentTypeDefinition<Equilibria, CollisionHandler> cComponentCollisionHandler;
    ComponentTypeDefinition<Equilibria, LiftType>         cComponentLift;
    ComponentTypeDefinition<Equilibria, ModelCycler>      cComponentModelCycler;
    ComponentTypeDefinition<Equilibria, PickUpType>       cComponentPickUp;
    ComponentTypeDefinition<Equilibria, PlayerType>       cComponentPlayer;
    ComponentTypeDefinition<Equilibria, TerrainType>      cComponentTerrain;
    ComponentTypeDefinition<Equilibria, TerrainState>     cComponentTerrainState;
    ComponentTypeDefinition<Equilibria, ThemeSet>         cComponentThemeSet;
    ComponentTypeDefinition<Equilibria, World>            cComponentWorld;
    ComponentTypeDefinition<Equilibria, WorldView>        cComponentWorldView;
    ComponentTypeDefinition<Equilibria, ZoneType>         cComponentZone;
    ComponentTypeDefinition<Equilibria, ZoneObjectType>   cComponentZoneObject;

    // Runtime data.
    bool cRuntimePaused = false;
    
    class EquilibriaBindingType : public IBindingType {
      public:
      EquilibriaBindingType(const std::string& typeName, const std::string& rootFolder);

      /**************************\
       * Implement IBindingType *
      \**************************/
      std::string getBindingTypeID() const override;
      std::string getBindingTypeRootFolder() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(IPropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;

      private:
      std::string cTypeName;
      std::string cRootFolder;
    };

    class EventBindingsPlayerWallBounce : public IEventBindings {
      public:
      EventBindingsPlayerWallBounce(Equilibria& equilibria);

      /*****************************\
       * Implements IEventBindings *
      \*****************************/
      IBinding* getBinding(const std::string& id) override;
      void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
      void releaseBinding(const IBinding* asset) override;

      private:
      Equilibria& cEquilibria;
    };

    EquilibriaBindingType cBindingTypeTerrainState{"TerrainState", "Equilibria/Terrain States"};

    // Event bindings.
    LuaBinding<Player> cRuntimeParameterPlayer;
    LuaBinding<Wall>   cRuntimeParameterWall;
    LuaBinding<Zone>   cRuntimeParameterZone1;
    LuaBinding<Zone>   cRuntimeParameterZone2;

    // Scripting support.
    LuaBinding<Equilibria> cLuaBinding;
    std::map<std::string, IBinding*> cRuntimeZoneBindings1;
    std::map<std::string, IBinding*> cRuntimeZoneBindings2;

    template <class TYPE> friend struct AssetContainerTraits;
    template <class TYPE> friend struct ComponentContainerTraits;
  };

  template<> struct AssetContainerTraits<IBoundaryType>        {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cBoundaryTypes;       }};
  template<> struct AssetContainerTraits<ICamera>              {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cCameras;             }};
  template<> struct AssetContainerTraits<IPhysicalObjectType>  {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cPhysicalObjectTypes; }};
  template<> struct AssetContainerTraits<ISurfacePattern>      {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cSurfacePatterns;     }};
  template<> struct AssetContainerTraits<IWallPattern>         {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cWallPatterns;        }};
  template<> struct AssetContainerTraits<IWorldEditorTool>     {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cWorldEditorTools;    }};
  template<> struct AssetContainerTraits<IZoneObjectTypeTrait> {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cZoneObjectTypeTraits;}};
  template<> struct AssetContainerTraits<IZoneViewType>        {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cZoneViewTypes;       }};

  template<> struct ComponentContainerTraits<AlienType>          {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentAlien;             }};
  template<> struct ComponentContainerTraits<BoundaryHandler>    {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentBoundaryHandler;   }};
  template<> struct ComponentContainerTraits<CollisionHandler>   {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentCollisionHandler;  }};
  template<> struct ComponentContainerTraits<LiftType>           {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentLift;              }};
  template<> struct ComponentContainerTraits<ModelCycler>        {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentModelCycler;      }};
  template<> struct ComponentContainerTraits<PickUpType>         {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentPickUp;            }};
  template<> struct ComponentContainerTraits<PlayerType>         {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentPlayer;            }};
  template<> struct ComponentContainerTraits<TerrainType>        {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentTerrain;           }};
  template<> struct ComponentContainerTraits<TerrainState>       {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentTerrainState;      }};
  template<> struct ComponentContainerTraits<ThemeSet>           {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentThemeSet;          }};
  template<> struct ComponentContainerTraits<World>              {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentWorld;             }};
  template<> struct ComponentContainerTraits<WorldView>          {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentWorldView;         }};
  template<> struct ComponentContainerTraits<ZoneType>           {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentZone;              }};
  template<> struct ComponentContainerTraits<ZoneObjectType>     {template <typename EQUILIBRIA> static auto& get(EQUILIBRIA& equilibria) {return equilibria.cComponentZoneObject;        }};
}

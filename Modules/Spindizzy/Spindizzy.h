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
#include "Ball/Ball.h"
#include "BoundaryHandler/BoundaryHandler.h"
#include "C64LiftGraphics/C64LiftGraphics.h"
#include "C64TerrainGraphics/C64TerrainGraphics.h"
#include "CollisionHandler/CollisionHandler.h"
#include "DebrisChunk/DebrisChunk.h"
#include "Gyroscope/Gyroscope.h"
#include "IBindingIdentifier.h"
#include "Jewel/Jewel.h"
#include "LiftType/LiftType.h"
#include "ModelCycler/ModelCycler.h"
#include "PickUpType/PickUpType.h"
#include "PlayerType/PlayerType.h"
#include "TerrainState/TerrainState.h"
#include "TerrainType/TerrainType.h"
#include "ThemeSet/ThemeSet.h"
#include "Top/Top.h"
#include "World/Editor/DeleteTool.h"
#include "World/Editor/PropertiesTool.h"
#include "World/Editor/ZoneTool.h"
#include "World/World.h"
#include "WorldView/WorldView.h"
#include "ZoneObjectType/ZoneObjectType.h"
#include "ZoneType/ZoneType.h"

namespace IsoRealms::Spindizzy {
  template <typename TYPE> struct AssetContainerTraits;
  template <typename TYPE> struct ResourceContainerTraits;

  class Spindizzy : public IModuleHandle,
                    public IBindingRegistry {
    public:
    
    // Module constants.
    static const double DEFAULT_VIEW_ANGLE_PITCH;
    static const double DEFAULT_VIEW_ANGLE_YAW;

    // Module utility functions.
    static int getTerrainSlope(int startA, int startB, int endA, int endB) {
      return endA - startA == endB - startB ? endA - startA : 0;
    }
  
    static float getTerrainSlope(float startA, float startB, float endA, float endB) {
      return endA - startA == endB - startB ? endA - startA : 0.0f;
    }

    // Module constructor.
    Spindizzy(Project& project, IResourceTypeRegistry& registry);
    const Metadata& getMetadata(const std::string& key) const;

    void init(std::function<void()> initialiser);
  
    // Interface access (used by all).
    Project& getProject() const;
    Spindizzy& getAssetManager();

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

    template <typename TYPE> TYPE* get(IResourceUser<TYPE>* user, const std::string& id) {
      return ResourceContainerTraits<TYPE>::get(*this).getResourceForClient(user, id);
    }

    template <typename TYPE> void release(IResourceUser<TYPE>* user, TYPE* resource) {
      ResourceContainerTraits<TYPE>::get(*this).release(user, resource);
    }

    template <typename TYPE> std::string getResourceID(const TYPE* resource) const {
      return ResourceContainerTraits<TYPE>::get(*this).getID(resource);
    }

    template <typename TYPE> std::vector<std::string> getAvailableResources() const {
      return ResourceContainerTraits<TYPE>::get(*this).getAvailableResources();
    }

    template <typename TYPE> bool isReadOnly(const TYPE* resource) const {
      return ResourceContainerTraits<TYPE>::get(*this).isReadOnly(resource);
    }

    template <typename TYPE> void setOwner(const TYPE* resource, ProjectFile* owner) {
      ResourceContainerTraits<TYPE>::get(*this).setOwner(resource, owner);
    }

    template <typename TYPE> bool hasReadOnlyResourceReferences(const TYPE* resource) const {
      return ResourceContainerTraits<TYPE>::get(*this).hasReadOnlyReferences(resource);
    }

    template <typename TYPE> void overrideReadOnlyResourceReferences(const TYPE* resource) {
      ResourceContainerTraits<TYPE>::get(*this).overrideReadOnlyReferences(resource);
    }

    template <typename TYPE> bool isUsedInReadOnlyWorld(const TYPE& resource) const {
      for (World* mWorld : cResourceWorld) {
        if (cResourceWorld.isReadOnly(mWorld) && mWorld->isUsed(resource)) {
          std::cout << "isUsedInReadOnlyWorld: " << cResourceWorld.getID(mWorld) << std::endl;
          return true;
        }
      }
      return false;
    }

    template <typename TYPE> void overrideReadOnlyWorlds(const TYPE& resource) {
      for (World* mWorld : cResourceWorld) {
        if (cResourceWorld.isReadOnly(mWorld) && mWorld->isUsed(resource)) {
          std::cout << "overrideReadOnlyWorlds: " << cResourceWorld.getID(mWorld) << std::endl;
          cResourceWorld.setOwner(mWorld, cProject.getProjectFile());
        }
      }
    }

    std::string getResourceID() const;
    std::string getResourceName() const;
    
    // Resource removal.
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
    void bindFallDistance(IFloat* value);
    void bindLaunchLocation(IVertex* value);
    void bindLaunchMomentum(IVertex* value);

    /***********************\
     * Scripting Interface *
    \***********************/
    void setPause(bool pause);
    bool isPaused();

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void registerAssets(ResourceAssetRegistry& assets) override;
    void updateInputs(unsigned int milliseconds) override;
    void updateRuntime(unsigned int milliseconds) override;
    void updateEditing(unsigned int milliseconds) override;
    void reset() override;

    void remove(IWorldEditorTool* asset);
    
    void remove(WorldEditorTool& asset);

    void addZoneBinding(IBinding* binding1, IBinding* binding2, const std::string& id);

    IBinding* getZoneBinding(const std::string& id);
    IBinding* getZoneBinding2(const std::string& id);
    std::string getZoneBindingID1(const IBinding* binding) const;
    std::string getZoneBindingID2(const IBinding* binding) const;
    
    void setBindingIdentifier(const IBindingIdentifier* bindingIdentifier) const;
    
    // TODO: Replace with local bindings.
    /*******************************\
     * Implements IBindingRegistry *
    \*******************************/
    IBinding* getBinding(const std::string& id) override;
    void saveBinding(JSONObject object, const IBinding* binding) const override;
    void releaseBinding(const IBinding* asset) override;

    /****************************************\
     * Implements IStateListener<ITexture*> *
    \****************************************/
    void stateChanged(ITexture* asset);

    private:
    
    // Resource strings.
    static const std::string RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS;
    static const std::string RESOURCE_CATEGORY_SPINDIZZY_GRAPHICS;
    static const std::string RESOURCE_CATEGORY_SPINDIZZY_LOGIC;

    // JSON members.
    static const std::string JSON_LOCAL;

    // Fixed tool names.
    static const std::string TOOL_DELETE;
    static const std::string TOOL_PROPERTIES;
    static const std::string TOOL_COPY_ZONE;
    static const std::string TOOL_MOVE_ZONE;
    static const std::string TOOL_DELETE_ZONE;

    static const std::string BIND_TO_FALL_DISTANCE;
    static const std::string BIND_TO_LAUNCH_LOCATION;
    static const std::string BIND_TO_LAUNCH_MOMENTUM;
    static const std::string BIND_TO_PLAYER;
    static const std::string BIND_TO_WALL;
    static const std::string BIND_TO_ZONE;

    // External interfaces.
    Project& cProject;
    IResourceTypeRegistry& cModule;

    // Basic world editing tools.
    DeleteTool     cToolDelete;
    PropertiesTool cToolProperties;
    ZoneTool       cToolCopyZone;
    ZoneTool       cToolMoveZone;
    ZoneTool       cToolDeleteZone;
    
    // Spindizzy Assets.
    BoundaryTypeRegistry        cBoundaryTypes;
    CameraRegistry              cCameras;
    PhysicalObjectTypeRegistry  cPhysicalObjectTypes;
    SurfacePatternRegistry      cSurfacePatterns;
    WallPatternRegistry         cWallPatterns;
    WorldEditorToolRegistry     cWorldEditorTools;
    ZoneObjectTypeTraitRegistry cZoneObjectTypeTraits;
    ZoneViewTypeRegistry        cZoneViewTypes;

    // Resource type definitions.
    ResourceTypeDefinition<Spindizzy, AlienType>          cResourceAlien;
    ResourceTypeDefinition<Spindizzy, Ball>               cResourceBall;
    ResourceTypeDefinition<Spindizzy, BoundaryHandler>    cResourceBoundaryHandler;
    ResourceTypeDefinition<Spindizzy, C64LiftGraphics>    cResourceC64LiftGraphics;
    ResourceTypeDefinition<Spindizzy, C64TerrainGraphics> cResourceC64TerrainGraphics;
    ResourceTypeDefinition<Spindizzy, CollisionHandler>   cResourceCollisionHandler;
    ResourceTypeDefinition<Spindizzy, DebrisChunk>        cResourceDebrisChunk;
    ResourceTypeDefinition<Spindizzy, Gyroscope>          cResourceGyroscope;
    ResourceTypeDefinition<Spindizzy, Jewel>              cResourceJewel;
    ResourceTypeDefinition<Spindizzy, LiftType>           cResourceLift;
    ResourceTypeDefinition<Spindizzy, ModelCycler>        cResourceModelCycler;
    ResourceTypeDefinition<Spindizzy, PickUpType>         cResourcePickUp;
    ResourceTypeDefinition<Spindizzy, PlayerType>         cResourcePlayer;
    ResourceTypeDefinition<Spindizzy, TerrainType>        cResourceTerrain;
    ResourceTypeDefinition<Spindizzy, TerrainState>       cResourceTerrainState;
    ResourceTypeDefinition<Spindizzy, ThemeSet>           cResourceThemeSet;
    ResourceTypeDefinition<Spindizzy, Top>                cResourceTop;
    ResourceTypeDefinition<Spindizzy, World>              cResourceWorld;
    ResourceTypeDefinition<Spindizzy, WorldView>          cResourceWorldView;
    ResourceTypeDefinition<Spindizzy, ZoneType>           cResourceZone;
    ResourceTypeDefinition<Spindizzy, ZoneObjectType>     cResourceZoneObject;

    // Runtime data.
    bool cRuntimePaused;
    
    class SpindizzyBindingType : public IBindingType {
      public:
      SpindizzyBindingType(const std::string& typeName);

      /**************************\
       * Implement IBindingType *
      \**************************/
      std::string getBindingTypeID() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(IPropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;

      private:
      std::string cTypeName;
    };

    SpindizzyBindingType cBindingTypeTerrainState;

    // Action parameters (TODO remove these)
    LuaBinding<Alien>   cRuntimeParameterAlien;
    LuaBinding<IFloat>  cRuntimeParameterFallDistance;
    LuaBinding<IVertex> cRuntimeParameterLaunchLocation;
    LuaBinding<IVertex> cRuntimeParameterLaunchMomentum;
    LuaBinding<Player>  cRuntimeParameterPlayer;
    LuaBinding<Wall>    cRuntimeParameterWall;
    LuaBinding<Zone>    cRuntimeParameterZone1;
    LuaBinding<Zone>    cRuntimeParameterZone2;

    // Scripting support.
    LuaBinding<Spindizzy> cLuaBinding;
    std::map<std::string, IBinding*> cRuntimeZoneBindings1;
    std::map<std::string, IBinding*> cRuntimeZoneBindings2;
    mutable const IBindingIdentifier* cRuntimeLocalBindingIdentifier;

    template <class TYPE> friend struct AssetContainerTraits;
    template <class TYPE> friend struct ResourceContainerTraits;
  };

  template<> struct AssetContainerTraits<IBoundaryType>        {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cBoundaryTypes;       }};
  template<> struct AssetContainerTraits<ICamera>              {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cCameras;             }};
  template<> struct AssetContainerTraits<IPhysicalObjectType>  {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cPhysicalObjectTypes; }};
  template<> struct AssetContainerTraits<ISurfacePattern>      {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cSurfacePatterns;     }};
  template<> struct AssetContainerTraits<IWallPattern>         {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cWallPatterns;        }};
  template<> struct AssetContainerTraits<IWorldEditorTool>     {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cWorldEditorTools;    }};
  template<> struct AssetContainerTraits<IZoneObjectTypeTrait> {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cZoneObjectTypeTraits;}};
  template<> struct AssetContainerTraits<IZoneViewType>        {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cZoneViewTypes;       }};

  template<> struct ResourceContainerTraits<AlienType>          {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceAlien;             }};
  template<> struct ResourceContainerTraits<Ball>               {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceBall;              }};
  template<> struct ResourceContainerTraits<BoundaryHandler>    {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceBoundaryHandler;   }};
  template<> struct ResourceContainerTraits<C64LiftGraphics>    {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceC64LiftGraphics;   }};
  template<> struct ResourceContainerTraits<C64TerrainGraphics> {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceC64TerrainGraphics;}};
  template<> struct ResourceContainerTraits<CollisionHandler>   {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceCollisionHandler;  }};
  template<> struct ResourceContainerTraits<DebrisChunk>        {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceDebrisChunk;       }};
  template<> struct ResourceContainerTraits<Gyroscope>          {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceGyroscope;         }};
  template<> struct ResourceContainerTraits<Jewel>              {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceJewel;             }};
  template<> struct ResourceContainerTraits<LiftType>           {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceLift;              }};
  template<> struct ResourceContainerTraits<ModelCycler>        {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceModelCycler;       }};
  template<> struct ResourceContainerTraits<PickUpType>         {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourcePickUp;            }};
  template<> struct ResourceContainerTraits<PlayerType>         {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourcePlayer;            }};
  template<> struct ResourceContainerTraits<TerrainType>        {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceTerrain;           }};
  template<> struct ResourceContainerTraits<TerrainState>       {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceTerrainState;      }};
  template<> struct ResourceContainerTraits<ThemeSet>           {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceThemeSet;          }};
  template<> struct ResourceContainerTraits<Top>                {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceTop;               }};
  template<> struct ResourceContainerTraits<World>              {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceWorld;             }};
  template<> struct ResourceContainerTraits<WorldView>          {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceWorldView;         }};
  template<> struct ResourceContainerTraits<ZoneType>           {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceZone;              }};
  template<> struct ResourceContainerTraits<ZoneObjectType>     {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceZoneObject;        }};
}

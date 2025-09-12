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

#include "Assets/Fixed/BoundaryType/BoundaryTypeDummy.h"
#include "Assets/Fixed/Camera/CameraGameplay.h"
#include "Assets/Fixed/Camera/CameraLinked.h"
#include "Assets/Fixed/Camera/CameraOverview.h"
#include "Assets/Fixed/Camera/CameraTransitional.h"
#include "Assets/Fixed/Camera/CameraVariant.h"
#include "Assets/Fixed/PhysicalObjectType/PhysicalObjectTypeDummy.h"
#include "Assets/Fixed/SurfacePattern/SurfacePatternOutline.h"
#include "Assets/Fixed/SurfacePattern/SurfacePatternTile.h"
#include "Assets/Fixed/SurfacePattern/SurfacePatternSplitVariant.h"
#include "Assets/Fixed/WallPattern/WallPatternCap.h"
#include "Assets/Fixed/WallPattern/WallPatternOutline.h"
#include "Assets/Fixed/WallPattern/WallPatternTile.h"
#include "Assets/Fixed/WorldEditorTool/WorldEditorToolDummy.h"
#include "Assets/Fixed/ZoneViewType/ZoneViewTypeActual.h"
#include "Assets/Fixed/ZoneViewType/ZoneViewTypeOverview.h"
#include "Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitBoundary.h"
#include "Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitCellLocation.h"
#include "Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitChaser.h"
#include "Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitModel.h"
#include "Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitMovable.h"
#include "Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitPhysics.h"
#include "Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitSpinner.h"

#include "AlienType/AlienType.h"
#include "Ball/Ball.h"
#include "BoundaryHandler/BoundaryHandler.h"
#include "C64LiftGraphics/C64LiftGraphics.h"
#include "C64TerrainGraphics/C64TerrainGraphics.h"
#include "CollisionHandler/CollisionHandler.h"
#include "DebrisChunk/DebrisChunk.h"
#include "Gyroscope/Gyroscope.h"
#include "IBindingIdentifier.h"
#include "ISpindizzyRegistry.h"
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

  class Spindizzy : public IModuleHandle,
                    public ISpindizzyRegistry,
                    public IBindingRegistry,
                    public IStateListener<ITexture*> {
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

    // Resource retrieval.
    AlienType*        getAlienType(      const std::string& id) const;
    LiftType*         getLiftType(       const std::string& id) const;
    PickUpType*       getPickUpType(     const std::string& id) const;
    PlayerType*       getPlayerType(     const std::string& id) const;
    TerrainType*      getTerrainType(    const std::string& id) const;
    ThemeSet*         getThemeSet(       const std::string& id) const;
    World*            getWorld(          const std::string& id) const;
    WorldView*        getWorldView(      const std::string& id) const;
    ZoneObjectType*   getZoneObjectType( const std::string& id) const;
    ZoneType*         getZoneType(       const std::string& id) const;

    // Resource identification.
    std::string getID(const AlienType*      asset) const;
    std::string getID(const LiftType*       asset) const;
    std::string getID(const PickUpType*     asset) const;
    std::string getID(const PlayerType*     asset) const;
    std::string getID(const TerrainType*    asset) const;
    std::string getID(const ThemeSet*       asset) const;
    std::string getID(const World*          asset) const;
    std::string getID(const WorldView*      asset) const;
    std::string getID(const ZoneObjectType* asset) const;
    std::string getID(const ZoneType*       asset) const;

    std::vector<IBoundaryType*>       getAllBoundaryTypeObjects();
    std::vector<IPhysicalObjectType*> getAllPhysicalObjectTypeObjects();

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

    // Event handling.
    void added(BoundaryHandler* handler);
    void added(CollisionHandler* handler);
    void added(IBoundaryType* boundaryType);
    void added(IPhysicalObjectType* physicalObjectType);
    void added(Zone* zone);
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
    void bind(Zone* zone);
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

    /*********************************\
     * Implements ISpindizzyRegistry *
    \*********************************/
    void add(IBoundaryType*       asset, const std::string& id) override;
    void add(IPhysicalObjectType* asset, const std::string& id) override;
    void add(IWorldEditorTool*    asset, const std::string& id) override;

    void remove(IWorldEditorTool* asset);
    
    void remove(WorldEditorTool& asset);

//     void add(IAssetProvider<IBoundaryType>*        provider, const std::string& id) override;
//     void add(IAssetProvider<ICamera>*              provider, const std::string& id) override;
//     void add(IAssetProvider<IPhysicalObjectType>*  provider, const std::string& id) override;
//     void add(IAssetProvider<ISurfacePattern>*      provider, const std::string& id) override;
//     void add(IAssetProvider<IWallPattern>*         provider, const std::string& id) override;
//     void add(IAssetProvider<IWorldEditorTool>*     provider, const std::string& id) override;
//     void add(IAssetProvider<IZoneObjectTypeTrait>* provider, const std::string& id) override;
//     void add(IAssetProvider<IZoneViewType>*        provider, const std::string& id) override;
    void addZoneBinding(IBinding* binding1, IBinding* binding2, const std::string& id) override;

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
    void stateChanged(ITexture* asset) override;

    private:
    
    // Resource strings.
    static const std::string RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS;
    static const std::string RESOURCE_CATEGORY_SPINDIZZY_GRAPHICS;
    static const std::string RESOURCE_CATEGORY_SPINDIZZY_LOGIC;

    // JSON members.
    static const std::string JSON_LOCAL;

    // Resource type names.
    static const std::string RESOURCE_TYPE_ALIEN;
    static const std::string RESOURCE_TYPE_LIFT;
    static const std::string RESOURCE_TYPE_PICK_UP;
    static const std::string RESOURCE_TYPE_PLAYER;
    static const std::string RESOURCE_TYPE_TERRAIN;
    static const std::string RESOURCE_TYPE_WORLD_VIEW;
    static const std::string RESOURCE_TYPE_ZONE;
    static const std::string RESOURCE_TYPE_ZONE_OBJECT;
    
    // Fixed tool names.
    static const std::string TOOL_DELETE;
    static const std::string TOOL_PROPERTIES;
    static const std::string TOOL_COPY_ZONE;
    static const std::string TOOL_MOVE_ZONE;
    static const std::string TOOL_DELETE_ZONE;

    // Type constants.
    static const std::string CAMERA_GAMEPLAY;
    static const std::string CAMERA_LINKED;
    static const std::string CAMERA_OVERVIEW;
    static const std::string CAMERA_TRANSITIONAL;
    static const std::string CAMERA_VARIANT;
    
    static const std::string SURFACE_PATTERN_OUTLINE;
    static const std::string SURFACE_PATTERN_TILE;
    static const std::string SURFACE_PATTERN_SPLIT_VARIANT;

    static const std::string WALL_PATTERN_CAPPED;
    static const std::string WALL_PATTERN_OUTLINE;
    static const std::string WALL_PATTERN_TILE;
    
    static const std::string ZONE_OBJECT_TYPE_TRAIT_BOUNDARY;
    static const std::string ZONE_OBJECT_TYPE_TRAIT_CELL_LOCATION;
    static const std::string ZONE_OBJECT_TYPE_TRAIT_CHASER;
    static const std::string ZONE_OBJECT_TYPE_TRAIT_MODEL;
    static const std::string ZONE_OBJECT_TYPE_TRAIT_MOVABLE;
    static const std::string ZONE_OBJECT_TYPE_TRAIT_PHYSICS;
    static const std::string ZONE_OBJECT_TYPE_TRAIT_SPINNER;

    static const std::string ZONE_VIEW_TYPE_ACTUAL;
    static const std::string ZONE_VIEW_TYPE_OVERVIEW;
    
    static const std::string BIND_TO_ZONE;

    // External interfaces.
    Project& cProject;
    IResourceTypeRegistry& cModule;

    // Spindizzy Assets.
    AssetClientManager<Spindizzy, Spindizzy,      IBoundaryType>        cBoundaryTypes;
    AssetClientManager<Spindizzy, WorldView,      ICamera>              cCameras;
    AssetClientManager<Spindizzy, Spindizzy,      IPhysicalObjectType>  cPhysicalObjectTypes;
    AssetClientManager<Spindizzy, TerrainType,    ISurfacePattern>      cSurfacePatterns;
    AssetClientManager<Spindizzy, TerrainType,    IWallPattern>         cWallPatterns;
    AssetClientManager<Spindizzy, Spindizzy,      IWorldEditorTool>     cWorldEditorTools;
    AssetClientManager<Spindizzy, ZoneObjectType, IZoneObjectTypeTrait> cZoneObjectTypeTraits;
    AssetClientManager<Spindizzy, WorldView,      IZoneViewType>        cZoneViewTypes;

    // Dummy asset providers.
    AssetLiteralDummy<Spindizzy,      IBoundaryType,        BoundaryTypeDummy>        cDummyProviderBoundaryType;
    AssetLiteralDummy<Spindizzy,      IPhysicalObjectType,  PhysicalObjectTypeDummy>  cDummyProviderPhysicalObjectType;
    AssetLiteralDummy<Spindizzy,      IWorldEditorTool,     WorldEditorToolDummy>     cDummyProviderWorldEditorTool;

    // Built-in providers for Spindizzy asset types.
    AssetInstanced<WorldView, ICamera, CameraGameplay>     cProviderCameraGameplay;
    AssetInstanced<WorldView, ICamera, CameraLinked>       cProviderCameraLinked;
    AssetInstanced<WorldView, ICamera, CameraOverview>     cProviderCameraOverview;
    AssetInstanced<WorldView, ICamera, CameraTransitional> cProviderCameraTransitional;
    AssetInstanced<WorldView, ICamera, CameraVariant>      cProviderCameraVariant;
    
    AssetInstanced<TerrainType, ISurfacePattern, SurfacePatternOutline>      cProviderSurfacePatternOutline;
    AssetInstanced<TerrainType, ISurfacePattern, SurfacePatternSplitVariant> cProviderSurfacePatternSplitVariant;
    AssetInstanced<TerrainType, ISurfacePattern, SurfacePatternTile>         cProviderSurfacePatternTile;
    
    AssetInstanced<TerrainType, IWallPattern, WallPatternCap>      cProviderWallPatternCap;
    AssetInstanced<TerrainType, IWallPattern, WallPatternOutline>  cProviderWallPatternOutline;
    AssetInstanced<TerrainType, IWallPattern, WallPatternTile>     cProviderWallPatternTile;

    AssetInstanced<WorldView, IZoneViewType, ZoneViewTypeActual>   cProviderZoneViewTypeActual;
    AssetInstanced<WorldView, IZoneViewType, ZoneViewTypeOverview> cProviderZoneViewTypeOverview;

    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitBoundary>     cProviderZoneObjectTypeTraitBoundary;
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitCellLocation> cProviderZoneObjectTypeTraitCellLocation;
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitChaser>       cProviderZoneObjectTypeTraitChaser;
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitModel>        cProviderZoneObjectTypeTraitModel;
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitMovable>      cProviderZoneObjectTypeTraitMovable;
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitPhysics>      cProviderZoneObjectTypeTraitPhysics;
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitSpinner>      cProviderZoneObjectTypeTraitSpinner;

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
      void getAssetProperties(PropertyMaker& owner) override;
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
    LuaBinding<Zone>    cRuntimeParameterZone;

    // Editing data.
    DeleteTool     cToolDelete;
    PropertiesTool cToolProperties;
    ZoneTool       cToolCopyZone;
    ZoneTool       cToolMoveZone;
    ZoneTool       cToolDeleteZone;
    
    // Scripting support.
    LuaBinding<Spindizzy> cLuaBinding;
    LuaBinding<Zone> cRuntimeParameterZone1;
    LuaBinding<Zone> cRuntimeParameterZone2;
    std::map<std::string, IBinding*> cRuntimeZoneBindings1;
    std::map<std::string, IBinding*> cRuntimeZoneBindings2;
    mutable const IBindingIdentifier* cRuntimeLocalBindingIdentifier;

    template <class TYPE> friend struct AssetContainerTraits;
  };

  template<> struct AssetContainerTraits<IBoundaryType>        {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cBoundaryTypes;       }};
  template<> struct AssetContainerTraits<ICamera>              {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cCameras;             }};
  template<> struct AssetContainerTraits<IPhysicalObjectType>  {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cPhysicalObjectTypes; }};
  template<> struct AssetContainerTraits<ISurfacePattern>      {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cSurfacePatterns;     }};
  template<> struct AssetContainerTraits<IWallPattern>         {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cWallPatterns;        }};
  template<> struct AssetContainerTraits<IWorldEditorTool>     {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cWorldEditorTools;    }};
  template<> struct AssetContainerTraits<IZoneObjectTypeTrait> {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cZoneObjectTypeTraits;}};
  template<> struct AssetContainerTraits<IZoneViewType>        {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cZoneViewTypes;       }};
}

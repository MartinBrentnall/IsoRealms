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

#include "IsoRealms/Assets/Providers/AssetInstanced.h"
#include "IsoRealms/IModuleHandle.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IResourceTypeRegistry.h"
#include "IsoRealms/IsoRealmsConstants.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/Options/IOptions.h"
#include "IsoRealms/ResourceTypeDefinition.h"

#include "Assets/Client/WorldEditorTool.h"
#include "Assets/Fixed/BoundaryType/BoundaryTypeDummy.h"
#include "Assets/Fixed/Camera/CameraDummy.h"
#include "Assets/Fixed/Camera/CameraGameplay.h"
#include "Assets/Fixed/Camera/CameraLinked.h"
#include "Assets/Fixed/Camera/CameraOverview.h"
#include "Assets/Fixed/Camera/CameraTransitional.h"
#include "Assets/Fixed/Camera/CameraVariant.h"
#include "Assets/Fixed/PhysicalObjectType/PhysicalObjectTypeDummy.h"
#include "Assets/Fixed/SurfacePattern/SurfacePatternDummy.h"
#include "Assets/Fixed/SurfacePattern/SurfacePatternOutline.h"
#include "Assets/Fixed/SurfacePattern/SurfacePatternTile.h"
#include "Assets/Fixed/SurfacePattern/SurfacePatternSplitVariant.h"
#include "Assets/Fixed/WallPattern/WallPatternDummy.h"
#include "Assets/Fixed/WallPattern/WallPatternCap.h"
#include "Assets/Fixed/WallPattern/WallPatternOutline.h"
#include "Assets/Fixed/WallPattern/WallPatternTile.h"
#include "Assets/Fixed/WorldEditorTool/WorldEditorToolDummy.h"
#include "Assets/Fixed/ZoneViewType/ZoneViewTypeDummy.h"
#include "Assets/Fixed/ZoneViewType/ZoneViewTypeActual.h"
#include "Assets/Fixed/ZoneViewType/ZoneViewTypeOverview.h"
#include "Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitBoundary.h"
#include "Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitCellLocation.h"
#include "Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitChaser.h"
#include "Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitDummy.h"
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
#include "DamageIndicator/DamageIndicator.h"
#include "DebrisChunk/DebrisChunk.h"
#include "Gyroscope/Gyroscope.h"
#include "IIconAnimator.h"
#include "ISpindizzyRegistry.h"
#include "Jewel/Jewel.h"
#include "LiftType/LiftType.h"
#include "PickUpType/PickUpType.h"
#include "PlayerType/PlayerType.h"
#include "TerrainState/TerrainState.h"
#include "TerrainType/TerrainType.h"
#include "ThemeSet/ThemeSet.h"
#include "Top/Top.h"
#include "World/Editor/DeleteTool.h"
#include "World/Editor/PropertiesTool.h"
#include "World/World.h"
#include "WorldView/WorldView.h"
#include "ZoneObjectType/ZoneObjectType.h"
#include "ZoneType/ZoneType.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy : public IModuleHandle,
                    public ISpindizzyRegistry,
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
    Spindizzy(IProject* project, IResourceTypeRegistry* registry, IAssetLiterals* literals);
  
    // Interface access (used by all).
    IProject* getProject() const;

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
    
    std::vector<IBoundaryType*>       getAllBoundaryTypes();
    std::vector<IPhysicalObjectType*> getAllPhysicalObjectTypes();

    // Resource removal.
    void removeAll(AlienType*      type);
    void removeAll(LiftType*       type);
    void removeAll(PickUpType*     type);
    void removeAll(PlayerType*     type);
    void removeAll(TerrainType*    type);
    void removeAll(ZoneObjectType* type);
    void removeAll(ZoneType*       type);

    // Module type removal.
//     void remove(IAssetProvider<IBoundaryType>*        provider);
//     void remove(IAssetProvider<ICamera>*              provider);
//     void remove(IAssetProvider<IPhysicalObjectType>*  provider);
//     void remove(IAssetProvider<ISurfacePattern>*      provider);
//     void remove(IAssetProvider<IWallPattern>*         provider);
//     void remove(IAssetProvider<IWorldEditorTool>*     provider);
//     void remove(IAssetProvider<IZoneObjectTypeTrait>* provider);
//     void remove(IAssetProvider<IZoneViewType>*        provider);

    IBoundaryType*        createLiteralBoundaryType(       IAssetUser<IBoundaryType>*        user);
    ICamera*              createLiteralCamera(             IAssetUser<ICamera>*              user);
    IPhysicalObjectType*  createLiteralPhysicalObjectType( IAssetUser<IPhysicalObjectType>*  user);
    ISurfacePattern*      createLiteralSurfacePattern(     IAssetUser<ISurfacePattern>*      user);
    IWallPattern*         createLiteralWallPattern(        IAssetUser<IWallPattern>*         user);
    IWorldEditorTool*     createLiteralWorldEditorTool(    IAssetUser<IWorldEditorTool>*     user);
    IZoneObjectTypeTrait* createLiteralZoneObjectTypeTrait(IAssetUser<IZoneObjectTypeTrait>* user);
    IZoneViewType*        createLiteralZoneViewType(       IAssetUser<IZoneViewType>*        user);

    IBoundaryType*        getBoundaryType(       IAssetUser<IBoundaryType>*        user, DOMNode& node);
    ICamera*              getCamera(             IAssetUser<ICamera>*              user, DOMNode& node, WorldView*      owner);
    IPhysicalObjectType*  getPhysicalObjectType( IAssetUser<IPhysicalObjectType>*  user, DOMNode& node);
    ISurfacePattern*      getSurfacePattern(     IAssetUser<ISurfacePattern>*      user, DOMNode& node);
    IWallPattern*         getWallPattern(        IAssetUser<IWallPattern>*         user, DOMNode& node);
    IWorldEditorTool*     getWorldEditorTool(    IAssetUser<IWorldEditorTool>*     user, DOMNode& node);
    IZoneObjectTypeTrait* getZoneObjectTypeTrait(IAssetUser<IZoneObjectTypeTrait>* user, DOMNode& node, ZoneObjectType* owner);
    IZoneViewType*        getZoneViewType(       IAssetUser<IZoneViewType>*        user, DOMNode& node, WorldView*      owner);
    
    void release(IAssetUser<IBoundaryType>*        user, IBoundaryType*        asset);
    void release(IAssetUser<ICamera>*              user, ICamera*              asset);
    void release(IAssetUser<IPhysicalObjectType>*  user, IPhysicalObjectType*  asset);
    void release(IAssetUser<ISurfacePattern>*      user, ISurfacePattern*      asset);
    void release(IAssetUser<IWallPattern>*         user, IWallPattern*         asset);
    void release(IAssetUser<IWorldEditorTool>*     user, IWorldEditorTool*     asset);
    void release(IAssetUser<IZoneObjectTypeTrait>* user, IZoneObjectTypeTrait* asset);
    void release(IAssetUser<IZoneViewType>*        user, IZoneViewType*        asset);

    std::string getID(const IBoundaryType*        asset) const;
    std::string getID(const ICamera*              asset) const;
    std::string getID(const IPhysicalObjectType*  asset) const;
    std::string getID(const ISurfacePattern*      asset) const;
    std::string getID(const IWallPattern*         asset) const;
    std::string getID(const IWorldEditorTool*     asset) const;
    std::string getID(const IZoneObjectTypeTrait* asset) const;
    std::string getID(const IZoneViewType*        asset) const;

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
    
    // TODO: Replace with local bindings.
    void bind(Alien* alien);
    void bind(Player* player);
    void bind(Wall* wall);
    void bind(Zone* zone);
    void bindFallDistance(IFloat* value);
    void bindLaunchLocation(IVertex* value);
    void bindLaunchMomentum(IVertex* value);

    // World editing (TODO: Move to World).
    IWorldEditorTool* getDefaultWorldEditorTool();
    ThemeSet* getDefaultThemeSet();
    ZoneType* getAutomaticZoneManagementType() const;
    int getAutomaticZoneXSize() const;
    int getAutomaticZoneYSize() const;
    int getAutomaticZoneZSize() const;
    int getEditorMinX() const;
    int getEditorMaxX() const;
    int getEditorMinY() const;
    int getEditorMaxY() const;
    int getEditorMinZ() const;
    int getEditorMaxZ() const;
    
    // TODO: Move this to World (Editing).
    std::vector<IWorldEditorToolInstance*> createToolSet(WorldEditor* editor);
    
    /***********************\
     * Scripting Interface *
    \***********************/
    void setPause(bool pause);
    bool isPaused();

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void load(IProject* project, DOMNode& node) override;
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) override;
    void registerAssets(IAssetRegistry* assets) override;
    void unregisterAssets(IAssetRemover* remover, IAssets* releaser) override;

    /*********************************\
     * Implements ISpindizzyRegistry *
    \*********************************/
    void add(IBoundaryType*       asset, const std::string& id) override;
    void add(IPhysicalObjectType* asset, const std::string& id) override;
    void add(IWorldEditorTool*    asset, const std::string& id) override;
    
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
    // TODO: Replace with local bindings.
    /*******************************\
     * Implements IBindingRegistry *
    \*******************************/
    IBinding* getBinding(const std::string& id) override;
    void releaseBinding(const IBinding* asset) override;

    private:
    
    // Resource strings.
    static const std::string TAG_RESOURCE_ALIEN;
    static const std::string TAG_RESOURCE_BALL;
    static const std::string TAG_RESOURCE_BOUNDARY_HANDLER;
    static const std::string TAG_RESOURCE_C64_LIFT_GRAPHICS;
    static const std::string TAG_RESOURCE_C64_TERRAIN_GRAPHICS;
    static const std::string TAG_RESOURCE_COLLISION_HANDLER;
    static const std::string TAG_RESOURCE_DAMAGE_INDICATOR;
    static const std::string TAG_RESOURCE_DEBRIS_CHUNK;
    static const std::string TAG_RESOURCE_GYROSCOPE;
    static const std::string TAG_RESOURCE_JEWEL;
    static const std::string TAG_RESOURCE_LIFT;
    static const std::string TAG_RESOURCE_PICK_UP;
    static const std::string TAG_RESOURCE_PLAYER;
    static const std::string TAG_RESOURCE_TERRAIN;
    static const std::string TAG_RESOURCE_TERRAIN_STATE;
    static const std::string TAG_RESOURCE_THEME_SET;
    static const std::string TAG_RESOURCE_TOP;
    static const std::string TAG_RESOURCE_WORLD;
    static const std::string TAG_RESOURCE_WORLD_VIEW;
    static const std::string TAG_RESOURCE_ZONE;
    static const std::string TAG_RESOURCE_ZONE_OBJECT;
    
    static const std::string NAME_RESOURCE_ALIEN;
    static const std::string NAME_RESOURCE_BALL;
    static const std::string NAME_RESOURCE_BOUNDARY_HANDLER;
    static const std::string NAME_RESOURCE_C64_LIFT_GRAPHICS;
    static const std::string NAME_RESOURCE_C64_TERRAIN_GRAPHICS;
    static const std::string NAME_RESOURCE_COLLISION_HANDLER;
    static const std::string NAME_RESOURCE_DAMAGE_INDICATOR;
    static const std::string NAME_RESOURCE_DEBRIS_CHUNK;
    static const std::string NAME_RESOURCE_GYROSCOPE;
    static const std::string NAME_RESOURCE_JEWEL;
    static const std::string NAME_RESOURCE_LIFT;
    static const std::string NAME_RESOURCE_PICK_UP;
    static const std::string NAME_RESOURCE_PLAYER;
    static const std::string NAME_RESOURCE_TERRAIN;
    static const std::string NAME_RESOURCE_TERRAIN_STATE;
    static const std::string NAME_RESOURCE_THEME_SET;
    static const std::string NAME_RESOURCE_TOP;
    static const std::string NAME_RESOURCE_WORLD;
    static const std::string NAME_RESOURCE_WORLD_VIEW;
    static const std::string NAME_RESOURCE_ZONE;
    static const std::string NAME_RESOURCE_ZONE_OBJECT;
    
    static const std::string RESOURCE_CATEGORY_SPINDIZZY_GRAPHICS;
    static const std::string RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS;
    
    // DOM strings.
    static const std::string TAG_DEFAULT_WORLD_EDITOR_TOOL;
    static const std::string TAG_EDITING_TOOL;

    static const std::string ATTRIBUTE_AUTOMATIC_ZONE_MANAGEMENT;
    static const std::string ATTRIBUTE_AUTOMATIC_ZONE_X_SIZE;
    static const std::string ATTRIBUTE_AUTOMATIC_ZONE_Y_SIZE;
    static const std::string ATTRIBUTE_AUTOMATIC_ZONE_Z_SIZE;
    static const std::string ATTRIBUTE_EDITOR_MAX_X;
    static const std::string ATTRIBUTE_EDITOR_MIN_X;
    static const std::string ATTRIBUTE_EDITOR_MAX_Y;
    static const std::string ATTRIBUTE_EDITOR_MIN_Y;
    static const std::string ATTRIBUTE_EDITOR_MAX_Z;
    static const std::string ATTRIBUTE_EDITOR_MIN_Z;
    static const std::string ATTRIBUTE_DEFAULT_THEME_SET;
    static const std::string ATTRIBUTE_ID;
    static const std::string ATTRIBUTE_PATTERN;
    static const std::string ATTRIBUTE_TYPE;

    // Default constants.
    static const int DEFAULT_AUTOMATIC_ZONE_X_SIZE;
    static const int DEFAULT_AUTOMATIC_ZONE_Y_SIZE;
    static const int DEFAULT_AUTOMATIC_ZONE_Z_SIZE;
    static const int DEFAULT_EDITOR_MAX_X;
    static const int DEFAULT_EDITOR_MIN_X;
    static const int DEFAULT_EDITOR_MAX_Y;
    static const int DEFAULT_EDITOR_MIN_Y;
    static const int DEFAULT_EDITOR_MAX_Z;
    static const int DEFAULT_EDITOR_MIN_Z;

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
    IProject* cProject;

    // Spindizzy Assets.
    AssetClientManager<Spindizzy,      IBoundaryType>        cBoundaryTypes;
    AssetClientManager<WorldView,      ICamera>              cCameras;
    AssetClientManager<Spindizzy,      IPhysicalObjectType>  cPhysicalObjectTypes;
    AssetClientManager<Spindizzy,      ISurfacePattern>      cSurfacePatterns;
    AssetClientManager<Spindizzy,      IWallPattern>         cWallPatterns;
    AssetClientManager<Spindizzy,      IWorldEditorTool>     cWorldEditorTools;
    AssetClientManager<ZoneObjectType, IZoneObjectTypeTrait> cZoneObjectTypeTraits;
    AssetClientManager<WorldView,      IZoneViewType>        cZoneViewTypes;

    // Dummy asset providers.
    AssetLiteralDummy<Spindizzy,      IBoundaryType,        BoundaryTypeDummy>        cDummyProviderBoundaryType;
    AssetLiteralDummy<WorldView,      ICamera,              CameraDummy>              cDummyProviderCamera;
    AssetLiteralDummy<Spindizzy,      IPhysicalObjectType,  PhysicalObjectTypeDummy>  cDummyProviderPhysicalObjectType;
    AssetLiteralDummy<Spindizzy,      ISurfacePattern,      SurfacePatternDummy>      cDummyProviderSurfacePattern;
    AssetLiteralDummy<Spindizzy,      IWallPattern,         WallPatternDummy>         cDummyProviderWallPattern;
    AssetLiteralDummy<Spindizzy,      IWorldEditorTool,     WorldEditorToolDummy>     cDummyProviderWorldEditorTool;
    AssetLiteralDummy<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitDummy> cDummyProviderZoneObjectTypeTrait;
    AssetLiteralDummy<WorldView,      IZoneViewType,        ZoneViewTypeDummy>        cDummyProviderZoneViewType;

    // Built-in providers for Spindizzy asset types.
    AssetInstanced<WorldView, ICamera, CameraGameplay>     cProviderCameraGameplay;
    AssetInstanced<WorldView, ICamera, CameraLinked>       cProviderCameraLinked;
    AssetInstanced<WorldView, ICamera, CameraOverview>     cProviderCameraOverview;
    AssetInstanced<WorldView, ICamera, CameraTransitional> cProviderCameraTransitional;
    AssetInstanced<WorldView, ICamera, CameraVariant>      cProviderCameraVariant;
    
    AssetInstanced<Spindizzy, ISurfacePattern, SurfacePatternOutline>      cProviderSurfacePatternOutline;
    AssetInstanced<Spindizzy, ISurfacePattern, SurfacePatternSplitVariant> cProviderSurfacePatternSplitVariant;
    AssetInstanced<Spindizzy, ISurfacePattern, SurfacePatternTile>         cProviderSurfacePatternTile;
    
    AssetInstanced<Spindizzy, IWallPattern, WallPatternCap>      cProviderWallPatternCap;
    AssetInstanced<Spindizzy, IWallPattern, WallPatternOutline>  cProviderWallPatternOutline;
    AssetInstanced<Spindizzy, IWallPattern, WallPatternTile>     cProviderWallPatternTile;

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
    ResourceTypeDefinition<Spindizzy, DamageIndicator>    cResourceDamageIndicator;
    ResourceTypeDefinition<Spindizzy, DebrisChunk>        cResourceDebrisChunk;
    ResourceTypeDefinition<Spindizzy, Gyroscope>          cResourceGyroscope;
    ResourceTypeDefinition<Spindizzy, Jewel>              cResourceJewel;
    ResourceTypeDefinition<Spindizzy, LiftType>           cResourceLift;
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

    // World editor configuration.
    ThemeSet* cDefaultThemeSet; // TODO: Set to nullptr when the ThemeSet is removed.
    WorldEditorTool cDefaultWorldEditorTool;
    ZoneType* cAutomaticZoneManagementType;
    int cAutomaticZoneXSize;
    int cAutomaticZoneYSize;
    int cAutomaticZoneZSize;
    int cEditorMinX;
    int cEditorMaxX;
    int cEditorMinY;
    int cEditorMaxY;
    int cEditorMinZ;
    int cEditorMaxZ;
    std::vector<std::unique_ptr<WorldEditorTool>> cAvailableWorldEditorTools;
    
    // Scripting support.
    LuaBinding<Spindizzy> cLuaBinding;
    LuaBinding<Zone>    cRuntimeParameterZone1;
    LuaBinding<Zone>    cRuntimeParameterZone2;
    std::map<std::string, IBinding*> cRuntimeZoneBindings1;
    std::map<std::string, IBinding*> cRuntimeZoneBindings2;
  };
}

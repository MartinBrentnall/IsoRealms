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
#include "Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const double Spindizzy::DEFAULT_VIEW_ANGLE_PITCH = -90.0 + std::atan(1.0 / std::sqrt(2.0)) * (180.0 / M_PI);
  const double Spindizzy::DEFAULT_VIEW_ANGLE_YAW = -45.0;

  Spindizzy::Spindizzy(IProject& project, IResourceTypeRegistry* registry) :
                    cProject(project),
                    cBoundaryTypes(&cDummyProviderBoundaryType),
                    cPhysicalObjectTypes(&cDummyProviderPhysicalObjectType),
                    cWorldEditorTools(&cDummyProviderWorldEditorTool),
                    cProviderCameraGameplay(project),
                    cProviderCameraLinked(project),
                    cProviderCameraOverview(project),
                    cProviderCameraTransitional(project),
                    cProviderCameraVariant(project),
                    cProviderSurfacePatternOutline(project),
                    cProviderSurfacePatternSplitVariant(project),
                    cProviderSurfacePatternTile(project),
                    cProviderWallPatternCap(project),
                    cProviderWallPatternOutline(project),
                    cProviderWallPatternTile(project),
                    cProviderZoneViewTypeActual(project),
                    cProviderZoneViewTypeOverview(project),
                    cProviderZoneObjectTypeTraitBoundary(project),
                    cProviderZoneObjectTypeTraitCellLocation(project),
                    cProviderZoneObjectTypeTraitChaser(project),
                    cProviderZoneObjectTypeTraitModel(project),
                    cProviderZoneObjectTypeTraitMovable(project),
                    cProviderZoneObjectTypeTraitPhysics(project),
                    cProviderZoneObjectTypeTraitSpinner(project),
                    cResourceAlien(*this),
                    cResourceBall(*this),
                    cResourceBoundaryHandler(*this),
                    cResourceC64LiftGraphics(*this),
                    cResourceC64TerrainGraphics(*this),
                    cResourceCollisionHandler(*this),
                    cResourceDebrisChunk(*this),
                    cResourceGyroscope(*this),
                    cResourceJewel(*this),
                    cResourceLift(*this),
                    cResourcePickUp(*this),
                    cResourcePlayer(*this),
                    cResourceTerrain(*this),
                    cResourceTerrainState(*this),
                    cResourceThemeSet(*this),
                    cResourceTop(*this),
                    cResourceWorld(*this),
                    cResourceWorldView(*this),
                    cResourceZone(*this),
                    cResourceZoneObject(*this),
                    cRuntimePaused(false),
                    cBindingTypeTerrainState(ID_RESOURCE_TERRAIN_STATE),
                    cRuntimeParameterAlien(project, nullptr),
                    cRuntimeParameterFallDistance(project, nullptr),
                    cRuntimeParameterLaunchLocation(project, nullptr),
                    cRuntimeParameterLaunchMomentum(project, nullptr),
                    cRuntimeParameterPlayer(project, nullptr),
                    cRuntimeParameterWall(project, nullptr),
                    cRuntimeParameterZone(project, nullptr),
                    cToolDelete(),
                    cToolProperties(),
                    cToolCopyZone(ZoneTool::Type::COPY),
                    cToolMoveZone(ZoneTool::Type::MOVE),
                    cToolDeleteZone(ZoneTool::Type::DELETE),
                    cDefaultThemeSet(nullptr),
                    cDefaultWorldEditorTool(*this),
                    cAutomaticZoneManagementType(nullptr),
                    cAutomaticZoneXSize(DEFAULT_AUTOMATIC_ZONE_X_SIZE),
                    cAutomaticZoneYSize(DEFAULT_AUTOMATIC_ZONE_Y_SIZE),
                    cAutomaticZoneZSize(DEFAULT_AUTOMATIC_ZONE_Z_SIZE),
                    cEditorMinX(DEFAULT_EDITOR_MIN_X),
                    cEditorMaxX(DEFAULT_EDITOR_MAX_X),
                    cEditorMinY(DEFAULT_EDITOR_MIN_Y),
                    cEditorMaxY(DEFAULT_EDITOR_MAX_Y),
                    cEditorMinZ(DEFAULT_EDITOR_MIN_Z),
                    cEditorMaxZ(DEFAULT_EDITOR_MAX_Z),
                    cLuaBinding(project, this),
                    cRuntimeParameterZone1(project, nullptr),
                    cRuntimeParameterZone2(project, nullptr),
                    cRuntimeLocalBindingIdentifier(nullptr) {
    registry->add(&cResourceAlien,              ID_RESOURCE_ALIEN,                "Alien",               "Aliens",               RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS);
    registry->add(&cResourceBall,               ID_RESOURCE_BALL,                 "Ball Texture",        "Ball Textures",        RESOURCE_CATEGORY_SPINDIZZY_GRAPHICS);
    registry->add(&cResourceBoundaryHandler,    ID_RESOURCE_BOUNDARY_HANDLER,     "Boundary Handler",    "Boundary Handlers",    RESOURCE_CATEGORY_SPINDIZZY_LOGIC);
    registry->add(&cResourceC64LiftGraphics,    ID_RESOURCE_C64_LIFT_GRAPHICS,    "Lift Textures",       "Lift Textures",        RESOURCE_CATEGORY_SPINDIZZY_GRAPHICS);
    registry->add(&cResourceC64TerrainGraphics, ID_RESOURCE_C64_TERRAIN_GRAPHICS, "Terrain Textures",    "Terrain Textures",     RESOURCE_CATEGORY_SPINDIZZY_GRAPHICS);
    registry->add(&cResourceCollisionHandler,   ID_RESOURCE_COLLISION_HANDLER,    "Collision Handler",   "Collision Handlers",   RESOURCE_CATEGORY_SPINDIZZY_LOGIC);
    registry->add(&cResourceDebrisChunk,        ID_RESOURCE_DEBRIS_CHUNK,         "Debris Chunk Model",  "Debris Chunk Models",  RESOURCE_CATEGORY_SPINDIZZY_GRAPHICS);
    registry->add(&cResourceGyroscope,          ID_RESOURCE_GYROSCOPE,            "Gyroscope Model",     "Gyroscope Models",     RESOURCE_CATEGORY_SPINDIZZY_GRAPHICS);
    registry->add(&cResourcePickUp,             ID_RESOURCE_PICK_UP,              "Pick Up",             "Pick Ups",             RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS);
    registry->add(&cResourcePlayer,             ID_RESOURCE_PLAYER,               "Player",              "Players",              RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS);
    registry->add(&cResourceTop,                ID_RESOURCE_TOP,                  "Top Model",           "Top Models",           RESOURCE_CATEGORY_SPINDIZZY_GRAPHICS);
    registry->add(&cResourceJewel,              ID_RESOURCE_JEWEL,                "Jewel Model",         "Jewel Models",         RESOURCE_CATEGORY_SPINDIZZY_GRAPHICS);
    registry->add(&cResourceLift,               ID_RESOURCE_LIFT,                 "Lift",                "Lifts",                RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS);
    registry->add(&cResourceTerrain,            ID_RESOURCE_TERRAIN,              "Terrain",             "Terrain",              RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS);
    registry->add(&cResourceTerrainState,       ID_RESOURCE_TERRAIN_STATE,        "Terrain State",       "Terrain States",       RESOURCE_CATEGORY_SPINDIZZY_LOGIC);
    registry->add(&cResourceThemeSet,           ID_RESOURCE_THEME_SET,            "Theme Set",           "Theme Sets",           RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS);
    registry->add(&cResourceWorld,              ID_RESOURCE_WORLD,                "World",               "Worlds",               RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS);
    registry->add(&cResourceWorldView,          ID_RESOURCE_WORLD_VIEW,           "World View",          "World Views",          RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS);
    registry->add(&cResourceZone,               ID_RESOURCE_ZONE,                 "Zone Type",           "Zone Types",           RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS);
    registry->add(&cResourceZoneObject,         ID_RESOURCE_ZONE_OBJECT,          "Entity",              "Entities",             RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS);
        
    // Register Spindizzy built-in asset providers.
    cCameras.add(&cProviderCameraGameplay,     CAMERA_GAMEPLAY,     "Spindizzy");
    cCameras.add(&cProviderCameraLinked,       CAMERA_LINKED,       "Spindizzy");
    cCameras.add(&cProviderCameraOverview,     CAMERA_OVERVIEW,     "Spindizzy");
    cCameras.add(&cProviderCameraTransitional, CAMERA_TRANSITIONAL, "Spindizzy");
    cCameras.add(&cProviderCameraVariant,      CAMERA_VARIANT,      "Spindizzy");
    
    cSurfacePatterns.add(&cProviderSurfacePatternOutline,      SURFACE_PATTERN_OUTLINE,       "Spindizzy");
    cSurfacePatterns.add(&cProviderSurfacePatternSplitVariant, SURFACE_PATTERN_SPLIT_VARIANT, "Spindizzy");
    cSurfacePatterns.add(&cProviderSurfacePatternTile,         SURFACE_PATTERN_TILE,          "Spindizzy");
    
    cWallPatterns.add(&cProviderWallPatternCap,      WALL_PATTERN_CAPPED,  "Spindizzy");
    cWallPatterns.add(&cProviderWallPatternOutline,  WALL_PATTERN_OUTLINE, "Spindizzy");
    cWallPatterns.add(&cProviderWallPatternTile,     WALL_PATTERN_TILE,    "Spindizzy");
    
    cZoneObjectTypeTraits.add(&cProviderZoneObjectTypeTraitBoundary,     ZONE_OBJECT_TYPE_TRAIT_BOUNDARY,      "Spindizzy");
    cZoneObjectTypeTraits.add(&cProviderZoneObjectTypeTraitCellLocation, ZONE_OBJECT_TYPE_TRAIT_CELL_LOCATION, "Spindizzy");
    cZoneObjectTypeTraits.add(&cProviderZoneObjectTypeTraitChaser,       ZONE_OBJECT_TYPE_TRAIT_CHASER,        "Spindizzy");
    cZoneObjectTypeTraits.add(&cProviderZoneObjectTypeTraitModel,        ZONE_OBJECT_TYPE_TRAIT_MODEL,         "Spindizzy");
    cZoneObjectTypeTraits.add(&cProviderZoneObjectTypeTraitMovable,      ZONE_OBJECT_TYPE_TRAIT_MOVABLE,       "Spindizzy");
    cZoneObjectTypeTraits.add(&cProviderZoneObjectTypeTraitPhysics,      ZONE_OBJECT_TYPE_TRAIT_PHYSICS,       "Spindizzy");
    cZoneObjectTypeTraits.add(&cProviderZoneObjectTypeTraitSpinner,      ZONE_OBJECT_TYPE_TRAIT_SPINNER,       "Spindizzy");

    cZoneViewTypes.add(&cProviderZoneViewTypeActual,   ZONE_VIEW_TYPE_ACTUAL,   "Spindizzy");
    cZoneViewTypes.add(&cProviderZoneViewTypeOverview, ZONE_VIEW_TYPE_OVERVIEW, "Spindizzy");
  }

  void Spindizzy::init(std::function<void(IAssets&)> initialiser) {
    cProject.init(initialiser);
  }
  
  IProject& Spindizzy::getProject() const {
    return cProject;
  }

  AlienType*      Spindizzy::getAlienType(     const std::string& id) const {return cResourceAlien.getResource(     id);}
  LiftType*       Spindizzy::getLiftType(      const std::string& id) const {return cResourceLift.getResource(      id);}
  PickUpType*     Spindizzy::getPickUpType(    const std::string& id) const {return cResourcePickUp.getResource(    id);}
  PlayerType*     Spindizzy::getPlayerType(    const std::string& id) const {return cResourcePlayer.getResource(    id);}
  TerrainType*    Spindizzy::getTerrainType(   const std::string& id) const {return cResourceTerrain.getResource(   id);}
  ThemeSet*       Spindizzy::getThemeSet(      const std::string& id) const {return cResourceThemeSet.getResource(  id);}
  World*          Spindizzy::getWorld(         const std::string& id) const {return cResourceWorld.getResource(     id);}
  WorldView*      Spindizzy::getWorldView(     const std::string& id) const {return cResourceWorldView.getResource( id);}
  ZoneObjectType* Spindizzy::getZoneObjectType(const std::string& id) const {return cResourceZoneObject.getResource(id);}
  ZoneType*       Spindizzy::getZoneType(      const std::string& id) const {return cResourceZone.getResource(      id);}
  
  std::string Spindizzy::getID(const AlienType*      asset) const {return cResourceAlien.getID(     asset);}
  std::string Spindizzy::getID(const LiftType*       asset) const {return cResourceLift.getID(      asset);}
  std::string Spindizzy::getID(const PickUpType*     asset) const {return cResourcePickUp.getID(    asset);}
  std::string Spindizzy::getID(const PlayerType*     asset) const {return cResourcePlayer.getID(    asset);}
  std::string Spindizzy::getID(const TerrainType*    asset) const {return cResourceTerrain.getID(   asset);}
  std::string Spindizzy::getID(const ThemeSet*       asset) const {return cResourceThemeSet.getID(  asset);}
  std::string Spindizzy::getID(const World*          asset) const {return cResourceWorld.getID(     asset);}
  std::string Spindizzy::getID(const WorldView*      asset) const {return cResourceWorldView.getID( asset);}
  std::string Spindizzy::getID(const ZoneObjectType* asset) const {return cResourceZoneObject.getID(asset);}
  std::string Spindizzy::getID(const ZoneType*       asset) const {return cResourceZone.getID(      asset);}
  
  void Spindizzy::removeAll(AlienType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }
  
  void Spindizzy::removeAll(LiftType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }
  
  void Spindizzy::removeAll(PickUpType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }
  
  void Spindizzy::removeAll(PlayerType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }
  
  void Spindizzy::removeAll(TerrainType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }
  
  void Spindizzy::removeAll(ZoneType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
    if (type == cAutomaticZoneManagementType) {
      cAutomaticZoneManagementType = nullptr;
    }
  }
  
  void Spindizzy::removeAll(ZoneObjectType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }

//   void Spindizzy::remove(IAssetProvider<IBoundaryType>*        provider) {cBoundaryTypes.remove(       provider);}
//   void Spindizzy::remove(IAssetProvider<ICamera>*              provider) {cCameras.remove(             provider);}
//   void Spindizzy::remove(IAssetProvider<IPhysicalObjectType>*  provider) {cPhysicalObjectTypes.remove( provider);}
//   void Spindizzy::remove(IAssetProvider<ISurfacePattern>*      provider) {cSurfacePatterns.remove(     provider);}
//   void Spindizzy::remove(IAssetProvider<IWallPattern>*         provider) {cWallPatterns.remove(        provider);}
//   void Spindizzy::remove(IAssetProvider<IWorldEditorTool>*     provider) {cWorldEditorTools.remove(    provider);}
//   void Spindizzy::remove(IAssetProvider<IZoneObjectTypeTrait>* provider) {cZoneObjectTypeTraits.remove(provider);}
//   void Spindizzy::remove(IAssetProvider<IZoneViewType>*        provider) {cZoneViewTypes.remove(       provider);}

  IBoundaryType*        Spindizzy::createLiteralBoundaryType(       IAssetUser<IBoundaryType>*        user)                        {return cBoundaryTypes.literal(       user, *this, "");}
  ICamera*              Spindizzy::createLiteralCamera(             IAssetUser<ICamera>*              user, WorldView&      owner) {return cCameras.literal(             user, owner, "");}
  IPhysicalObjectType*  Spindizzy::createLiteralPhysicalObjectType( IAssetUser<IPhysicalObjectType>*  user)                        {return cPhysicalObjectTypes.literal( user, *this, "");}
  ISurfacePattern*      Spindizzy::createLiteralSurfacePattern(     IAssetUser<ISurfacePattern>*      user)                        {return cSurfacePatterns.literal(     user, *this, "");}
  IWallPattern*         Spindizzy::createLiteralWallPattern(        IAssetUser<IWallPattern>*         user)                        {return cWallPatterns.literal(        user, *this, "");}
  IWorldEditorTool*     Spindizzy::createLiteralWorldEditorTool(    IAssetUser<IWorldEditorTool>*     user)                        {return cWorldEditorTools.literal(    user, *this, "");}
  IZoneObjectTypeTrait* Spindizzy::createLiteralZoneObjectTypeTrait(IAssetUser<IZoneObjectTypeTrait>* user, ZoneObjectType& owner) {return cZoneObjectTypeTraits.literal(user, owner, "");}
  IZoneViewType*        Spindizzy::createLiteralZoneViewType(       IAssetUser<IZoneViewType>*        user, WorldView&      owner) {return cZoneViewTypes.literal(       user, owner, "");}

  IBoundaryType*        Spindizzy::getBoundaryType(       IAssetUser<IBoundaryType>*        user, const std::string& id)                                             {return cBoundaryTypes.get(       user, *this, id, nullptr);}
  ICamera*              Spindizzy::getCamera(             IAssetUser<ICamera>*              user, const std::string& id, WorldView&      owner)                      {return cCameras.get(             user, owner, id, nullptr);}
  IPhysicalObjectType*  Spindizzy::getPhysicalObjectType( IAssetUser<IPhysicalObjectType>*  user, const std::string& id)                                             {return cPhysicalObjectTypes.get( user, *this, id, nullptr);}
  ISurfacePattern*      Spindizzy::getSurfacePattern(     IAssetUser<ISurfacePattern>*      user, const std::string& id, IStateListener<ISurfacePattern*>* listener) {return cSurfacePatterns.get(     user, *this, id, listener);}
  IWallPattern*         Spindizzy::getWallPattern(        IAssetUser<IWallPattern>*         user, const std::string& id, IStateListener<IWallPattern*>*    listener) {return cWallPatterns.get(        user, *this, id, listener);}
  IWorldEditorTool*     Spindizzy::getWorldEditorTool(    IAssetUser<IWorldEditorTool>*     user, const std::string& id)                                             {return cWorldEditorTools.get(    user, *this, id, nullptr);}
  IZoneObjectTypeTrait* Spindizzy::getZoneObjectTypeTrait(IAssetUser<IZoneObjectTypeTrait>* user, const std::string& id, ZoneObjectType& owner)                      {return cZoneObjectTypeTraits.get(user, owner, id, nullptr);}
  IZoneViewType*        Spindizzy::getZoneViewType(       IAssetUser<IZoneViewType>*        user, const std::string& id, WorldView&      owner)                      {return cZoneViewTypes.get(       user, owner, id, nullptr);}

  IBoundaryType*        Spindizzy::getBoundaryType(       IAssetUser<IBoundaryType>*        user, JSONObject object)                                             {return cBoundaryTypes.get(       user, *this, object, nullptr,  true, [this](JSONObject object, IStateListener<IBoundaryType*>*        listener) -> IBoundaryType*        {return nullptr;});}
  ICamera*              Spindizzy::getCamera(             IAssetUser<ICamera>*              user, JSONObject object, WorldView&      owner)                      {return cCameras.get(             user, owner, object, nullptr,  true, [this](JSONObject object, IStateListener<ICamera*>*              listener) -> ICamera*              {return nullptr;});}
  IPhysicalObjectType*  Spindizzy::getPhysicalObjectType( IAssetUser<IPhysicalObjectType>*  user, JSONObject object)                                             {return cPhysicalObjectTypes.get( user, *this, object, nullptr,  true, [this](JSONObject object, IStateListener<IPhysicalObjectType*>*  listener) -> IPhysicalObjectType*  {return nullptr;});}
  ISurfacePattern*      Spindizzy::getSurfacePattern(     IAssetUser<ISurfacePattern>*      user, JSONObject object, IStateListener<ISurfacePattern*>* listener) {return cSurfacePatterns.get(     user, *this, object, listener, true, [this](JSONObject object, IStateListener<ISurfacePattern*>*      listener) -> ISurfacePattern*      {return nullptr;});}
  IWallPattern*         Spindizzy::getWallPattern(        IAssetUser<IWallPattern>*         user, JSONObject object, IStateListener<IWallPattern*>*    listener) {return cWallPatterns.get(        user, *this, object, listener, true, [this](JSONObject object, IStateListener<IWallPattern*>*         listener) -> IWallPattern*         {return nullptr;});}
  IWorldEditorTool*     Spindizzy::getWorldEditorTool(    IAssetUser<IWorldEditorTool>*     user, JSONObject object)                                             {return cWorldEditorTools.get(    user, *this, object, nullptr,  true, [this](JSONObject object, IStateListener<IWorldEditorTool*>*     listener) -> IWorldEditorTool*     {return nullptr;});}
  IZoneObjectTypeTrait* Spindizzy::getZoneObjectTypeTrait(IAssetUser<IZoneObjectTypeTrait>* user, JSONObject object, ZoneObjectType& owner)                      {return cZoneObjectTypeTraits.get(user, owner, object, nullptr,  true, [this](JSONObject object, IStateListener<IZoneObjectTypeTrait*>* listener) -> IZoneObjectTypeTrait* {return nullptr;});}
  IZoneViewType*        Spindizzy::getZoneViewType(       IAssetUser<IZoneViewType>*        user, JSONObject object, WorldView&      owner)                      {return cZoneViewTypes.get(       user, owner, object, nullptr,  true, [this](JSONObject object, IStateListener<IZoneViewType*>*        listener) -> IZoneViewType*        {return nullptr;});}

  void Spindizzy::release(IAssetUser<IBoundaryType>*        user, IBoundaryType*        asset) {cBoundaryTypes.release(       user, asset);}
  void Spindizzy::release(IAssetUser<ICamera>*              user, ICamera*              asset) {cCameras.release(             user, asset);}
  void Spindizzy::release(IAssetUser<IPhysicalObjectType>*  user, IPhysicalObjectType*  asset) {cPhysicalObjectTypes.release( user, asset);}
  void Spindizzy::release(IAssetUser<ISurfacePattern>*      user, ISurfacePattern*      asset) {cSurfacePatterns.release(     user, asset);}
  void Spindizzy::release(IAssetUser<IWallPattern>*         user, IWallPattern*         asset) {cWallPatterns.release(        user, asset);}
  void Spindizzy::release(IAssetUser<IWorldEditorTool>*     user, IWorldEditorTool*     asset) {cWorldEditorTools.release(    user, asset);}
  void Spindizzy::release(IAssetUser<IZoneObjectTypeTrait>* user, IZoneObjectTypeTrait* asset) {cZoneObjectTypeTraits.release(user, asset);}
  void Spindizzy::release(IAssetUser<IZoneViewType>*        user, IZoneViewType*        asset) {cZoneViewTypes.release(       user, asset);}

  void Spindizzy::save(JSONObject object, IBoundaryType*        asset) const {cBoundaryTypes.save(       object, asset);}
  void Spindizzy::save(JSONObject object, ICamera*              asset) const {cCameras.save(             object, asset);}
  void Spindizzy::save(JSONObject object, IPhysicalObjectType*  asset) const {cPhysicalObjectTypes.save( object, asset);}
  void Spindizzy::save(JSONObject object, ISurfacePattern*      asset) const {cSurfacePatterns.save(     object, asset);}
  void Spindizzy::save(JSONObject object, IWallPattern*         asset) const {cWallPatterns.save(        object, asset);}
  void Spindizzy::save(JSONObject object, IWorldEditorTool*     asset) const {cWorldEditorTools.save(    object, asset);}
  void Spindizzy::save(JSONObject object, IZoneObjectTypeTrait* asset) const {cZoneObjectTypeTraits.save(object, asset);}
  void Spindizzy::save(JSONObject object, IZoneViewType*        asset) const {cZoneViewTypes.save(       object, asset);}

  std::vector<IBoundaryType*> Spindizzy::getAllBoundaryTypeObjects() {
    std::vector<IBoundaryType*> mTypes;
    for (PickUpType* mType : cResourcePickUp) {mTypes.push_back(mType);}
    for (ZoneType*   mType : cResourceZone)   {mTypes.push_back(mType);}
    return mTypes;
  }

  std::vector<IPhysicalObjectType*> Spindizzy::getAllPhysicalObjectTypeObjects() {
    std::vector<IPhysicalObjectType*> mTypes;
    for (AlienType*  mType : cResourceAlien)  {mTypes.push_back(mType);}
    for (PlayerType* mType : cResourcePlayer) {mTypes.push_back(mType);}
    return mTypes;
  }  

  void Spindizzy::added(BoundaryHandler* handler) {
    for (World* mWorld : cResourceWorld) {
      mWorld->addBoundaryHandler(handler->createInstance(mWorld));
    }
  }

  void Spindizzy::added(CollisionHandler* handler) {
    for (World* mWorld : cResourceWorld) {
      mWorld->addCollisionHandler(handler->createInstance(mWorld));
    }
  }

  void Spindizzy::added(IBoundaryType* boundaryType) {
    for (World* mWorld : cResourceWorld) {
      mWorld->added(boundaryType);
    }
  }

  void Spindizzy::added(IPhysicalObjectType* physicalObjectType) {
    for (World* mWorld : cResourceWorld) {
      mWorld->added(physicalObjectType);
    }
  }

  void Spindizzy::added(Zone* zone) {
    for (WorldView* mWorldView : cResourceWorldView) {
      if (mWorldView->getWorld() == &zone->getWorld()) {
        zone->registerView(*mWorldView);
        mWorldView->addZoneView(zone);
      }
    }
  }
  
  void Spindizzy::removed(IBoundaryType* boundaryType) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removed(boundaryType);
    }
  }
  
  void Spindizzy::removed(IPhysicalObjectType* physicalObjectType) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removed(physicalObjectType);
    }
  }
  
  void Spindizzy::removed(Zone* zone) {
    for (WorldView* mWorldView : cResourceWorldView) {
      if (mWorldView->getWorld() == &zone->getWorld()) {
        mWorldView->removeZoneView(zone);
      }
    }
  }

  std::vector<ConditionElement*> Spindizzy::getTerrainStateConditionElements() {
    std::vector<ConditionElement*> mConditionElements;
    for (TerrainState* mTerrainState : cResourceTerrainState) {
      mConditionElements.push_back(&mTerrainState->getConditionElement());
    }
    return mConditionElements;
  }

  TerrainState* Spindizzy::getTerrainState(IBoolean* input) {
    for (TerrainState* mTerrainState : cResourceTerrainState) {
      if (mTerrainState == input) {
        return mTerrainState;
      }
    }
    return nullptr;
  }

  void Spindizzy::setAllThemesInUse(bool inUse) {
    for (ThemeSet* mThemeSet : cResourceThemeSet) {
      mThemeSet->hintInUse(inUse);
    }
  }

  void Spindizzy::setNextTheme() {
    for (ThemeSet* mThemeSet : cResourceThemeSet) {
      mThemeSet->setNextTheme();
    }
  }

  void Spindizzy::setPreviousTheme() {
    for (ThemeSet* mThemeSet : cResourceThemeSet) {
      mThemeSet->setPreviousTheme();
    }
  }

  void Spindizzy::bind(              Alien*   value) {cRuntimeParameterAlien.setValue(         value);}
  void Spindizzy::bind(              Player*  value) {cRuntimeParameterPlayer.setValue(        value);}
  void Spindizzy::bind(              Zone*    value) {cRuntimeParameterZone.setValue(          value);}
  void Spindizzy::bind(              Wall*    value) {cRuntimeParameterWall.setValue(          value);}
  void Spindizzy::bindFallDistance(  IFloat*  value) {cRuntimeParameterFallDistance.setValue(  value);}
  void Spindizzy::bindLaunchLocation(IVertex* value) {cRuntimeParameterLaunchLocation.setValue(value);}
  void Spindizzy::bindLaunchMomentum(IVertex* value) {cRuntimeParameterLaunchMomentum.setValue(value);}

  IWorldEditorTool* Spindizzy::getDefaultWorldEditorTool() {
    return *cDefaultWorldEditorTool;
  }

  ThemeSet* Spindizzy::getDefaultThemeSet() {
    return cDefaultThemeSet;
  }
  
  void Spindizzy::applyDefaultThemes() {
    for (ThemeSet* mThemeSet : cResourceThemeSet) {
      mThemeSet->applyDefaultTheme();
    }
  }

  ZoneType* Spindizzy::getAutomaticZoneManagementType() const {
    return cAutomaticZoneManagementType;
  }
  
  int Spindizzy::getAutomaticZoneXSize() const {
    return cAutomaticZoneXSize;
  }
  
  int Spindizzy::getAutomaticZoneYSize() const {
    return cAutomaticZoneYSize;
  }
  
  int Spindizzy::getAutomaticZoneZSize() const {
    return cAutomaticZoneZSize;
  }
  
  int Spindizzy::getEditorMinX() const {
    return cEditorMinX;
  }
  
  int Spindizzy::getEditorMaxX() const {
    return cEditorMaxX;
  }
  
  int Spindizzy::getEditorMinY() const {
    return cEditorMinY;
  }
  
  int Spindizzy::getEditorMaxY() const {
    return cEditorMaxY;
  }
  
  int Spindizzy::getEditorMinZ() const {
    return cEditorMinZ;
  }
  
  int Spindizzy::getEditorMaxZ() const {
    return cEditorMaxZ;
  }
  
  std::vector<IWorldEditorToolInstance*> Spindizzy::createToolSet(WorldEditor& editor) {
    std::vector<IWorldEditorToolInstance*> mTools;
    for (std::unique_ptr<WorldEditorTool>& mTool : cAvailableWorldEditorTools) {
      mTools.emplace_back((*mTool)->createToolInstance(editor));
    }
    return mTools;
  }

  void Spindizzy::setPause(bool pause) {
    cRuntimePaused = pause;
  }

  bool Spindizzy::isPaused() {
    return cRuntimePaused;
  }

  void Spindizzy::load(IProject& project, JSONObject object) {
    cAutomaticZoneXSize = object.getInteger(JSON_AUTOMATIC_ZONE_X_SIZE, DEFAULT_AUTOMATIC_ZONE_X_SIZE);
    cAutomaticZoneYSize = object.getInteger(JSON_AUTOMATIC_ZONE_Y_SIZE, DEFAULT_AUTOMATIC_ZONE_Y_SIZE);
    cAutomaticZoneZSize = object.getInteger(JSON_AUTOMATIC_ZONE_Z_SIZE, DEFAULT_AUTOMATIC_ZONE_Z_SIZE);
    cEditorMinX = object.getInteger(JSON_EDITOR_MIN_X, DEFAULT_EDITOR_MIN_X);
    cEditorMaxX = object.getInteger(JSON_EDITOR_MAX_X, DEFAULT_EDITOR_MAX_X);
    cEditorMinY = object.getInteger(JSON_EDITOR_MIN_Y, DEFAULT_EDITOR_MIN_Y);
    cEditorMaxY = object.getInteger(JSON_EDITOR_MAX_Y, DEFAULT_EDITOR_MAX_Y);
    cEditorMinZ = object.getInteger(JSON_EDITOR_MIN_Z, DEFAULT_EDITOR_MIN_Z);
    cEditorMaxZ = object.getInteger(JSON_EDITOR_MAX_Z, DEFAULT_EDITOR_MAX_Z);
    cDefaultWorldEditorTool.init(object, JSON_DEFAULT_WORLD_EDITOR_TOOL);
    project.init([this, object](IAssets& assets) {
      cAutomaticZoneManagementType = cResourceZone.getResource(object.getString(JSON_AUTOMATIC_ZONE_MANAGEMENT), false);
      cDefaultThemeSet             = getThemeSet(object.getString(JSON_DEFAULT_THEME_SET));
      for (JSONObject mEditingToolObject : object.getArray(JSON_EDITOR_TOOLS)) {
        cAvailableWorldEditorTools.emplace_back(std::make_unique<WorldEditorTool>(*this)).get()->set(mEditingToolObject, JSON_EDITOR_TOOL);
      }
    });
  }

  void Spindizzy::save(JSONObject object, IAssetIdentifier& identifier) {
    object.addInteger(JSON_AUTOMATIC_ZONE_X_SIZE,     cAutomaticZoneXSize, DEFAULT_AUTOMATIC_ZONE_X_SIZE);
    object.addInteger(JSON_AUTOMATIC_ZONE_Y_SIZE,     cAutomaticZoneYSize, DEFAULT_AUTOMATIC_ZONE_Y_SIZE);
    object.addInteger(JSON_AUTOMATIC_ZONE_Z_SIZE,     cAutomaticZoneZSize, DEFAULT_AUTOMATIC_ZONE_Z_SIZE);
    object.addInteger(JSON_EDITOR_MIN_X,              cEditorMinX,         DEFAULT_EDITOR_MIN_X);
    object.addInteger(JSON_EDITOR_MAX_X,              cEditorMaxX,         DEFAULT_EDITOR_MAX_X);
    object.addInteger(JSON_EDITOR_MIN_Y,              cEditorMinY,         DEFAULT_EDITOR_MIN_Y);
    object.addInteger(JSON_EDITOR_MAX_Y,              cEditorMaxY,         DEFAULT_EDITOR_MAX_Y);
    object.addInteger(JSON_EDITOR_MIN_Z,              cEditorMinZ,         DEFAULT_EDITOR_MIN_Z);
    object.addInteger(JSON_EDITOR_MAX_Z,              cEditorMaxZ,         DEFAULT_EDITOR_MAX_Z);
    object.addString(JSON_AUTOMATIC_ZONE_MANAGEMENT, getID(cAutomaticZoneManagementType));
    object.addString(JSON_DEFAULT_THEME_SET,         getID(cDefaultThemeSet));
    cDefaultWorldEditorTool.save(object, JSON_DEFAULT_WORLD_EDITOR_TOOL);
    JSONArray mEditorToolsArray = object.addArray(JSON_EDITOR_TOOLS);
    for (std::unique_ptr<WorldEditorTool>& mTool : cAvailableWorldEditorTools) {
      mTool->save(mEditorToolsArray.addObject(), JSON_EDITOR_TOOL);
    }
  }

  void Spindizzy::registerAssets(IAssetRegistry& assets) {
    assets.add(&cBindingTypeTerrainState, "Terrain State", "Spindizzy");

    // TODO: The following things should not be needed!!!
    assets.add(&cRuntimeParameterAlien,          "Alien",          "Spindizzy");
    assets.add(&cRuntimeParameterFallDistance,   "FallDistance",   "Spindizzy");
    assets.add(&cRuntimeParameterLaunchLocation, "LaunchLocation", "Spindizzy");
    assets.add(&cRuntimeParameterLaunchMomentum, "LaunchMomentum", "Spindizzy");
    assets.add(&cRuntimeParameterPlayer,         "Player",         "Spindizzy");
    assets.add(&cRuntimeParameterWall,           "Wall",           "Spindizzy");
    assets.add(&cRuntimeParameterZone,           "Zone",           "Spindizzy");
    assets.add(&cLuaBinding,                     "",               "Spindizzy");
    
    for (AlienType* mResource : cResourceAlien) {
      LocalSpindizzyRegistry mLocalRegistry(this, RESOURCE_TYPE_ALIEN + "/" + getID(mResource));
      mResource->registerAssets(&mLocalRegistry);
    }
    for (LiftType* mResource : cResourceLift) {
      LocalSpindizzyRegistry mLocalRegistry(this, RESOURCE_TYPE_LIFT + "/" + getID(mResource));
      mResource->registerAssets(&mLocalRegistry);
    }
    for (PickUpType* mResource : cResourcePickUp) {
      LocalSpindizzyRegistry mLocalRegistry(this, RESOURCE_TYPE_PICK_UP + "/" + getID(mResource));
      mResource->registerAssets(&mLocalRegistry);
    }
    for (PlayerType* mResource : cResourcePlayer) {
      LocalSpindizzyRegistry mLocalRegistry(this, RESOURCE_TYPE_PLAYER + "/" + getID(mResource));
      mResource->registerAssets(&mLocalRegistry);
    }
    for (TerrainType* mResource : cResourceTerrain) {
      LocalSpindizzyRegistry mLocalRegistry(this, RESOURCE_TYPE_TERRAIN + "/" + getID(mResource));
      mResource->registerAssets(&mLocalRegistry);
    }
    for (ZoneType* mResource : cResourceZone) {
      LocalSpindizzyRegistry mLocalRegistry(this, RESOURCE_TYPE_ZONE + "/" + getID(mResource));
      mResource->registerAssets(&mLocalRegistry);
    }
    for (ZoneObjectType* mResource : cResourceZoneObject) {
      LocalSpindizzyRegistry mLocalRegistry(this, RESOURCE_TYPE_ZONE_OBJECT + "/" + getID(mResource));
      mResource->registerAssets(&mLocalRegistry);
    }
    for (WorldView* mResource : cResourceWorldView) {
      LocalSpindizzyRegistry mLocalRegistry(this, RESOURCE_TYPE_WORLD_VIEW + "/" + getID(mResource));
      mResource->registerAssets(&mLocalRegistry);
    }
    add(&cToolDelete,     TOOL_DELETE);
    add(&cToolProperties, TOOL_PROPERTIES);
    add(&cToolCopyZone,   TOOL_COPY_ZONE);
    add(&cToolMoveZone,   TOOL_MOVE_ZONE);
    add(&cToolDeleteZone, TOOL_DELETE_ZONE);
  }
  
  void Spindizzy::unregisterAssets(IAssetRemover& remover, IAssets& releaser) {
    remover.remove(&cRuntimeParameterAlien,          true);
    remover.remove(&cRuntimeParameterFallDistance,   true);
    remover.remove(&cRuntimeParameterLaunchLocation, true);
    remover.remove(&cRuntimeParameterLaunchMomentum, true);
    remover.remove(&cRuntimeParameterWall,           true);
    remover.remove(&cRuntimeParameterZone,           true);
    remover.remove(&cRuntimeParameterPlayer,         true);
    remover.remove(&cLuaBinding,                     true);
  }

  std::vector<std::unique_ptr<IProperty>> Spindizzy::getProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>(           "Default Zone Width",    "TODO", [this]() {return cAutomaticZoneXSize;}, [this](int value) {cAutomaticZoneXSize = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>(           "Default Zone Length",   "TODO", [this]() {return cAutomaticZoneYSize;}, [this](int value) {cAutomaticZoneYSize = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>(           "Default Zone Height",   "TODO", [this]() {return cAutomaticZoneZSize;}, [this](int value) {cAutomaticZoneZSize = value; return true;}));
//    mProperties.emplace_back(std::make_unique<PropertyAsset<ZoneType>>("Default Zone Type", *cAutomaticZoneManagementType));
//    mProperties.emplace_back(std::make_unique<PropertyAsset<ThemeSet>>("Default Zone Theme", *cDefaultThemeSet));
    mProperties.emplace_back(std::make_unique<PropertyAsset<WorldEditorTool>>(  "Default Editor Tool",   "TODO", cDefaultWorldEditorTool));
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>(           "World West Boundary",   "TODO", [this]() {return cEditorMinX;},         [this](int value) {cEditorMinX         = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>(           "World East Boundary",   "TODO", [this]() {return cEditorMaxX;},         [this](int value) {cEditorMaxX         = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>(           "World South Boundary",  "TODO", [this]() {return cEditorMinY;},         [this](int value) {cEditorMinY         = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>(           "World North Boundary",  "TODO", [this]() {return cEditorMaxY;},         [this](int value) {cEditorMaxY         = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>(           "World Depth Boundary",  "TODO", [this]() {return cEditorMinZ;},         [this](int value) {cEditorMinZ         = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>(           "World Height Boundary", "TODO", [this]() {return cEditorMaxZ;},         [this](int value) {cEditorMaxZ         = value; return true;}));
    for (unsigned int i = 0; i < cAvailableWorldEditorTools.size(); i++) {
      mProperties.emplace_back(std::make_unique<PropertyAsset<WorldEditorTool>>("World Editing Tool " + Utils::toString(i + 1), "TODO", *cAvailableWorldEditorTools[i].get(), [this, i]() {
        cAvailableWorldEditorTools.erase(cAvailableWorldEditorTools.begin() + i);
      }));
    }
    return mProperties;
  }

  void Spindizzy::add(IBoundaryType*       asset, const std::string& id) {cBoundaryTypes.add(      asset, id);}
  void Spindizzy::add(IPhysicalObjectType* asset, const std::string& id) {cPhysicalObjectTypes.add(asset, id);}
  void Spindizzy::add(IWorldEditorTool*    asset, const std::string& id) {cWorldEditorTools.add(   asset, id);}
  
  void Spindizzy::remove(IWorldEditorTool*    asset) {cWorldEditorTools.remove(   asset, true);}

  void Spindizzy::remove(WorldEditorTool& tool) {
    // TODO: Remove from our selection.
  }

  void Spindizzy::addZoneBinding(IBinding* binding1, IBinding* binding2, const std::string& id) {
    cRuntimeZoneBindings1[id] = binding1;
    cRuntimeZoneBindings2[id] = binding2;
  }

  std::vector<std::string> Spindizzy::getAllBoundaryTypes()        {return cBoundaryTypes.getAll();}
  std::vector<std::string> Spindizzy::getAllCameras()              {return cCameras.getAll();}
  std::vector<std::string> Spindizzy::getAllPhysicalObjectTypes()  {return cPhysicalObjectTypes.getAll();}
  std::vector<std::string> Spindizzy::getAllSurfacePatterns()      {return cSurfacePatterns.getAll();}
  std::vector<std::string> Spindizzy::getAllWallPatterns()         {return cWallPatterns.getAll();}
  std::vector<std::string> Spindizzy::getAllWorldEditorTools()     {return cWorldEditorTools.getAll();}
  std::vector<std::string> Spindizzy::getAllZoneObjectTypeTraits() {return cZoneObjectTypeTraits.getAll();}
  std::vector<std::string> Spindizzy::getAllZoneViewTypes()        {return cZoneViewTypes.getAll();}

  std::string Spindizzy::getID(const IBoundaryType*        asset) const {return cBoundaryTypes.getID(asset);}
  std::string Spindizzy::getID(const ICamera*              asset) const {return cCameras.getID(asset);}
  std::string Spindizzy::getID(const IPhysicalObjectType*  asset) const {return cPhysicalObjectTypes.getID(asset);}
  std::string Spindizzy::getID(const ISurfacePattern*      asset) const {return cSurfacePatterns.getID(asset);}
  std::string Spindizzy::getID(const IWallPattern*         asset) const {return cWallPatterns.getID(asset);}
  std::string Spindizzy::getID(const IWorldEditorTool*     asset) const {return cWorldEditorTools.getID(asset);}
  std::string Spindizzy::getID(const IZoneObjectTypeTrait* asset) const {return cZoneObjectTypeTraits.getID(asset);}
  std::string Spindizzy::getID(const IZoneViewType*        asset) const {return cZoneViewTypes.getID(asset);}

  bool Spindizzy::renderBoundaryTypeIcon(       const std::string& id) const {return cBoundaryTypes.renderIcon(id);}
  bool Spindizzy::renderCameraIcon(             const std::string& id) const {return cCameras.renderIcon(id);}
  bool Spindizzy::renderPhysicalObjectTypeIcon( const std::string& id) const {return cPhysicalObjectTypes.renderIcon(id);}
  bool Spindizzy::renderSurfacePatternIcon(     const std::string& id) const {return cSurfacePatterns.renderIcon(id);}
  bool Spindizzy::renderWallPatternIcon(        const std::string& id) const {return cWallPatterns.renderIcon(id);}
  bool Spindizzy::renderWorldEditorToolIcon(    const std::string& id) const {return cWorldEditorTools.renderIcon(id);}
  bool Spindizzy::renderZoneObjectTypeTraitIcon(const std::string& id) const {return cZoneObjectTypeTraits.renderIcon(id);}
  bool Spindizzy::renderZoneViewTypeIcon(       const std::string& id) const {return cZoneViewTypes.renderIcon(id);}

  bool Spindizzy::isBoundaryTypeConfigurable(       const std::string& id) const {return cBoundaryTypes.hasConfiguration(id);}
  bool Spindizzy::isCameraConfigurable(             const std::string& id) const {return cCameras.hasConfiguration(id);}
  bool Spindizzy::isPhysicalObjectTypeConfigurable( const std::string& id) const {return cPhysicalObjectTypes.hasConfiguration(id);}
  bool Spindizzy::isSurfacePatternConfigurable(     const std::string& id) const {return cSurfacePatterns.hasConfiguration(id);}
  bool Spindizzy::isWallPatternConfigurable(        const std::string& id) const {return cWallPatterns.hasConfiguration(id);}
  bool Spindizzy::isWorldEditorToolConfigurable(    const std::string& id) const {return cWorldEditorTools.hasConfiguration(id);}
  bool Spindizzy::isZoneObjectTypeTraitConfigurable(const std::string& id) const {return cZoneObjectTypeTraits.hasConfiguration(id);}
  bool Spindizzy::isZoneViewTypeConfigurable(       const std::string& id) const {return cZoneViewTypes.hasConfiguration(id);}

  IBinding* Spindizzy::getZoneBinding(const std::string& id) {
    std::size_t mSplit = id.find('/');
    if (mSplit == std::string::npos) {
      return id == BIND_TO_ZONE ? static_cast<IBinding*>(&cRuntimeParameterZone1)
                                : nullptr;
    }

    std::string mBindTo = id.substr(0, mSplit);
    std::string mBindPath = id.substr(mSplit + 1);
    if (mBindTo == BIND_TO_ZONE) {
      std::map<std::string, IBinding*>::iterator mZoneProperty = cRuntimeZoneBindings1.find(mBindPath);
      if (mZoneProperty != cRuntimeZoneBindings1.end()) {
        return mZoneProperty->second;
      }
    }
    return nullptr;
  }

  IBinding* Spindizzy::getZoneBinding2(const std::string& id) {
    std::size_t mSplit = id.find('/');
    if (mSplit == std::string::npos) {
      return id == BIND_TO_ZONE ? static_cast<IBinding*>(&cRuntimeParameterZone2)
                                : nullptr;
    }

    std::string mBindTo = id.substr(0, mSplit);
    std::string mBindPath = id.substr(mSplit + 1);
    if (mBindTo == BIND_TO_ZONE) {
      std::map<std::string, IBinding*>::iterator mZoneProperty = cRuntimeZoneBindings2.find(mBindPath);
      if (mZoneProperty != cRuntimeZoneBindings2.end()) {
        return mZoneProperty->second;
      }
    }
    return nullptr;
  }
  
  std::string Spindizzy::getZoneBindingID1(const IBinding* binding) const {
    for (const std::pair<const std::string, IBinding*>& mZoneBinding : cRuntimeZoneBindings1) {
      if (binding == mZoneBinding.second) {
        return BIND_TO_ZONE + "/" + mZoneBinding.first;
      }
    }
    
    if (binding == &cRuntimeParameterZone1) {
      return BIND_TO_ZONE;
    }
    return "";
  }

  std::string Spindizzy::getZoneBindingID2(const IBinding* binding) const {
    for (const std::pair<const std::string, IBinding*>& mZoneBinding : cRuntimeZoneBindings2) {
      if (binding == mZoneBinding.second) {
        return BIND_TO_ZONE + "/" + mZoneBinding.first;
      }
    }
    
    if (binding == &cRuntimeParameterZone2) {
      return BIND_TO_ZONE;
    }
    return "";
  }

  void Spindizzy::setBindingIdentifier(const IBindingIdentifier* bindingIdentifier) const {
    cRuntimeLocalBindingIdentifier = bindingIdentifier;
  }

  IBinding* Spindizzy::getBinding(const std::string& id) {
    return id == "alien"          ? static_cast<IBinding*>(&cRuntimeParameterAlien)
         : id == "player"         ? static_cast<IBinding*>(&cRuntimeParameterPlayer)
         : id == "fallDistance"   ? static_cast<IBinding*>(&cRuntimeParameterFallDistance)
         : id == "launchLocation" ? static_cast<IBinding*>(&cRuntimeParameterLaunchLocation)
         : id == "LaunchMomentum" ? static_cast<IBinding*>(&cRuntimeParameterLaunchMomentum)
         : id == "wall"           ? static_cast<IBinding*>(&cRuntimeParameterWall)
         : id == "zone"           ? static_cast<IBinding*>(&cRuntimeParameterZone)
         :                          nullptr;
  }

  void Spindizzy::saveBinding(JSONObject object, const IBinding* binding) const {
    if (cRuntimeLocalBindingIdentifier != nullptr) {
      object.addString(JSON_LOCAL, cRuntimeLocalBindingIdentifier->getBindingID(binding));
    }
  }

  void Spindizzy::releaseBinding(const IBinding* asset) {
    // Nothing to do.
  }

  void Spindizzy::stateChanged(ITexture* asset) {
    for (World* mWorld : cResourceWorld) {
      mWorld->updateDisplayLists();
    }
  }

  Spindizzy::SpindizzyBindingType::SpindizzyBindingType(const std::string& typeName) :
            cTypeName(typeName) {
  }

  std::string Spindizzy::SpindizzyBindingType::getBindingTypeID() const {
    return "Spindizzy/" + cTypeName;
  }

  bool Spindizzy::SpindizzyBindingType::renderAssetIcon() const {
    return false;
  }

  void Spindizzy::SpindizzyBindingType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Spindizzy::SpindizzyBindingType::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Spindizzy::SpindizzyBindingType::isDefaultConfiguration() const {
    return true;
  }

  const std::string Spindizzy::ID_RESOURCE_ALIEN                = "Alien";
  const std::string Spindizzy::ID_RESOURCE_BALL                 = "Ball";
  const std::string Spindizzy::ID_RESOURCE_BOUNDARY_HANDLER     = "BoundaryHandler";
  const std::string Spindizzy::ID_RESOURCE_C64_LIFT_GRAPHICS    = "C64LiftGraphics";
  const std::string Spindizzy::ID_RESOURCE_C64_TERRAIN_GRAPHICS = "C64TerrainGraphics";
  const std::string Spindizzy::ID_RESOURCE_COLLISION_HANDLER    = "CollisionHandler";
  const std::string Spindizzy::ID_RESOURCE_DEBRIS_CHUNK         = "DebrisChunk";
  const std::string Spindizzy::ID_RESOURCE_GYROSCOPE            = "Gyroscope";
  const std::string Spindizzy::ID_RESOURCE_JEWEL                = "Jewel";
  const std::string Spindizzy::ID_RESOURCE_LIFT                 = "Lift";
  const std::string Spindizzy::ID_RESOURCE_PICK_UP              = "PickUp";
  const std::string Spindizzy::ID_RESOURCE_PLAYER               = "Player";
  const std::string Spindizzy::ID_RESOURCE_TERRAIN              = "Terrain";
  const std::string Spindizzy::ID_RESOURCE_TERRAIN_STATE        = "TerrainState";
  const std::string Spindizzy::ID_RESOURCE_THEME_SET            = "ThemeSet";
  const std::string Spindizzy::ID_RESOURCE_TOP                  = "Top";
  const std::string Spindizzy::ID_RESOURCE_WORLD                = "World";
  const std::string Spindizzy::ID_RESOURCE_WORLD_VIEW           = "WorldView";
  const std::string Spindizzy::ID_RESOURCE_ZONE                 = "Zone";
  const std::string Spindizzy::ID_RESOURCE_ZONE_OBJECT          = "ZoneObject";
    
  const std::string Spindizzy::RESOURCE_CATEGORY_SPINDIZZY_ELEMENTS = "Spindizzy Elements";
  const std::string Spindizzy::RESOURCE_CATEGORY_SPINDIZZY_GRAPHICS = "Spindizzy Graphics";
  const std::string Spindizzy::RESOURCE_CATEGORY_SPINDIZZY_LOGIC    = "Spindizzy Logic";

  const std::string Spindizzy::JSON_AUTOMATIC_ZONE_MANAGEMENT = "automaticZoneManagement";
  const std::string Spindizzy::JSON_AUTOMATIC_ZONE_X_SIZE     = "automaticZoneXSize";
  const std::string Spindizzy::JSON_AUTOMATIC_ZONE_Y_SIZE     = "automaticZoneYSize";
  const std::string Spindizzy::JSON_AUTOMATIC_ZONE_Z_SIZE     = "automaticZoneZSize";
  const std::string Spindizzy::JSON_DEFAULT_THEME_SET         = "defaultThemeSet";
  const std::string Spindizzy::JSON_DEFAULT_WORLD_EDITOR_TOOL = "defaultWorldEditorTool";
  const std::string Spindizzy::JSON_EDITOR_MAX_X              = "editorMaxX";
  const std::string Spindizzy::JSON_EDITOR_MAX_Y              = "editorMaxY";
  const std::string Spindizzy::JSON_EDITOR_MAX_Z              = "editorMaxZ";
  const std::string Spindizzy::JSON_EDITOR_MIN_X              = "editorMinX";
  const std::string Spindizzy::JSON_EDITOR_MIN_Y              = "editorMinY";
  const std::string Spindizzy::JSON_EDITOR_MIN_Z              = "editorMinZ";
  const std::string Spindizzy::JSON_EDITOR_TOOL               = "editorTool";
  const std::string Spindizzy::JSON_EDITOR_TOOLS              = "editorTools";
  const std::string Spindizzy::JSON_LOCAL                     = "local";

  const int Spindizzy::DEFAULT_AUTOMATIC_ZONE_X_SIZE =  8;
  const int Spindizzy::DEFAULT_AUTOMATIC_ZONE_Y_SIZE =  8;
  const int Spindizzy::DEFAULT_AUTOMATIC_ZONE_Z_SIZE =  8;
  const int Spindizzy::DEFAULT_EDITOR_MAX_X          =  511;
  const int Spindizzy::DEFAULT_EDITOR_MIN_X          = -512;
  const int Spindizzy::DEFAULT_EDITOR_MAX_Y          =  511;
  const int Spindizzy::DEFAULT_EDITOR_MIN_Y          = -512;
  const int Spindizzy::DEFAULT_EDITOR_MAX_Z          =  7;
  const int Spindizzy::DEFAULT_EDITOR_MIN_Z          =  0;

  const std::string Spindizzy::RESOURCE_TYPE_ALIEN       = "Alien";
  const std::string Spindizzy::RESOURCE_TYPE_LIFT        = "Lift";
  const std::string Spindizzy::RESOURCE_TYPE_PICK_UP     = "PickUp";
  const std::string Spindizzy::RESOURCE_TYPE_PLAYER      = "Player";
  const std::string Spindizzy::RESOURCE_TYPE_TERRAIN     = "Terrain";
  const std::string Spindizzy::RESOURCE_TYPE_WORLD_VIEW  = "WorldView";
  const std::string Spindizzy::RESOURCE_TYPE_ZONE        = "Zone";
  const std::string Spindizzy::RESOURCE_TYPE_ZONE_OBJECT = "ZoneObject";
  
  const std::string Spindizzy::TOOL_DELETE      = "DeleteTool";
  const std::string Spindizzy::TOOL_PROPERTIES  = "PropertiesTool";
  const std::string Spindizzy::TOOL_COPY_ZONE   = "ZoneCopyTool";
  const std::string Spindizzy::TOOL_MOVE_ZONE   = "ZoneMoveTool";
  const std::string Spindizzy::TOOL_DELETE_ZONE = "ZoneDeleteTool";

  const std::string Spindizzy::CAMERA_GAMEPLAY     = "Gameplay";
  const std::string Spindizzy::CAMERA_LINKED       = "Linked";
  const std::string Spindizzy::CAMERA_OVERVIEW     = "Overview";
  const std::string Spindizzy::CAMERA_TRANSITIONAL = "Transitional";
  const std::string Spindizzy::CAMERA_VARIANT      = "Variant";

  const std::string Spindizzy::SURFACE_PATTERN_OUTLINE       = "Outline";
  const std::string Spindizzy::SURFACE_PATTERN_TILE          = "Tile";
  const std::string Spindizzy::SURFACE_PATTERN_SPLIT_VARIANT = "SplitVariant";
    
  const std::string Spindizzy::WALL_PATTERN_OUTLINE   = "Outline";
  const std::string Spindizzy::WALL_PATTERN_TILE      = "Tile";
  const std::string Spindizzy::WALL_PATTERN_CAPPED    = "Capped";
    
  const std::string Spindizzy::ZONE_VIEW_TYPE_ACTUAL   = "Actual";
  const std::string Spindizzy::ZONE_VIEW_TYPE_OVERVIEW = "Overview";

  const std::string Spindizzy::ZONE_OBJECT_TYPE_TRAIT_BOUNDARY      = "Boundary";
  const std::string Spindizzy::ZONE_OBJECT_TYPE_TRAIT_CELL_LOCATION = "CellLocation";
  const std::string Spindizzy::ZONE_OBJECT_TYPE_TRAIT_CHASER        = "Chaser";
  const std::string Spindizzy::ZONE_OBJECT_TYPE_TRAIT_MODEL         = "Model";
  const std::string Spindizzy::ZONE_OBJECT_TYPE_TRAIT_MOVABLE       = "Movable";
  const std::string Spindizzy::ZONE_OBJECT_TYPE_TRAIT_PHYSICS       = "Physics";
  const std::string Spindizzy::ZONE_OBJECT_TYPE_TRAIT_SPINNER       = "Spinner";

  const std::string Spindizzy::BIND_TO_ZONE = "Zone";

  std::mutex cModuleInstantiationMutex;
  std::vector<std::unique_ptr<Spindizzy>> ModuleInstances;
}

#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::IProject* project, IsoRealms::IResourceTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::IProject * project, IsoRealms::IResourceTypeRegistry * registry) {
#endif
  std::unique_ptr<IsoRealms::Spindizzy::Spindizzy> mModule = std::make_unique<IsoRealms::Spindizzy::Spindizzy>(*project, registry);
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::Spindizzy::cModuleInstantiationMutex);
    return IsoRealms::Spindizzy::ModuleInstances.emplace_back(std::move(mModule)).get();
  }
}

#ifdef __linux__
extern "C" void destroy(IsoRealms::IModuleHandle* module) {
#elif _WIN32
extern "C" void __declspec(dllexport) __stdcall destroy(IsoRealms::IModuleHandle* module) {
#endif
  std::unique_ptr<IsoRealms::Spindizzy::Spindizzy> mModule;
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::Spindizzy::cModuleInstantiationMutex);
    mModule = IsoRealms::Utils::removeElementUnique(IsoRealms::Spindizzy::ModuleInstances, module);
  }
}

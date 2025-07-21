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
#include "Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const double Spindizzy::DEFAULT_VIEW_ANGLE_PITCH = -90.0 + std::atan(1.0 / std::sqrt(2.0)) * (180.0 / M_PI);
  const double Spindizzy::DEFAULT_VIEW_ANGLE_YAW = -45.0;

  Spindizzy::Spindizzy(Project& project, IResourceTypeRegistry* registry) :
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
                    cResourceModelCycler(*this),
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
                    cBindingTypeTerrainState("TerrainState"),
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
    registry->add(&cResourceAlien,              "Alien");
    registry->add(&cResourceBall,               "Ball");
    registry->add(&cResourceBoundaryHandler,    "BoundaryHandler");
    registry->add(&cResourceC64LiftGraphics,    "C64LiftGraphics");
    registry->add(&cResourceC64TerrainGraphics, "C64TerrainGraphics");
    registry->add(&cResourceCollisionHandler,   "CollisionHandler");
    registry->add(&cResourceDebrisChunk,        "DebrisChunk");
    registry->add(&cResourceGyroscope,          "Gyroscope");
    registry->add(&cResourceJewel,              "Jewel");
    registry->add(&cResourceLift,               "Lift");
    registry->add(&cResourceModelCycler,        "ModelCycler");
    registry->add(&cResourcePickUp,             "PickUp");
    registry->add(&cResourcePlayer,             "Player");
    registry->add(&cResourceTerrain,            "Terrain");
    registry->add(&cResourceTerrainState,       "TerrainState");
    registry->add(&cResourceThemeSet,           "ThemeSet");
    registry->add(&cResourceTop,                "Top");
    registry->add(&cResourceWorld,              "World");
    registry->add(&cResourceWorldView,          "WorldView");
    registry->add(&cResourceZone,               "Zone");
    registry->add(&cResourceZoneObject,         "ZoneObject");
        
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
  
  Project& Spindizzy::getProject() const {
    return cProject;
  }

  Spindizzy& Spindizzy::getAssetManager() {
    return *this;
  }

  bool Spindizzy::isReadOnly() const {
    return false;
  } // TODO: Probably shouldn't be here.

  void Spindizzy::setOwner(ProjectFile* owner) {
  } // TODO: Probably shouldn't be here.

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

  IBoundaryType*        Spindizzy::getBoundaryType(       IAssetUser<IBoundaryType>*        user, const std::string& id)                                                                    {return cBoundaryTypes.get(       user, *this, id, nullptr);}
  ICamera*              Spindizzy::getCamera(             IAssetUser<ICamera>*              user, const std::string& id, WorldView&      owner)                                             {return cCameras.get(             user, owner, id, nullptr);}
  IPhysicalObjectType*  Spindizzy::getPhysicalObjectType( IAssetUser<IPhysicalObjectType>*  user, const std::string& id)                                                                    {return cPhysicalObjectTypes.get( user, *this, id, nullptr);}
  ISurfacePattern*      Spindizzy::getSurfacePattern(     IAssetUser<ISurfacePattern>*      user, const std::string& id, TerrainType&    owner, IStateListener<ISurfacePattern*>* listener) {return cSurfacePatterns.get(     user, owner, id, listener);}
  IWallPattern*         Spindizzy::getWallPattern(        IAssetUser<IWallPattern>*         user, const std::string& id, TerrainType&    owner, IStateListener<IWallPattern*>*    listener) {return cWallPatterns.get(        user, owner, id, listener);}
  IWorldEditorTool*     Spindizzy::getWorldEditorTool(    IAssetUser<IWorldEditorTool>*     user, const std::string& id)                                                                    {return cWorldEditorTools.get(    user, *this, id, nullptr);}
  IZoneObjectTypeTrait* Spindizzy::getZoneObjectTypeTrait(IAssetUser<IZoneObjectTypeTrait>* user, const std::string& id, ZoneObjectType& owner)                                             {return cZoneObjectTypeTraits.get(user, owner, id, nullptr);}
  IZoneViewType*        Spindizzy::getZoneViewType(       IAssetUser<IZoneViewType>*        user, const std::string& id, WorldView&      owner)                                             {return cZoneViewTypes.get(       user, owner, id, nullptr);}

  IBoundaryType*        Spindizzy::getBoundaryType(       IAssetUser<IBoundaryType>*        user, JSONObject object)                                                                    {return cBoundaryTypes.get(       user, *this, object, nullptr,  true, [this](JSONObject object, IStateListener<IBoundaryType*>*        listener) -> IBoundaryType*        {return nullptr;});}
  ICamera*              Spindizzy::getCamera(             IAssetUser<ICamera>*              user, JSONObject object, WorldView&      owner)                                             {return cCameras.get(             user, owner, object, nullptr,  true, [this](JSONObject object, IStateListener<ICamera*>*              listener) -> ICamera*              {return nullptr;});}
  IPhysicalObjectType*  Spindizzy::getPhysicalObjectType( IAssetUser<IPhysicalObjectType>*  user, JSONObject object)                                                                    {return cPhysicalObjectTypes.get( user, *this, object, nullptr,  true, [this](JSONObject object, IStateListener<IPhysicalObjectType*>*  listener) -> IPhysicalObjectType*  {return nullptr;});}
  ISurfacePattern*      Spindizzy::getSurfacePattern(     IAssetUser<ISurfacePattern>*      user, JSONObject object, TerrainType&    owner, IStateListener<ISurfacePattern*>* listener) {return cSurfacePatterns.get(     user, owner, object, listener, true, [this](JSONObject object, IStateListener<ISurfacePattern*>*      listener) -> ISurfacePattern*      {return nullptr;});}
  IWallPattern*         Spindizzy::getWallPattern(        IAssetUser<IWallPattern>*         user, JSONObject object, TerrainType&    owner, IStateListener<IWallPattern*>*    listener) {return cWallPatterns.get(        user, owner, object, listener, true, [this](JSONObject object, IStateListener<IWallPattern*>*         listener) -> IWallPattern*         {return nullptr;});}
  IWorldEditorTool*     Spindizzy::getWorldEditorTool(    IAssetUser<IWorldEditorTool>*     user, JSONObject object)                                                                    {return cWorldEditorTools.get(    user, *this, object, nullptr,  true, [this](JSONObject object, IStateListener<IWorldEditorTool*>*     listener) -> IWorldEditorTool*     {return nullptr;});}
  IZoneObjectTypeTrait* Spindizzy::getZoneObjectTypeTrait(IAssetUser<IZoneObjectTypeTrait>* user, JSONObject object, ZoneObjectType& owner)                                             {return cZoneObjectTypeTraits.get(user, owner, object, nullptr,  true, [this](JSONObject object, IStateListener<IZoneObjectTypeTrait*>* listener) -> IZoneObjectTypeTrait* {return nullptr;});}
  IZoneViewType*        Spindizzy::getZoneViewType(       IAssetUser<IZoneViewType>*        user, JSONObject object, WorldView&      owner)                                             {return cZoneViewTypes.get(       user, owner, object, nullptr,  true, [this](JSONObject object, IStateListener<IZoneViewType*>*        listener) -> IZoneViewType*        {return nullptr;});}

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
  
  std::vector<IWorldEditorToolInstance*> Spindizzy::createToolSet(WorldEditor& editor, IPropertyOwner& owner) {
    std::vector<IWorldEditorToolInstance*> mTools;
    for (std::unique_ptr<WorldEditorTool>& mTool : cAvailableWorldEditorTools) {
      mTools.emplace_back((*mTool)->createToolInstance(editor, owner));
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

  void Spindizzy::save(JSONObject object) {
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

  void Spindizzy::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IBindingType>(&cBindingTypeTerrainState, "Terrain State", "Spindizzy");

    // TODO: The following things should not be needed!!!
    assets.add<IBinding>(&cRuntimeParameterAlien,          "Alien",          "Spindizzy");
    assets.add<IBinding>(&cRuntimeParameterFallDistance,   "FallDistance",   "Spindizzy");
    assets.add<IBinding>(&cRuntimeParameterLaunchLocation, "LaunchLocation", "Spindizzy");
    assets.add<IBinding>(&cRuntimeParameterLaunchMomentum, "LaunchMomentum", "Spindizzy");
    assets.add<IBinding>(&cRuntimeParameterPlayer,         "Player",         "Spindizzy");
    assets.add<IBinding>(&cRuntimeParameterWall,           "Wall",           "Spindizzy");
    assets.add<IBinding>(&cRuntimeParameterZone,           "Zone",           "Spindizzy");
    assets.add<IBinding>(&cLuaBinding,                     "",               "Spindizzy");
    
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

  std::vector<std::unique_ptr<IProperty>> Spindizzy::getProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
//     mProperties.emplace_back(owner.createPropertyNativeInteger("DefaultZoneWidth",    [this]() {return cAutomaticZoneXSize;}, [this](int value) {cAutomaticZoneXSize = value; return true;}));
//     mProperties.emplace_back(owner.createPropertyNativeInteger("DefaultZoneLength",   [this]() {return cAutomaticZoneYSize;}, [this](int value) {cAutomaticZoneYSize = value; return true;}));
//     mProperties.emplace_back(owner.createPropertyNativeInteger("DefaultZoneHeight",   [this]() {return cAutomaticZoneZSize;}, [this](int value) {cAutomaticZoneZSize = value; return true;}));
// //    mProperties.emplace_back(std::make_unique<PropertyAsset<ZoneType>>("Default Zone Type", *cAutomaticZoneManagementType));
// //    mProperties.emplace_back(std::make_unique<PropertyAsset<ThemeSet>>("Default Zone Theme", *cDefaultThemeSet));
//     mProperties.emplace_back(std::make_unique<PropertyAsset<WorldEditorTool>>(  PropertyData("Default Editor Tool",   "TODO"), cDefaultWorldEditorTool));
//     mProperties.emplace_back(owner.createPropertyNativeInteger("WorldWestBoundary",   [this]() {return cEditorMinX;},         [this](int value) {cEditorMinX         = value; return true;}));
//     mProperties.emplace_back(owner.createPropertyNativeInteger("WorldEastBoundary",   [this]() {return cEditorMaxX;},         [this](int value) {cEditorMaxX         = value; return true;}));
//     mProperties.emplace_back(owner.createPropertyNativeInteger("WorldSouthBoundary",  [this]() {return cEditorMinY;},         [this](int value) {cEditorMinY         = value; return true;}));
//     mProperties.emplace_back(owner.createPropertyNativeInteger("WorldNorthBoundary",  [this]() {return cEditorMaxY;},         [this](int value) {cEditorMaxY         = value; return true;}));
//     mProperties.emplace_back(owner.createPropertyNativeInteger("WorldDepthBoundary",  [this]() {return cEditorMinZ;},         [this](int value) {cEditorMinZ         = value; return true;}));
//     mProperties.emplace_back(owner.createPropertyNativeInteger("WorldHeightBoundary", [this]() {return cEditorMaxZ;},         [this](int value) {cEditorMaxZ         = value; return true;}));
//     for (unsigned int i = 0; i < cAvailableWorldEditorTools.size(); i++) {
//       mProperties.emplace_back(std::make_unique<PropertyAsset<WorldEditorTool>>(PropertyData("World Editing Tool " + Utils::toString(i + 1), "TODO"), *cAvailableWorldEditorTools[i].get(), [this, i]() {
//         cAvailableWorldEditorTools.erase(cAvailableWorldEditorTools.begin() + i);
//       }));
//     }
    return mProperties;
  }

  void Spindizzy::updateRuntime(unsigned int milliseconds) {
    updateRuntime2(cResourceWorld,     milliseconds);
    updateRuntime2(cResourceWorldView, milliseconds);
  }
  
  void Spindizzy::updateEditing(unsigned int milliseconds) {
    updateEditing2(cResourceDebrisChunk, milliseconds);
    updateEditing2(cResourceGyroscope,   milliseconds);
    updateEditing2(cResourceJewel,       milliseconds);
    updateEditing2(cResourceModelCycler, milliseconds);
    updateEditing2(cResourceThemeSet,    milliseconds);
    updateEditing2(cResourceTop,         milliseconds);
    updateEditing2(cResourceWorld,       milliseconds);
  }
  
  void Spindizzy::reset() {
    reset2(cResourceAlien);
    reset2(cResourcePlayer);
    reset2(cResourceModelCycler);
    reset2(cResourceTerrainState);
    reset2(cResourceWorld);
    reset2(cResourceWorldView);
  }  
  
  void Spindizzy::add(IBoundaryType*       asset, const std::string& id) {cBoundaryTypes.add(      asset, id);}
  void Spindizzy::add(IPhysicalObjectType* asset, const std::string& id) {cPhysicalObjectTypes.add(asset, id);}
  void Spindizzy::add(IWorldEditorTool*    asset, const std::string& id) {cWorldEditorTools.add(   asset, id);}
  
  void Spindizzy::remove(IWorldEditorTool*    asset) {cWorldEditorTools.remove(   asset);}

  void Spindizzy::remove(WorldEditorTool& tool) {
    // TODO: Remove from our selection.
  }

  void Spindizzy::addZoneBinding(IBinding* binding1, IBinding* binding2, const std::string& id) {
    cRuntimeZoneBindings1[id] = binding1;
    cRuntimeZoneBindings2[id] = binding2;
  }

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

  std::vector<std::unique_ptr<IProperty>> Spindizzy::SpindizzyBindingType::getAssetProperties(IPropertyOwner& owner) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Spindizzy::SpindizzyBindingType::isDefaultConfiguration() const {
    return true;
  }

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
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::Project* project, IsoRealms::IResourceTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::Project * project, IsoRealms::IResourceTypeRegistry * registry) {
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

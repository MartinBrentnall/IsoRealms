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
#include "World.h"

#include "Modules/Spindizzy/Assets/Type/IPhysicalObjectType.h"
#include "Modules/Spindizzy/Assets/Type/IWorldEditorTool.h"
#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const std::string World::JSON_BASIC_PROPERTIES  = "basicProperties";
  const std::string World::JSON_BOUNCE_CONTROL    = "bounceControl";
  const std::string World::JSON_DEBRIS_GENERATORS = "debrisGenerators";
  const std::string World::JSON_GRAVITY           = "gravity";
  const std::string World::JSON_PLAYERS           = "players";
  const std::string World::JSON_SLOPE_FORCE       = "slopeForce";
  const std::string World::JSON_ZONES             = "zones";

  const std::string World::JSON_AUTOMATIC_ZONE_MANAGEMENT = "automaticZoneManagement";
  const std::string World::JSON_AUTOMATIC_ZONE_X_SIZE     = "automaticZoneXSize";
  const std::string World::JSON_AUTOMATIC_ZONE_Y_SIZE     = "automaticZoneYSize";
  const std::string World::JSON_AUTOMATIC_ZONE_Z_SIZE     = "automaticZoneZSize";
  const std::string World::JSON_DEFAULT_THEME_SET         = "defaultThemeSet";
  const std::string World::JSON_DEFAULT_WORLD_EDITOR_TOOL = "defaultWorldEditorTool";
  const std::string World::JSON_EDITOR_MAX_X              = "editorMaxX";
  const std::string World::JSON_EDITOR_MAX_Y              = "editorMaxY";
  const std::string World::JSON_EDITOR_MAX_Z              = "editorMaxZ";
  const std::string World::JSON_EDITOR_MIN_X              = "editorMinX";
  const std::string World::JSON_EDITOR_MIN_Y              = "editorMinY";
  const std::string World::JSON_EDITOR_MIN_Z              = "editorMinZ";
  const std::string World::JSON_EDITOR_TOOL               = "editorTool";
  const std::string World::JSON_EDITOR_TOOLS              = "editorTools";

  const unsigned int World::DEFAULT_BOUNCE_CONTROL = 10;

  const int World::DEFAULT_AUTOMATIC_ZONE_X_SIZE =  8;
  const int World::DEFAULT_AUTOMATIC_ZONE_Y_SIZE =  8;
  const int World::DEFAULT_AUTOMATIC_ZONE_Z_SIZE =  8;
  const int World::DEFAULT_EDITOR_MAX_X          =  511;
  const int World::DEFAULT_EDITOR_MIN_X          = -512;
  const int World::DEFAULT_EDITOR_MAX_Y          =  511;
  const int World::DEFAULT_EDITOR_MIN_Y          = -512;
  const int World::DEFAULT_EDITOR_MAX_Z          =  7;
  const int World::DEFAULT_EDITOR_MIN_Z          =  0;

  World::World(Spindizzy& spindizzy, IResourceData& data) :
            cSpindizzy(spindizzy),
            cResourceData(data),
            cDefPhysicalSurfaceProcessor(true),
            cDefVisualSurfaceProcessor(false),
            cEditorBasicProperties(false),
            cDefaultThemeSet(*this),
            cAutomaticZoneManagementType(*this),
            cDefaultWorldEditorTool(spindizzy),
            cAutomaticZoneXSize(DEFAULT_AUTOMATIC_ZONE_X_SIZE),
            cAutomaticZoneYSize(DEFAULT_AUTOMATIC_ZONE_Y_SIZE),
            cAutomaticZoneZSize(DEFAULT_AUTOMATIC_ZONE_Z_SIZE),
            cEditorMinX(DEFAULT_EDITOR_MIN_X),
            cEditorMaxX(DEFAULT_EDITOR_MAX_X),
            cEditorMinY(DEFAULT_EDITOR_MIN_Y),
            cEditorMaxY(DEFAULT_EDITOR_MAX_Y),
            cEditorMinZ(DEFAULT_EDITOR_MIN_Z),
            cEditorMaxZ(DEFAULT_EDITOR_MAX_Z),
            cLuaBinding(data.getProject().getLuaState(), this) {
    // Physical object types.
    std::vector<IPhysicalObjectType*> mPhysicalObjectTypes = cSpindizzy.getAllPhysicalObjectTypeObjects();
    for (IPhysicalObjectType* mPhysicalObjectType : mPhysicalObjectTypes) {
      added(mPhysicalObjectType);
    }
    
    // Boundary types.
    std::vector<IBoundaryType*> mBoundaryTypes = cSpindizzy.getAllBoundaryTypeObjects();
    for (IBoundaryType* mBoundaryType : mBoundaryTypes) {
      added(mBoundaryType);
    }

    data.getProject().getApplication().mainThreadInit([this]() {
      glColor3f(1.0f, 1.0f, 1.0f);
      for (std::unique_ptr<Zone>& mZone : cDefZones) {
        mZone->updateDisplayList();
      }
    });

    // Dummy physical object type.
    added(cSpindizzy.getAsset<IPhysicalObjectType>(&cDummyPhysicalObjectTypeUser, "None", cSpindizzy));
  }

  World::World(Spindizzy& spindizzy, IResourceData& data, JSONObject object) :
            World(spindizzy, data) {
    for (JSONValue mDebrisGeneratorValue : object.getArray(JSON_DEBRIS_GENERATORS)) {
      cDefDebrisGenerators.emplace_back(std::make_unique<DebrisGenerator>(mDebrisGeneratorValue.getObject(), data));
    }

    for (JSONValue mPlayerValue : object.getArray(JSON_PLAYERS)) {
      cDefPlayers.emplace_back(std::make_unique<Player>(*this, mPlayerValue.getObject()));
    }

    for (JSONValue mZoneValue : object.getArray(JSON_ZONES)) {
      cDefZones.emplace_back(std::make_unique<Zone>(*this, mZoneValue.getObject()));
    }

    cDefSurfaceAccelerationFactor = object.getFloat(JSON_SLOPE_FORCE);
    cDefGravity                   = object.getFloat(JSON_GRAVITY);
    cDefBounceTime                = object.getInteger(JSON_BOUNCE_CONTROL, DEFAULT_BOUNCE_CONTROL);
    cEditorBasicProperties        = object.getBoolean(JSON_BASIC_PROPERTIES);

    data.getProject().init([this, &data]() {

      // Try to open terrain cache
      std::string mCachePath = cResourceData.getPath("Terrain.cache", data.getProject().isUser());
//      std::cout << "Cache path: " << mCachePath << std::endl;
      std::ifstream mCache(mCachePath, std::ios::binary);
      bool mUsingCache = false;
      if (mCache) {
//        std::filesystem::file_time_type mCacheTime = std::filesystem::last_write_time(mCachePath);
//        std::filesystem::file_time_type mProjectTime = project->getLastWriteTime();
//
//        if (mCacheTime > mProjectTime) {
          mUsingCache = true;
//        } else {
//          mCache.close();
//        }
      }
      updateBounds();

      if (mUsingCache) {
        for (std::unique_ptr<Zone>& mZone : cDefZones) {
          mZone->initialiseObjects();
          mZone->initialiseTerrain(mCache);
        }
      } else {

        // Multi-threaded world initialisation
        std::vector<std::function<void()>> mTask;
        for (Zone* mZone : cRuntimeZonesToInitialise) {
//        mTask.push_back([&mZone, mUsingCache]() { TODO: Enable this for multi-threaded initialisation.
          mZone->initialiseObjects();
          mZone->initialiseTerrain();
//        });
        }
//         Application& mApplication = data.getProject().getApplication();
//         mApplication.executeAndWait(mTask);
//         std::cout << "INFO: World::World: Updating cache..." << std::endl;
        updateCache();
      }
      cRuntimeZonesToInitialise.clear();
    });

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
    data.getProject().init([this, object]() {
      cAutomaticZoneManagementType.setID(object.getString(JSON_AUTOMATIC_ZONE_MANAGEMENT));
      cDefaultThemeSet.setID(object.getString(JSON_DEFAULT_THEME_SET));
      for (JSONValue mEditingToolValue : object.getArray(JSON_EDITOR_TOOLS)) {
        cAvailableWorldEditorTools.emplace_back(std::make_unique<WorldEditorTool>(cSpindizzy)).get()->set(mEditingToolValue.getObject(), JSON_EDITOR_TOOL);
      }
    });
  }

  void World::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IEditable>(this, "", "Spindizzy Worlds");
    assets.add<IBinding>(&cLuaBinding, "", "Spindizzy/Worlds");
    for (std::unique_ptr<DebrisGenerator>& mDebrisGenerator : cDefDebrisGenerators) {
      mDebrisGenerator->registerAssets(assets, "DebrisGenerator");
    }
    for (std::unique_ptr<Player>& mPlayer : cDefPlayers) {
      mPlayer->registerAssets(assets, "Player");
    }
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->registerAssets();
    }
  }

  void World::save(JSONObject object) const {
    object.addFloat(JSON_SLOPE_FORCE, cDefSurfaceAccelerationFactor);
    object.addFloat(JSON_GRAVITY, cDefGravity);
    object.addInteger(JSON_BOUNCE_CONTROL, cDefBounceTime, DEFAULT_BOUNCE_CONTROL);
    object.addBoolean(JSON_BASIC_PROPERTIES, cEditorBasicProperties);

    JSONArray mDebrisGeneratorsArray = object.addArray(JSON_DEBRIS_GENERATORS);
    JSONArray mPlayersArray = object.addArray(JSON_PLAYERS);
    JSONArray mZonesArray = object.addArray(JSON_ZONES);

    for (const std::unique_ptr<DebrisGenerator>& mDebrisGenerator : cDefDebrisGenerators) {
      JSONObject mDebrisGeneratorObject = mDebrisGeneratorsArray.addObject();
      mDebrisGenerator->save(mDebrisGeneratorObject);
    }
    for (const std::unique_ptr<Player>& mPlayer : cDefPlayers) {
      JSONObject mPlayerObject = mPlayersArray.addObject();
      mPlayer->save(mPlayerObject);
    }
    for (const std::unique_ptr<Zone>& mZone : cDefZones) {
      JSONObject mZoneObject = mZonesArray.addObject();
      mZone->save(mZoneObject);
    }

    object.addInteger(JSON_AUTOMATIC_ZONE_X_SIZE,     cAutomaticZoneXSize, DEFAULT_AUTOMATIC_ZONE_X_SIZE);
    object.addInteger(JSON_AUTOMATIC_ZONE_Y_SIZE,     cAutomaticZoneYSize, DEFAULT_AUTOMATIC_ZONE_Y_SIZE);
    object.addInteger(JSON_AUTOMATIC_ZONE_Z_SIZE,     cAutomaticZoneZSize, DEFAULT_AUTOMATIC_ZONE_Z_SIZE);
    object.addInteger(JSON_EDITOR_MIN_X,              cEditorMinX,         DEFAULT_EDITOR_MIN_X);
    object.addInteger(JSON_EDITOR_MAX_X,              cEditorMaxX,         DEFAULT_EDITOR_MAX_X);
    object.addInteger(JSON_EDITOR_MIN_Y,              cEditorMinY,         DEFAULT_EDITOR_MIN_Y);
    object.addInteger(JSON_EDITOR_MAX_Y,              cEditorMaxY,         DEFAULT_EDITOR_MAX_Y);
    object.addInteger(JSON_EDITOR_MIN_Z,              cEditorMinZ,         DEFAULT_EDITOR_MIN_Z);
    object.addInteger(JSON_EDITOR_MAX_Z,              cEditorMaxZ,         DEFAULT_EDITOR_MAX_Z);
    cAutomaticZoneManagementType.save(object, JSON_AUTOMATIC_ZONE_MANAGEMENT);
    cDefaultThemeSet.save(object, JSON_DEFAULT_THEME_SET);
    cDefaultWorldEditorTool.save(object, JSON_DEFAULT_WORLD_EDITOR_TOOL);
    JSONArray mEditorToolsArray = object.addArray(JSON_EDITOR_TOOLS);
    for (const std::unique_ptr<WorldEditorTool>& mTool : cAvailableWorldEditorTools) {
      mTool->save(mEditorToolsArray.addObject(), JSON_EDITOR_TOOL);
    }

    updateCache();
  }

  void World::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool World::renderIcon() {
    return false;
  }

  void World::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeFloat(    metadata.getPropertyData("Gravity"),                    [this]() {return cDefGravity;},                   [this](float value) {cDefGravity                   = value;});
    owner.createPropertyNativeFloat(    metadata.getPropertyData("SlopeEffect"),                [this]() {return cDefSurfaceAccelerationFactor;}, [this](float value) {cDefSurfaceAccelerationFactor = value;});
    owner.createPropertyNativeInteger(  metadata.getPropertyData("BounceTime"),                 [this]() {return cDefBounceTime;},                [this](bool  value) {cDefBounceTime                = value;});
    owner.createPropertyStruct(         metadata.getPropertyData("Editing"), "Edit...",         [this, &metadata](PropertyMaker& owner) {
      owner.createPropertyNativeBoolean(metadata.getPropertyData("EditingAdvancedProperties"),  [this]() {return !cEditorBasicProperties;},       [this](bool  value) {cEditorBasicProperties        = !value;});
      owner.createPropertyNativeInteger(metadata.getPropertyData("EditingDefaultZoneWidth"),    [this]() {return cAutomaticZoneXSize;},           [this](int   value) {cAutomaticZoneXSize           = value;});
      owner.createPropertyNativeInteger(metadata.getPropertyData("EditingDefaultZoneLength"),   [this]() {return cAutomaticZoneYSize;},           [this](int   value) {cAutomaticZoneYSize           = value;});
      owner.createPropertyNativeInteger(metadata.getPropertyData("EditingDefaultZoneHeight"),   [this]() {return cAutomaticZoneZSize;},           [this](int   value) {cAutomaticZoneZSize           = value;});
      owner.createPropertyTreeSelector( metadata.getPropertyData("EditingAutomaticZoneType"),   cAutomaticZoneManagementType);
      owner.createPropertyTreeSelector( metadata.getPropertyData("EditingDefaultZoneThemeSet"), cDefaultThemeSet);
      owner.createPropertyTreeSelector( metadata.getPropertyData("EditingDefaultEditingTool"),  cDefaultWorldEditorTool);
      owner.createPropertyNativeInteger(metadata.getPropertyData("EditingBoundaryWest"),        [this]() {return cEditorMinX;},                   [this](int   value) {cEditorMinX                   = value;});
      owner.createPropertyNativeInteger(metadata.getPropertyData("EditingBoundaryEast"),        [this]() {return cEditorMaxX;},                   [this](int   value) {cEditorMaxX                   = value;});
      owner.createPropertyNativeInteger(metadata.getPropertyData("EditingBoundarySouth"),       [this]() {return cEditorMinY;},                   [this](int   value) {cEditorMinY                   = value;});
      owner.createPropertyNativeInteger(metadata.getPropertyData("EditingBoundaryNorth"),       [this]() {return cEditorMaxY;},                   [this](int   value) {cEditorMaxY                   = value;});
      owner.createPropertyNativeInteger(metadata.getPropertyData("EditingBoundaryDepth"),       [this]() {return cEditorMinZ;},                   [this](int   value) {cEditorMinZ                   = value;});
      owner.createPropertyNativeInteger(metadata.getPropertyData("EditingBoundaryHeight"),      [this]() {return cEditorMaxZ;},                   [this](int   value) {cEditorMaxZ                   = value;});
      for (unsigned int i = 0; i < cAvailableWorldEditorTools.size(); i++) {
        owner.createPropertyTreeSelector(metadata.getPropertyData("EditingEditingTool"), *cAvailableWorldEditorTools[i].get(), [this, i]() {
          cAvailableWorldEditorTools.erase(cAvailableWorldEditorTools.begin() + i);
        });
      }
    });
    owner.createPropertyEditor(metadata.getPropertyData("Content"), this);
  }

  void World::removed() {
    // Nothing to do.
  }

  Spindizzy& World::getAssetManager() {
    return cSpindizzy;
  }

  Project& World::getProject() const {
    return cSpindizzy.getProject();
  }

  bool World::isReadOnly() const {
    return cResourceData.isReadOnly();
  }

  void World::setOwner(ProjectFile* owner) {
    cResourceData.setOwner(owner);
  }

  bool World::hasReadOnlyReferences() const {
    return cSpindizzy.hasReadOnlyResourceReferences(this);
  }

  void World::overrideReadOnlyReferences() {
    cSpindizzy.overrideReadOnlyResourceReferences(this);
  }

  void World::updateRuntime(unsigned int milliseconds) {
    for (std::unique_ptr<Player>&                   mPlayer           : cDefPlayers)               {mPlayer->updateRuntime(milliseconds);}
    for (std::unique_ptr<DebrisGenerator>&          mDebrisGenerator  : cDefDebrisGenerators)      {mDebrisGenerator->updateRuntime(milliseconds);}
    for (std::unique_ptr<Zone>&                     mZone             : cDefZones)                 {mZone->updateRuntime(milliseconds);}
    for (std::unique_ptr<CollisionHandlerInstance>& mCollisionHandler : cRuntimeCollisionHandlers) {mCollisionHandler->processCollisions();}
    for (std::unique_ptr<BoundaryHandlerInstance>&  mBoundaryHandler  : cRuntimeBoundaryHandlers)  {mBoundaryHandler->processCrossings();}
  }

  void World::updateEditing(unsigned int milliseconds) {
    for (const std::pair<IEditableScreen* const, std::unique_ptr<WorldEditor>>& mEditor : cEditors) {
      mEditor.second->updateScreen(milliseconds);
    }
    for (Zone* mZone : cRuntimeZonesToInitialise) {
      mZone->initialiseTerrain();
    }
    cRuntimeZonesToInitialise.clear();
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->updateEditing(milliseconds);
    }
    for (std::unique_ptr<Player>& mPlayer : cDefPlayers) {
      mPlayer->updateEditing(milliseconds);
    }
  }

  void World::reset() {
    for (std::unique_ptr<Player>& mPlayer : cDefPlayers) {
      mPlayer->reset();
    }
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->reset();
    }
  }

  IResourceData& World::getResourceData() {
    return cResourceData;
  }

  void World::renderRuntime() {
    for (std::unique_ptr<Player>& mPlayer : cDefPlayers) {
      mPlayer->renderRuntime();
    }
    for (std::unique_ptr<DebrisGenerator>& mDebrisGenerator : cDefDebrisGenerators) {
      mDebrisGenerator->renderRuntime();
    }
  }

  Spindizzy& World::getSpindizzy() const {
    return cSpindizzy;
  }

  std::vector<std::unique_ptr<Zone>>& World::getZones() {
    return cDefZones;
  }
  
  Zone* World::getZone(IVertex* location) {
    return getZone(WorldEditorCursorCell(std::round(location->getX()), std::round(location->getY()), std::round(location->getZ())));
  }

  int World::getStartX() {
    return cRuntimeCacheStartX;
  }

  int World::getStartY() {
    return cRuntimeCacheStartY;
  }

  int World::getStartZ() {
    return cRuntimeCacheStartZ;
  }

  int World::getEndX() {
    return cRuntimeCacheEndX;
  }

  int World::getEndY() {
    return cRuntimeCacheEndY;
  }

  int World::getEndZ() {
    return cRuntimeCacheEndZ;
  }

  void World::updateDisplayLists() {
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->updateDisplayList();
    }
  }

  void World::added(IBoundaryType* boundaryType) {
    cRuntimeBoundaries.emplace(boundaryType, std::make_unique<SpatialContainerTest<IBoundary*>>());
  }

  void World::removed(IBoundaryType* boundaryType) {
    cRuntimeBoundaries.erase(boundaryType);
  }

  void World::registerBoundary(IBoundaryType* boundaryType, IBoundary* boundary, int xStart, int xEnd, int yStart, int yEnd) {
    std::map<IBoundaryType*, std::unique_ptr<SpatialContainerTest<IBoundary*>>>::iterator mBoundaries = cRuntimeBoundaries.find(boundaryType);
    if (mBoundaries == cRuntimeBoundaries.end()) {
      throw ArgumentException("ERROR: World::registerBoundary: Specified boundary type must be registered first (\"" + boundaryType->getBoundaryTypeID() + "\")");
    }
    mBoundaries->second->add(xStart, xEnd, yStart, yEnd, boundary);
  }

  void World::unregisterBoundary(IBoundaryType* boundaryType, IBoundary* boundary) {
    std::map<IBoundaryType*, std::unique_ptr<SpatialContainerTest<IBoundary*>>>::iterator mBoundaries = cRuntimeBoundaries.find(boundaryType);
    if (mBoundaries == cRuntimeBoundaries.end()) {
      throw ArgumentException("ERROR: World::unregisterBoundary: Specified boundary type must be registered first (\"" + boundaryType->getBoundaryTypeID() + "\")");
    }
    mBoundaries->second->remove(boundary);
  }

  SpatialContainerTest<IBoundary*>* World::getBoundaries(IBoundaryType* type) {
    return cRuntimeBoundaries.find(type)->second.get();
  }

  void World::addBoundaryHandler(std::unique_ptr<BoundaryHandlerInstance> handler) {
    cRuntimeBoundaryHandlers.emplace_back(std::move(handler));
  }

  void World::removeBoundaryHandler(BoundaryHandler* handler) {
    std::remove_if(cRuntimeBoundaryHandlers.begin(), cRuntimeBoundaryHandlers.end(), [handler](const std::unique_ptr<BoundaryHandlerInstance>& mHandler) {
      return mHandler->getType() == handler;
    });
  }

  void World::addCollisionHandler(std::unique_ptr<CollisionHandlerInstance> handler) {
    cRuntimeCollisionHandlers.emplace_back(std::move(handler));
  }

  void World::removeCollisionHandler(CollisionHandler* handler) {
    std::remove_if(cRuntimeCollisionHandlers.begin(), cRuntimeCollisionHandlers.end(), [handler](const std::unique_ptr<CollisionHandlerInstance>& mHandler) {
      return mHandler->getType() == handler;
    });
  }

  void World::physicalObjectTypeChanged(CollisionHandler* handler, const IPhysicalObjectType* oldPhysicalObjectType, const IPhysicalObjectType* newPhysicalObjectType) {
    for (std::unique_ptr<CollisionHandlerInstance>& mCollisionHandler : cRuntimeCollisionHandlers) {
      if (mCollisionHandler->getType() == handler) {
        mCollisionHandler->typeChanged(oldPhysicalObjectType, newPhysicalObjectType);
      }
    }
  }

  void World::added(IPhysicalObjectType* type) {
    cRuntimeMovementHandlers.emplace(type, std::make_unique<MovementHandler>());
  }
  
  void World::removed(IPhysicalObjectType* type) {
    cRuntimeMovementHandlers.erase(type);
  }

  void World::addMovementListener(const IPhysicalObjectType* type, IMovementListener* listener) {
    std::map<const IPhysicalObjectType*, std::unique_ptr<MovementHandler>>::iterator mIterator = cRuntimeMovementHandlers.find(type);
    if (mIterator == cRuntimeMovementHandlers.end()) {
      throw ArgumentException("ERROR: World::addMovementListener: Specified physical object type isn't known.");
    }
    mIterator->second->addListener(listener);
  }
  
  void World::removeMovementListener(const IPhysicalObjectType* type, IMovementListener* listener) {
    std::map<const IPhysicalObjectType*, std::unique_ptr<MovementHandler>>::iterator mIterator = cRuntimeMovementHandlers.find(type);
    if (mIterator == cRuntimeMovementHandlers.end()) {
      throw ArgumentException("ERROR: World::removeMovementListener: Specified physical object type isn't known.");
    }
    mIterator->second->removeListener(listener);
  }

  MovementHandler* World::getMovementHandler(IPhysicalObjectType* type) {
    std::map<const IPhysicalObjectType*, std::unique_ptr<MovementHandler>>::iterator mIterator = cRuntimeMovementHandlers.find(type);
    if (mIterator == cRuntimeMovementHandlers.end()) {
      throw ArgumentException("ERROR: World::getMovementListener: Specified physical object type isn't known.");
    }
    return mIterator->second.get();
  }

  void World::move(PhysicsObject* object, unsigned int milliseconds) {

    // Get next collision based on current and proposed state
    PhysicalState mCurrentState  = PhysicalState(object->cLocation, object->cMomentum);
//     std::cout << "Calculating new state... ===========================================================================================================" << std::endl;
    PhysicalState mProposedState = calculateNewState(object, milliseconds);
//     std::cout << "Calculated new state!" << std::endl;
    std::unique_ptr<CollisionData> mNextEvent = getNextEvent(object, mCurrentState.cLocation, mProposedState.cLocation, 0.0, milliseconds);
//     std::cout << "Got event!" << std::endl;

    // Loop until all collision events have been processed
    double mMillisecondsProcessed = 0.0;
    unsigned int mRapidEventCount = 0;
    bool mEventProcessed = false;
//     std::cout << "Moving object..." << std::endl;
    while (mNextEvent != nullptr) {
//       std::cout << "Processing event..." << std::endl;
      processEvent(object, *mNextEvent, mMillisecondsProcessed, milliseconds);
//       std::cout << "Processed event!" << std::endl;
      mEventProcessed = true;

      // Consume time, and count events that occur in extremely rapid succession
      float mOldMilliseconds = mMillisecondsProcessed;
      mMillisecondsProcessed += (milliseconds - mMillisecondsProcessed) * mNextEvent->getGradient();
      mRapidEventCount = (mMillisecondsProcessed - mOldMilliseconds < 0.1f) ? mRapidEventCount + 1 : 0;
//       std::cout << "milliseconds processed: " << mMillisecondsProcessed << "   Rapid events: " << mRapidEventCount << "   Event gradient: " << mNextEvent->getGradient() << "   Event type: " << static_cast<int>(mNextEvent->getType()) << std::endl;

      // If time remains, get next collision event
      mNextEvent = nullptr;
      if (mMillisecondsProcessed < milliseconds) {
        mEventProcessed = false;
        mCurrentState  = PhysicalState(object->cLocation, object->cMomentum);
//         std::cout << "Calculating another new state..." << std::endl;
        mProposedState = calculateNewState(object, milliseconds - mMillisecondsProcessed);
//         std::cout << "Calculated another new state!" << std::endl;
        mNextEvent     = getNextEvent(object, mCurrentState.cLocation, mProposedState.cLocation, mMillisecondsProcessed, milliseconds);
//         std::cout << "Got another event!" << std::endl;

        // Lots of events in rapid succession usually means the object is "resting" between two slopes
        if (mRapidEventCount > 4 && mNextEvent != nullptr && mNextEvent->getType() == CollisionData::Type::SURFACE_LEAVE) {
          ISurface* mEventSurface = mNextEvent->getSurface();
          mEventSurface->getRestingLocation(mProposedState.cLocation);

          // Process last event in case object has moved off the resting point
          mNextEvent = getNextEvent(object, mCurrentState.cLocation, mProposedState.cLocation, mMillisecondsProcessed, milliseconds);
          if (mNextEvent != nullptr) {
//             std::cout << "Processing another event..." << std::endl;
            processEvent(object, *mNextEvent, mMillisecondsProcessed, milliseconds);
//             std::cout << "Processed another event!" << std::endl;
            mNextEvent = nullptr;
          }
        }
      } else {
        mEventProcessed = true;
      }
    }

    if (!mEventProcessed) {
      object->setPhysicalState(mProposedState);
    }
//     std::cout << "Moved object!" << std::endl;
  }

  ISurface* World::getSurfaceAt(LiteralVertex& location, float stepReach, bool nonSolid) {
    for (ISurface* mSurface : cRuntimeSurfaces.search(location.x, location.y)) {
      if (nonSolid || mSurface->isSolid()) {
        if (mSurface->contains(location, stepReach)) {
          return mSurface;
        }
      }
    }
    return nullptr;
  }
  
  void World::registerTerrain(Terrain* terrain, bool visual, bool physical) {
    if (physical) {
      cDefPhysicalSurfaceProcessor.registerTerrain(terrain);
    }
    if (visual) {
      cDefVisualSurfaceProcessor.registerTerrain(terrain);
    }
  }

  void World::updateTerrain(Terrain* terrain, bool visual, bool physical) {
    cDefPhysicalSurfaceProcessor.updateTerrain(terrain, physical);
    cDefVisualSurfaceProcessor.updateTerrain(terrain, visual);
  }

  void World::unregisterTerrain(Terrain* terrain) {
    cDefPhysicalSurfaceProcessor.unregisterTerrain(terrain);
    cDefVisualSurfaceProcessor.unregisterTerrain(terrain);
  }

  std::vector<std::unique_ptr<SurfaceTemplate>> World::createSurfaceTemplates(Terrain* terrain, ISurface::Direction facing, bool visual) {
    TerrainProcessor* mProcessor = visual ? &cDefVisualSurfaceProcessor : &cDefPhysicalSurfaceProcessor;
    return mProcessor->getSurfaces(terrain, facing);
  }

  std::vector<std::unique_ptr<WallTemplate>> World::createWallTemplates(Terrain* terrain, Wall::Direction facing, bool visual) {
    TerrainProcessor* mProcessor = visual ? &cDefVisualSurfaceProcessor : &cDefPhysicalSurfaceProcessor;
    return mProcessor->getWalls(terrain, facing);
  }

  void World::attachPhysicalSurface(ISurface* surface, bool priority) {
    (priority ? cRuntimePrioritySurfaces : cRuntimeSurfaces).add(surface->getXStart(), surface->getXEnd(), surface->getYStart(), surface->getYEnd(), surface);
  }

  void World::attachPhysicalWall(Wall* wall) {
    cRuntimeWalls.add(wall->getXStart(), wall->getXEnd(), wall->getYStart(), wall->getYEnd(), wall);
  }

  void World::detachPhysicalSurface(ISurface* surface) {
    cRuntimePrioritySurfaces.remove(surface);
    cRuntimeSurfaces.remove(surface);
  }
  
  void World::detachPhysicalWall(Wall* wall) {
    cRuntimeWalls.remove(wall);
  }

  void World::flagForInitialisation(Zone* zone) {
    cRuntimeZonesToInitialise.insert(zone);
  }
  
  void World::flagTerrainForInitialisation(int xStart, int xEnd, int yStart, int yEnd) {
    cDefPhysicalSurfaceProcessor.flagForInitialisation(xStart, xEnd, yStart, yEnd);
    cDefVisualSurfaceProcessor.flagForInitialisation(xStart, xEnd, yStart, yEnd);
  }
  
  void World::registerView(IScreen& screen) {
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->registerView(screen);
    }
  }

  int World::getMaxZoneHeight(int startX, int endX, int startY, int endY, int startZ, int endZ) const {
    int mLowest = std::numeric_limits<int>::max();
    for (const std::unique_ptr<Zone>& mZone : cDefZones) {
      if (mZone->intersects(startX, startY, startZ, endX, endY, endZ)) {
        mLowest = std::min(mZone->getStartZ(), mLowest);
      }
    }
    return mLowest;
  }  

  int World::getMinX() const {
    return cEditorMinX;
  }

  int World::getMaxX() const {
    return cEditorMaxX;
  }

  int World::getMinY() const {
    return cEditorMinY;
  }

  int World::getMaxY() const {
    return cEditorMaxY;
  }

  int World::getMinZ() const {
    return cEditorMinZ;
  }

  int World::getMaxZ() const {
    return cEditorMaxZ;
  }

  IWorldEditorTool* World::getDefaultWorldEditorTool() {
    return *cDefaultWorldEditorTool;
  }

  ThemeSet* World::getDefaultThemeSet() {
    return cDefaultThemeSet.get();
  }

  std::vector<IWorldEditorToolInstance*> World::createToolSet(WorldEditor& editor, IResourceData& owner) {
    std::vector<IWorldEditorToolInstance*> mTools;
    for (std::unique_ptr<WorldEditorTool>& mTool : cAvailableWorldEditorTools) {
      mTools.emplace_back((*mTool)->createToolInstance(editor, owner));
    }
    return mTools;
  }

  Alien* World::draw(AlienType& type, const WorldEditorCursorCell& cell, IScreen& screen) {
    Zone* mZone = getOrDrawZone(cell, screen, nullptr);
    if (mZone != nullptr) {
      Alien* mAlien = mZone->draw(type, cell);
      if (mZone->empty()) {
        remove(mZone);
      }
      return mAlien;
    }
    return nullptr;
  }
  
  Lift* World::draw(LiftType& type, const WorldEditorCursorCell& cell, int bottomRange, int topRange, IScreen& screen) {
    Zone* mZone = getOrDrawZone(cell, screen, nullptr);
    if (mZone != nullptr) {
      Lift* mLift = mZone->draw(type, cell, bottomRange, topRange);
      if (mZone->empty()) {
        remove(mZone);
      }
      return mLift;
    }
    return nullptr;
  }
  
  PickUp* World::draw(PickUpType& type, const WorldEditorCursorCell& cell, IScreen& screen) {
    Zone* mZone = getOrDrawZone(cell, screen, nullptr);
    if (mZone != nullptr) {
      PickUp* mPickUp = mZone->draw(type, cell);
      if (mZone->empty()) {
        remove(mZone);
      }

      if (mPickUp != nullptr) {
        mPickUp->initialise();
      }
      return mPickUp;
    }
    return nullptr;
  }
  
  Player* World::draw(PlayerType& type, const LiteralVertex& location) {
    if (cDefPlayers.empty()) {
      cDefPlayers.emplace_back(std::make_unique<Player>(*this, type, location.x, location.y, location.z));
    }
    cDefPlayers[0]->reposition(location.x, location.y, location.z);
    return cDefPlayers[0].get();
  }
  
  Terrain* World::draw(TerrainType& type, const WorldEditorCursorCell& start, const WorldEditorCursorCell& end, int southWestHeight, int southEastHeight, int northWestHeight, int northEastHeight, bool alternativeSplit, bool steppedBase, bool negation, IScreen& screen) {
    Zone* mZone = getOrDrawZone(start, screen, nullptr);
    if (mZone != nullptr) {
      Terrain* mTerrain = mZone->draw(type, start, end, southWestHeight, southEastHeight, northWestHeight, northEastHeight, alternativeSplit, steppedBase, negation);
      if (mZone->empty()) {
        remove(mZone);
      }
      return mTerrain;
    }
    return nullptr;
  }
  
  Zone* World::draw(ZoneType& type, const WorldEditorCursorCell& start, const WorldEditorCursorCell& end, IScreen& screen, Zone* clone) {
    if (!intersectsZone(start.cDefX, start.cDefY, start.cDefZ, end.cDefX, end.cDefY, end.cDefZ)) {
      Zone* mNewZone = cDefZones.emplace_back(std::make_unique<Zone>(*this, type, start.cDefX, start.cDefY, start.cDefZ, end.cDefX, end.cDefY, end.cDefZ, clone)).get();
      mNewZone->registerView(screen);
      mNewZone->initialiseObjects();
      mNewZone->initialiseTerrain();
      updateBounds();
      cSpindizzy.added(mNewZone);
      return mNewZone;
    }
    return nullptr;
  }

  ZoneObject* World::draw(ZoneObjectType& type) {
    return nullptr; // TODO: Implement this.
  }

  Zone* World::copy(Zone* zone, const WorldEditorCursorCell& cell, IScreen& screen) {
    return getOrDrawZone(cell, screen, zone);
  }

  void World::remove(Zone* zone) {
    cSpindizzy.removed(zone);
    Utils::removeElementUnique(cDefZones, zone);
    cRuntimeZonesToInitialise.erase(zone);
    updateBounds();
  }

  void World::removeTool(IWorldEditorTool* tool) {
    for (const std::pair<IEditableScreen* const, std::unique_ptr<WorldEditor>>& mEditor : cEditors) {
      mEditor.second->removeTool(tool);
    }
  }
  
  void World::removeAll(AlienType* type) {
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->removeAll(type);
    }
    removeTool(type);
  }

  void World::removeAll(LiftType* type) {
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->removeAll(type);
    }
    removeTool(type);
  }

  void World::removeAll(PickUpType* type) {
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->removeAll(type);
    }
    removeTool(type);
  }

  void World::removeAll(PlayerType* type) {
    for (int i = cDefPlayers.size() - 1; i >= 0; i--) {
      if (cDefPlayers[i]->isType(type)) {
        cDefPlayers.erase(cDefPlayers.begin() + i);
      }
    }
    removeTool(type);
  }

  void World::removeAll(TerrainType* type) {
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->removeAll(type);
    }
    removeTool(type);
  }

  void World::removeAll(ZoneType* type) {
    for (int i = cDefZones.size() - 1; i >= 0; i--) {
      if (cDefZones[i]->isType(type)) {
        cSpindizzy.removed(cDefZones[i].get());
        cDefZones.erase(cDefZones.begin() + i);
      }
    }
    if (type == cAutomaticZoneManagementType.get()) {
      cAutomaticZoneManagementType.setID("");
    }
    removeTool(type);
  }

  void World::removeAll(ZoneObjectType* type) {
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->removeAll(type);
    }
    removeTool(type);
  }

  Zone* World::getOrDrawZone(const WorldEditorCursorCell& cell, IScreen& screen, Zone* clone) {
    Zone* mZone = getZone(cell);
    if (mZone == nullptr) {
      if (cAutomaticZoneManagementType.get() != nullptr) {
        int mXStart = (std::floor(cell.cDefX / static_cast<float>(cAutomaticZoneXSize))) * cAutomaticZoneXSize;
        int mXEnd   = mXStart + (cAutomaticZoneXSize - 1);
        int mYStart = (std::floor(cell.cDefY / static_cast<float>(cAutomaticZoneYSize))) * cAutomaticZoneYSize;
        int mYEnd   = mYStart + (cAutomaticZoneYSize - 1);
        int mZStart = (std::floor(cell.cDefZ / static_cast<float>(cAutomaticZoneZSize))) * cAutomaticZoneZSize;
        int mZEnd   = mZStart + (cAutomaticZoneZSize - 1);
        mZone = draw(*cAutomaticZoneManagementType.get(), WorldEditorCursorCell(mXStart, mYStart, mZStart), WorldEditorCursorCell(mXEnd, mYEnd, mZEnd), screen, clone);
      }
    }
    return mZone;
  }

  bool World::isBasicProperties() const {
    return cEditorBasicProperties;
  }
  
  void World::renderEditing(const IScreen* screen) const {
    for (const std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->renderEditing(screen);
    }
    for (const std::unique_ptr<Player>& mPlayer : cDefPlayers) {
      mPlayer->renderEditing();
    }
  }

  Zone* World::getZone(const WorldEditorCursorCell& cell) {
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      if (mZone->contains(cell.cDefX, cell.cDefY, cell.cDefZ)) {
        return mZone.get();
      }
    }
    return nullptr;
  }

  bool World::intersectsZone(int xStart, int yStart, int zStart, int xEnd, int yEnd, int zEnd) const {
    for (const std::unique_ptr<Zone>& mZone : cDefZones) {
      if (mZone->intersects(xStart, yStart, zStart, xEnd, yEnd, zEnd)) {
        return true;
      }
    }
    return false;
  }
  
  void World::selectObjects(LiteralVertex* start, LiteralVertex& end, std::function<bool(IWorldObject*)> condition, std::function<void(IWorldObject*)> select) {
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->selectObjects(start, end, condition, select);
    }
  }

  unsigned int World::getZoneCount() {
    return cDefZones.size();
  }

  unsigned int World::getPickUpCount() {
    unsigned int mCount = 0;
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mCount += mZone->getPickUpCount();
    }
    return mCount;
  }

  IEditableScreen* World::createEditableScreen(Project* project, IDialogManager& dialogManager) {
    std::unique_ptr<WorldEditor> mScreen = std::make_unique<WorldEditor>(*project, *this, dialogManager);
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      mZone->registerView(*mScreen.get());
    }
    IEditableScreen* mReturnValue = mScreen.get();
    cEditors[mReturnValue] = std::move(mScreen);
    return mReturnValue;
  }

  bool World::renderAssetIcon() const {
    return false;
  }

  void World::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void World::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool World::isDefaultConfiguration() const {
    return true;
  }

  void World::DummyPhysicalObjectTypeUser::relinquish(IPhysicalObjectType* asset) {
    // Nothing to do.
  }

  bool World::DummyPhysicalObjectTypeUser::isReadOnly() const {
    return false;
  }

  void World::DummyPhysicalObjectTypeUser::setOwner(ProjectFile* owner) {
    // Nothing to do.
  }

  float World::getAbyssDepth() const {
    return -20.0f;
  }

  void World::updateBounds() {
    cRuntimeCacheStartX = std::numeric_limits<int>::max();
    cRuntimeCacheStartY = std::numeric_limits<int>::max();
    cRuntimeCacheStartZ = std::numeric_limits<int>::max();
    cRuntimeCacheEndX   = std::numeric_limits<int>::lowest();
    cRuntimeCacheEndY   = std::numeric_limits<int>::lowest();
    cRuntimeCacheEndZ   = std::numeric_limits<int>::lowest();
    for (std::unique_ptr<Zone>& mZone : cDefZones) {
      cRuntimeCacheStartX = std::min(cRuntimeCacheStartX, mZone->getStartX());
      cRuntimeCacheStartY = std::min(cRuntimeCacheStartY, mZone->getStartY());
      cRuntimeCacheStartZ = std::min(cRuntimeCacheStartZ, mZone->getStartZ());
      cRuntimeCacheEndX   = std::max(cRuntimeCacheEndX,   mZone->getEndX());
      cRuntimeCacheEndY   = std::max(cRuntimeCacheEndY,   mZone->getEndY());
      cRuntimeCacheEndZ   = std::max(cRuntimeCacheEndZ,   mZone->getEndZ());
      mZone->updateBounds();
    }
  }

  void World::updateCache() const {
    if (!cResourceData.isIncluded() && cSpindizzy.getProject().isUser()) {
      cResourceData.makeUserDataDirectory();
      std::string mCachePath = cResourceData.getPath("Terrain.cache", true);
//      std::cout << "CACHE PATH: " << mCachePath << std::endl;
      std::ofstream mCacheOutput(mCachePath, std::ios::out | std::ios::binary);
      if (!mCacheOutput) {
        std::cout << "WARNING: World::updateCache: Couldn't open \"" << mCachePath << "\" to cache terrain." << std::endl;
      } else {
        for (const std::unique_ptr<Zone>& mZone : cDefZones) {
          mZone->saveCache(mCacheOutput);
        }
        mCacheOutput.close();
      }
    }
  }

  PhysicalState World::calculateNewState(PhysicsObject* object, double milliseconds) {
    PhysicalState mObjectState(object->cLocation, object->cMomentum);

    // Manual thrust (e.g. player input, alien direction, etc.)
    float mXThrust = object->cObject->getXThrust();
    float mYThrust = object->cObject->getYThrust();

    // Contact surfaces
    Wall* mWestWall  = object->cWestWall;
    Wall* mEastWall  = object->cEastWall;
    Wall* mSouthWall = object->cSouthWall;
    Wall* mNorthWall = object->cNorthWall;

    if (object->cSurface != nullptr) {

      // Rolling; get surface properties
      float mSurfaceGrip     = object->cSurface->getSurfaceGrip();
      float mSurfaceFriction = 1.0f - object->cSurface->getSurfaceFriction();

      // Get surface acceleration (e.g. slope, force arrow, etc.)
      float mSurfaceAccelerationX = object->cSurface->getXAcceleration(mObjectState.cLocation.x, mObjectState.cLocation.y) * cDefSurfaceAccelerationFactor;
      float mSurfaceAccelerationY = object->cSurface->getYAcceleration(mObjectState.cLocation.x, mObjectState.cLocation.y) * cDefSurfaceAccelerationFactor;

      // Adjust manual thrust for stability against surface acceleration
      if      (mXThrust > 0.0f && mSurfaceAccelerationX < 0.0f) {mXThrust = std::max(mXThrust, -mSurfaceAccelerationX);}
      else if (mXThrust < 0.0f && mSurfaceAccelerationX > 0.0f) {mXThrust = std::min(mXThrust, -mSurfaceAccelerationX);}

      if      (mYThrust > 0.0f && mSurfaceAccelerationY < 0.0f) {mYThrust = std::max(mYThrust, -mSurfaceAccelerationY);}
      else if (mYThrust < 0.0f && mSurfaceAccelerationY > 0.0f) {mYThrust = std::min(mYThrust, -mSurfaceAccelerationY);}

      // Apply update consistently regardless of tick interval
      for (unsigned int i = 0; i < milliseconds; i++) {

        // Update momentum Disallow momentum above/below zero when craft is forced against wall by a slope.
        mObjectState.cMomentum.x = mEastWall  != nullptr ? std::min(mObjectState.cMomentum.x + (mXThrust * mSurfaceGrip) + mSurfaceAccelerationX, 0.0)
                                 : mWestWall  != nullptr ? std::max(mObjectState.cMomentum.x + (mXThrust * mSurfaceGrip) + mSurfaceAccelerationX, 0.0)
                                 :                                  mObjectState.cMomentum.x + (mXThrust * mSurfaceGrip) + mSurfaceAccelerationX;
        mObjectState.cMomentum.y = mNorthWall != nullptr ? std::min(mObjectState.cMomentum.y + (mYThrust * mSurfaceGrip) + mSurfaceAccelerationY, 0.0)
                                 : mSouthWall != nullptr ? std::max(mObjectState.cMomentum.y + (mYThrust * mSurfaceGrip) + mSurfaceAccelerationY, 0.0)
                                 :                                  mObjectState.cMomentum.y + (mYThrust * mSurfaceGrip) + mSurfaceAccelerationY;

        // Friction slows momentum
        mObjectState.cMomentum.x *= mSurfaceFriction;
        mObjectState.cMomentum.y *= mSurfaceFriction;

        // Momentum makes object move
        mObjectState.cLocation.x += mObjectState.cMomentum.x;
        mObjectState.cLocation.y += mObjectState.cMomentum.y;

        // Moving away from touching a wall
        if      (mObjectState.cMomentum.x > 0.0f) {mWestWall = nullptr;}
        else if (mObjectState.cMomentum.x < 0.0f) {mEastWall = nullptr;}

        if      (mObjectState.cMomentum.y > 0.0f) {mSouthWall = nullptr;}
        else if (mObjectState.cMomentum.y < 0.0f) {mNorthWall = nullptr;}
      }

      // Height is determined by the surface on which the object currently lies
      mObjectState.cLocation.z = object->cSurface->getHeightAt(mObjectState.cLocation.x, mObjectState.cLocation.y);

      // Adjust position for upcoming surface movement
      object->cSurface->adjustPosition(mObjectState.cLocation, milliseconds);
    } else {

      // Airborne: Momentum is not affected by thrust
      for (unsigned int i = 0; i < milliseconds; i++) {
        mObjectState.cMomentum.z += cDefGravity;
        mObjectState.cLocation.x += mObjectState.cMomentum.x;
        mObjectState.cLocation.y += mObjectState.cMomentum.y;
        mObjectState.cLocation.z += mObjectState.cMomentum.z;
      }
    }
    return mObjectState;
  }

  std::unique_ptr<CollisionData> World::getNextEvent(PhysicsObject* object, LiteralVertex& startLocation, LiteralVertex& endLocation, double startTime, double endTime) {
    std::unique_ptr<CollisionData> mEvent = nullptr;

    // If object is rolling on a surface, check if it leaves the surface
    if (object->cSurface != nullptr) {
      std::unique_ptr<CollisionData> mSurfaceLeftEvent = object->cSurface->getRollingEvent(startLocation, endLocation, startTime, endTime);
      if (mSurfaceLeftEvent != nullptr) {
        mEvent = std::move(mSurfaceLeftEvent);
      }
    }

    // Check if the object moves away from any touching walls
    Wall* mWallContacts[4] = {object->cNorthWall,
                              object->cSouthWall,
                              object->cEastWall,
                              object->cWestWall};
    for (unsigned int i = 0; i < 4; i++) {
      if (mWallContacts[i] != nullptr) {
        std::unique_ptr<CollisionData> mSurfaceLeftEvent = mWallContacts[i]->getSlidingEvent(startLocation, endLocation, object->cObject);
        if (mSurfaceLeftEvent != nullptr) {
          if (mEvent == nullptr || mSurfaceLeftEvent->getGradient() < mEvent->getGradient()) {
            mEvent = std::move(mSurfaceLeftEvent);
          }
        }
      }
    }

    int mSouth = std::floor(std::min(startLocation.getY(), endLocation.getY())) - 1;
    int mNorth = std::ceil(std::max(startLocation.getY(), endLocation.getY())) + 1;
    int mWest  = std::floor(std::min(startLocation.getX(), endLocation.getX())) - 1;
    int mEast  = std::ceil(std::max(startLocation.getX(), endLocation.getX())) + 1;
    Zone* mHomeZone = object->cObject->getHome();
    float mStepReach = object->cObject->getStepReach();

    float mEventGradient = -1.0f;
    if (object->cSurface == nullptr) {
      for (ISurface* mSurface : cRuntimeSurfaces.search(mWest, mEast, mSouth, mNorth)) {
        if (object->cObject->allowTraversal(mSurface)) {
          std::unique_ptr<CollisionData> mSurfaceEvent = mSurface->getCollision(startLocation, endLocation, mStepReach, startTime, endTime);
          if (mSurfaceEvent != nullptr) {
            float mGradient = mSurfaceEvent->getGradient();
            if (mEventGradient == -1.0f || mGradient < mEventGradient) {
              mEventGradient = mGradient;
              mEvent = std::move(mSurfaceEvent);
            }
          }
        }
      }

      // Aliens simulate an infinite surface outside of their zone... check if this surface has been mounted.
      ISurface* mObjectSurface = object->cObject->getObjectSurface();
      if (mObjectSurface != nullptr) {
        std::unique_ptr<CollisionData> mSurfaceEvent = mObjectSurface->getCollision(startLocation, endLocation, mStepReach, startTime, endTime);
        if (mSurfaceEvent != nullptr) {
          float mGradient = mSurfaceEvent->getGradient();
          if (mEventGradient == -1.0f || mGradient < mEventGradient) {
            mEventGradient = mGradient;
            mEvent = std::move(mSurfaceEvent);
          }
        }
      }
    }

    for (Wall* mWall : cRuntimeWalls.search(mWest, mEast, mSouth, mNorth)) {
      if (mHomeZone == nullptr || &mWall->getZone() == mHomeZone) {
        std::unique_ptr<CollisionData> mWallEvent = mWall->getCollision(startLocation, endLocation, object->cObject);
        if (mWallEvent != nullptr) {
          float mGradient = mWallEvent->getGradient();
          if (mEventGradient == -1.0f || mGradient < mEventGradient) {
            mEventGradient = mGradient;
            mEvent = std::move(mWallEvent);
          }
        }
      }
    }

    for (ISurface* mSurface : cRuntimePrioritySurfaces.search(mWest, mEast, mSouth, mNorth)) {
      if (mSurface != object->cSurface) {
        std::unique_ptr<CollisionData> mPrioritySurfaceEvent = mSurface->getCollision(startLocation, endLocation, mStepReach, startTime, endTime);
        if (mPrioritySurfaceEvent != nullptr) {

          // Check if there's a regular surface to mount first (for respawning and switch activation).
          bool mOtherSurfaceEvent = false;
          for (ISurface* mSurface : cRuntimeSurfaces.search(mWest, mEast, mSouth, mNorth)) {
            if (object->cSurface != mSurface && object->cObject->allowTraversal(mSurface)) {
              std::unique_ptr<CollisionData> mSurfaceEvent = mSurface->getCollision(startLocation, endLocation, mStepReach, startTime, endTime);
              if (mSurfaceEvent != nullptr) {
                float mGradient = mSurfaceEvent->getGradient();
                if (mEventGradient == -1.0f || mGradient < mEventGradient) {
                  mEventGradient = mGradient;
                  mEvent = std::move(mSurfaceEvent);
                  mOtherSurfaceEvent = true;
                }
              }
            }
          }

          float mGradient = mPrioritySurfaceEvent->getGradient();
          LiteralVertex mLocation = mPrioritySurfaceEvent->getEventLocation();
          if (!mOtherSurfaceEvent && ((mEventGradient == -1.0f || mGradient < mEventGradient) || /*mEvent->getEventLocation()->getZ() > mLocation->getZ() ||*/ (mEvent->getEventLocation().getZ() == mLocation.getZ()))) {
            mEvent = std::move(mPrioritySurfaceEvent);
          }
          mEventGradient = mGradient;
        }
      }
    }
    return mEvent;
  }

  void World::processEvent(PhysicsObject* object, CollisionData& event, double& startTime, double endTime) {
    PhysicalState mState = PhysicalState(object->cLocation, object->cMomentum);
    mState.cLocation     = event.getEventLocation();

    // Add time passed to reach this event
    startTime += (endTime - startTime) * event.getGradient();

//     std::cout << "Event is type: " << static_cast<int>(event.getType()) << std::endl;
    switch (event.getType()) {
      case CollisionData::Type::SURFACE_LEAVE: {
        mState.cMomentum.z = mState.cMomentum.getX() * -event.getXSlope() + mState.cMomentum.getY() * -event.getYSlope();
        object->leaveSurface(event.getSurface(), mState);
        break;
      }

      case CollisionData::Type::SURFACE_MOUNT: {
        ISurface* mMountedSurface = event.getSurface();
        double mRemainingHeight = object->cLocation.z - mState.cLocation.z;
        float mFallHeight = ((mState.cMomentum.z * mState.cMomentum.z) / -cDefGravity) / 2.0 + mRemainingHeight + -(mState.cMomentum.z) / 2.0;
        if (mMountedSurface->getSurfaceBounce() == 0.0f || mFallHeight == 0.0f) {
          object->mountSurface(mMountedSurface, mState, mFallHeight);
        } else {
          // NO MOMENTUM CONTROL WHILE AIRBORNE!
          object->bounceSurface(mMountedSurface, mState, mFallHeight);
          mState.cMomentum.z      = std::sqrt(std::max(0.0f, (mFallHeight + object->cObject->getBounceFactor()) * -cDefGravity * 2.0f));
          PhysicalState mNewState = calculateNewState(object, cDefBounceTime);
          mState.cMomentum.x      = mNewState.cMomentum.x;
          mState.cMomentum.y      = mNewState.cMomentum.y;
          object->leaveSurface(event.getSurface(), mState);
        }
        break;
      }

      case CollisionData::Type::SURFACE_MOVEMENT: {
        LiteralVertex mEventLocation = event.getEventLocation();
        ISurface* mContactSurface = getSurfaceAt(mEventLocation, object->cObject->getStepReach(), false);
        if (mContactSurface != nullptr) {
          mContactSurface->notifyContact();
        }
        break;
      }

      case CollisionData::Type::WALL_IMPACT: {

        // Rolling upwards when hitting a wall may cause object to become airborne
        if (object->cSurface != nullptr) {
          mState.cMomentum.z = mState.cMomentum.x * -object->cSurface->getXAcceleration(mState.cLocation.x, mState.cLocation.y)
                             + mState.cMomentum.y * -object->cSurface->getYAcceleration(mState.cLocation.x, mState.cLocation.y);
          if (mState.cMomentum.z > 0.0f) {
            object->leaveSurface(object->cSurface, mState);
          }
        }

        // Wall bounce
        Wall* mWall = event.getWall();
        Wall::Direction mFaceDirection = mWall->getFaceDirection();
        float mSurfaceBounce = mWall->getBounce();
        double* mAffectedMomentum = (mFaceDirection == Wall::Direction::NORTH || mFaceDirection == Wall::Direction::SOUTH) ? &(mState.cMomentum.y) : &(mState.cMomentum.x);
        *mAffectedMomentum = -(*mAffectedMomentum) * mSurfaceBounce;
        if (fabs(*mAffectedMomentum) < object->cObject->getHugMomentum()) {

          // Wall hugging
          (*mAffectedMomentum) = 0.0f;
          if (object->isHuggable(mWall)) {
            object->hugWall(mWall, mState);
          } else {
            mWall->updateState(mState);
            object->setPhysicalState(mState);
          }
        } else {

          // Wall bounce
          object->cObject->bounceWall(mWall);
          object->setPhysicalState(mState);
        }
        break;
      }

      case CollisionData::Type::WALL_CLIP: {
        object->setPhysicalState(mState);
        break;
      }

      case CollisionData::Type::WALL_LEAVE: {
        object->unhugWall(event.getWall(), mState);
        break;
      }
    }
  }
}

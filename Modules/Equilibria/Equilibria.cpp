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
#include "Equilibria.h"

namespace IsoRealms::Equilibria {
  const double Equilibria::DEFAULT_VIEW_ANGLE_PITCH = -90.0 + std::atan(1.0 / std::sqrt(2.0)) * (180.0 / M_PI);
  const double Equilibria::DEFAULT_VIEW_ANGLE_YAW = -45.0;

  Equilibria::Equilibria(Project& project, IResourceTypeRegistry& registry) :
                    cProject(project),
                    cModule(registry),
                    cToolDelete(),
                    cToolProperties(),
                    cToolCopyZone(ZoneTool::Type::COPY),
                    cToolMoveZone(ZoneTool::Type::MOVE),
                    cToolDeleteZone(ZoneTool::Type::DELETE),
                    cCameras(registry),
                    cSurfacePatterns(registry),
                    cWallPatterns(registry),
                    cZoneObjectTypeTraits(registry),
                    cZoneViewTypes(registry),
                    cResourceAlien(*this),
                    cResourceBoundaryHandler(*this),
                    cResourceCollisionHandler(*this),
                    cResourceLift(*this),
                    cResourcePickUp(*this),
                    cResourcePlayer(*this),
                    cResourceTerrain(*this),
                    cResourceTerrainState(*this),
                    cResourceThemeSet(*this),
                    cResourceWorld(*this),
                    cResourceWorldView(*this),
                    cResourceZone(*this),
                    cResourceZoneObject(*this),
                    cRuntimePaused(false),
                    cBindingTypeTerrainState("TerrainState", "Equilibria/Terrain States"),
                    cRuntimeParameterPlayer(project.getLuaState(), nullptr, nullptr, true),
                    cRuntimeParameterWall(  project.getLuaState(), nullptr, nullptr, true),
                    cRuntimeParameterZone1( project.getLuaState(), nullptr, nullptr, true),
                    cRuntimeParameterZone2( project.getLuaState(), nullptr, nullptr, true),
                    cLuaBinding(project.getLuaState(), this) {
    registry.add(&cResourceAlien,            "Alien");
    registry.add(&cResourceBoundaryHandler,  "BoundaryHandler");
    registry.add(&cResourceCollisionHandler, "CollisionHandler");
    registry.add(&cResourceLift,             "Lift");
    registry.add(&cResourcePickUp,           "PickUp");
    registry.add(&cResourcePlayer,           "Player");
    registry.add(&cResourceTerrain,          "Terrain");
    registry.add(&cResourceTerrainState,     "TerrainState");
    registry.add(&cResourceThemeSet,         "ThemeSet");
    registry.add(&cResourceWorld,            "World");
    registry.add(&cResourceWorldView,        "WorldView");
    registry.add(&cResourceZone,             "Zone");
    registry.add(&cResourceZoneObject,       "ZoneObject");
  }
  
  const Metadata& Equilibria::getMetadata(const std::string& key) const {
    return cModule.getAssetMetadata(key);
  }

  void Equilibria::init(std::function<void()> initialiser) {
    cProject.init(initialiser);
  }
  
  Project& Equilibria::getProject() const {
    return cProject;
  }

  Equilibria& Equilibria::getAssetManager() {
    return *this;
  }

  bool Equilibria::isReadOnly() const {
    return false;
  } // TODO: Probably shouldn't be here.

  void Equilibria::setOwner(ProjectFile* owner) {
  } // TODO: Probably shouldn't be here.

  std::string Equilibria::getResourceID() const {
    return ""; // TODO: Implement this.
  }

  std::string Equilibria::getResourceName() const {
    return ""; // TODO: Implement this.
  }

  void Equilibria::removeAll(AlienType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }
  
  void Equilibria::removeAll(LiftType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }
  
  void Equilibria::removeAll(PickUpType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }
  
  void Equilibria::removeAll(PlayerType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }
  
  void Equilibria::removeAll(TerrainType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }
  
  void Equilibria::removeAll(ZoneType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }
  
  void Equilibria::removeAll(ZoneObjectType* type) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeAll(type);
    }
  }

//   void Equilibria::remove(IAssetProvider<IBoundaryType>*        provider) {cBoundaryTypes.remove(       provider);}
//   void Equilibria::remove(IAssetProvider<ICamera>*              provider) {cCameras.remove(             provider);}
//   void Equilibria::remove(IAssetProvider<IPhysicalObjectType>*  provider) {cPhysicalObjectTypes.remove( provider);}
//   void Equilibria::remove(IAssetProvider<ISurfacePattern>*      provider) {cSurfacePatterns.remove(     provider);}
//   void Equilibria::remove(IAssetProvider<IWallPattern>*         provider) {cWallPatterns.remove(        provider);}
//   void Equilibria::remove(IAssetProvider<IWorldEditorTool>*     provider) {cWorldEditorTools.remove(    provider);}
//   void Equilibria::remove(IAssetProvider<IZoneObjectTypeTrait>* provider) {cZoneObjectTypeTraits.remove(provider);}
//   void Equilibria::remove(IAssetProvider<IZoneViewType>*        provider) {cZoneViewTypes.remove(       provider);}

  std::vector<IBoundaryType*> Equilibria::getAllBoundaryTypeObjects() {
    std::vector<IBoundaryType*> mTypes;
    for (PickUpType* mType : cResourcePickUp) {mTypes.push_back(mType);}
    for (ZoneType*   mType : cResourceZone)   {mTypes.push_back(mType);}
    return mTypes;
  }

  std::vector<IPhysicalObjectType*> Equilibria::getAllPhysicalObjectTypeObjects() {
    std::vector<IPhysicalObjectType*> mTypes;
    for (AlienType*  mType : cResourceAlien)  {mTypes.push_back(mType);}
    for (PlayerType* mType : cResourcePlayer) {mTypes.push_back(mType);}
    return mTypes;
  }  

  void Equilibria::physicalObjectTypeChanged(CollisionHandler* handler, const IPhysicalObjectType* oldPhysicalObjectType, const IPhysicalObjectType* newPhysicalObjectType) {
    for (World* mWorld : cResourceWorld) {
      mWorld->physicalObjectTypeChanged(handler, oldPhysicalObjectType, newPhysicalObjectType);
    }
  }

  void Equilibria::added(BoundaryHandler* handler) {
    for (World* mWorld : cResourceWorld) {
      mWorld->addBoundaryHandler(handler->createInstance(mWorld));
    }
  }

  void Equilibria::added(CollisionHandler* handler) {
    for (World* mWorld : cResourceWorld) {
      mWorld->addCollisionHandler(handler->createInstance(mWorld));
    }
  }

  void Equilibria::added(IBoundaryType* boundaryType) {
    for (World* mWorld : cResourceWorld) {
      mWorld->added(boundaryType);
    }
  }

  void Equilibria::added(IPhysicalObjectType* physicalObjectType) {
    for (World* mWorld : cResourceWorld) {
      mWorld->added(physicalObjectType);
    }
  }

  void Equilibria::added(Zone* zone) {
    for (WorldView* mWorldView : cResourceWorldView) {
      if (mWorldView->getWorld() == &zone->getWorld()) {
        zone->registerView(*mWorldView);
        mWorldView->addZoneView(zone);
      }
    }
  }

  void Equilibria::removed(BoundaryHandler* handler) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeBoundaryHandler(handler);
    }
  }

  void Equilibria::removed(CollisionHandler* handler) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removeCollisionHandler(handler);
    }
  }

  void Equilibria::removed(IBoundaryType* boundaryType) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removed(boundaryType);
    }
  }
  
  void Equilibria::removed(IPhysicalObjectType* physicalObjectType) {
    for (World* mWorld : cResourceWorld) {
      mWorld->removed(physicalObjectType);
    }
  }
  
  void Equilibria::removed(Zone* zone) {
    zone->removed();
    for (WorldView* mWorldView : cResourceWorldView) {
      if (mWorldView->getWorld() == &zone->getWorld()) {
        mWorldView->removeZoneView(zone);
      }
    }
  }

  std::vector<ConditionElement*> Equilibria::getTerrainStateConditionElements() {
    std::vector<ConditionElement*> mConditionElements;
    for (TerrainState* mTerrainState : cResourceTerrainState) {
      mConditionElements.push_back(&mTerrainState->getConditionElement());
    }
    return mConditionElements;
  }

  TerrainState* Equilibria::getTerrainState(IBoolean* input) {
    for (TerrainState* mTerrainState : cResourceTerrainState) {
      if (mTerrainState == input) {
        return mTerrainState;
      }
    }
    return nullptr;
  }

  void Equilibria::setAllThemesInUse(bool inUse) {
    for (ThemeSet* mThemeSet : cResourceThemeSet) {
      mThemeSet->hintInUse(inUse);
    }
  }

  void Equilibria::setNextTheme() {
    for (ThemeSet* mThemeSet : cResourceThemeSet) {
      mThemeSet->setNextTheme();
    }
  }

  void Equilibria::setPreviousTheme() {
    for (ThemeSet* mThemeSet : cResourceThemeSet) {
      mThemeSet->setPreviousTheme();
    }
  }

  void Equilibria::applyDefaultThemes() {
    for (ThemeSet* mThemeSet : cResourceThemeSet) {
      mThemeSet->applyDefaultTheme();
    }
  }

  void Equilibria::bind( Player* value) {cRuntimeParameterPlayer.setValue(value);}
  void Equilibria::bind1(Zone*   value) {cRuntimeParameterZone1.setValue( value);}
  void Equilibria::bind2(Zone*   value) {cRuntimeParameterZone2.setValue( value);}
  void Equilibria::bind( Wall*   value) {cRuntimeParameterWall.setValue(  value);}

  void Equilibria::setPause(bool pause) {
    cRuntimePaused = pause;
  }

  bool Equilibria::isPaused() {
    return cRuntimePaused;
  }

  void Equilibria::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IBindingType>(&cBindingTypeTerrainState, "Terrain State", "Terrain States");

    // Bind the module.
    assets.add<IBinding>(&cLuaBinding, "", "Modules/Equilibria");
    
    for (AlienType* mResource : cResourceAlien) {
      mResource->registerAssets("Alien/" + getResourceID(mResource));
    }
    for (LiftType* mResource : cResourceLift) {
      mResource->registerAssets("Lift/" + getResourceID(mResource));
    }
    for (PickUpType* mResource : cResourcePickUp) {
      mResource->registerAssets("PickUp/" + getResourceID(mResource));
    }
    for (PlayerType* mResource : cResourcePlayer) {
      mResource->registerAssets("Player/" + getResourceID(mResource));
    }
    for (TerrainType* mResource : cResourceTerrain) {
      mResource->registerAssets("Terrain/" + getResourceID(mResource));
    }
    for (WorldView* mResource : cResourceWorldView) {
      mResource->registerAssets("WorldView/" + getResourceID(mResource));
    }
    for (ZoneType* mResource : cResourceZone) {
      mResource->registerAssets("Zone/" + getResourceID(mResource));
    }
    for (ZoneObjectType* mResource : cResourceZoneObject) {
      mResource->registerAssets("ZoneObject/" + getResourceID(mResource));
    }
    add<IWorldEditorTool>(&cToolDelete,     TOOL_DELETE,      TOOL_DELETE);
    add<IWorldEditorTool>(&cToolProperties, TOOL_PROPERTIES,  TOOL_PROPERTIES);
    add<IWorldEditorTool>(&cToolCopyZone,   TOOL_COPY_ZONE,   TOOL_COPY_ZONE);
    add<IWorldEditorTool>(&cToolMoveZone,   TOOL_MOVE_ZONE,   TOOL_MOVE_ZONE);
    add<IWorldEditorTool>(&cToolDeleteZone, TOOL_DELETE_ZONE, TOOL_DELETE_ZONE);
  }

  void Equilibria::updateInputs(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void Equilibria::updateRuntime(unsigned int milliseconds) {
    updateRuntime2(cResourceWorld,     milliseconds);
    updateRuntime2(cResourceWorldView, milliseconds);
  }
  
  void Equilibria::updateEditing(unsigned int milliseconds) {
    updateEditing2(cResourceThemeSet, milliseconds);
    updateEditing2(cResourceWorld,    milliseconds);
  }
  
  void Equilibria::reset() {
    reset2(cResourceAlien);
    reset2(cResourcePlayer);
    reset2(cResourceTerrainState);
    reset2(cResourceWorld);
    reset2(cResourceWorldView);
  }  
  
  void Equilibria::remove(IWorldEditorTool* asset) {
    cWorldEditorTools.remove(asset);
  }

  void Equilibria::remove(WorldEditorTool& tool) {
    // TODO: Remove from our selection.
  }

  void Equilibria::addZoneBinding(IBinding* binding1, IBinding* binding2, const std::string& id) {
    cRuntimeZoneBindings1[id] = binding1;
    cRuntimeZoneBindings2[id] = binding2;
  }

  IBinding* Equilibria::getZonePropertyBinding(std::map<std::string, IBinding*>& bindings, const std::string& id) {
    std::size_t mSplit = id.find('/');
    std::string mBindTo = id.substr(0, mSplit);
    std::string mBindPath = id.substr(mSplit + 1);
    if (mBindTo == BIND_TO_ZONE) {
      std::map<std::string, IBinding*>::iterator mZoneProperty = bindings.find(mBindPath);
      if (mZoneProperty != bindings.end()) {
        return mZoneProperty->second;
      }
    }
    throw ArgumentException("ERROR: Equilibria::getZonePropertyBinding: No zone property binding found for: \"" + id + "\"");
  }

  IBinding* Equilibria::getBindingPlayer(const std::string& id) {
    if (id == BIND_TO_PLAYER) {
      return &cRuntimeParameterPlayer;
    }
    return getZonePropertyBinding(cRuntimeZoneBindings1, id);
  }

  std::string Equilibria::getBindingIDPlayer(const IBinding* binding) const {
    if (&cRuntimeParameterPlayer == binding) {return BIND_TO_PLAYER;}
    if (&cRuntimeParameterZone1  == binding) {return BIND_TO_ZONE;}
    for (const std::pair<const std::string, IBinding*>& mZoneBinding : cRuntimeZoneBindings1) {
      if (mZoneBinding.second == binding) {
        return BIND_TO_ZONE + "/" + mZoneBinding.first;
      }
    }
    return "";
  }

  void Equilibria::getTreeItemsPlayer(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    getTreeItemInfoFunction(TreeItemInfo{BIND_TO_PLAYER, "Player"});
    getTreeItemInfoFunction(TreeItemInfo{BIND_TO_ZONE,   "Player's Zone"});
    for (const std::pair<const std::string, IBinding*>& mZoneBinding : cRuntimeZoneBindings1) {
      getTreeItemInfoFunction(TreeItemInfo{BIND_TO_ZONE + "/" + mZoneBinding.first, "Player's Zone Properties/" + mZoneBinding.first});
    }
  }

  IBinding* Equilibria::getBindingZone(const std::string& id) {
    if (id == BIND_TO_ZONE) {
      return &cRuntimeParameterZone1;
    }
    return getZonePropertyBinding(cRuntimeZoneBindings1, id);
  }

  std::string Equilibria::getBindingIDZone(const IBinding* binding) const {
    if (&cRuntimeParameterZone1 == binding) {return BIND_TO_ZONE;}
    for (const std::pair<const std::string, IBinding*>& mZoneBinding : cRuntimeZoneBindings1) {
      if (mZoneBinding.second == binding) {
        return BIND_TO_ZONE + "/" + mZoneBinding.first;
      }
    }
    return "";
  }

  void Equilibria::getTreeItemsZone(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    getTreeItemInfoFunction(TreeItemInfo{BIND_TO_ZONE, BIND_TO_ZONE});
    for (const std::pair<const std::string, IBinding*>& mZoneBinding : cRuntimeZoneBindings1) {
      getTreeItemInfoFunction(TreeItemInfo{BIND_TO_ZONE + "/" + mZoneBinding.first, "Zone Properties/" + mZoneBinding.first});
    }
  }

  IBinding* Equilibria::getBindingPickUp(const std::string& id) {
    if (id == BIND_TO_ZONE) {
      return &cRuntimeParameterZone1;
    }
    std::size_t mSplit = id.find('/');
    std::string mBindTo = id.substr(0, mSplit);
    std::string mBindPath = id.substr(mSplit + 1);
    if (mBindTo == BIND_TO_ZONE) {
      return getZonePropertyBinding(cRuntimeZoneBindings1, mBindPath);
    }
    return nullptr;
  }

  std::string Equilibria::getBindingIDPickUp(const IBinding* binding) const {
    if (&cRuntimeParameterZone1 == binding) {return BIND_TO_ZONE;}
    for (const std::pair<const std::string, IBinding*>& mZoneBinding : cRuntimeZoneBindings1) {
      if (mZoneBinding.second == binding) {
        return BIND_TO_ZONE + "/" + mZoneBinding.first;
      }
    }
    return "";
  }

  void Equilibria::getTreeItemsPickUp(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    getTreeItemInfoFunction(TreeItemInfo{BIND_TO_ZONE, "Pick Up's Zone"});
    for (const std::pair<const std::string, IBinding*>& mZoneBinding : cRuntimeZoneBindings1) {
      getTreeItemInfoFunction(TreeItemInfo{BIND_TO_ZONE + "/" + mZoneBinding.first, "Pick Up's Zone Properties/" + mZoneBinding.first});
    }
  }

  IBinding* Equilibria::getBindingWallBounce(const std::string& id) {
    std::size_t mSplit = id.find('/');
    std::string mBindTo = id.substr(0, mSplit);
    std::string mBindPath = id.substr(mSplit + 1);
    if (mBindTo == BIND_TO_PLAYER) {
      mSplit = mBindPath.find('/');
      if (mSplit == std::string::npos) {
        if (mBindPath == BIND_TO_PLAYER) {return &cRuntimeParameterPlayer;}
        if (mBindPath == BIND_TO_ZONE)   {return &cRuntimeParameterZone1;}
      } else {
        return getZonePropertyBinding(cRuntimeZoneBindings1, mBindPath);
      }
    } else if (mBindTo == BIND_TO_TERRAIN) {
      mSplit = mBindPath.find('/');
      if (mSplit == std::string::npos) {
        if (mBindPath == BIND_TO_WALL) {return &cRuntimeParameterWall;}
        if (mBindPath == BIND_TO_ZONE) {return &cRuntimeParameterZone2;}
      } else {
        return getZonePropertyBinding(cRuntimeZoneBindings2, mBindPath);
      }
    }
    throw ArgumentException("ERROR: Equilibria::getBindingWallBounce: No wall bounce binding found for: \"" + id + "\"");
  }

  std::string Equilibria::getBindingIDWallBounce(const IBinding* binding) const {
    if (&cRuntimeParameterPlayer == binding) {return "Player/Player";}
    if (&cRuntimeParameterZone2  == binding) {return "Terrain/Zone";}
    if (&cRuntimeParameterWall   == binding) {return "Terrain/Wall";}
    for (const std::pair<const std::string, IBinding*>& mZoneBinding : cRuntimeZoneBindings1) {
      if (mZoneBinding.second == binding) {
        return "Player/" + BIND_TO_ZONE + "/" + mZoneBinding.first;
      }
    }
    for (const std::pair<const std::string, IBinding*>& mZoneBinding : cRuntimeZoneBindings2) {
      if (mZoneBinding.second == binding) {
        return "Terrain/" + BIND_TO_ZONE + "/" + mZoneBinding.first;
      }
    }
    return "";
  }

  void Equilibria::getTreeItemsWallBounce(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    getTreeItemInfoFunction(TreeItemInfo{"Player/Player", "Player/Player"});
    getTreeItemInfoFunction(TreeItemInfo{"Terrain/Wall",  "Terrain/Wall"});
    getTreeItemInfoFunction(TreeItemInfo{"Terrain/Zone",  "Terrain/Terrain's Zone"});
    for (const std::pair<const std::string, IBinding*>& mZoneBinding : cRuntimeZoneBindings1) {
      getTreeItemInfoFunction(TreeItemInfo{"Player/" + BIND_TO_ZONE + "/" + mZoneBinding.first, "Player/Player's Zone Properties/" + mZoneBinding.first});
    }
    for (const std::pair<const std::string, IBinding*>& mZoneBinding : cRuntimeZoneBindings2) {
      getTreeItemInfoFunction(TreeItemInfo{"Terrain/" + BIND_TO_ZONE + "/" + mZoneBinding.first, "Terrain/Terrain's Zone Properties/" + mZoneBinding.first});
    }
  }

  void Equilibria::stateChanged(ITexture* asset) {
    for (World* mWorld : cResourceWorld) {
      mWorld->updateDisplayLists();
    }
  }

  Equilibria::EquilibriaBindingType::EquilibriaBindingType(const std::string& typeName, const std::string& rootFolder) :
            cTypeName(typeName),
            cRootFolder(rootFolder) {
  }

  std::string Equilibria::EquilibriaBindingType::getBindingTypeID() const {
    return "Equilibria/" + cTypeName;
  }

  std::string Equilibria::EquilibriaBindingType::getBindingTypeRootFolder() const {
    return cRootFolder;
  }

  bool Equilibria::EquilibriaBindingType::renderAssetIcon() const {
    return false;
  }

  void Equilibria::EquilibriaBindingType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Equilibria::EquilibriaBindingType::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool Equilibria::EquilibriaBindingType::isDefaultConfiguration() const {
    return true;
  }

  Equilibria::EventBindingsPlayerWallBounce::EventBindingsPlayerWallBounce(Equilibria& equilibria) :
            cEquilibria(equilibria) {
  }

  IBinding* Equilibria::EventBindingsPlayerWallBounce::getBinding(const std::string& id) {
    return nullptr;
  }

  void Equilibria::EventBindingsPlayerWallBounce::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    getTreeItemInfoFunction(TreeItemInfo{"player", "Player"});
    getTreeItemInfoFunction(TreeItemInfo{"wall",   "Wall"});
  }

  void Equilibria::EventBindingsPlayerWallBounce::releaseBinding(const IBinding* asset) {
    // Nothing to do.
  }

  const std::string Equilibria::RESOURCE_CATEGORY_EQUILIBRIA_ELEMENTS = "Equilibria Elements";
  const std::string Equilibria::RESOURCE_CATEGORY_EQUILIBRIA_GRAPHICS = "Equilibria Graphics";
  const std::string Equilibria::RESOURCE_CATEGORY_EQUILIBRIA_LOGIC    = "Equilibria Logic";

  const std::string Equilibria::TOOL_DELETE      = "DeleteTool";
  const std::string Equilibria::TOOL_PROPERTIES  = "PropertiesTool";
  const std::string Equilibria::TOOL_COPY_ZONE   = "ZoneCopyTool";
  const std::string Equilibria::TOOL_MOVE_ZONE   = "ZoneMoveTool";
  const std::string Equilibria::TOOL_DELETE_ZONE = "ZoneDeleteTool";

  const std::string Equilibria::BIND_TO_ZONE = "Zone";

  std::mutex cModuleInstantiationMutex;
  std::vector<std::unique_ptr<Equilibria>> ModuleInstances;
}

#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::Project* project, IsoRealms::IResourceTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::Project * project, IsoRealms::IResourceTypeRegistry * registry) {
#endif
  std::unique_ptr<IsoRealms::Equilibria::Equilibria> mModule = std::make_unique<IsoRealms::Equilibria::Equilibria>(*project, *registry);
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::Equilibria::cModuleInstantiationMutex);
    return IsoRealms::Equilibria::ModuleInstances.emplace_back(std::move(mModule)).get();
  }
}

#ifdef __linux__
extern "C" void destroy(IsoRealms::IModuleHandle* module) {
#elif _WIN32
extern "C" void __declspec(dllexport) __stdcall destroy(IsoRealms::IModuleHandle* module) {
#endif
  std::unique_ptr<IsoRealms::Equilibria::Equilibria> mModule;
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::Equilibria::cModuleInstantiationMutex);
    mModule = IsoRealms::Utils::removeElementUnique(IsoRealms::Equilibria::ModuleInstances, module);
  }
}

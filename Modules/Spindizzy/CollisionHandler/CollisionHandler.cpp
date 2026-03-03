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
#include "CollisionHandler.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/CollisionHandler/CollisionHandlerInstance.h"

namespace IsoRealms::Spindizzy {
  const std::string CollisionHandler::JSON_OBJECT_A     = "objectA";
  const std::string CollisionHandler::JSON_OBJECT_B     = "objectB";
  const std::string CollisionHandler::JSON_ON_COLLISION = "onCollision";
  const std::string CollisionHandler::JSON_ON_PARTING   = "onParting";

  CollisionHandler::CollisionHandler(Spindizzy& spindizzy, IResourceData& data) :
            cSpindizzy(spindizzy),
            cDefPhysicalObjectTypeA(spindizzy),
            cDefPhysicalObjectTypeB(spindizzy),
            cDefEnteredAction(data.getDummyActionClient()),
            cDefExitedAction(data.getDummyActionClient()) {
    cDefPhysicalObjectTypeA.addNotifyAssetChangedFunction(this);
    cDefPhysicalObjectTypeB.addNotifyAssetChangedFunction(this);
  }

  CollisionHandler::CollisionHandler(Spindizzy& spindizzy, IResourceData& data, JSONObject object) :
            CollisionHandler(spindizzy, data) {
    cDefPhysicalObjectTypeA.init(object, JSON_OBJECT_A);
    cDefPhysicalObjectTypeB.init(object, JSON_OBJECT_B);
    cDefEnteredAction.init(object, JSON_ON_COLLISION);
    cDefExitedAction.init(object, JSON_ON_PARTING);
    spindizzy.getProject().init([this, &spindizzy]() {
      spindizzy.added(this);
    });
  }

  void CollisionHandler::registerAssets(ResourceAssetRegistry& assets) {
    // Nothing to do.
  }

  void CollisionHandler::save(JSONObject object) const {
    cDefPhysicalObjectTypeA.save(object, JSON_OBJECT_A);
    cDefPhysicalObjectTypeB.save(object, JSON_OBJECT_B);
    cDefEnteredAction.save(object, JSON_ON_COLLISION);
    cDefExitedAction.save(object, JSON_ON_PARTING);
  }

  void CollisionHandler::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool CollisionHandler::renderIcon() {
    return false;
  }

  void CollisionHandler::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector<PhysicalObjectType>(metadata.getPropertyData("EntityA"),      cDefPhysicalObjectTypeA);
    owner.createPropertyTreeSelector<PhysicalObjectType>(metadata.getPropertyData("EntityB"),      cDefPhysicalObjectTypeB);
    owner.createPropertyTreeSelector<Action>(            metadata.getPropertyData("OnCollision"),  cDefEnteredAction);
    owner.createPropertyTreeSelector<Action>(            metadata.getPropertyData("OnSeparation"), cDefExitedAction);
  }

  void CollisionHandler::removed() {
    cSpindizzy.removed(this);
    cDefPhysicalObjectTypeA.removeNotifyAssetChangedFunction(this);
    cDefPhysicalObjectTypeB.removeNotifyAssetChangedFunction(this);
  }

  const PhysicalObjectType* CollisionHandler::getPhysicalObjectTypeA() const {
    return &cDefPhysicalObjectTypeA;
  }

  const PhysicalObjectType* CollisionHandler::getPhysicalObjectTypeB() const {
    return &cDefPhysicalObjectTypeB;
  }

  std::unique_ptr<CollisionHandlerInstance> CollisionHandler::createInstance(World* world) {
    return std::make_unique<CollisionHandlerInstance>(this, world);
  }

  void CollisionHandler::executeAction(bool exited) {
    (exited ? cDefExitedAction : cDefEnteredAction).execute();
  }

  void CollisionHandler::physicalObjectTypeChanged(const IPhysicalObjectType* oldPhysicalObjectType, const IPhysicalObjectType* newPhysicalObjectType) {
    cSpindizzy.physicalObjectTypeChanged(this, oldPhysicalObjectType, newPhysicalObjectType);
  }
}

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
#include "CollisionHandler.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/CollisionHandler/CollisionHandlerInstance.h"

namespace IsoRealms::Spindizzy {
  const std::string CollisionHandler::TAG_ENTERED_ACTION = "EnteredAction";
  const std::string CollisionHandler::TAG_EXITED_ACTION  = "ExitedAction";
  const std::string CollisionHandler::TAG_TYPE_A         = "TypeA";
  const std::string CollisionHandler::TAG_TYPE_B         = "TypeB";

  CollisionHandler::CollisionHandler(IProject* project, Spindizzy* spindizzy) :
            cDefPhysicalObjectTypeA(*spindizzy),
            cDefPhysicalObjectTypeB(*spindizzy),
            cDefEnteredAction(project),
            cDefExitedAction(project) {
  }

  CollisionHandler::CollisionHandler(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            CollisionHandler(project, spindizzy) {
    cDefPhysicalObjectTypeA.init(node.getNode(TAG_TYPE_A));
    cDefPhysicalObjectTypeB.init(node.getNode(TAG_TYPE_B));
    cDefEnteredAction.init(node, TAG_ENTERED_ACTION);
    cDefExitedAction.init(node, TAG_EXITED_ACTION);
    spindizzy->getProject()->init([this, spindizzy](IAssets* assets) {
      spindizzy->added(this);
    });
  }

  void CollisionHandler::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }

  void CollisionHandler::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    // Nothing to do.
  }

  void CollisionHandler::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefPhysicalObjectTypeA.save(node, TAG_TYPE_A);
    cDefPhysicalObjectTypeB.save(node, TAG_TYPE_B);
    cDefEnteredAction.save(node, TAG_ENTERED_ACTION);
    cDefExitedAction.save(node, TAG_EXITED_ACTION);
  }

  void CollisionHandler::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool CollisionHandler::renderIcon() {
    return false;
  }

  std::vector<IProperty*> CollisionHandler::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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
}

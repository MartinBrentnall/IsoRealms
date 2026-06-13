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
#include "BoundaryHandler.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/BoundaryHandler/BoundaryHandlerInstance.h"

namespace IsoRealms::Equilibria {
  BoundaryHandler::BoundaryHandler(Equilibria& equilibria, IComponentData& data) :
            cEquilibria(equilibria),
            cActionContext(data, *this),
            cDefBoundaryType(equilibria),
            cDefObjectType(equilibria),
            cDefEnteredAction(cActionContext),
            cDefExitedAction(cActionContext) {
  }

  void BoundaryHandler::registerAssets(ComponentAssetRegistry& assets) {
    // Nothing to do.
  }

  void BoundaryHandler::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool BoundaryHandler::renderIcon() {
    return false;
  }

  void BoundaryHandler::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector("object",   cDefObjectType);
    owner.createPropertyTreeSelector("boundary", cDefBoundaryType);
    owner.createPropertyTreeSelector("onEntry",  cDefEnteredAction);
    owner.createPropertyTreeSelector("onExit",   cDefExitedAction);

    if (owner.loadsPersistedValues()) {
      cEquilibria.getProject().init([this]() {
        cEquilibria.added(this);
      });
    }
  }

  void BoundaryHandler::removed() {
    cEquilibria.removed(this);
  }

  const BoundaryType* BoundaryHandler::getBoundaryType() const {
    return &cDefBoundaryType;
  }

  const PhysicalObjectType* BoundaryHandler::getObjectType() const {
    return &cDefObjectType;
  }

  std::unique_ptr<BoundaryHandlerInstance> BoundaryHandler::createInstance(World* world) {
    return std::make_unique<BoundaryHandlerInstance>(this, world);
  }

  void BoundaryHandler::executeAction(bool exited) {
    (exited ? cDefExitedAction : cDefEnteredAction).execute();
  }
  
  IBinding* BoundaryHandler::getBinding(const std::string& id) {
    std::size_t mSplit = id.find('/');
    std::string mBindTo = id.substr(0, mSplit);
    std::string mBindPath = id.substr(mSplit + 1);
    return mBindTo == BIND_TO_BOUNDARY ? cDefBoundaryType.getBoundaryBinding(mBindPath)
         : mBindTo == BIND_TO_OBJECT   ? cDefObjectType.getBinding(mBindPath)
         :                               nullptr;
  }
  
  void BoundaryHandler::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    cDefBoundaryType->forEachAvailableBoundaryTypeTreeItem([&getTreeItemInfoFunction](const TreeItemInfo& mTreeItemInfo) {
      getTreeItemInfoFunction(TreeItemInfo{BIND_TO_BOUNDARY + "/" + mTreeItemInfo.cID, BIND_TO_BOUNDARY + "/" + mTreeItemInfo.cPath});
    });
    cDefObjectType->forEachAvailablePhysicalObjectTypeTreeItem([&getTreeItemInfoFunction](const TreeItemInfo& mTreeItemInfo) {
      getTreeItemInfoFunction(TreeItemInfo{BIND_TO_OBJECT + "/" + mTreeItemInfo.cID, BIND_TO_OBJECT + "/" + mTreeItemInfo.cPath});
    });
  }
  
  std::string BoundaryHandler::getBindingID(const IBinding* binding) const {
    std::string mBindingID = cDefBoundaryType.getBoundaryBindingID(binding);
    if (mBindingID != "") {
      return BIND_TO_BOUNDARY + "/" + mBindingID;
    }
    mBindingID = cDefObjectType.getBindingID(binding);
    if (mBindingID != "") {
      return BIND_TO_OBJECT + "/" + mBindingID;
    }
    return "";
  }
  
  void BoundaryHandler::releaseBinding(const IBinding* asset) {
    // Nothing to do.
  }
}

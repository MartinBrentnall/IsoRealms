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
#include "PhysicalObjectType.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  PhysicalObjectType::PhysicalObjectType(Spindizzy& spindizzy) : 
            Asset<PhysicalObjectType, IPhysicalObjectType, Spindizzy>(spindizzy) {
  }

  IBinding* PhysicalObjectType::getBinding(const std::string& id) const {
    return cAsset->getPhysicalObjectTypeBinding(id);
  }

  std::string PhysicalObjectType::getBindingID(const IBinding* binding) const {
    return cAsset->getPhysicalObjectTypeBindingID(binding);
  }

  void PhysicalObjectType::forEachAvailablePhysicalObjectTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    cAsset->forEachAvailablePhysicalObjectTypeTreeItem(getTreeItemInfoFunction);
  }

  void PhysicalObjectType::addNotifyAssetChangedFunction(IPhysicalObjectTypeListener* listener) {
    cNotifyAssetChangedListeners.push_back(listener);
  }

  void PhysicalObjectType::removeNotifyAssetChangedFunction(IPhysicalObjectTypeListener* listener) {
    cNotifyAssetChangedListeners.erase(std::remove(cNotifyAssetChangedListeners.begin(), cNotifyAssetChangedListeners.end(), listener), cNotifyAssetChangedListeners.end());
  }

  void PhysicalObjectType::notifyAssetChanged(const IPhysicalObjectType* oldAsset, const IPhysicalObjectType* newAsset) {
    for (IPhysicalObjectTypeListener* listener : cNotifyAssetChangedListeners) {
      listener->physicalObjectTypeChanged(oldAsset, newAsset);
    }
  }

  bool PhysicalObjectType::isDefaultConfiguration() const {
    return true;
  }
}

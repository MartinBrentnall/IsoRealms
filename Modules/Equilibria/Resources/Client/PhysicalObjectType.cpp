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

#include "Modules/Equilibria/Equilibria.h"

namespace IsoRealms::Equilibria {
  PhysicalObjectType::PhysicalObjectType(Equilibria& equilibria) : 
            Resource<PhysicalObjectType, IPhysicalObjectType, Equilibria>(equilibria) {
  }

  IBinding* PhysicalObjectType::getBinding(const std::string& id) const {
    return cResource->getPhysicalObjectTypeBinding(id);
  }

  std::string PhysicalObjectType::getBindingID(const IBinding* binding) const {
    return cResource->getPhysicalObjectTypeBindingID(binding);
  }

  void PhysicalObjectType::forEachAvailablePhysicalObjectTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    cResource->forEachAvailablePhysicalObjectTypeTreeItem(getTreeItemInfoFunction);
  }

  void PhysicalObjectType::addNotifyResourceChangedFunction(IPhysicalObjectTypeListener* listener) {
    cNotifyResourceChangedListeners.push_back(listener);
  }

  void PhysicalObjectType::removeNotifyResourceChangedFunction(IPhysicalObjectTypeListener* listener) {
    cNotifyResourceChangedListeners.erase(std::remove(cNotifyResourceChangedListeners.begin(), cNotifyResourceChangedListeners.end(), listener), cNotifyResourceChangedListeners.end());
  }

  void PhysicalObjectType::notifyResourceChanged(const IPhysicalObjectType* oldResource, const IPhysicalObjectType* newResource) {
    for (IPhysicalObjectTypeListener* listener : cNotifyResourceChangedListeners) {
      listener->physicalObjectTypeChanged(oldResource, newResource);
    }
  }
}

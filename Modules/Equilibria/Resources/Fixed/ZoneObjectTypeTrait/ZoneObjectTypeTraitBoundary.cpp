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
#include <cmath>

#include "ZoneObjectTypeTraitBoundary.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/Object/ZoneObject/Traits/Boundary/Boundary.h"
#include "Modules/Equilibria/ZoneObjectType/ZoneObjectType.h"

namespace IsoRealms::Equilibria {
  ZoneObjectTypeTraitBoundary::ZoneObjectTypeTraitBoundary(const Metadata& metadata, ZoneObjectType& type) :
            cDefType(type) {
    cDefType.getEquilibria().added(this);
  }
  
  void ZoneObjectTypeTraitBoundary::publish(EquilibriaResourceRegistry& registry, const std::string& parentID) {
    // TODO: project.add(this); // Boundary type
  }
  
  std::string ZoneObjectTypeTraitBoundary::getStartID() const {
    return cDefStartID;
  }
  
  std::string ZoneObjectTypeTraitBoundary::getEndID() const {
    return cDefEndID;
  }

  bool ZoneObjectTypeTraitBoundary::isInitiallyEnabled() const {
    return cDefInitiallyEnabled;
  }    
  
  std::unique_ptr<IZoneObjectTrait> ZoneObjectTypeTraitBoundary::createTrait(ZoneObject& object) {
    return std::make_unique<Boundary>(*this, object);
  }
  
  std::string ZoneObjectTypeTraitBoundary::getBoundaryTypeID() const {
    return "ZoneObject/" + cDefType.getEquilibria().getComponentID(&cDefType) + "/" + cDefType.getID(this);
  }
  
  IBinding* ZoneObjectTypeTraitBoundary::getBounderyTypeBinding(const std::string& id) const {
    return cDefType.getBinding(id);
  }
  
  std::string ZoneObjectTypeTraitBoundary::getBoundaryTypeBindingID(const IBinding* binding) const {
    return cDefType.getBindingID(binding);
  }

  void ZoneObjectTypeTraitBoundary::forEachAvailableBoundaryTypeTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    // TODO: Implement this.
  }
}

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
#include "BoundaryType.h"

#include "Modules/Equilibria/Equilibria.h"

namespace IsoRealms::Equilibria {
  BoundaryType::BoundaryType(Equilibria& equilibria) : 
            Resource<BoundaryType, IBoundaryType, Equilibria>(equilibria) {
  }

  IBinding* BoundaryType::getBoundaryBinding(const std::string& id) const {
    return cResource->getBounderyTypeBinding(id);
  }

  std::string BoundaryType::getBoundaryBindingID(const IBinding* binding) const {
    return cResource->getBoundaryTypeBindingID(binding);
  }

  void BoundaryType::forEachAvailableBoundaryTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    cResource->forEachAvailableBoundaryTypeTreeItem(getTreeItemInfoFunction);
  }
}

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
  const std::string ZoneObjectTypeTraitBoundary::JSON_ENABLED = "enabled";
  const std::string ZoneObjectTypeTraitBoundary::JSON_END     = "end";
  const std::string ZoneObjectTypeTraitBoundary::JSON_START   = "start";

  ZoneObjectTypeTraitBoundary::ZoneObjectTypeTraitBoundary(const Metadata& metadata, ZoneObjectType& type) :
            cDefType(type) {
    cDefType.getEquilibria().added(this);
  }

  ZoneObjectTypeTraitBoundary::ZoneObjectTypeTraitBoundary(const Metadata& metadata, ZoneObjectType& type, JSONObject object) :
            ZoneObjectTypeTraitBoundary(metadata, type) {
    cDefInitiallyEnabled = object.getBoolean(JSON_ENABLED, true);
    cDefStartID = object.getString(JSON_START);
    cDefEndID = object.getString(JSON_END, cDefStartID);
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
  
  void ZoneObjectTypeTraitBoundary::save(JSONObject object) const {
    object.addBoolean(JSON_ENABLED, cDefInitiallyEnabled, true);
    object.addString(JSON_START, cDefStartID);
    object.addString(JSON_END, cDefEndID, cDefStartID);
  }

  std::unique_ptr<IZoneObjectTrait> ZoneObjectTypeTraitBoundary::createTrait(ZoneObject& object) {
    return std::make_unique<Boundary>(*this, object);
  }
  
  void ZoneObjectTypeTraitBoundary::registerAssets(EquilibriaAssetRegistry& assets, const std::string& parentID) {
    // TODO: assets.add(this); // Boundary type
  }
  
  std::string ZoneObjectTypeTraitBoundary::getBoundaryTypeID() const {
    return "ZoneObject/" + cDefType.getEquilibria().getResourceID(&cDefType) + "/" + cDefType.getID(this);
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

  bool ZoneObjectTypeTraitBoundary::renderAssetIcon() const {
    return false;
  }

  void ZoneObjectTypeTraitBoundary::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void ZoneObjectTypeTraitBoundary::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool ZoneObjectTypeTraitBoundary::isDefaultConfiguration() const {
    return true;
  }
}

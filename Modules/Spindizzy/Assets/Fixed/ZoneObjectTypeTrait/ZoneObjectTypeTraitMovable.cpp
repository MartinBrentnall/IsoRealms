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

#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/Movable/Movable.h"

#include "ZoneObjectTypeTraitMovable.h"

namespace IsoRealms::Spindizzy {
  const std::string ZoneObjectTypeTraitMovable::JSON_LOCATION = "location";

  ZoneObjectTypeTraitMovable::ZoneObjectTypeTraitMovable(IProject& project, ZoneObjectType& type) {
    // Nothing to do.
  }

  ZoneObjectTypeTraitMovable::ZoneObjectTypeTraitMovable(IProject& project, ZoneObjectType& type, JSONObject object) :
            ZoneObjectTypeTraitMovable(project, type) {
    cDefInitialLocationID = object.getString(JSON_LOCATION);
  }

  std::string ZoneObjectTypeTraitMovable::getInitialLocationID() const {
    return cDefInitialLocationID;
  }  
  
  void ZoneObjectTypeTraitMovable::save(JSONObject object) const {
    object.addString(JSON_LOCATION, cDefInitialLocationID);
  }

  std::unique_ptr<IZoneObjectTrait> ZoneObjectTypeTraitMovable::createTrait(ZoneObject& object) {
    return std::make_unique<Movable>(object, *this);
  }
  
  void ZoneObjectTypeTraitMovable::registerAssets(ISpindizzyRegistry* registry) {
    // Nothing to do.
  }

  bool ZoneObjectTypeTraitMovable::renderAssetIcon() const {
    return false;
  }

  void ZoneObjectTypeTraitMovable::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ZoneObjectTypeTraitMovable::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ZoneObjectTypeTraitMovable::isDefaultConfiguration() const {
    return true;
  }
}

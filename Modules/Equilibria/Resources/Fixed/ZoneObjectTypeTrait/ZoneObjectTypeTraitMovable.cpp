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

#include "Modules/Equilibria/World/Object/ZoneObject/Traits/Movable/Movable.h"

#include "ZoneObjectTypeTraitMovable.h"

namespace IsoRealms::Equilibria {
  ZoneObjectTypeTraitMovable::ZoneObjectTypeTraitMovable(const Metadata& metadata, ZoneObjectType& type) {
    // Nothing to do.
  }
  
  ZoneObjectTypeTraitMovable::ZoneObjectTypeTraitMovable(const Metadata& metadata, ZoneObjectType& type, JSONObject object) :
            ZoneObjectTypeTraitMovable(metadata, type) {
    cDefInitialLocationID = object.getString(JSON_LOCATION);
  }

  void ZoneObjectTypeTraitMovable::publish(EquilibriaResourceRegistry& registry, const std::string& parentID) {
    // Nothing to do.
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

  ZoneObjectTypeTraitMovable::ZoneObjectTypeTraitMovable(const Metadata& metadata, ZoneObjectType& type, JSONObject object) :
            ZoneObjectTypeTraitMovable(metadata, type) {
    cDefInitialLocationID = object.getString(JSON_LOCATION);
  }
}

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
#include <cmath>

#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/Movable/Movable.h"

#include "ZoneObjectTypeTraitMovable.h"

namespace IsoRealms::Spindizzy {
  const std::string ZoneObjectTypeTraitMovable::ATTRIBUTE_INIT = "init";
  
  ZoneObjectTypeTraitMovable::ZoneObjectTypeTraitMovable(IProject* project, ZoneObjectType* type, DOMNode& node) {
    cDefInitialLocationID = node.getAttribute(ATTRIBUTE_INIT);
  }
  
  std::string ZoneObjectTypeTraitMovable::getInitialLocationID() const {
    return cDefInitialLocationID;
  }  
  
  void ZoneObjectTypeTraitMovable::save(DOMNodeWriter& node) const {
    node.addAttribute(ATTRIBUTE_INIT, cDefInitialLocationID);
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
}

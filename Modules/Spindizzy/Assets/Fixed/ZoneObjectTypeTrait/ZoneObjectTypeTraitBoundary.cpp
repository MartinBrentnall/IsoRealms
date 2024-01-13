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

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/Boundary/Boundary.h"
#include "Modules/Spindizzy/ZoneObjectType/ZoneObjectType.h"

#include "ZoneObjectTypeTraitBoundary.h"

namespace IsoRealms::Spindizzy {
  const std::string ZoneObjectTypeTraitBoundary::ATTRIBUTE_ENABLED = "enabled";
  const std::string ZoneObjectTypeTraitBoundary::ATTRIBUTE_END     = "end";
  const std::string ZoneObjectTypeTraitBoundary::ATTRIBUTE_START   = "start";
  
  ZoneObjectTypeTraitBoundary::ZoneObjectTypeTraitBoundary(IProject* project, ZoneObjectType* type, DOMNode& node) :
            cDefType(*type) {
    cDefType.getSpindizzy().added(this);
    cDefInitiallyEnabled = node.getBooleanAttribute(ATTRIBUTE_ENABLED, true);
    cDefStartID = node.getAttribute(ATTRIBUTE_START);
    cDefEndID = node.getAttribute(ATTRIBUTE_END, cDefStartID);
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
  
  void ZoneObjectTypeTraitBoundary::save(DOMNodeWriter& node) const {
    node.addAttribute(ATTRIBUTE_ENABLED, cDefInitiallyEnabled, true);
    node.addAttribute(ATTRIBUTE_START, cDefStartID);
    node.addAttribute(ATTRIBUTE_END, cDefEndID, cDefStartID);
  }
  
  std::unique_ptr<IZoneObjectTrait> ZoneObjectTypeTraitBoundary::createTrait(ZoneObject& object) {
    return std::make_unique<Boundary>(*this, object);
  }
  
  void ZoneObjectTypeTraitBoundary::registerAssets(ISpindizzyRegistry* registry) {
    // TODO: registry->add(this); // Boundary type
  }
  
  std::string ZoneObjectTypeTraitBoundary::getBoundaryTypeID() const {
    return "ZoneObject/" + cDefType.getSpindizzy().getID(&cDefType) + "/" + cDefType.getID(this);
  }
  
  IBinding* ZoneObjectTypeTraitBoundary::getBinding(const std::string& id) const {
    return cDefType.getBinding(id);
  }
}

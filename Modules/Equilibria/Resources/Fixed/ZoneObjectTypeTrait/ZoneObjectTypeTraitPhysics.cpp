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
#include "Modules/Equilibria/World/Object/ZoneObject/Traits/Physics/Physics.h"

#include "ZoneObjectTypeTraitPhysics.h"

namespace IsoRealms::Equilibria {
  ZoneObjectTypeTraitPhysics::ZoneObjectTypeTraitPhysics(ZoneObjectType& type) {
    // Nothing to do.
  }
  
  void ZoneObjectTypeTraitPhysics::publish(EquilibriaResourceRegistry& registry, const std::string& parentID) {
    // Nothing to do.
  }

  std::string ZoneObjectTypeTraitPhysics::getMovableID() const {
    return cDefMovableID;
  }  
  
  float ZoneObjectTypeTraitPhysics::getStepReach() const {
    return cDefStepReach;
  }
  
  float ZoneObjectTypeTraitPhysics::getHeight() const {
    return cDefHeight;
  }
  
  float ZoneObjectTypeTraitPhysics::getRadius() const {
    return cDefRadius;
  }
  
  float ZoneObjectTypeTraitPhysics::getHugMomentum() const {
    return cDefHugMomentum;
  }
  
  float ZoneObjectTypeTraitPhysics::getBounceFactor() const {
    return cDefBounceFactor;
  }
  
  bool ZoneObjectTypeTraitPhysics::allowTraversal(ISurface* surface) const {
    return cDefUseNonSolid ? true : surface->isSolid();
  }

  bool ZoneObjectTypeTraitPhysics::triggersContacts() const {
    return false; // TODO: Configurable!
  }

  std::unique_ptr<IZoneObjectTrait> ZoneObjectTypeTraitPhysics::createTrait(ZoneObject& object) {
    return std::make_unique<Physics>(object, *this);
  }
}

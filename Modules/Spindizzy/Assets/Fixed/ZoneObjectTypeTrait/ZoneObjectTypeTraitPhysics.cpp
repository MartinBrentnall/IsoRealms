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
#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/Physics/Physics.h"

#include "ZoneObjectTypeTraitPhysics.h"

namespace IsoRealms::Spindizzy {
  const std::string ZoneObjectTypeTraitPhysics::ATTRIBUTE_BOUNCE_FACTOR = "bounceFactor";
  const std::string ZoneObjectTypeTraitPhysics::ATTRIBUTE_CONTROLS      = "controls";
  const std::string ZoneObjectTypeTraitPhysics::ATTRIBUTE_HEIGHT        = "height";
  const std::string ZoneObjectTypeTraitPhysics::ATTRIBUTE_HUG_MOMENTUM  = "hugMomentum";
  const std::string ZoneObjectTypeTraitPhysics::ATTRIBUTE_RADIUS        = "radius";
  const std::string ZoneObjectTypeTraitPhysics::ATTRIBUTE_STEP_REACH    = "stepReach";
  const std::string ZoneObjectTypeTraitPhysics::ATTRIBUTE_USE_NON_SOLID = "useNonSolid";

  const float ZoneObjectTypeTraitPhysics::DEFAULT_BOUNCE_FACTOR = 1.0f;
  const float ZoneObjectTypeTraitPhysics::DEFAULT_HEIGHT        = 1.7f;
  const float ZoneObjectTypeTraitPhysics::DEFAULT_HUG_MOMENTUM  = 0.0001f;
  const float ZoneObjectTypeTraitPhysics::DEFAULT_RADIUS        = 0.4f;
  const float ZoneObjectTypeTraitPhysics::DEFAULT_STEP_REACH    = 0.5f;
  const bool  ZoneObjectTypeTraitPhysics::DEFAULT_USE_NON_SOLID = false;
  
  ZoneObjectTypeTraitPhysics::ZoneObjectTypeTraitPhysics(IProject* project, ZoneObjectType* type, DOMNode& node) {
    cDefMovableID = node.getAttribute(ATTRIBUTE_CONTROLS);
    cDefStepReach = node.getFloatAttribute(ATTRIBUTE_STEP_REACH, DEFAULT_STEP_REACH);
    cDefHeight = node.getFloatAttribute(ATTRIBUTE_HEIGHT, DEFAULT_HEIGHT);
    cDefRadius = node.getFloatAttribute(ATTRIBUTE_RADIUS, DEFAULT_RADIUS);
    cDefHugMomentum = node.getFloatAttribute(ATTRIBUTE_HUG_MOMENTUM, DEFAULT_HUG_MOMENTUM);
    cDefBounceFactor = node.getFloatAttribute(ATTRIBUTE_BOUNCE_FACTOR, DEFAULT_BOUNCE_FACTOR);
    cDefUseNonSolid = node.getBooleanAttribute(ATTRIBUTE_USE_NON_SOLID, DEFAULT_USE_NON_SOLID);
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

  void ZoneObjectTypeTraitPhysics::save(DOMNodeWriter& node) const {
    node.addAttribute(ATTRIBUTE_CONTROLS, cDefMovableID);
    node.addAttribute(ATTRIBUTE_STEP_REACH, cDefStepReach, DEFAULT_STEP_REACH);
    node.addAttribute(ATTRIBUTE_HEIGHT, cDefHeight, DEFAULT_HEIGHT);
    node.addAttribute(ATTRIBUTE_RADIUS, cDefRadius, DEFAULT_RADIUS);
    node.addAttribute(ATTRIBUTE_HUG_MOMENTUM, cDefHugMomentum, DEFAULT_HUG_MOMENTUM);
    node.addAttribute(ATTRIBUTE_BOUNCE_FACTOR, cDefBounceFactor, DEFAULT_BOUNCE_FACTOR);
    node.addAttribute(ATTRIBUTE_USE_NON_SOLID, cDefUseNonSolid, DEFAULT_USE_NON_SOLID);
  }
  
  std::unique_ptr<IZoneObjectTrait> ZoneObjectTypeTraitPhysics::createTrait(ZoneObject& object) {
    return std::make_unique<Physics>(object, *this);
  }
  
  void ZoneObjectTypeTraitPhysics::registerAssets(ISpindizzyRegistry* registry) {
    // Nothing to do.
  }
}

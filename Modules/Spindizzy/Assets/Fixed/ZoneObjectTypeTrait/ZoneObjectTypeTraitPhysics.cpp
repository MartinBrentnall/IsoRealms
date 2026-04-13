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
#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/Physics/Physics.h"

#include "ZoneObjectTypeTraitPhysics.h"

namespace IsoRealms::Spindizzy {
  const std::string ZoneObjectTypeTraitPhysics::JSON_BOUNCE_FACTOR = "bounceFactor";
  const std::string ZoneObjectTypeTraitPhysics::JSON_CONTROLS      = "controls";
  const std::string ZoneObjectTypeTraitPhysics::JSON_HEIGHT        = "height";
  const std::string ZoneObjectTypeTraitPhysics::JSON_HUG_MOMENTUM  = "hugMomentum";
  const std::string ZoneObjectTypeTraitPhysics::JSON_RADIUS        = "radius";
  const std::string ZoneObjectTypeTraitPhysics::JSON_STEP_REACH    = "stepReach";
  const std::string ZoneObjectTypeTraitPhysics::JSON_USE_NON_SOLID = "useNonSolid";

  const float ZoneObjectTypeTraitPhysics::DEFAULT_BOUNCE_FACTOR = 1.0f;
  const float ZoneObjectTypeTraitPhysics::DEFAULT_HEIGHT        = 1.7f;
  const float ZoneObjectTypeTraitPhysics::DEFAULT_HUG_MOMENTUM  = 0.0001f;
  const float ZoneObjectTypeTraitPhysics::DEFAULT_RADIUS        = 0.4f;
  const float ZoneObjectTypeTraitPhysics::DEFAULT_STEP_REACH    = 0.5f;
  const bool  ZoneObjectTypeTraitPhysics::DEFAULT_USE_NON_SOLID = false;
  
  ZoneObjectTypeTraitPhysics::ZoneObjectTypeTraitPhysics(const Metadata& metadata, ZoneObjectType& type) {
    // Nothing to do.
  }

  ZoneObjectTypeTraitPhysics::ZoneObjectTypeTraitPhysics(const Metadata& metadata, ZoneObjectType& type, JSONObject object) :
            ZoneObjectTypeTraitPhysics(metadata, type) {
    cDefMovableID = object.getString(JSON_CONTROLS);
    cDefStepReach = object.getFloat(JSON_STEP_REACH, DEFAULT_STEP_REACH);
    cDefHeight = object.getFloat(JSON_HEIGHT, DEFAULT_HEIGHT);
    cDefRadius = object.getFloat(JSON_RADIUS, DEFAULT_RADIUS);
    cDefHugMomentum = object.getFloat(JSON_HUG_MOMENTUM, DEFAULT_HUG_MOMENTUM);
    cDefBounceFactor = object.getFloat(JSON_BOUNCE_FACTOR, DEFAULT_BOUNCE_FACTOR);
    cDefUseNonSolid = object.getBoolean(JSON_USE_NON_SOLID, DEFAULT_USE_NON_SOLID);
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

  void ZoneObjectTypeTraitPhysics::save(JSONObject object) const {
    object.addString(JSON_CONTROLS, cDefMovableID);
    object.addFloat(JSON_STEP_REACH, cDefStepReach, DEFAULT_STEP_REACH);
    object.addFloat(JSON_HEIGHT, cDefHeight, DEFAULT_HEIGHT);
    object.addFloat(JSON_RADIUS, cDefRadius, DEFAULT_RADIUS);
    object.addFloat(JSON_HUG_MOMENTUM, cDefHugMomentum, DEFAULT_HUG_MOMENTUM);
    object.addFloat(JSON_BOUNCE_FACTOR, cDefBounceFactor, DEFAULT_BOUNCE_FACTOR);
    object.addBoolean(JSON_USE_NON_SOLID, cDefUseNonSolid, DEFAULT_USE_NON_SOLID);
  }

  std::unique_ptr<IZoneObjectTrait> ZoneObjectTypeTraitPhysics::createTrait(ZoneObject& object) {
    return std::make_unique<Physics>(object, *this);
  }
  
  void ZoneObjectTypeTraitPhysics::registerAssets(SpindizzyAssetRegistry& assets, const std::string& parentID) {
    // Nothing to do.
  }

  bool ZoneObjectTypeTraitPhysics::renderAssetIcon() const {
    return false;
  }

  void ZoneObjectTypeTraitPhysics::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void ZoneObjectTypeTraitPhysics::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool ZoneObjectTypeTraitPhysics::isDefaultConfiguration() const {
    return true;
  }
}

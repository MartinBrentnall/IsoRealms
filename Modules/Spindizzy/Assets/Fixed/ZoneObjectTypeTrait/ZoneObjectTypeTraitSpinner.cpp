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

#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/Spinner/Spinner.h"

#include "ZoneObjectTypeTraitSpinner.h"

namespace IsoRealms::Spindizzy {
  const std::string ZoneObjectTypeTraitSpinner::TAG_MODEL = "Model";
  
  const std::string ZoneObjectTypeTraitSpinner::ATTRIBUTE_LOCATION   = "location";
  const std::string ZoneObjectTypeTraitSpinner::ATTRIBUTE_SPIN_SPEED = "spinSpeed";
  
  ZoneObjectTypeTraitSpinner::ZoneObjectTypeTraitSpinner(IProject* project, ZoneObjectType* type, DOMNode& node) :
            cDefModel(project) {
    cDefSpinSpeed = node.getFloatAttribute(ATTRIBUTE_SPIN_SPEED);
    cDefLocationID = node.getAttribute(ATTRIBUTE_LOCATION);
    cDefModel.init(node, TAG_MODEL);
  }

  std::unique_ptr<ModelInstance> ZoneObjectTypeTraitSpinner::createModel() {
    return cDefModel.createInstance();
  }
  
  float ZoneObjectTypeTraitSpinner::getSpinSpeed() const {
    return cDefSpinSpeed;
  }
  
  std::string ZoneObjectTypeTraitSpinner::getLocationID() const {
    return cDefLocationID;
  }
  
  void ZoneObjectTypeTraitSpinner::save(DOMNodeWriter& node) const {
    cDefModel.save(&node, TAG_MODEL);
  }
  
  std::unique_ptr<IZoneObjectTrait> ZoneObjectTypeTraitSpinner::createTrait(ZoneObject& object) {
    return std::make_unique<Spinner>(object, *this);
  }
  
  void ZoneObjectTypeTraitSpinner::registerAssets(ISpindizzyRegistry* registry) {
    // Nothing to do.
  }

  bool ZoneObjectTypeTraitSpinner::renderAssetIcon() const {
    return false;
  }
}

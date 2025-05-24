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
  const std::string ZoneObjectTypeTraitSpinner::JSON_LOCATION   = "location";
  const std::string ZoneObjectTypeTraitSpinner::JSON_MODEL      = "model";
  const std::string ZoneObjectTypeTraitSpinner::JSON_SPIN_SPEED = "spinSpeed";
  
  ZoneObjectTypeTraitSpinner::ZoneObjectTypeTraitSpinner(IProject& project, ZoneObjectType& type) :
            cDefModel(project) {
  }

  ZoneObjectTypeTraitSpinner::ZoneObjectTypeTraitSpinner(IProject& project, ZoneObjectType& type, JSONObject object) :
            ZoneObjectTypeTraitSpinner(project, type) {
    cDefSpinSpeed = object.getFloat(JSON_SPIN_SPEED);
    cDefLocationID = object.getString(JSON_LOCATION);
    cDefModel.init(object, JSON_MODEL);
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
  
  void ZoneObjectTypeTraitSpinner::save(JSONObject object) const {
    cDefModel.save(object, JSON_MODEL);
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

  void ZoneObjectTypeTraitSpinner::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ZoneObjectTypeTraitSpinner::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ZoneObjectTypeTraitSpinner::isDefaultConfiguration() const {
    return true;
  }
}

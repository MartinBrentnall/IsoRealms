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
#include "ZoneObjectTypeTraitModel.h"

#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/Model/Model.h"
#include "Modules/Spindizzy/ZoneObjectType/ZoneObjectType.h"

namespace IsoRealms::Spindizzy {
  const std::string ZoneObjectTypeTraitModel::JSON_LOCATION = "location";
  const std::string ZoneObjectTypeTraitModel::JSON_MODEL    = "model";

  ZoneObjectTypeTraitModel::ZoneObjectTypeTraitModel(IProject& project, ZoneObjectType& type) :
            cDefModel(type.getResourceData()) {
  }

  ZoneObjectTypeTraitModel::ZoneObjectTypeTraitModel(IProject& project, ZoneObjectType& type, JSONObject object) :
            ZoneObjectTypeTraitModel(project, type) {
    cDefLocationID = object.getString(JSON_LOCATION);
    cDefModel.init(object, JSON_MODEL);
  }

  bool ZoneObjectTypeTraitModel::isInitiallyEnabled() const {
    return cDefInitiallyEnabled;
  }

  std::unique_ptr<ModelInstance> ZoneObjectTypeTraitModel::createModel() {
    return cDefModel.createInstance();
  }
  
  std::string ZoneObjectTypeTraitModel::getLocationID() const {
    return cDefLocationID;
  }
  
  void ZoneObjectTypeTraitModel::save(JSONObject object) const {
    cDefModel.save(object, JSON_MODEL);
    object.addString(JSON_LOCATION, cDefLocationID);
  }

  std::unique_ptr<IZoneObjectTrait> ZoneObjectTypeTraitModel::createTrait(ZoneObject& object) {
    return std::make_unique<Model>(object, *this);
  }
  
  void ZoneObjectTypeTraitModel::registerAssets(ISpindizzyRegistry* registry) {
    // Nothing to do.
  }

  bool ZoneObjectTypeTraitModel::renderAssetIcon() const {
    return false;
  }

  void ZoneObjectTypeTraitModel::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ZoneObjectTypeTraitModel::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ZoneObjectTypeTraitModel::isDefaultConfiguration() const {
    return true;
  }
}

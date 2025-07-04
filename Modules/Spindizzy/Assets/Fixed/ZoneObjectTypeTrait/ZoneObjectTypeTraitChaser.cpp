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
#include "ZoneObjectTypeTraitChaser.h"

#include "IsoRealms/Project.h"

#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/Chaser/Chaser.h"
#include "Modules/Spindizzy/ZoneObjectType/ZoneObjectType.h"

namespace IsoRealms::Spindizzy {
  const std::string ZoneObjectTypeTraitChaser::JSON_OBJECT = "object";
  const std::string ZoneObjectTypeTraitChaser::JSON_TARGET = "target";

  ZoneObjectTypeTraitChaser::ZoneObjectTypeTraitChaser(IProject& project, ZoneObjectType& type) :
            cDefTarget(type.getResourceData()) {
  }

  ZoneObjectTypeTraitChaser::ZoneObjectTypeTraitChaser(IProject& project, ZoneObjectType& type, JSONObject object) :
            ZoneObjectTypeTraitChaser(project, type) {
    cDefTarget.init(object, JSON_TARGET);
    cDefObjectID = object.getString(JSON_OBJECT);
  }

  const Vertex& ZoneObjectTypeTraitChaser::getTarget() const {
    return cDefTarget;
  }
  
  std::string ZoneObjectTypeTraitChaser::getObjectID() const {
    return cDefObjectID;
  }

  void ZoneObjectTypeTraitChaser::save(JSONObject object) const {
    cDefTarget.save(object, JSON_TARGET);
  }

  std::unique_ptr<IZoneObjectTrait> ZoneObjectTypeTraitChaser::createTrait(ZoneObject& object) {
    return std::make_unique<Chaser>(object, *this);
  }
  
  void ZoneObjectTypeTraitChaser::registerAssets(ISpindizzyRegistry* registry) {
    // Nothing to do.
  }

  bool ZoneObjectTypeTraitChaser::renderAssetIcon() const {
    return false;
  }

  void ZoneObjectTypeTraitChaser::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ZoneObjectTypeTraitChaser::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ZoneObjectTypeTraitChaser::isDefaultConfiguration() const {
    return true;
  }
}

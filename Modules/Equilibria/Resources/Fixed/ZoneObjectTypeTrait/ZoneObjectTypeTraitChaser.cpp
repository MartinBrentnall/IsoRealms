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
#include "ZoneObjectTypeTraitChaser.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/Object/ZoneObject/Traits/Chaser/Chaser.h"
#include "Modules/Equilibria/ZoneObjectType/ZoneObjectType.h"

namespace IsoRealms::Equilibria {
  ZoneObjectTypeTraitChaser::ZoneObjectTypeTraitChaser(const Metadata& metadata, ZoneObjectType& type) :
            cDefTarget(type.getComponentData()) {
  }
  
  ZoneObjectTypeTraitChaser::ZoneObjectTypeTraitChaser(const Metadata& metadata, ZoneObjectType& type, JSONObject object) :
            ZoneObjectTypeTraitChaser(metadata, type) {
    cDefTarget.init(object, JSON_TARGET);
    cDefObjectID = object.getString("object");
  }

  void ZoneObjectTypeTraitChaser::publish(EquilibriaResourceRegistry& registry, const std::string& parentID) {
    // Nothing to do.
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

  ZoneObjectTypeTraitChaser::ZoneObjectTypeTraitChaser(const Metadata& metadata, ZoneObjectType& type, JSONObject object) :
            ZoneObjectTypeTraitChaser(metadata, type) {
    cDefTarget.init(object, JSON_TARGET);
    cDefObjectID = object.getString("object");
  }
}

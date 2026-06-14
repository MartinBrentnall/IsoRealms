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
#include "Chaser.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/Object/Zone/Zone.h"
#include "Modules/Equilibria/World/Object/ZoneObject/ZoneObject.h"
#include "Modules/Equilibria/Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitChaser.h"

namespace IsoRealms::Equilibria {
  Chaser::Chaser(ZoneObject& object, ZoneObjectTypeTraitChaser& type) :
            cDefType(type) {
    object.getZone().getWorld().getEquilibria().getProject().init([this, &object]() {
      std::string mObjectID = cDefType.getObjectID();
      cDefPhysicalObject = object.getPhysics(mObjectID);
    });
  }

  void Chaser::publish(ITraitRegistry& registry) {
    registry.registerProcessor(this);
  }

  void Chaser::save(JSONObject object) const {
    // Nothing to do.
  }

  bool Chaser::hasConfiguration() const {
    return false;
  }

  void Chaser::reset() {
    // Nothing to do.
  }

  IBinding* Chaser::getTraitBinding(const std::string& id) {
    return nullptr;
  }

  void Chaser::update(unsigned int milliseconds) {
    // TODO: Enable this
//     const Vertex& mTarget = cDefType.getTarget();
//     float mXThrust = (cDefPhysicalObject->getX() > mTarget.getX() ? -1.0f : 1.0f) * cDefType->getThrust();
//     float mYThrust = (cDefPhysicalObject->getY() > mTarget.getY() ? -1.0f : 1.0f) * cDefType->getThrust();
//     cDefPhysicalObject.setXThrust(mXThrust);
//     cDefPhysicalObject.setYThrust(mYThrust);
  }
}

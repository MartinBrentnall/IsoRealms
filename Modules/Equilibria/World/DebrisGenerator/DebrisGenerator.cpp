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
#include "DebrisGenerator.h"

namespace IsoRealms::Equilibria {
  DebrisGenerator::DebrisGenerator(IComponentData& owner, const std::string& id) :
            cDefID(id),
            cDefModel(owner),
            cDefLifeTime(DEFAULT_LIFE),
            cDefHeight(DEFAULT_HEIGHT),
            cDefRadius(DEFAULT_RADIUS),
            cDefStepReach(DEFAULT_STEP_REACH),
            cLuaBinding(owner.getProject().getLuaState(), this) {
  }

  void DebrisGenerator::define(IComponentDefiner& definer) {
    definer.propertyString(         "id",         cDefID);
    definer.propertyResource(       "appearance", cDefModel);
    definer.propertyUnsignedInteger("life",       cDefLifeTime,  DEFAULT_LIFE);
    definer.propertyFloat(          "height",     cDefHeight,    DEFAULT_HEIGHT);
    definer.propertyFloat(          "radius",     cDefRadius,    DEFAULT_RADIUS);
    definer.propertyFloat(          "stepReach",  cDefStepReach, DEFAULT_STEP_REACH);
  }

  const std::string& DebrisGenerator::getID() const {
    return cDefID;
  }

  void DebrisGenerator::publish(ResourcePublisher& publisher, const std::string& parentID) {
    publisher.publish<IBinding>(&cLuaBinding, parentID + "/" + cDefID, "Debris Generators");
  }
  
  void DebrisGenerator::updateRuntime(unsigned int milliseconds) {
    for (std::unique_ptr<Debris>& mDebris : cRuntimeDebris) {
      mDebris->update(milliseconds);
    }
    while (!cRuntimeDebris.empty() && cRuntimeDebris.front()->isDead()) {
      cRuntimeDebris.pop_front();
    }
  }

  void DebrisGenerator::renderRuntime() {
    for (std::unique_ptr<Debris>& mDebris : cRuntimeDebris) {
      mDebris->render();
    }
  }

  void DebrisGenerator::reset() {
    while (!cRuntimeDebris.empty()) {
      cRuntimeDebris.pop_front();
    }
  }
  
  float DebrisGenerator::getStepReach() const {
    return cDefStepReach;
  }

  float DebrisGenerator::getHeight() const {
    return cDefHeight;
  }

  float DebrisGenerator::getRadius() const {
    return cDefRadius;
  }

  void DebrisGenerator::generateDebris(IVertex* location, double xMomentum, double yMomentum, double zMomentum, Zone& zone) {
    cRuntimeDebris.emplace_back(std::make_unique<Debris>(*this, location, xMomentum, yMomentum, zMomentum, cDefModel, cDefLifeTime, zone));
  }

  void DebrisGenerator::clear() {
    cRuntimeDebris.clear();
  }
}

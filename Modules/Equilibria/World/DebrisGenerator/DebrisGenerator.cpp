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
  DebrisGenerator::DebrisGenerator(JSONObject object, IResourceData& owner) :
            cDefID(object.getString(JSON_ID)),
            cDefModel(owner),
            cDefLifeTime(object.getInteger(JSON_LIFE, DEFAULT_LIFE)),
            cDefHeight(object.getFloat(JSON_HEIGHT, DEFAULT_HEIGHT)),
            cDefRadius(object.getFloat(JSON_RADIUS, DEFAULT_RADIUS)),
            cDefStepReach(object.getFloat(JSON_STEP_REACH, DEFAULT_STEP_REACH)),
            cLuaBinding(owner.getProject().getLuaState(), this) {
    cDefModel.init(object, JSON_APPEARANCE);
  }

  void DebrisGenerator::registerAssets(ResourceAssetRegistry& assets, const std::string& parentID) {
    assets.add<IBinding>(&cLuaBinding, parentID + "/" + cDefID, "Debris Generators");
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

  void DebrisGenerator::save(JSONObject object) const {
    object.addString(JSON_ID, cDefID);
    object.addInteger(JSON_LIFE, cDefLifeTime, DEFAULT_LIFE);
    object.addFloat(JSON_HEIGHT, cDefHeight, DEFAULT_HEIGHT);
    object.addFloat(JSON_RADIUS, cDefRadius, DEFAULT_RADIUS);
    object.addFloat(JSON_STEP_REACH, cDefStepReach, DEFAULT_STEP_REACH);
    cDefModel.save(object, JSON_APPEARANCE);
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

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
#include "DebrisGenerator.h"

namespace IsoRealms::Spindizzy {
  const std::string DebrisGenerator::TAG_MODEL = "Model";
  
  const std::string DebrisGenerator::ATTRIBUTE_HEIGHT     = "height";
  const std::string DebrisGenerator::ATTRIBUTE_ID         = "id";
  const std::string DebrisGenerator::ATTRIBUTE_LIFE       = "life";
  const std::string DebrisGenerator::ATTRIBUTE_RADIUS     = "radius";
  const std::string DebrisGenerator::ATTRIBUTE_STEP_REACH = "stepReach";

  const float        DebrisGenerator::DEFAULT_HEIGHT     = 0.7f;
  const unsigned int DebrisGenerator::DEFAULT_LIFE       = 2000U;
  const float        DebrisGenerator::DEFAULT_RADIUS     = 0.3f;
  const float        DebrisGenerator::DEFAULT_STEP_REACH = 0.2f;

  DebrisGenerator::DebrisGenerator(DOMNode& node, IProject* project) :
            cDefID(node.getAttribute(ATTRIBUTE_ID)),
            cDefModel(project),
            cDefLifeTime(node.getIntegerAttribute(ATTRIBUTE_LIFE, DEFAULT_LIFE)),
            cDefHeight(node.getFloatAttribute(ATTRIBUTE_HEIGHT, DEFAULT_HEIGHT)),
            cDefRadius(node.getFloatAttribute(ATTRIBUTE_RADIUS, DEFAULT_RADIUS)),
            cDefStepReach(node.getFloatAttribute(ATTRIBUTE_STEP_REACH, DEFAULT_STEP_REACH)),
            cLuaBinding(project, this) {
    cDefModel.init(node, TAG_MODEL);
  }

  void DebrisGenerator::registerAssets(IAssetRegistry* assets) {
    assets->add(&cLuaBinding, cDefID, "Debris Generators");
  }
  
  void DebrisGenerator::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(&cLuaBinding);
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

  void DebrisGenerator::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_ID, cDefID);
    node->addAttribute(ATTRIBUTE_LIFE, cDefLifeTime, DEFAULT_LIFE);
    node->addAttribute(ATTRIBUTE_HEIGHT, cDefHeight, DEFAULT_HEIGHT);
    node->addAttribute(ATTRIBUTE_RADIUS, cDefRadius, DEFAULT_RADIUS);
    node->addAttribute(ATTRIBUTE_STEP_REACH, cDefStepReach, DEFAULT_STEP_REACH);
    cDefModel.save(node, TAG_MODEL);
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

  void DebrisGenerator::generateDebris(IVertex* location, double xMomentum, double yMomentum, double zMomentum, Zone* zone) {
    cRuntimeDebris.emplace_back(std::make_unique<Debris>(*this, location, xMomentum, yMomentum, zMomentum, cDefModel, cDefLifeTime, zone));
  }

  void DebrisGenerator::clear() {
    cRuntimeDebris.clear();
  }
}

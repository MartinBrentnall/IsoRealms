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
#include "AxisMapping.h"

namespace IsoRealms {
  const std::string AxisMapping::TAG_AXIS = "Axis";
  
  const std::string AxisMapping::ATTRIBUTE_AXIS      = "axis";
  const std::string AxisMapping::ATTRIBUTE_DEAD_ZONE = "deadZone";

  AxisMapping::AxisMapping(unsigned int axis, bool positive, int threshold) :
          cDefAxis(axis),
          cDefDeadZone(0.16f) {
  }

  AxisMapping::AxisMapping(DOMNode& node) :
          cDefAxis(node.getIntegerAttribute(ATTRIBUTE_AXIS)),
          cDefDeadZone(node.getFloatAttribute(ATTRIBUTE_DEAD_ZONE)) {
  }

  bool AxisMapping::matches(const sf::Event& event) const {
    return event.type == sf::Event::JoystickMoved && event.joystickMove.axis == cDefAxis;
  }

  float AxisMapping::getState(const sf::Event& event) const {
    float mValue = event.joystickMove.position / 100.0f;
    return std::abs(mValue) < cDefDeadZone ? 0 : (mValue - (mValue < 0 ? -cDefDeadZone : cDefDeadZone)) * (1.0f / (1.0f - cDefDeadZone));
  }

  void AxisMapping::save(DOMNodeWriter* node) const {
    DOMNodeWriter mAxisNode = node->addBranch(TAG_AXIS);
    mAxisNode.addAttribute(ATTRIBUTE_AXIS,      cDefAxis);
    mAxisNode.addAttribute(ATTRIBUTE_DEAD_ZONE, cDefDeadZone);
  }

  std::string AxisMapping::getShortName() const {
    return "A" + Utils::toString(cDefAxis);
  }
    
  std::string AxisMapping::getLongName() const {
    return "Axis " + Utils::toString(cDefAxis);
  }
  
  void AxisMapping::loadCustomMapping(DOMNode& node) {
    // Nothing to do.
  }

  void AxisMapping::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }
  
  void AxisMapping::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    // Nothing to do.
  }
}

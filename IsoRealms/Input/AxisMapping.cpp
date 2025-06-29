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
  const std::string AxisMapping::JSON_AXIS      = "axis";
  const std::string AxisMapping::JSON_DEAD_ZONE = "deadZone";
  const std::string AxisMapping::JSON_TYPE      = "type";

  const std::string AxisMapping::TYPE_AXIS = "Axis";

  AxisMapping::AxisMapping(unsigned int axis, bool positive, int threshold) :
          cDefAxis(axis),
          cDefDeadZone(0.16f) {
  }

  AxisMapping::AxisMapping(JSONObject object) :
          cDefAxis(object.getInteger(JSON_AXIS)),
          cDefDeadZone(object.getFloat(JSON_DEAD_ZONE)) {
  }

  bool AxisMapping::matches(const sf::Event& event) const {
    return event.type == sf::Event::JoystickMoved && event.joystickMove.axis == cDefAxis;
  }

  float AxisMapping::getState(const sf::Event& event) const {
    float mValue = event.joystickMove.position / 100.0f;
    return std::abs(mValue) < cDefDeadZone ? 0 : (mValue - (mValue < 0 ? -cDefDeadZone : cDefDeadZone)) * (1.0f / (1.0f - cDefDeadZone));
  }

  void AxisMapping::save(JSONObject object, const std::string& name) const {
    object.addString(JSON_TYPE, TYPE_AXIS);
    object.addInteger(JSON_AXIS, cDefAxis);
    object.addFloat(JSON_DEAD_ZONE, cDefDeadZone);
  }

  std::string AxisMapping::getShortName() const {
    return "A" + Utils::toString(cDefAxis);
  }
    
  std::string AxisMapping::getLongName() const {
    return "Axis " + Utils::toString(cDefAxis);
  }
  
  void AxisMapping::loadCustomMapping(JSONObject object) {
    // Nothing to do.
  }

  void AxisMapping::registerAssets(IAssetRegistry& assets) {
    // Nothing to do.
  }
}

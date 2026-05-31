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
#include "AxisMapping.h"

#include "IsoRealms/Editing/Property/IPropertyMaker.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  AxisMapping::AxisMapping(unsigned int axis, bool positive, int threshold) :
          cDefAxis(axis),
          cDefDeadZone(0.16f) {
  }

  AxisMapping::AxisMapping(JSONObject object) :
          cDefAxis(object.getInteger(JSON_AXIS)),
          cDefDeadZone(object.getFloat(JSON_DEAD_ZONE)) {
  }

  AxisMapping::AxisMapping(const Metadata& metadata, IResourceData& owner) :
          cDefAxis(0),
          cDefDeadZone(0.16f) {
  }

  AxisMapping::AxisMapping(const Metadata& metadata, IResourceData& owner, JSONObject object) :
          AxisMapping(metadata, owner) {
    cDefAxis = object.getInteger(JSON_AXIS);
    cDefDeadZone = object.getFloat(JSON_DEAD_ZONE);
  }

  std::string AxisMapping::getName() const {
    return ""; // TODO: Implement this.
  }

  bool AxisMapping::matches(const sf::Event& event) const {
    return event.type == sf::Event::JoystickMoved && event.joystickMove.axis == cDefAxis;
  }

  float AxisMapping::getState(const sf::Event& event) const {
    float mValue = event.joystickMove.position / 100.0f;
    return std::abs(mValue) < cDefDeadZone ? 0 : (mValue - (mValue < 0 ? -cDefDeadZone : cDefDeadZone)) * (1.0f / (1.0f - cDefDeadZone));
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

  void AxisMapping::registerAssets(ResourceAssetRegistry& assets) {
    // Nothing to do.
  }

  bool AxisMapping::renderAssetIcon() const {
    return true;
  }

  void AxisMapping::saveAsset(JSONObject object) const {
    object.addInteger(JSON_AXIS, cDefAxis);
    object.addFloat(JSON_DEAD_ZONE, cDefDeadZone);
  }

  void AxisMapping::getAssetProperties(IPropertyMaker& owner) {
    // TODO: Implement this.
  }

  bool AxisMapping::isDefaultConfiguration() const {
    return cDefAxis == 0 && cDefDeadZone == 0.16f;
  }
}

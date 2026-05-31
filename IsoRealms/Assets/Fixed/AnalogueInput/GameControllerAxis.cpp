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
#include "GameControllerAxis.h"

#include "IsoRealms/Editing/Property/IPropertyMaker.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  GameControllerAxis::GameControllerAxis(const Metadata& metadata, IResourceData& owner) :
          cMetadata(metadata),
          cDefAxis(0),
          cDefDeadZone(0.16f) {
  }

  GameControllerAxis::GameControllerAxis(const Metadata& metadata, IResourceData& owner, JSONObject object) :
          GameControllerAxis(metadata, owner) {
    cDefAxis = object.getInteger(JSON_AXIS);
    cDefDeadZone = object.getFloat(JSON_DEAD_ZONE);
  }

  std::string GameControllerAxis::getName() const {
    return ""; // TODO: Implement this.
  }

  bool GameControllerAxis::matches(const sf::Event& event) const {
    return event.type == sf::Event::JoystickMoved && event.joystickMove.axis == cDefAxis;
  }

  float GameControllerAxis::getState(const sf::Event& event) const {
    float mValue = event.joystickMove.position / 100.0f;
    return std::abs(mValue) < cDefDeadZone ? 0 : (mValue - (mValue < 0 ? -cDefDeadZone : cDefDeadZone)) * (1.0f / (1.0f - cDefDeadZone));
  }

  std::string GameControllerAxis::getShortName() const {
    return "A" + Utils::toString(cDefAxis);
  }
    
  std::string GameControllerAxis::getLongName() const {
    return "Axis " + Utils::toString(cDefAxis);
  }
  
  void GameControllerAxis::loadCustomMapping(JSONObject object) {
    // Nothing to do.
  }

  void GameControllerAxis::registerAssets(ResourceAssetRegistry& assets) {
    // Nothing to do.
  }

  bool GameControllerAxis::renderAssetIcon() const {
    return false;
  }

  void GameControllerAxis::saveAsset(JSONObject object) const {
    object.addInteger(JSON_AXIS, cDefAxis);
    object.addFloat(JSON_DEAD_ZONE, cDefDeadZone);
  }

  void GameControllerAxis::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyNativeInteger(cMetadata.getPropertyData("Axis"),     [this]() {return cDefAxis;},     [this](int   axis)     {cDefAxis     = axis;});
    owner.createPropertyNativeFloat(  cMetadata.getPropertyData("DeadZone"), [this]() {return cDefDeadZone;}, [this](float deadZone) {cDefDeadZone = deadZone;});
  }

  bool GameControllerAxis::isDefaultConfiguration() const {
    return cDefAxis == 0 && cDefDeadZone == 0.16f;
  }
}

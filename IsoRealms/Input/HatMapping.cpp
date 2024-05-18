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
#include "HatMapping.h"

#include "IsoRealms/Utils.h"

namespace IsoRealms {
  const std::string HatMapping::JSON_DIRECTION = "direction";
  const std::string HatMapping::JSON_TYPE      = "type";

  const std::string HatMapping::TYPE_HAT = "Hat";

  const std::map<std::string, HatHandler::Direction> HatMapping::cDirectionsByName = {
    {"LeftUp",    HatHandler::Direction::HAT_LEFTUP},
    {"Up",        HatHandler::Direction::HAT_UP},
    {"RightUp",   HatHandler::Direction::HAT_RIGHTUP},
    {"Left",      HatHandler::Direction::HAT_LEFT},
    {"Centered",  HatHandler::Direction::HAT_CENTERED},
    {"Right",     HatHandler::Direction::HAT_RIGHT},
    {"LeftDown",  HatHandler::Direction::HAT_LEFTDOWN},
    {"Down",      HatHandler::Direction::HAT_DOWN},
    {"RightDown", HatHandler::Direction::HAT_RIGHTDOWN}
  };

  HatMapping::HatMapping(HatHandler& hatHandler, const HatHandler::Direction direction) :
          cHatHandler(hatHandler),
          cDirection(direction) {
  }

  HatMapping::HatMapping(HatHandler& hatHandler, JSONObject object) :
          cHatHandler(hatHandler),
          cDirection(getDirection(object.getString(JSON_DIRECTION))) {
  }

  bool HatMapping::matches(const sf::Event& event) const {
    if (event.type == sf::Event::JoystickMoved) {
      if (event.joystickMove.axis == sf::Joystick::Axis::PovX) {
        switch (cDirection) {
          case HatHandler::Direction::HAT_LEFT:  return cHatHandler.leftPressed()  || cHatHandler.leftReleased();
          case HatHandler::Direction::HAT_RIGHT: return cHatHandler.rightPressed() || cHatHandler.rightReleased();
          default: break;
        }
      } else if (event.joystickMove.axis == sf::Joystick::Axis::PovY) {
        switch (cDirection) {
          case HatHandler::Direction::HAT_UP:   return cHatHandler.upPressed()   || cHatHandler.upReleased();
          case HatHandler::Direction::HAT_DOWN: return cHatHandler.downPressed() || cHatHandler.downReleased();
          default: break;
        }
      }
    }
    return false;
  }

  bool HatMapping::getState(const sf::Event& event) const {
    switch (cDirection) {
      case HatHandler::Direction::HAT_LEFT:      return cHatHandler.leftPressed();
      case HatHandler::Direction::HAT_UP:        return cHatHandler.upPressed();
      case HatHandler::Direction::HAT_RIGHT:     return cHatHandler.rightPressed();
      case HatHandler::Direction::HAT_DOWN:      return cHatHandler.downPressed();
      case HatHandler::Direction::HAT_LEFTUP:    return cHatHandler.leftPressed()  && cHatHandler.upPressed();
      case HatHandler::Direction::HAT_LEFTDOWN:  return cHatHandler.leftPressed()  && cHatHandler.downPressed();
      case HatHandler::Direction::HAT_RIGHTUP:   return cHatHandler.rightPressed() && cHatHandler.upPressed();
      case HatHandler::Direction::HAT_RIGHTDOWN: return cHatHandler.rightPressed() && cHatHandler.downPressed();
      case HatHandler::Direction::HAT_CENTERED:  return true;
    }
    return false;
  }

  void HatMapping::save(JSONObject object) const {
    object.addString(JSON_TYPE, TYPE_HAT);
    object.addString(JSON_DIRECTION, getName(cDirection));
  }

  std::string HatMapping::getShortName() const {
    return getName(cDirection);
  }

  std::string HatMapping::getLongName() const {
    return "Direction " + getName(cDirection);
  }

  HatHandler::Direction HatMapping::getDirection(const std::string& name) {
    std::map<std::string, HatHandler::Direction>::const_iterator i = cDirectionsByName.find(name);
    if (i == cDirectionsByName.end()) {
      throw ArgumentException("ERROR: HatMapping::getDirection: Direction of name \"" + name + "\" not known.");
    }
    return i->second;
  }

  std::string HatMapping::getName(const HatHandler::Direction value) {
    for (std::map<std::string, HatHandler::Direction>::const_iterator i = cDirectionsByName.begin(); i != cDirectionsByName.end(); i++) {
      if (i->second == value) {
        return i->first;
      }
    }
    throw ArgumentException("ERROR: HatMapping::getName: Specified direction value is not known.");
  }
}

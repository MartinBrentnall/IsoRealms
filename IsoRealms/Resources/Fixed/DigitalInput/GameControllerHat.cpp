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
#include "GameControllerHat.h"

#include "IsoRealms/Editing.h"
#include "IsoRealms/Editing/Property/IComponentDefiner.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  GameControllerHat::DirectionChooser::DirectionChooser(const Metadata& metadata) :
          cMetadata(metadata) {
  }

  GameControllerHat::GameControllerHat(const Metadata& metadata, IComponentData& owner) :
          cMetadata(metadata),
          cHatHandler(owner.getProject().getApplication().getHatHandler()),
          cDirectionChooser(metadata),
          cDirection(HatHandler::Direction::HAT_RIGHT) {
  }

  GameControllerHat::GameControllerHat(const Metadata& metadata, IComponentData& owner, JSONObject object) :
          GameControllerHat(metadata, owner) {
    cDirection = getDirection(object.getString(JSON_DIRECTION));
  }

  bool GameControllerHat::matches(const sf::Event& event) const {
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

  bool GameControllerHat::getState(const sf::Event& event) const {
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

  std::string GameControllerHat::getShortName() const {
    return getName(cDirection);
  }

  std::string GameControllerHat::getLongName() const {
    return "Direction " + getName(cDirection);
  }

  std::string GameControllerHat::getLocalizedName() const {
    return cMetadata.getPropertyData(getName(cDirection)).getName();
  }

  HatHandler::Direction GameControllerHat::getDirection(const std::string& name) {
    std::map<std::string, HatHandler::Direction>::const_iterator i = cDirectionsByName.find(name);
    if (i == cDirectionsByName.end()) {
      throw ArgumentException("ERROR: HatMapping::getDirection: Direction of name \"" + name + "\" not known.");
    }
    return i->second;
  }

  std::string GameControllerHat::getName(const HatHandler::Direction value) {
    for (std::map<std::string, HatHandler::Direction>::const_iterator i = cDirectionsByName.begin(); i != cDirectionsByName.end(); i++) {
      if (i->second == value) {
        return i->first;
      }
    }
    throw ArgumentException("ERROR: HatMapping::getName: Specified direction value is not known.");
  }

  void GameControllerHat::saveResource(JSONObject object) const {
    object.addString(JSON_DIRECTION, getName(cDirection));
  }

  void GameControllerHat::getResourceProperties(IComponentDefiner& definer) {
    definer.propertyOptional(JSON_DIRECTION, cDirectionChooser, "", []() {
      return true;
    }, [this](const std::string& direction) {
      cDirection = getDirection(direction);
    }, [this]() {
      return getLocalizedName();
    });
  }

  void GameControllerHat::DirectionChooser::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    for (std::map<std::string, HatHandler::Direction>::const_iterator i = cDirectionsByName.begin(); i != cDirectionsByName.end(); i++) {
      getTreeItemInfoFunction(TreeItemInfo{i->first, cMetadata.getPropertyData(i->first).getName()});
    }
  }
}

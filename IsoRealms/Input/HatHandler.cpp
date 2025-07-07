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
#include "HatHandler.h"

namespace IsoRealms {
  HatHandler::HatHandler() :
            cUpPressed(false),
            cDownPressed(false),
            cLeftPressed(false),
            cRightPressed(false),
            cUpReleased(false),
            cDownReleased(false),
            cLeftReleased(false),
            cRightReleased(false),
            cPressedUp(false),
            cPressedDown(false),
            cPressedLeft(false),
            cPressedRight(false) {
  }

  void HatHandler::pressLeft(bool left) {
    if (cPressedLeft != left) {
      cPressedLeft = left;
      if (cPressedLeft) {
        cLeftPressed = true;
      } else {
        cLeftReleased = true;
      }
    }
  }

  void HatHandler::pressRight(bool right) {
    if (cPressedRight != right) {
      cPressedRight = right;
      if (cPressedRight) {
        cRightPressed = true;
      } else {
        cRightReleased = true;
      }
    }
  }

  void HatHandler::pressUp(bool up) {
    if (cPressedUp != up) {
      cPressedUp = up;
      if (cPressedUp) {
        cUpPressed = true;
      } else {
        cUpReleased = true;
      }
    }
  }

  void HatHandler::pressDown(bool down) {
    if (cPressedDown != down) {
      cPressedDown = down;
      if (cPressedDown) {
        cDownPressed = true;
      } else {
        cDownReleased = true;
      }
    }
  }

  void HatHandler::input(const sf::Event& event) {
    switch (event.type) {
      case sf::Event::JoystickMoved: {
        switch (event.joystickMove.axis) {
          case sf::Joystick::Axis::PovX: {
            cLeftPressed   = false;
            cLeftReleased  = false;
            cRightPressed  = false;
            cRightReleased = false;
            if      (event.joystickMove.position < 0) {pressLeft(true);  pressRight(false);}
            else if (event.joystickMove.position > 0) {pressLeft(false); pressRight(true);}
            else                                      {pressLeft(false); pressRight(false);}
            break;
          }

          case sf::Joystick::Axis::PovY: {
            cUpReleased    = false;
            cUpPressed     = false;
            cDownPressed   = false;
            cDownReleased  = false;
#if _WIN32
            if      (event.joystickMove.position > 0) {pressUp(true);  pressDown(false);}
            else if (event.joystickMove.position < 0) {pressUp(false); pressDown(true);}
#elif __linux__
            if      (event.joystickMove.position < 0) {pressUp(true);  pressDown(false);}
            else if (event.joystickMove.position > 0) {pressUp(false); pressDown(true);}
#endif
            else                                      {pressUp(false); pressDown(false);}
            break;
          }

          default: {
            cUpReleased    = false;
            cUpPressed     = false;
            cDownPressed   = false;
            cDownReleased  = false;
            cLeftPressed   = false;
            cLeftReleased  = false;
            cRightPressed  = false;
            cRightReleased = false;
            break;
          }
        }
        break;
      }

      default: break;
    }
  }

  bool HatHandler::upPressed() {
    return cUpPressed;
  }

  bool HatHandler::downPressed() {
    return cDownPressed;
  }

  bool HatHandler::leftPressed() {
    return cLeftPressed;
  }

  bool HatHandler::rightPressed() {
    return cRightPressed;
  }

  bool HatHandler::upReleased() {
    return cUpReleased;
  }

  bool HatHandler::downReleased() {
    return cDownReleased;
  }

  bool HatHandler::leftReleased() {
    return cLeftReleased;
  }

  bool HatHandler::rightReleased() {
    return cRightReleased;
  }

  bool HatHandler::matches(HatHandler::Direction direction) {
    switch (direction) {
      case HAT_LEFTUP:    return cLeftPressed  || cLeftReleased  || cUpPressed    || cUpReleased;
      case HAT_UP:        return cUpPressed    || cUpReleased;
      case HAT_RIGHTUP:   return cUpPressed    || cUpReleased    || cRightPressed || cRightReleased;
      case HAT_LEFT:      return cLeftPressed  || cLeftReleased;
      case HAT_RIGHT:     return cRightPressed || cRightReleased;
      case HAT_LEFTDOWN:  return cLeftPressed  || cLeftReleased  || cDownPressed  || cDownReleased;
      case HAT_DOWN:      return cDownPressed  || cDownReleased;
      case HAT_RIGHTDOWN: return cRightPressed || cRightReleased || cDownPressed  || cDownReleased;
      case HAT_CENTERED:  return true;
    }
    return false;
  }
}



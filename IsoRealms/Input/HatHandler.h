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
#pragma once

#include <iostream>

#include <SFML/Window/Event.hpp>

namespace IsoRealms {
  class HatHandler {
    private:
    bool cUpPressed = false;
    bool cDownPressed = false;
    bool cLeftPressed = false;
    bool cRightPressed = false;
    bool cUpReleased = false;
    bool cDownReleased = false;
    bool cLeftReleased = false;
    bool cRightReleased = false;

    bool cPressedUp = false;
    bool cPressedDown = false;
    bool cPressedLeft = false;
    bool cPressedRight = false;

    void pressUp(bool up);
    void pressDown(bool down);
    void pressLeft(bool left);
    void pressRight(bool right);

    public:
    enum Direction {
      HAT_LEFTUP,
      HAT_UP,
      HAT_RIGHTUP,
      HAT_LEFT,
      HAT_CENTERED,
      HAT_RIGHT,
      HAT_LEFTDOWN,
      HAT_DOWN,
      HAT_RIGHTDOWN
    };

    HatHandler();
    void reset();
    void input(const sf::Event& event);
    bool upPressed();
    bool downPressed();
    bool leftPressed();
    bool rightPressed();
    bool upReleased();
    bool downReleased();
    bool leftReleased();
    bool rightReleased();
    bool matches(Direction direction);
  };
}

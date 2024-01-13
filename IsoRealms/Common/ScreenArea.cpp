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
#include "ScreenArea.h"

namespace IsoRealms {
  ScreenArea::ScreenArea(const float left, const float right, const float bottom, const float top):
    cLeft(left),
    cRight(right),
    cBottom(bottom),
    cTop(top) {
  }
  
  float ScreenArea::getLeft() const {
    return cLeft;
  }
  
  float ScreenArea::getRight() const {
    return cRight;
  }
  
  float ScreenArea::getBottom() const {
    return cBottom;
  }
  
  float ScreenArea::getTop() const {
    return cTop;
  }
}


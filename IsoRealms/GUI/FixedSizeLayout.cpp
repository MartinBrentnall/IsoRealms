/*
 * Copyright 2016 Martin Brentnall
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
#include "FixedSizeLayout.h"

FixedSizeLayout::FixedSizeLayout(bool top, bool left, IComponentLocation* location, IComponentSizeCalculator* size) {
  cTop = top;
  cLeft = left;
  cLocation = location;
  cSize = size;
}

float FixedSizeLayout::getLeft() {
  return cLeft ? cLocation->getX() : cLocation->getX() - cSize->getWidth();
}

float FixedSizeLayout::getRight() {
  return cLeft ? cLocation->getX() + cSize->getWidth() : cLocation->getX();
}

float FixedSizeLayout::getTop() {
  return cTop ? cLocation->getY() : cLocation->getY() + cSize->getHeight();
}

float FixedSizeLayout::getBottom() {
  return cTop ? cLocation->getY() - cSize->getHeight() : cLocation->getY();
}


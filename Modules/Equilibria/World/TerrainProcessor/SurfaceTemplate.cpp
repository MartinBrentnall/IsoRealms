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
#include "SurfaceTemplate.h"

namespace IsoRealms::Equilibria {
  SurfaceTemplate::SurfaceTemplate(int north, int east, int south, int west, std::optional<Condition>& condition) {
    cNorth = north;
    cEast = east;
    cSouth = south;
    cWest = west;
    cCondition = condition;
  }

  int SurfaceTemplate::getNorth() {
    return cNorth;
  }

  int SurfaceTemplate::getEast() {
    return cEast;
  }

  int SurfaceTemplate::getSouth() {
    return cSouth;
  }

  int SurfaceTemplate::getWest() {
    return cWest;
  }

  std::optional<Condition>& SurfaceTemplate::getCondition() {
    return cCondition;
  }

  bool SurfaceTemplate::alligned(int x, int y) {
    return y >= cSouth && y <= cNorth && x >= cWest && x <= cEast;
  }
}

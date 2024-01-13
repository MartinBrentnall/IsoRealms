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
#include "CollisionData.h"

namespace IsoRealms {
  CollisionData::CollisionData() {
    cGradient = 0.0f;
  }

  CollisionData::CollisionData(LiteralVertex& location, float gradient) {
    cLocation = location;
    cGradient = gradient;
  }

  CollisionData::CollisionData(double x, double y, double z, float gradient):cLocation(x, y, z) {
    cGradient = gradient;
  }

  double CollisionData::getX() {
    return cLocation.getX();  
  }

  double CollisionData::getY() {
    return cLocation.getY();
  }

  double CollisionData::getZ() {
    return cLocation.getZ();
  }

  LiteralVertex CollisionData::getLocation() {
    return cLocation;
  }

  float CollisionData::getGradient() {
    return cGradient;
  }
}

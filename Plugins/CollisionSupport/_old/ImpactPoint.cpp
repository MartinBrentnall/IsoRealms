/*
 * Copyright 2009 Martin Brentnall
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
#include "ImpactPoint.h"

ImpactPoint::ImpactPoint() {
}

ImpactPoint::ImpactPoint(int type, float gradient, Coordinate location) {
  cType = type;
  cGradient = gradient;
  cLocation = location;
}

int ImpactPoint::getType() {
  return cType;
}

float ImpactPoint::getGradient() {
  return cGradient;
}

Coordinate ImpactPoint::getLocation() {
  return cLocation;
}

void ImpactPoint::setRelocationPoint(Coordinate& relocation) {
  cRelocation = relocation;
}

Coordinate ImpactPoint::getRelocation() {
  return cRelocation;
}




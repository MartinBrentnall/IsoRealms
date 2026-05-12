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
#include "CollisionData.h"

#include "Modules/Equilibria/World/Object/Terrain/Wall.h"

namespace IsoRealms::Equilibria {
  CollisionData::CollisionData(ISurface* surface, CollisionData::Type type, LiteralVertex location, float xSlope, float ySlope, float gradient) :
            cType(type),
            cThing(surface, xSlope, ySlope),
            cLocation(location),
            cGradient(gradient) {
  }

  CollisionData::CollisionData(Wall* wall, CollisionData::Type type, LiteralVertex location, float gradient) :
            cType(type),
            cThing(wall),
            cLocation(location),
            cGradient(gradient) {
  }

  ISurface* CollisionData::getSurface() {
    return cThing.cSurface.cSurface;
  }

  Wall* CollisionData::getWall() {
    return cThing.cWall;
  }

  CollisionData::Type CollisionData::getType() {
    return cType;
  }

  LiteralVertex CollisionData::getEventLocation() {
    return cLocation;
  }

  float CollisionData::getXSlope() {
    return cThing.cSurface.cXSlope;
  }

  float CollisionData::getYSlope() {
    return cThing.cSurface.cYSlope;
  }

  float CollisionData::getGradient() {
    return cGradient;
  }
}

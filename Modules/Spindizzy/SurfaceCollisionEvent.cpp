/*
 * Copyright 2015 Martin Brentnall
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
#include "SurfaceCollisionEvent.h"

SurfaceCollisionEvent::SurfaceCollisionEvent(IRollableSurface* surface, ICollisionData::CollisionType type, Vertex* location, float xSlope, float ySlope, float gradient) {
  cSurface = surface;
  cType = type;
  cLocation = location;
  cXSlope = xSlope;
  cYSlope = ySlope;
  cGradient = gradient;
}

SurfaceCollisionEvent::SurfaceCollisionEvent(ICollidableWallSurface* surface, ICollisionData::CollisionType type, Vertex* location, float gradient) {
  cWallSurface = surface;
  cType = type;
  cLocation = location;
  cGradient = gradient;
}

IRollableSurface* SurfaceCollisionEvent::getSurface() {
  return cSurface;
}

ICollidableWallSurface* SurfaceCollisionEvent::getWallSurface() {
  return cWallSurface;
}

ICollisionData::CollisionType SurfaceCollisionEvent::getType() {
  return cType;  
}

Vertex* SurfaceCollisionEvent::getEventLocation() {
  return cLocation;
}

float SurfaceCollisionEvent::getXSlope() {
  return cXSlope;
}

float SurfaceCollisionEvent::getYSlope() {
  return cYSlope;
}

float SurfaceCollisionEvent::getGradient() {
  return cGradient;
}


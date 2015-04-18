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
#include "LiftSurfaceCollisionEvent.h"

LiftSurfaceCollisionEvent::LiftSurfaceCollisionEvent(IRollableSurface* surface, ICollisionData::CollisionType type, Vertex* location, float gradient) {
  cSurface = surface;
  cType = type;
  cLocation = location;
  cGradient = gradient;
}

IRollableSurface* LiftSurfaceCollisionEvent::getSurface() {
  return cSurface;
}

ICollidableWallSurface* LiftSurfaceCollisionEvent::getWallSurface() {
  return NULL;
}

ICollisionData::CollisionType LiftSurfaceCollisionEvent::getType() {
  return cType;  
}

Vertex* LiftSurfaceCollisionEvent::getEventLocation() {
  return cLocation;
}

float LiftSurfaceCollisionEvent::getXSlope() {
  return 0.0f;
}

float LiftSurfaceCollisionEvent::getYSlope() {
  return 0.0f;
}

float LiftSurfaceCollisionEvent::getGradient() {
  return cGradient;
}





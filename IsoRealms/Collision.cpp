/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "Collision.h"

Vertex* Collision::getXPlaneIntersection(float location, Vertex& start, Vertex& end, float* gradient) {
  float mXMovement = end.x - start.x;
  float mTempGradient = (location - start.x) / mXMovement;
  bool mTowardsPositive = mXMovement >= 0.0f;
  if (mTowardsPositive ? (mTempGradient >= 0.0f && mTempGradient < 1.0f) : (mTempGradient > 0.0f && mTempGradient <= 1.0f)) {
    float mEventYLocation = start.y + (end.y - start.y) * mTempGradient;
    float mEventZLocation = start.z + (end.z - start.z) * mTempGradient;
    *gradient = mTempGradient;
    return new Vertex(location, mEventYLocation, mEventZLocation);
  }
  return NULL;
}

Vertex* Collision::getYPlaneIntersection(float location, Vertex& start, Vertex& end, float* gradient) {
  float mYMovement = end.y - start.y;
  float mTempGradient = (location - start.y) / mYMovement;
  bool mTowardsPositive = mYMovement >= 0.0f;
  if (mTowardsPositive ? (mTempGradient >= 0.0f && mTempGradient < 1.0f) : (mTempGradient > 0.0f && mTempGradient <= 1.0f)) {
    float mEventXLocation = start.x + (end.x - start.x) * mTempGradient;
    float mEventZLocation = start.z + (end.z - start.z) * mTempGradient;
    *gradient = mTempGradient;
    return new Vertex(mEventXLocation, location, mEventZLocation);
  }
  return NULL;
}

Vertex* Collision::getZPlaneIntersection(float location, Vertex& start, Vertex& end, float* gradient) {
  float mZMovement = end.z - start.z;
  float mTempGradient = (location - start.z) / mZMovement;
  bool mTowardsPositive = mZMovement >= 0.0f;
  if (mTowardsPositive ? (mTempGradient >= 0.0f && mTempGradient < 1.0f) : (mTempGradient > 0.0f && mTempGradient <= 1.0f)) {
    float mEventXLocation = start.x + (end.x - start.x) * mTempGradient;
    float mEventYLocation = start.y + (end.y - start.y) * mTempGradient;
    *gradient = mTempGradient;
    return new Vertex(mEventXLocation, mEventYLocation, location);
  }
  return NULL;
}










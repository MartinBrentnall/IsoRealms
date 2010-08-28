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
#include "ZoneEvent.h"

ZoneEvent::ZoneEvent(IZone* zone, Type type, CollisionVertex* collisionVertex) {
  cZone = zone;
  cType = type;
  cCollisionVertex = collisionVertex;
}

IZone* ZoneEvent::getZone() {
  return cZone;
}

ZoneEvent::Type ZoneEvent::getType() {
  return cType;
}

float ZoneEvent::getTime() {
  return cCollisionVertex->gradient;
}

Vertex* ZoneEvent::getLocation() {
  return NULL; // TODO: Implement this... although do we really need it?
}

bool ZoneEvent::operator==(const ZoneEvent& other) const {
  float mThisTime = cCollisionVertex->gradient;
  float mOtherTime = other.cCollisionVertex->gradient;
  return mThisTime == mOtherTime && cType == other.cType;
}

bool ZoneEvent::operator!=(const ZoneEvent& other) const {
  return !(*this == other);
}

bool ZoneEvent::operator<(const ZoneEvent& other) const {
  float mThisTime = cCollisionVertex->gradient;
  float mOtherTime = other.cCollisionVertex->gradient;
  if (cZone == other.cZone) {
    return mThisTime < mOtherTime || (mThisTime == mOtherTime && cType == ENTERED && other.cType == EXITED);
  } else {
    return mThisTime < mOtherTime || (mThisTime == mOtherTime && cType == EXITED && other.cType == ENTERED);
  }
}

bool ZoneEvent::operator>(const ZoneEvent& other) const {
  float mThisTime = cCollisionVertex->gradient;
  float mOtherTime = other.cCollisionVertex->gradient;
  if (cZone == other.cZone) {
    return mThisTime > mOtherTime || (mThisTime == mOtherTime && cType == EXITED && other.cType == ENTERED);
  } else {
    return mThisTime > mOtherTime || (mThisTime == mOtherTime && cType == ENTERED && other.cType == EXITED);
  }
}

bool ZoneEvent::operator>=(const ZoneEvent& other) const {
  return !(*this < other);
}

bool ZoneEvent::operator<=(const ZoneEvent& other) const {
  return !(*this > other);
}



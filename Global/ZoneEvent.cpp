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

ZoneEvent::ZoneEvent(IZone* zone, Type type, float time, Vertex* location) {
  cZone = zone;
  cType = type;
  cTime = time;
  cLocation = location;
}

IZone* ZoneEvent::getZone() {
  return cZone;
}

ZoneEvent::Type ZoneEvent::getType() {
  return cType;
}

float ZoneEvent::getTime() {
  return cTime;
}

Vertex* ZoneEvent::getLocation() {
  return cLocation;
}

bool ZoneEvent::operator==(const ZoneEvent& other) const {
  return cTime == other.cTime && cType == other.cType;
}

bool ZoneEvent::operator!=(const ZoneEvent& other) const {
  return !(*this == other);
}

bool ZoneEvent::operator<(const ZoneEvent& other) const {
  if (cZone == other.cZone) {
    return cTime < other.cTime || (cTime == other.cTime && cType == ENTERED && other.cType == EXITED);
  } else {
    return cTime < other.cTime || (cTime == other.cTime && cType == EXITED && other.cType == ENTERED);
  }
}

bool ZoneEvent::operator>(const ZoneEvent& other) const {
  if (cZone == other.cZone) {
    return cTime > other.cTime || (cTime == other.cTime && cType == EXITED && other.cType == ENTERED);
  } else {
    return cTime > other.cTime || (cTime == other.cTime && cType == ENTERED && other.cType == EXITED);
  }
}

bool ZoneEvent::operator>=(const ZoneEvent& other) const {
  return !(*this < other);
}

bool ZoneEvent::operator<=(const ZoneEvent& other) const {
  return !(*this > other);
}



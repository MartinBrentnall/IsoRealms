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



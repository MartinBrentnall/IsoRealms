#include "ZoneContextDummy.h"

void ZoneContextDummy::setZoneContext(IZone*) {
  // Nothing to do.
}

IZone* ZoneContextDummy::getZoneContext() {
  return NULL;
}

void ZoneContextDummy::addZoneContextListener(IZoneContextListener*) {
  // Nothing to do.
}

void ZoneContextDummy::removeZoneContextListener(IZoneContextListener*) {
  // Nothing to do.
}

extern "C" IPlugin* create() {
  return new ZoneContextDummy();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

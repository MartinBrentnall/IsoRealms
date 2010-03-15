#include "LocationAwarenessDummy.h"

void LocationAwarenessDummy::setLocation(Vertex*) {
  // Nothing to do.
}

Vertex* LocationAwarenessDummy::getLocation() {
  return &cDummyLocation;
}

extern "C" IPlugin* create() {
  return new LocationAwarenessDummy();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

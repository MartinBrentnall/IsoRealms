#include "FixedLocationAwareness.h"

void FixedLocationAwareness::setLocation(Vertex* location) {
  cLocation = location;
}

Vertex* FixedLocationAwareness::getLocation() {
  return cLocation;
}

std::string FixedLocationAwareness::getName() {
  return "Fixed Location Awaraness";
}

extern "C" IPlugin* create() {
  return new FixedLocationAwareness();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

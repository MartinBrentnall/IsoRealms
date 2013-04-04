#include "ZoneHandlerRegistry.h"

IZoneHandler* ZoneHandlerRegistry::getSpecialResource(const std::string& value) {
  return value == "!default" ? &DEFAULT_ZONE_HANDLER : NULL;
}

std::string ZoneHandlerRegistry::getSpecialLocation(IZoneHandler* zoneHandler) {
  if (zoneHandler == &DEFAULT_ZONE_HANDLER) {
    return "!default";
  }
  return "";
}

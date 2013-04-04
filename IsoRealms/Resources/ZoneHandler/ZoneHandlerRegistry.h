#ifndef ZONE_HANDLER_REGISTRY_H
#define ZONE_HANDLER_REGISTRY_H

#include <IsoRealms/IZone.h>
#include <IsoRealms/Resources/ResourceRegistry.h>

#include "DefaultZoneHandler.h"
#include "IZoneHandler.h"

class ZoneHandlerRegistry:public ResourceRegistry<IZoneHandler> {
  private:
  DefaultZoneHandler DEFAULT_ZONE_HANDLER;
  
  public:
  IZoneHandler* getSpecialResource(const std::string&);
  std::string getSpecialLocation(IZoneHandler* resource);
};

#endif

#ifndef I_ZONE_CONTEXT_LISTENER_H
#define I_ZONE_CONTEXT_LISTENER_H

#include "IMap.h"
#include "IZone.h"

class IZoneContextListener {
  public:
  virtual void zoneContextChanged(IMap*, IZone*) = 0;
};

#endif

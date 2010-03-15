#ifndef I_ZONE_CONTEXT_H
#define I_ZONE_CONTEXT_H

#include "../../Global/IPlugin.h"
#include "../../Global/IZone.h"

#include "IZoneContextListener.h"

class IZoneContext:public virtual IPlugin {
  public:
  virtual void setZoneContext(IZone*) = 0;
  virtual IZone* getZoneContext() = 0;
  virtual void addZoneContextListener(IZoneContextListener*) = 0;
  virtual void removeZoneContextListener(IZoneContextListener*) = 0;
};

#endif

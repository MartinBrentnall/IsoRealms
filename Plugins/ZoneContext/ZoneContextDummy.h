#ifndef ZONE_CONTEXT_DUMMY_H
#define ZONE_CONTEXT_DUMMY_H

#include "../../Global/DummyPlugin.h"

#include "IZoneContext.h"

class ZoneContextDummy:public IZoneContext,
                       public DummyPlugin {
  public:

  /***************************\
   * Implements IZoneContext *
  \***************************/
  void setZoneContext(IZone*);
  IZone* getZoneContext();
  void addZoneContextListener(IZoneContextListener*);
  void removeZoneContextListener(IZoneContextListener*);
};

#endif

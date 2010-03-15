#ifndef I_ZONE_H
#define I_ZONE_H

#include <vector>

#include "Vertex.h"
#include "ZoneEvent.h"

class IZone {
  public:

  virtual bool contains(Vertex&) = 0;

  /**
   *
   */
  virtual std::vector<ZoneEvent*> getZoneEvents(Vertex&, Vertex&) = 0;

  /**
   * TODO: THIS SHOULD NOT BE PART OF THIS INTERFACE!
   */ 
  virtual bool initZone() = 0;
};

#endif

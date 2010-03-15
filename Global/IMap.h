#ifndef I_MAP_H
#define I_MAP_H

#include <vector>

#include "BlockLocation.h"
#include "Vertex.h"
#include "IPluginRegistry.h"
#include "IZone.h"
#include "ZoneEvent.h"

class IMap {
  public:

  virtual IZone* getZone(BlockLocation&) = 0;

  /**
   * Return the zone at the specified vertex location.
   * 
   * @param Vertex&  The location.
   * @return  The zone at the location.
   */
  virtual IZone* getZone(Vertex&) = 0;

  /**
   * Return a list of zone events triggered by a point moving from the first
   * specified location to the second.
   *
   * @param Vertex&  The starting location.
   * @param Vertex&  The ending location.
   * @return  A list of ZoneEvents in order of occurrence.
   */
  virtual std::vector<ZoneEvent*> getZoneEvents(Vertex&, Vertex&) = 0;

  virtual void zoneChanged(IZone*) = 0;

  virtual IPluginRegistry* getPluginRegistry() = 0;
};

#endif

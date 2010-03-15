#ifndef LOCATION_AWARENESS_DUMMY_H
#define LOCATION_AWARENESS_DUMMY_H

#include "../../Global/DummyPlugin.h"

#include "ILocationAwareness.h"

class LocationAwarenessDummy:public ILocationAwareness,
                             public DummyPlugin {
  private:
  Vertex cDummyLocation;

  public:

  /*********************************\
   * Implements ILocationAwareness *
  \*********************************/
  void setLocation(Vertex*);
  Vertex* getLocation();
};

#endif

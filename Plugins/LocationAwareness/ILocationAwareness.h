#ifndef I_LOCATION_AWARENESS_H
#define I_LOCATION_AWARENESS_H

#include "../../Global/IPlugin.h"
#include "../../Global/Vertex.h"

class ILocationAwareness:public virtual IPlugin {
  public:
  virtual void setLocation(Vertex*) = 0;
  virtual Vertex* getLocation() = 0;
};

#endif

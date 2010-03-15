#ifndef I_COLLECTABLES_H
#define I_COLLECTABLES_H

#include "../../Global/IPlugin.h"
#include "../../Global/Vertex.h"

#include "ICollectable.h"
#include "ICollector.h"

class ICollectables:public virtual IPlugin {
  public:
  virtual void registerCollectable(ICollectable*) = 0;
  virtual void collect(ICollector*, Vertex&, Vertex&) = 0;
  virtual void reinitialise() = 0;
};

#endif

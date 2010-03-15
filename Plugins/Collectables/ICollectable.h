#ifndef I_COLLECTABLE_H
#define I_COLLECTABLE_H

#include "../../Global/Vertex.h"

class ICollectable {
  public:
  virtual bool isCollected(Vertex&, Vertex&) = 0;
  virtual void setDirty() = 0;
};

#endif

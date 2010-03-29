#ifndef I_ROLLABLE_SURFACE_H
#define I_ROLLABLE_SURFACE_H

#include "../../Global/Vertex.h"

#include "ICollisionData.h"

class IRollableSurface {
  public:
  virtual ICollisionData* getCollision(Vertex&, Vertex&) = 0;
  virtual ICollisionData* getRollingEvent(Vertex&, Vertex&) = 0;
  virtual float getHeightAt(float, float) = 0;
  virtual bool contains(Vertex&) = 0;
};

#endif

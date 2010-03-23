#ifndef I_ROLLABLE_SURFACE_H
#define I_ROLLABLE_SURFACE_H

#include "../../Global/Vertex.h"

#include "ICollisionData.h"

class IRollableSurface {
  public:
  ICollisionData getCollision(Vertex&, Vertex&);
};

#endif

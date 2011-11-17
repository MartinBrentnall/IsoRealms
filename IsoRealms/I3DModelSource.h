#ifndef I_3D_MODEL_SOURCE_H
#define I_3D_MODEL_SOURCE_H

#include <string>

#include "DOMNodeWrapper.h"
#include "I3DModel.h"
#include "Vertex.h"

class I3DModelSource {
  public:
  virtual I3DModel* getModel(DOMNodeWrapper*, Vertex*) = 0;
  virtual I3DModel* getModel(const std::string&, Vertex*) = 0;
};

#endif

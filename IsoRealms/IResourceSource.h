#ifndef I_RESOURCE_SOURCE_H
#define I_RESOURCE_SOURCE_H

#include "Persistence/DOMNodeWrapper.h"

class IRuntimeContext;

class IResourceSource {
  public:
  virtual void createResources(DOMNodeWrapper*, IRuntimeContext*) {};
};

#endif

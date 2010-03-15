#ifndef I_COLLECTOR_H
#define I_COLLECTOR_H

#include "ICollectable.h"

class ICollector {
  public:
  virtual void collected(ICollectable*) = 0;
};

#endif

#ifndef I_RUNTIME_CONTEXT_H
#define I_RUNTIME_CONTEXT_H

#include "ICommandRegistry.h"
#include "IMap.h"

class IRuntimeContext {
  public:
  virtual IMap* getMap() = 0;
  virtual ICommandRegistry* getCommandRegistry() = 0;
};

#endif

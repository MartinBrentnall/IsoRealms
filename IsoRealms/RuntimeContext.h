#ifndef RUNTIME_CONTEXT_H
#define RUNTIME_CONTEXT_H

#include "IRuntimeContext.h"

class RuntimeContext:public IRuntimeContext {
  private:
  IMap* cMap;
  ICommandRegistry* cCommandRegistry;
  
  public:
  RuntimeContext(IMap*, ICommandRegistry*);

  /******************************\
   * Implements IRuntimeContext *
  \******************************/
  IMap* getMap();
  ICommandRegistry* getCommandRegistry();
};

#endif

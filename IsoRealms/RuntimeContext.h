#ifndef RUNTIME_CONTEXT_H
#define RUNTIME_CONTEXT_H

#include "IRuntimeContext.h"

class RuntimeContext:public IRuntimeContext {
  private:
  IMap* cMap;
  ICommandRegistry* cCommandRegistry;
  bool cEditing;
  
  public:
  RuntimeContext(IMap*, ICommandRegistry*, bool);

  /******************************\
   * Implements IRuntimeContext *
  \******************************/
  IMap* getMap();
  ICommandRegistry* getCommandRegistry();
  bool isEditing();
};

#endif

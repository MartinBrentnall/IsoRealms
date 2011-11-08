#ifndef I_SCRIPT_SOURCE_H
#define I_SCRIPT_SOURCE_H

#include "DOMNodeWrapper.h"
#include "Script.h"

class IScriptSource {
  public:
  virtual Script* getScript(DOMNodeWrapper*) = 0;
};

#endif

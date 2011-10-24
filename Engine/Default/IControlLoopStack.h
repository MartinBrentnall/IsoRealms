#ifndef I_CONTROL_LOOP_STACK_H
#define I_CONTROL_LOOP_STACK_H

#include <IsoRealms/DOMNodeWrapper.h>

class IControlLoopStack {
  public:
  virtual void pushControlLoop(DOMNodeWrapper*) = 0;
  virtual void popControlLoop() = 0;
};

#endif

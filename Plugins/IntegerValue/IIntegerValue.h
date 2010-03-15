#ifndef I_INTEGER_VALUE_H
#define I_INTEGER_VALUE_H

#include "../../Global/IPlugin.h"

#include "IIntegerValueListener.h"

class IIntegerValue:public virtual IPlugin {
  public:
  virtual IIntegerValue& operator+=(const int&) = 0;
  virtual void addIntegerValueListener(IIntegerValueListener*) = 0;
  virtual void removeIntegerValueListener(IIntegerValueListener*) = 0;
};

#endif

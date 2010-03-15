#ifndef DUMMY_INTEGER_VALUE_H
#define DUMMY_INTEGER_VALUE_H

#include "../../Global/DummyPlugin.h"

#include "IIntegerValue.h"

class DummyIntegerValue:public DummyPlugin,
                        public IIntegerValue {
  public:
  IIntegerValue& operator+=(const int&);
  void addIntegerValueListener(IIntegerValueListener*);
  void removeIntegerValueListener(IIntegerValueListener*);
};

#endif

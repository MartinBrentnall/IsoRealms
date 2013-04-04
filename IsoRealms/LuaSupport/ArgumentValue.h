#ifndef ARGUMENT_VALUE_H
#define ARGUMENT_VALUE_H

#include "IArgumentValue.h"

template <class T> class ArgumentValue:public IArgumentValue {
  private:
  T* cValue;
  
  public:
  void setValue(T* value) {
    cValue = value;
  }
  
  T* getValue() {
    return cValue;
  }
};

#endif

#include "IntegerProxy.h"

IntegerProxy::IntegerProxy() {
  cInteger = NULL;
}

void IntegerProxy::setImplementation(IInteger* integer) {
  cInteger = integer;
}

void IntegerProxy::setValue(int value) {
  if (cInteger != NULL) {
    cInteger->setValue(value);
  } else {
    std::cout << "CAN'T SET INTEGER!  NOT CONNECTED!" << std::endl;
  }
}

int IntegerProxy::getValue() {
  return cInteger != NULL ? cInteger->getValue() : 0;
}



#ifndef INTEGER_PROXY_H
#define INTEGER_PROXY_H

#include <iostream>

#include "IInteger.h"

class IntegerProxy:public IInteger {
  private:
  IInteger* cInteger;

  public:
  IntegerProxy();

  void setImplementation(IInteger*);

  /***********************\
   * Implements IInteger *
  \***********************/
  void setValue(int);
  int getValue();
};

#endif

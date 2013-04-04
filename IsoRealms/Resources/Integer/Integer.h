#ifndef INTEGER_H
#define INTEGER_H

#include "IInteger.h"

class Integer:public IInteger {
  private:
  int cValue;
  
  public:
  Integer(int);
    
  /***********************\
   * Implements IInteger *
  \***********************/
  void setValue(int);
  int getValue();

  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
};

#endif

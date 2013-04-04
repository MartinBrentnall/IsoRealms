#ifndef FLOAT_H
#define FLOAT_H

#include "IFloat.h"

class Float:public IFloat {
  private:
  float cValue;
  
  public:
  Float(float);
    
  /*********************\
   * Implements IFloat *
  \*********************/
  void setValue(float);
  float getValue();

  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
};

#endif

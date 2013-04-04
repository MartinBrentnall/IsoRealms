#ifndef I_ARGUMENT_GENERATOR_H
#define I_ARGUMENT_GENERATOR_H

#include "../Persistence/DOMNodeWrapper.h"
#include "IArgumentSource.h"

class IArgumentGenerator {
  
  public:
  virtual IArgumentSource* getArgument(DOMNodeWrapper*) = 0;
};

#endif

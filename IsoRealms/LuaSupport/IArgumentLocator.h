#ifndef I_ARGUMENT_LOCATOR_H
#define I_ARGUMENT_LOCATOR_H

#include <string>

#include "IArgumentValue.h"

class IArgumentLocator {
  public:
  virtual std::string getPath(IArgumentValue*) = 0;
};

#endif

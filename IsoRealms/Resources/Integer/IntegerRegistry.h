#ifndef INTEGER_REGISTRY_H
#define INTEGER_REGISTRY_H

#include <IsoRealms/Resources/ResourceRegistry.h>

#include "Integer.h"

class IntegerRegistry:public ResourceRegistry<IInteger> {
  private:
  std::vector<Integer*> cLiteralIntegers;
  
  public:
  IInteger* getSpecialResource(const std::string&);
  std::string getSpecialLocation(IInteger*);
};

#endif

#ifndef STRING_REGISTRY_H
#define STRING_REGISTRY_H

#include <IsoRealms/Resources/ResourceRegistry.h>

#include "IntegerToString.h"
#include "IString.h"

class StringRegistry:public ResourceRegistry<IString> {
  private:
  std::vector<IntegerToString*> cIntegerStrings;
  ResourceRegistry<IInteger>* cIntegers;
  
  public:
  void setIntegers(ResourceRegistry<IInteger>*);
  IString* getSpecialResource(const std::string&);
  std::string getSpecialLocation(IString*);
};

#endif

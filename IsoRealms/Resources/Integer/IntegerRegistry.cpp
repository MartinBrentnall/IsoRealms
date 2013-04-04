#include "IntegerRegistry.h"

std::string IntegerRegistry::getSpecialLocation(IInteger* integer) {
  for (unsigned int i = 0; i < cLiteralIntegers.size(); i++) {
    if (cLiteralIntegers[i] == integer) {
      return "#" + Utils::toString(integer->getValue());
    }
  }
  return "";
}

IInteger* IntegerRegistry::getSpecialResource(const std::string& value) {
  if (value[0] == '#') {
    std::string mValue = value.substr(1);
    int mInteger = atoi(mValue.c_str());
    Integer* mIntegerObj = new Integer(mInteger);
    cLiteralIntegers.push_back(mIntegerObj);
    return mIntegerObj;
  }
  return NULL;
}

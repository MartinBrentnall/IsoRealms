#include "StringRegistry.h"

void StringRegistry::setIntegers(ResourceRegistry<IInteger>* integerRegistry) {
  cIntegers = integerRegistry;
}

IString* StringRegistry::getSpecialResource(const std::string& path) {
  if (path[0] == ':') {
    std::size_t mSplit = path.find('/');
    std::string mType = path.substr(1, mSplit - 1);
    if (mType == "Integer") {
      std::string mIntegerPath = path.substr(mSplit + 1);
      IInteger* mInteger = cIntegers->get(mIntegerPath);
      IntegerToString* mIntegerString = new IntegerToString(mInteger);
      cIntegerStrings.push_back(mIntegerString);
      return mIntegerString;
    } else {
      std::cout << "Unknown type \"" << mType << "\"" << std::endl;
      exit(1);
    }
  }
  return NULL;
}

std::string StringRegistry::getSpecialLocation(IString* string) {
  for (unsigned int i = 0; i < cIntegerStrings.size(); i++) {
    if (string == cIntegerStrings[i]) {
      return ":Integer/" + cIntegers->getLocation(cIntegerStrings[i]->getInteger());
    }
  }
  return ""; 
}


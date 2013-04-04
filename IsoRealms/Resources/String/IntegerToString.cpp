#include "IntegerToString.h"

IntegerToString::IntegerToString(IInteger* value) {
  cValue = value;
}

IInteger* IntegerToString::getInteger() {
  return cValue;
}

std::string IntegerToString::getValue() {
  return Utils::toString(cValue->getValue());
}

void IntegerToString::setValue(const std::string&) {
  // TODO: Try to convert value to int?
  std::cout << "WARNING: Trying to set integer with string value!" << std::endl;
  exit(1);
}

void IntegerToString::initialiseResource(DOMNodeWrapper*, IResourceAccessor*) {
  std::cout << "WARNING: Trying to initialise wrapper!" << std::endl;
  exit(1);
}

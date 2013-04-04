#include "Integer.h"

Integer::Integer(int value) {
  setValue(value);
}

void Integer::setValue(int value) {
  cValue = value;
}

int Integer::getValue() {
  return cValue;
}

void Integer::initialiseResource(DOMNodeWrapper*, IResourceAccessor*) {
}


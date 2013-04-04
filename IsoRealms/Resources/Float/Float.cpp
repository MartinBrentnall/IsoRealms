#include "Float.h"

Float::Float(float value) {
  setValue(value);
}

void Float::setValue(float value) {
  cValue = value;
}

float Float::getValue() {
  return cValue;
}

void Float::initialiseResource(DOMNodeWrapper*, IResourceAccessor*) {
}


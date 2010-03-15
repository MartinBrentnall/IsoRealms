#include "DummyIntegerValue.h"

IIntegerValue& DummyIntegerValue::operator+=(const int&) {
  return *this;
}

void DummyIntegerValue::addIntegerValueListener(IIntegerValueListener*) {
  // Nothing to do.
}

void DummyIntegerValue::removeIntegerValueListener(IIntegerValueListener*) {
  // Nothing to do.
}

extern "C" IPlugin* create() {
  return new DummyIntegerValue();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

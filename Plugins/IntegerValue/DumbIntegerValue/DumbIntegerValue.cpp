#include "DumbIntegerValue.h"

IIntegerValue& DumbIntegerValue::operator+=(const int& value) {
  cValue += value;
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->valueChanged(cValue);
  }
  return *this;
}

void DumbIntegerValue::addIntegerValueListener(IIntegerValueListener* listener) {
  cListeners.push_back(listener);
}

void DumbIntegerValue::removeIntegerValueListener(IIntegerValueListener* listener) {
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    if (cListeners[i] == listener) {
      cListeners.erase(cListeners.begin() + i);
      return;
    }
  }
}

std::string DumbIntegerValue::getName() {
  return "Dumb Integer Value";
}

extern "C" IPlugin* create() {
  return new DumbIntegerValue();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

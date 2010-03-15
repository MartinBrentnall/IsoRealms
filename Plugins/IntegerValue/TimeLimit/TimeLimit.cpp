#include "TimeLimit.h"

IIntegerValue& TimeLimit::operator+=(const int& value) {
  cMilliseconds += value;
  // TODO: Maximum limit
  return *this;
}

void TimeLimit::addIntegerValueListener(IIntegerValueListener*) {
  // Nothing to do.  Listeners not supported.
}

void TimeLimit::removeIntegerValueListener(IIntegerValueListener*) {
  // Nothing to do.  Listeners not supported.
}

// TODO: Count down

std::string TimeLimit::getName() {
  return "Time Limit";
}

extern "C" IPlugin* create() {
  return new TimeLimit();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

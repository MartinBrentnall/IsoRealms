#include "PluginSupportException.h"

PluginSupportException::PluginSupportException(const std::string& message) {
  cMessage = message;
}

std::string& PluginSupportException::getMessage() {
  return cMessage;
}



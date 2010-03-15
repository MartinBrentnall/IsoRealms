#include "DummyCommandRegistry.h"

void DummyCommandRegistry::registerCommand(IUserCommand* command) {
  // Nothing to do.
}

void DummyCommandRegistry::unregisterCommand(IUserCommand* command) {
  // Nothing to do.
}

IUserCommand* DummyCommandRegistry::getCommand(const std::string& name) {
  return NULL;
}

extern "C" IPlugin* create() {
  return new DummyCommandRegistry();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

#include "DefaultCommandRegistry.h"

void DefaultCommandRegistry::registerCommand(IUserCommand* command) {
  // TODO: Shouldn't permit multiple commands of same name.
  cCommands.push_back(command);
}

void DefaultCommandRegistry::unregisterCommand(IUserCommand* command) {
  for (unsigned int i = 0; i < cCommands.size(); i++) {
    if (cCommands[i] == command) {
      cCommands.erase(cCommands.begin() + i);
      return;
    }
  }
  // TODO: Throw
}

IUserCommand* DefaultCommandRegistry::getCommand(const std::string& commandName) {
  for (unsigned int i = 0; i < cCommands.size(); i++) {
    if (cCommands[i]->getCommandName() == commandName) {
      return cCommands[i];
    }
  }
  return NULL;
}

std::string DefaultCommandRegistry::getName() {
  return "Default Command Registry";
}

extern "C" IPlugin* create() {
  return new DefaultCommandRegistry();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

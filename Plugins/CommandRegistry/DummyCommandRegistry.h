#ifndef DUMMY_COMMAND_REGISTRY_H
#define DUMMY_COMMAND_REGISTRY_H

#include "../../Global/DummyPlugin.h"

#include "ICommandRegistry.h"

class DummyCommandRegistry:public ICommandRegistry,
                           public DummyPlugin {
  public:
  void registerCommand(IUserCommand*);
  void unregisterCommand(IUserCommand*);
  IUserCommand* getCommand(const std::string&);
};

#endif

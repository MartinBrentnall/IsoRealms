#ifndef I_COMMAND_REGISTRY_H
#define I_COMMAND_REGISTRY_H

#include <string>

#include "../../Global/IPlugin.h"

#include "IUserCommand.h"

class ICommandRegistry:public virtual IPlugin {
  public:
  virtual void registerCommand(IUserCommand*) = 0;
  virtual void unregisterCommand(IUserCommand*) = 0;
  virtual IUserCommand* getCommand(const std::string&) = 0;
};

#endif

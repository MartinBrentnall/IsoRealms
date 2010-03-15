#ifndef I_INTEGER_COMMAND_REGISTRY_H
#define I_INTEGER_COMMAND_REGISTRY_H

#include "AddIntegerCommand.h"

class IIntegerCommandRegistry {
  public:
  virtual void addCommand(AddIntegerCommand*) = 0;
  virtual void removeCommand(AddIntegerCommand*) = 0;
  virtual std::vector<AddIntegerCommand*> getCommands() = 0;
};

#endif

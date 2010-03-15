#ifndef DEFAULT_COMMAND_REGISTRY_H
#define DEFAULT_COMMAND_REGISTRY_H

#include <vector>

#include "../ICommandRegistry.h"

class DefaultCommandRegistry:public ICommandRegistry {
  private:
  std::vector<IUserCommand*> cCommands;

  public:

  /*******************************\
   * Implements ICommandRegistry *
  \*******************************/
  void registerCommand(IUserCommand*);
  void unregisterCommand(IUserCommand*);
  IUserCommand* getCommand(const std::string&);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();
};

#endif

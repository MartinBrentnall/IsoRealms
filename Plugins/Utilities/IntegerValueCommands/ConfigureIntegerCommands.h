#ifndef CONFIGURE_INTEGER_COMMANDS_H
#define CONFIGURE_INTEGER_COMMANDS_H

#include "../../../Global/ICommand.h"
#include "../../../Global/IComponentContainer.h"

#include "IIntegerCommandRegistry.h"
#include "IntegerCommandDialog.h"

class ConfigureIntegerCommands:public ICommand {
  private:
  IComponentContainer* cComponentContainer;
  IIntegerCommandRegistry* cIntegerCommandRegistry;

  public:
  ConfigureIntegerCommands(IIntegerCommandRegistry*);

  void setComponentContainer(IComponentContainer*);

  /***********************\
   * Implements ICommand *
  \***********************/
  void execute();
};

#endif

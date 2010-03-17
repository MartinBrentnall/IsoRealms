#ifndef INTEGER_VALUE_COMMANDS_H
#define INTEGER_VALUE_COMMANDS_H

#include "../../../Global/DefaultCommandInfo.h"

#include "../../CommandRegistry/ICommandRegistry.h"
#include "../../IntegerValue/IIntegerValue.h"

#include "../IUtilities.h"

#include "AddIntegerCommand.h"
#include "ConfigureIntegerCommands.h"
#include "IIntegerCommandRegistry.h"

class IntegerValueCommands:public IUtilities,
                           public IIntegerCommandRegistry {
  private:
  std::vector<PlugSocket*> cSockets;
  IIntegerValue* cVariable;
  ICommandRegistry* cCommandRegistry;
  std::vector<AddIntegerCommand*> cCommands;
  ConfigureIntegerCommands* cConfigureIntegerCommands;
  std::vector<ICommandInfo*> cPluginCommands;

  public:
  IntegerValueCommands();

  /**************************************\
   * Implements IIntegerCommandRegistry *
  \**************************************/
  void addCommand(AddIntegerCommand*);
  void removeCommand(AddIntegerCommand*);
  std::vector<AddIntegerCommand*> getCommands();

  /**********************\
   * Implements IPlugin *
  \**********************/
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);
  std::vector<ICommandInfo*> getCommandInfo();
  void setEditingContext(BlockLocation*, IComponentContainer*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
};

#endif

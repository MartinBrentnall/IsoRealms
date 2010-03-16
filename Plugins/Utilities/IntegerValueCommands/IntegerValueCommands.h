#ifndef INTEGER_VALUE_COMMANDS_H
#define INTEGER_VALUE_COMMANDS_H

#include "../../CommandRegistry/ICommandRegistry.h"
#include "../../IntegerValue/IIntegerValue.h"

#include "../IUtilities.h"

#include "AddIntegerCommand.h"
#include "IIntegerCommandRegistry.h"

class IntegerValueCommands:public IUtilities,
                           public IIntegerCommandRegistry {
  private:
  std::vector<PlugSocket*> cSockets;
  IIntegerValue* cVariable;
  ICommandRegistry* cCommandRegistry;
  std::vector<AddIntegerCommand*> cCommands;

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

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
};

#endif

/*
 * Copyright 2009,2010 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef INTEGER_VALUE_COMMANDS_H
#define INTEGER_VALUE_COMMANDS_H

#include <IsoRealms/DefaultCommandInfo.h>
#include <IsoRealms/ICommandRegistry.h>
#include <IsoRealms/OpenDialogCommand.h>

#include "../../IntegerValue/IIntegerValue.h"

#include "../IUtilities.h"

#include "AddIntegerCommand.h"
#include "IIntegerCommandRegistry.h"
#include "IntegerCommandDialog.h"

class IntegerValueCommands:public IUtilities,
                           public IIntegerCommandRegistry,
                           public IComponentSource {
  private:
  std::vector<PlugSocket*> cSockets;
  IIntegerValue* cVariable;
  ICommandRegistry* cCommandRegistry;
  std::vector<AddIntegerCommand*> cCommands;
  IComponentContainer* cComponentContainer;

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
  void setRuntimeContext(IRuntimeContext*);
  void setEditingContext(IEditingContext*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
  
  /*******************************\
   * Implements IComponentSource *
  \*******************************/
  IHUDComponent* createComponent();
};

#endif

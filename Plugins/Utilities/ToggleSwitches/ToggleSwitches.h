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
#ifndef TOGGLE_SWITCHES_H
#define TOGGLE_SWITCHES_H

#include <vector>

#include <IsoRealms/ICommandRegistry.h>

#include "../IUtilities.h"

class ToggleSwitches:public IUtilities {
  private:
  class SwitchCommand:public IUserCommand {
    private:
    std::string cName;
    bool cState;
    Script* cOnScript;
    Script* cOffScript;
      
    public:
    SwitchCommand(const std::string&, Script*, Script*);
    
    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getCommandName();
  };
    
  ICommandRegistry* cCommandRegistry;
  std::vector<PlugSocket*> cCommandRegistrySocket;
  std::vector<IUserCommand*> cSwitchCommands;
  
  SwitchCommand* createSwitchCommand(DOMNodeWrapper*);

  public:

  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);
  void setEditingContext(BlockLocation*, IComponentContainer*, ICommandRegistry*);
};

#endif

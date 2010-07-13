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
#ifndef RANDOM_COMMAND_H
#define RANDOM_COMMAND_H

#include "../../CommandRegistry/ICommandRegistry.h"
#include "../../CommandRegistry/IUserCommand.h"

#include "../IUtilities.h"

class RandomCommand:public IUtilities {
  private:
  class ARandomCommand:public IUserCommand {
    private:
    std::string cName;
    std::vector<IUserCommand*> cCommands;
    
    public:
    ARandomCommand(const std::string&, std::vector<IUserCommand*>);
    
    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getCommandName();
  };
    
  ICommandRegistry* cCommandRegistry;
  std::vector<PlugSocket*> cCommandRegistrySocket;
  std::vector<IUserCommand*> cRandomCommands;
  
  std::vector<IUserCommand*> getCommands(DOMNodeWrapper*);

  public:
  RandomCommand();
  
  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
    
};

#endif

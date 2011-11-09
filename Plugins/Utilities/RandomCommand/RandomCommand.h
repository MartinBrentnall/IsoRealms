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

#include <IsoRealms/ICommand.h>
#include <IsoRealms/Script.h>

#include "../IUtilities.h"

class RandomCommand:public IUtilities {
  private:
  class ARandomCommand:public ICommand {
    private:
    std::string cName;
    std::vector<Script*> cScripts;
    
    public:
    ARandomCommand(const std::string&, std::vector<Script*>);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
    
  IRuntimeContext* cRuntimeContext;
  std::vector<PlugSocket*> cCommandRegistrySocket;
  std::vector<ICommand*> cRandomCommands;
  
  std::vector<Script*> getScripts(DOMNodeWrapper*);
  
  public:
  RandomCommand(IRuntimeContext*);
  
  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);
};

#endif

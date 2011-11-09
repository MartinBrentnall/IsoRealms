/*
 * Copyright 2009,2010,2011 Martin Brentnall
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

#include "../IUtilities.h"

#include "IToggleSwitchCommand.h"
#include "ToggleSwitch.h"

class ToggleSwitches:public IUtilities {
  private:
  class ToggleCommand:public IToggleSwitchCommand {
    private:
    ToggleSwitch* cSwitch;
    std::string cName;
      
    public:
    ToggleCommand(ToggleSwitch*, const std::string&);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
    void save(DOMNodeWriter*);
  };
  
  class RefreshCommand:public IToggleSwitchCommand {
    private:
    ToggleSwitch* cSwitch;
    std::string cName;
    
    public:
    RefreshCommand(ToggleSwitch*, const std::string&);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
    void save(DOMNodeWriter*);
  };

  IRuntimeContext* cRuntimeContext;
  std::vector<PlugSocket*> cCommandRegistrySocket;
  std::vector<IToggleSwitchCommand*> cSwitchCommands;
  
  void createSwitchCommand(DOMNodeWrapper*);

  public:
  ToggleSwitches(IRuntimeContext*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);
};

#endif

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
#ifndef SPINDIZZY_SWITCHES_H
#define SPINDIZZY_SWITCHES_H

#include <map>

#include <IsoRealms/ICommandRegistry.h>
#include <IsoRealms/IUserCommand.h>

#include "../IUtilities.h"

#include "Switch.h"

class SpindizzySwitches:public IUtilities {
  private:
  class ResetCommand:public IUserCommand {
    private:
    SpindizzySwitches* cParent;
    Script* cResetScript;

    public:
    ResetCommand(SpindizzySwitches*);
    void setScript(Script*);

    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getCommandName();
  };

  class SwitchCommand:public IUserCommand {
    private:
    SpindizzySwitches* cParent;
    Switch* cSwitch;
    bool cPrimary;

    public:
    SwitchCommand(SpindizzySwitches*, Switch*, bool);
    SwitchCommand(SpindizzySwitches*, DOMNodeWrapper*);
    void deactivate();

    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getCommandName();
  };

  ICommandRegistry* cCommandRegistry;
  std::vector<SwitchCommand*> cSwitchCommands;
  std::vector<PlugSocket*> cCommandRegistrySocket;
  ResetCommand* cResetCommand;
  SwitchCommand* cActiveSwitchA;
  SwitchCommand* cActiveSwitchB;
  SwitchCommand** cNextSwitch;

  public:
  SpindizzySwitches();

  void addSwitch(const std::string&, bool);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);
  void setEditingContext(BlockLocation*, IComponentContainer*, ICommandRegistry*);
};

#endif

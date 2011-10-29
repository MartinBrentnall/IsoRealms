/*
 * Copyright 2011 Martin Brentnall
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
#ifndef STATE_TILE_FACTORY_H
#define STATE_TILE_FACTORY_H

#include <map>
#include <vector>

#include <IsoRealms/IElementFactory.h>
#include <IsoRealms/IUserCommand.h>
#include <IsoRealms/IZone.h>
#include <IsoRealms/Script.h>

#include "IStateTileFactory.h"

class StateTileFactory:public IStateTileFactory {
  private:
    
  class ToggleState:public IUserCommand {
    private:
    StateTileFactory* cParent;

    public:
    unsigned int cOnState;
    unsigned int cOffState;
    
    ToggleState(StateTileFactory*, unsigned int, unsigned int);
    void execute();
    std::string getCommandName();
  };
  
  std::vector<ToggleState*> cCommands;
  std::map<IZone*, StateTileStateController*> cStateControllers;
  std::map<unsigned int, Script*> cContactScripts;
  IZone* cCurrentZone;

  public:
  void addToggleCommand(unsigned int, unsigned int);
  bool canChange(unsigned int);  
  void zoneContextChanged(IZone*);
};

#endif

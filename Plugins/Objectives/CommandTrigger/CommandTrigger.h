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
#ifndef COMMAND_TRIGGER_H
#define COMMAND_TRIGGER_H

#include <set>

#include <IsoRealms/ICommandRegistry.h>
#include <IsoRealms/Script.h>

#include "../IObjectives.h"

class CommandTrigger:public IObjectives {
  private:
  std::set<IObjective*> cObjectives;
  IRuntimeContext* cRuntimeContext;
  Script* cObjectivesMetScript;

  public:
  CommandTrigger();
    
  /**************************\
   * Implements IObjectives *
  \**************************/
  void check();
  void registerObjective(IObjective*);
  void unregisterObjective(IObjective*);

  void setRuntimeContext(IRuntimeContext*);
};

#endif

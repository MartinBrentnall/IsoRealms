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
#include "CommandTrigger.h"

CommandTrigger::CommandTrigger() {
  cObjectivesMetScript = Script::getDummy();
}

void CommandTrigger::check() {
  for (std::set<IObjective*>::iterator i = cObjectives.begin(); i != cObjectives.end(); i++) {
    if (!(*i)->isMet()) {
      return;
    }
  }
  cObjectivesMetScript->execute();
}

void CommandTrigger::registerObjective(IObjective* objective) {
  cObjectives.insert(objective);
}

void CommandTrigger::unregisterObjective(IObjective* objective) {
  cObjectives.erase(objective);
}

void CommandTrigger::setEditingContext(BlockLocation*, IComponentContainer*, ICommandRegistry* commandRegistry) {
  cCommandRegistry = commandRegistry;
}

extern "C" IPlugin* create() {
  return new CommandTrigger();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}


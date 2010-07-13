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
  assignDummyPlugin(&cCommandRegistry, "CommandRegistry");
  cSockets.push_back(new PlugSocket("CommandRegistry"));
}

void CommandTrigger::check() {
  for (std::set<IObjective*>::iterator i = cObjectives.begin(); i != cObjectives.end(); i++) {
    if (!(*i)->isMet()) {
      return;
    }
  }
  std::cout << "Triggering ending!" << std::endl;
  for (unsigned int i = 0; i < cObjectivesMetCommands.size(); i++) {
    cObjectivesMetCommands[i]->execute();
  }
}

void CommandTrigger::registerObjective(IObjective* objective) {
  cObjectives.insert(objective);
}

void CommandTrigger::unregisterObjective(IObjective* objective) {
  cObjectives.erase(objective);
}

std::vector<PlugSocket*> CommandTrigger::getPlugSockets() {
  return cSockets;
}

void CommandTrigger::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "CommandRegistry") {
    if (assignPlugin(plugin, &cCommandRegistry, *socket)) {
      cObjectivesMetCommands.clear();
    }
  }
}

IPlugin* CommandTrigger::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "CommandRegistry") {return cCommandRegistry;}
  // TODO: Throw
  return NULL;
}

extern "C" IPlugin* create() {
  return new CommandTrigger();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}


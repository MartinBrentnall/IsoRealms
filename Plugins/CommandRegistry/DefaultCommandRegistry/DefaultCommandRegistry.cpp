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
#include "DefaultCommandRegistry.h"

void DefaultCommandRegistry::registerCommand(IUserCommand* command) {
  // TODO: Shouldn't permit multiple commands of same name.
  cCommands.push_back(command);
}

void DefaultCommandRegistry::unregisterCommand(IUserCommand* command) {
  for (unsigned int i = 0; i < cCommands.size(); i++) {
    if (cCommands[i] == command) {
      cCommands.erase(cCommands.begin() + i);
      return;
    }
  }
  // TODO: Throw
}

IUserCommand* DefaultCommandRegistry::getCommand(const std::string& commandName) {
  for (unsigned int i = 0; i < cCommands.size(); i++) {
    if (cCommands[i]->getCommandName() == commandName) {
      return cCommands[i];
    }
  }
  return NULL;
}

std::string DefaultCommandRegistry::getName() {
  return "Default Command Registry";
}

extern "C" IPlugin* create() {
  return new DefaultCommandRegistry();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

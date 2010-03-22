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
#include "Switch.h"

Switch::Switch(const std::string& name) {
  cName = name;
}

std::string Switch::getName() {
  return cName;
}

void Switch::addOnCommand(IUserCommand* command) {
  cOnCommands.push_back(command);
}

void Switch::addOffCommand(IUserCommand* command) {
  cOffCommands.push_back(command);
}

std::vector<IUserCommand*>& Switch::getOnCommands() {
  return cOnCommands;
}

std::vector<IUserCommand*>& Switch::getOffCommands() {
  return cOffCommands;
}

void Switch::removeOnCommand(IUserCommand* command) {
  for (unsigned int i = 0; i < cOnCommands.size(); i++) {
    if (cOnCommands[i] == command) {
      cOnCommands.erase(cOnCommands.begin() + i);
      return;
    }
  }
}

void Switch::removeOffCommand(IUserCommand* command) {
  for (unsigned int i = 0; i < cOffCommands.size(); i++) {
    if (cOffCommands[i] == command) {
      cOffCommands.erase(cOffCommands.begin() + i);
      return;
    }
  }
}

void Switch::switchOn() {
  for (unsigned int i = 0; i < cOnCommands.size(); i++) {
    cOnCommands[i]->execute();
  }
}

void Switch::switchOff() {
  for (unsigned int i = 0; i < cOffCommands.size(); i++) {
    cOffCommands[i]->execute();
  }
}

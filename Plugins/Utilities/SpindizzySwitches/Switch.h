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
#ifndef SWITCH_H
#define SWITCH_H

#include <string>
#include <vector>

#include <IsoRealms/DOMNodeWrapper.h>

#include "../../CommandRegistry/IUserCommand.h"

class Switch {
  private:
  std::string cName;
  std::vector<IUserCommand*> cOnCommands;
  std::vector<IUserCommand*> cOffCommands;

  public:
  Switch(const std::string&);
  Switch(const std::string&, std::vector<IUserCommand*>, std::vector<IUserCommand*>);

  std::string getName();
  void addOnCommand(IUserCommand*);
  void addOffCommand(IUserCommand*);
  std::vector<IUserCommand*>& getOnCommands();
  std::vector<IUserCommand*>& getOffCommands();
  void removeOnCommand(IUserCommand*);
  void removeOffCommand(IUserCommand*);
  void switchOn();
  void switchOff();
};

#endif
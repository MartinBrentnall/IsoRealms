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
#ifndef DEFAULT_COMMAND_REGISTRY_H
#define DEFAULT_COMMAND_REGISTRY_H

#include <map>

#include "../ICommandRegistry.h"

#include "CommandProxy.h"

class DefaultCommandRegistry:public ICommandRegistry {
  private:
  std::map<std::string, CommandProxy*> cCommands;

  CommandProxy* getCommandProxy(const std::string&);
  
  public:

  /*******************************\
   * Implements ICommandRegistry *
  \*******************************/
  void registerCommand(IUserCommand*);
  void unregisterCommand(IUserCommand*);
  IUserCommand* getCommand(const std::string&);
};

#endif

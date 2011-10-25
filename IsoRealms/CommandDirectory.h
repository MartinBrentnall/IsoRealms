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
#ifndef COMMAND_DIRECTORY_H
#define COMMAND_DIRECTORY_H

#include <map>
#include <set>
#include <vector>

#include "CommandProxy.h"
#include "DOMNodeWrapper.h"

#include "Script.h"

class CommandDirectory:public ICommandRegistry {
  private:
  std::map<std::string, CommandDirectory*> cSubDirectories;
  std::map<std::string, CommandProxy*> cCommands;

  CommandProxy* getCommandProxy(const std::string&);
  
  public:
  CommandDirectory* getSubDirectory(const std::string&);
  CommandDirectory* createSubDirectory(const std::string&);
  std::vector<std::string> getSubDirectories();
  
  /*******************************\
   * Implements ICommandRegistry *
  \*******************************/
  void selectCommands(ICommandSelectionListener*);
  Script* getScript(DOMNodeWrapper*);
  std::string getLocation(IUserCommand*);
  void registerCommand(IUserCommand*);
  void unregisterCommand(IUserCommand*);
};

#endif

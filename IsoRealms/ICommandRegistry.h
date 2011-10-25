/*
 * Copyright 2009 Martin Brentnall
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
#ifndef I_COMMAND_REGISTRY_H
#define I_COMMAND_REGISTRY_H

#include <string>

#include "DOMNodeWrapper.h"
#include "ICommandSelectionListener.h"
#include "IUserCommand.h"

class Script;

class ICommandRegistry {
  public:
  virtual void selectCommands(ICommandSelectionListener*) = 0;
  virtual Script* getScript(DOMNodeWrapper*) = 0;
  virtual std::string getLocation(IUserCommand*) = 0;
  virtual void registerCommand(IUserCommand*) = 0;
  virtual void unregisterCommand(IUserCommand*) = 0;
};

#endif

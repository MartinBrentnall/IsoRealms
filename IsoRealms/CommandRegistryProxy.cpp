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
#include "CommandRegistryProxy.h"

CommandRegistryProxy::CommandRegistryProxy(CommandDirectory* rootDirectory, std::vector<std::string> registrationDirectory) {
  cRootDirectory = rootDirectory;
  CommandDirectory* mCurrentDirectory = cRootDirectory;
  for (unsigned int i = 0; i < registrationDirectory.size(); i++) {
    CommandDirectory* mSubDirectory = mCurrentDirectory->getSubDirectory(registrationDirectory[i]);
    if (mSubDirectory == NULL) {
      mCurrentDirectory = mCurrentDirectory->createSubDirectory(registrationDirectory[i]);
    } else {
      mCurrentDirectory = mSubDirectory;
    }
  }
  cRegistrationDirectory = mCurrentDirectory;
}

void CommandRegistryProxy::selectCommands(ICommandSelectionListener* listener) {
  // TODO: Implement this
}

Script* CommandRegistryProxy::getScript(DOMNodeWrapper* node) {
  return cRootDirectory->getScript(node);
}

void CommandRegistryProxy::registerCommand(IUserCommand* command) {
  cRegistrationDirectory->registerCommand(command);
}

void CommandRegistryProxy::unregisterCommand(IUserCommand* command) {
  cRegistrationDirectory->unregisterCommand(command);
}

std::string CommandRegistryProxy::getLocation(IUserCommand* command) {
  return cRegistrationDirectory->getLocation(command);  
}

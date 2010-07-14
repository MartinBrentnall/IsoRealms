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
#include "CommandDirectory.h"

std::vector<std::string> CommandDirectory::getSubDirectories() {
  std::vector<std::string> mSubDirectories;
  for (std::map<std::string, CommandDirectory*>::iterator i = cSubDirectories.begin(); i != cSubDirectories.end(); i++) {
    mSubDirectories.push_back(i->first);
  }
  return mSubDirectories;
}

CommandDirectory* CommandDirectory::getSubDirectory(const std::string& name) {
  std::map<std::string, CommandDirectory*>::iterator mSubDirectory = cSubDirectories.find(name);
  return mSubDirectory == cSubDirectories.end() ? NULL : mSubDirectory->second;
}

CommandDirectory* CommandDirectory::createSubDirectory(const std::string& name) {
  std::map<std::string, CommandDirectory*>::iterator mSubDirectory = cSubDirectories.find(name);
  if (mSubDirectory != cSubDirectories.end()) {
    return mSubDirectory->second;
  }
  cSubDirectories[name] = new CommandDirectory();
  return cSubDirectories[name];
}

CommandProxy* CommandDirectory::getCommandProxy(const std::string& name) {
  CommandProxy* mCommandProxy = cCommands[name];
  if (mCommandProxy == NULL) {
    mCommandProxy = new CommandProxy();
    cCommands[name] = mCommandProxy;
  }
  return mCommandProxy;
}

Script* CommandDirectory::getScript(DOMNodeWrapper* node) {
  std::vector<IUserCommand*> mCommands;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Command") {
      std::string mCommandName = mNode->getStringValue();
      std::string::size_type mDirectorySeparator = mCommandName.find("/");
      CommandDirectory* mDirectoryOfCommand = this;
      while (mDirectorySeparator != std::string::npos) {
        std::string mDirectory = mCommandName.substr(0, mDirectorySeparator);
        mDirectoryOfCommand->createSubDirectory(mDirectory);
        mDirectoryOfCommand = mDirectoryOfCommand->getSubDirectory(mDirectory);
        mCommandName = mCommandName.substr(mDirectorySeparator + 1, mCommandName.length());
        mDirectorySeparator = mCommandName.find("/");
      }
      mCommands.push_back(mDirectoryOfCommand->getCommandProxy(mCommandName));
    } else {
      // TODO: Throw
    }
  }
  return new Script(mCommands);
}

void CommandDirectory::registerCommand(IUserCommand* command) {
  // TODO: Check if name already in use.
  std::string mCommandName = command->getCommandName();
  CommandProxy* mCommandProxy = getCommandProxy(mCommandName);
  mCommandProxy->setUserCommand(command);
}

void CommandDirectory::unregisterCommand(IUserCommand* command) {
  // TODO: Check if name already in use.
  std::string mCommandName = command->getCommandName();
  CommandProxy* mCommandProxy = getCommandProxy(mCommandName);
  mCommandProxy->setUserCommand(NULL);
}


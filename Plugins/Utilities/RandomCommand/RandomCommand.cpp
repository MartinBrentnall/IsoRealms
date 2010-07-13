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
#include "RandomCommand.h"

RandomCommand::RandomCommand() {
  assignDummyPlugin(&cCommandRegistry, "CommandRegistry");
  cCommandRegistrySocket.push_back(new PlugSocket("CommandRegistry"));
}

std::vector<PlugSocket*> RandomCommand::getPlugSockets() {
  return cCommandRegistrySocket;
}

void RandomCommand::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "CommandRegistry") {
    ICommandRegistry* mPreviousCommandRegistry = cCommandRegistry;
    if (assignPlugin(plugin, &cCommandRegistry, *socket)) {
      for (unsigned int i = 0; i < cRandomCommands.size(); i++) {
        mPreviousCommandRegistry->unregisterCommand(cRandomCommands[i]);
        cCommandRegistry->registerCommand(cRandomCommands[i]);
      }
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* RandomCommand::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "CommandRegistry") {return cCommandRegistry;}
  // TODO: Throw
  return NULL;
}

std::vector<IUserCommand*> RandomCommand::getCommands(DOMNodeWrapper* node) {
  std::vector<IUserCommand*> mCommands;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "RandomCommand") {
      std::string mCommandName = mNode->getStringValue();
      mCommands.push_back(cCommandRegistry->getCommand(mCommandName));
    } else {
      // TODO: Throw
    }
  }
  return mCommands;
}

void RandomCommand::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Command") {
      std::string mCommandName = mNode->getAttribute("name");
      std::vector<IUserCommand*> mCommands = getCommands(mNode);
      ARandomCommand* mRandomCommand = new ARandomCommand(mCommandName, mCommands);
      cRandomCommands.push_back(mRandomCommand);
      cCommandRegistry->registerCommand(mRandomCommand);
    } else {
      // TODO: Throw
    }
  }
}

void RandomCommand::save(DOMNodeWriter* node) {
  // TODO: Implement this
}

RandomCommand::ARandomCommand::ARandomCommand(const std::string& name, std::vector<IUserCommand*> commands) {
  cName = name;
  cCommands = commands;
}

void RandomCommand::ARandomCommand::execute() {
  cCommands[rand() % cCommands.size()]->execute(); 
}

std::string RandomCommand::ARandomCommand::getCommandName() {
  return "Random " + cName;
}

extern "C" IPlugin* create() {
  return new RandomCommand();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}


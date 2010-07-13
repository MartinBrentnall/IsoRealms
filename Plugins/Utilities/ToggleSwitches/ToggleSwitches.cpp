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
#include "ToggleSwitches.h"

ToggleSwitches::ToggleSwitches() {
  assignDummyPlugin(&cCommandRegistry, "CommandRegistry");
  cCommandRegistrySocket.push_back(new PlugSocket("CommandRegistry"));
}

std::vector<PlugSocket*> ToggleSwitches::getPlugSockets() {
  return cCommandRegistrySocket;
}

void ToggleSwitches::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "CommandRegistry") {
    ICommandRegistry* mPreviousCommandRegistry = cCommandRegistry;
    if (assignPlugin(plugin, &cCommandRegistry, *socket)) {
      for (unsigned int i = 0; i < cSwitchCommands.size(); i++) {
        mPreviousCommandRegistry->unregisterCommand(cSwitchCommands[i]);
        cCommandRegistry->registerCommand(cSwitchCommands[i]);
      }
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* ToggleSwitches::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "CommandRegistry") {return cCommandRegistry;}
  // TODO: Throw
  return NULL;
}

std::vector<IUserCommand*> ToggleSwitches::getCommands(DOMNodeWrapper* node) {
  std::vector<IUserCommand*> mCommands;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Command") {
      std::string mCommandName = mNode->getStringValue();
      mCommands.push_back(cCommandRegistry->getCommand(mCommandName));
    } else {
      // TODO: Throw
    }
  }
  return mCommands;
}

ToggleSwitches::SwitchCommand* ToggleSwitches::createSwitchCommand(DOMNodeWrapper* node) {
  std::vector<IUserCommand*> mOnCommands;
  std::vector<IUserCommand*> mOffCommands;
  std::string mCommandName = node->getAttribute("name");
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "On") {
      mOnCommands = getCommands(mNode);
    } else if (mValueAsString == "Off") {
      mOffCommands = getCommands(mNode);
    }
  }
  return new SwitchCommand(mCommandName, mOnCommands, mOffCommands);
}

void ToggleSwitches::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Switch") {
      SwitchCommand* mSwitchCommand = createSwitchCommand(mNode);
      cSwitchCommands.push_back(mSwitchCommand);
      cCommandRegistry->registerCommand(mSwitchCommand);
    } else {
      // TODO: Throw
    }
  }
}

void ToggleSwitches::save(DOMNodeWriter* node) {
  // TODO: Implement this
}

ToggleSwitches::SwitchCommand::SwitchCommand(const std::string& name, std::vector<IUserCommand*> onCommands, std::vector<IUserCommand*> offCommands) {
  cName = name;
  cState = false;
  cOnCommands = onCommands;
  cOffCommands = offCommands;
}

void ToggleSwitches::SwitchCommand::execute() {
  std::vector<IUserCommand*>* mCommandsToExecute = cState ? &cOffCommands : &cOnCommands;
  for (unsigned int i = 0; i < mCommandsToExecute->size(); i++) {
    (*mCommandsToExecute)[i]->execute();
  }
  cState = !cState;
}

std::string ToggleSwitches::SwitchCommand::getCommandName() {
  return "Toggle " + cName;
}

extern "C" IPlugin* create() {
  return new ToggleSwitches();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

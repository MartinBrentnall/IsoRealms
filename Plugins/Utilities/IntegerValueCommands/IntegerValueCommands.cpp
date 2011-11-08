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
#include "IntegerValueCommands.h"

IntegerValueCommands::IntegerValueCommands() {
  assignDummyPlugin(&cVariable, "IntegerValue");
}

void IntegerValueCommands::addCommand(AddIntegerCommand* command) {
  cCommands.push_back(command);
  cCommandRegistry->registerCommand(command);
}

void IntegerValueCommands::removeCommand(AddIntegerCommand* command) {
  cCommandRegistry->registerCommand(command);
  for (unsigned int i = 0; i < cCommands.size(); i++) {
    if (cCommands[i] == command) {
      cCommands.erase(cCommands.end() + i);
      return;
    }
  }
  // TODO: Chuck
  std::cout << "WARNING: Removed command wasn't found in local registry!" << std::endl;
}

std::vector<AddIntegerCommand*> IntegerValueCommands::getCommands() {
  return cCommands;
}

void IntegerValueCommands::save(DOMNodeWriter* node) {
  for (unsigned int i = 0; i < cCommands.size(); i++) {
    DOMNodeWriter* mCommandNode = node->addBranch("Command");
    cCommands[i]->save(mCommandNode);
  }
}

void IntegerValueCommands::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Command") {
      AddIntegerCommand* mCommand = new AddIntegerCommand(mNode, cVariable);
      addCommand(mCommand);
    }
  }
}

void IntegerValueCommands::setRuntimeContext(IRuntimeContext* runtimeContext) {
  cCommandRegistry = runtimeContext->getCommandRegistry();
}

void IntegerValueCommands::setEditingContext(IEditingContext* editingContext) {
  std::vector<std::string> mPath;
  mPath.push_back("Configure");
  mPath.push_back("Variable Commands");
  cComponentContainer = editingContext->getComponentContainer();
  OpenDialogCommand* mConfigureIntegerCommands = new OpenDialogCommand(cComponentContainer, this);
  editingContext->registerCommand(new DefaultCommandInfo(mPath, mConfigureIntegerCommands));
}

void IntegerValueCommands::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "IntegerValue") {
    if (assignPlugin(plugin, &cVariable, *socket)) {
      for (unsigned int i = 0; i < cCommands.size(); i++) {
        cCommands[i]->setVariable(cVariable);
      }
    }
  } else {
    // TODO: Chuck
  }
}

IPlugin* IntegerValueCommands::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "IntegerValue")    {return cVariable;}
  // TODO: chuck
  return NULL;
}

IHUDComponent* IntegerValueCommands::createComponent() {
  return new IntegerCommandsDialog(cComponentContainer, this);
}

extern "C" IPlugin* create() {
  return new IntegerValueCommands();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

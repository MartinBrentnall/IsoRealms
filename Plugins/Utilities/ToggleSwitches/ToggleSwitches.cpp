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

ToggleSwitches::SwitchCommand* ToggleSwitches::createSwitchCommand(DOMNodeWrapper* node) {
  Script* mOnScript;
  Script* mOffScript;
  std::string mCommandName = node->getAttribute("name");
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "On") {
      mOnScript = cCommandRegistry->getScript(mNode);
    } else if (mValueAsString == "Off") {
      mOffScript = cCommandRegistry->getScript(mNode);
    }
  }
  return new SwitchCommand(mCommandName, mOnScript, mOffScript);
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

void ToggleSwitches::setEditingContext(BlockLocation*, IComponentContainer*, ICommandRegistry* commandRegistry) {
  cCommandRegistry = commandRegistry;
}

ToggleSwitches::SwitchCommand::SwitchCommand(const std::string& name, Script* onScript, Script* offScript) {
  cName = name;
  cState = false;
  cOnScript = onScript;
  cOffScript = offScript;
}

void ToggleSwitches::SwitchCommand::execute() {
  Script* mScriptToExecute = cState ? cOffScript : cOnScript;
  mScriptToExecute->execute();
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

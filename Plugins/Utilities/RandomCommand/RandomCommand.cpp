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

RandomCommand::RandomCommand(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
}

std::vector<Script*> RandomCommand::getScripts(DOMNodeWrapper* node) {
  std::vector<Script*> mScripts;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "PossibleScript") {
      mScripts.push_back(cRuntimeContext->getScript(mNode));
    } else {
      // TODO: Throw
    }
  }
  return mScripts;
}

void RandomCommand::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Command") {
      std::string mCommandName = mNode->getAttribute("name");
      std::vector<Script*> mScripts = getScripts(mNode);
      ARandomCommand* mRandomCommand = new ARandomCommand(mCommandName, mScripts);
      cRandomCommands.push_back(mRandomCommand);
      cRuntimeContext->add(mRandomCommand);
    } else {
      // TODO: Throw
    }
  }
}

void RandomCommand::save(DOMNodeWriter* node) {
  // TODO: Implement this
}

RandomCommand::ARandomCommand::ARandomCommand(const std::string& name, std::vector<Script*> scripts) {
  cName = name;
  cScripts = scripts;
}

void RandomCommand::ARandomCommand::execute() {
  if (!cScripts.empty()) {
    cScripts[rand() % cScripts.size()]->execute(); 
  }
}

std::string RandomCommand::ARandomCommand::getName() {
  return "Random " + cName;
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new RandomCommand(runtimeContext);
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}


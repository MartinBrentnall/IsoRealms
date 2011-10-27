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
#include "Runtime.h"

Runtime::Runtime(DOMNodeWrapper* node) {
  ICommand* mCommand = CommandManager::getCommand("Pop");
  cExitCommands.push_back(mCommand);
  
  DOMNodeWrapper* mConfigurationRootNode = new DOMNodeWrapper("Test.isorealms");
  for (int i = 0; i < mConfigurationRootNode->getChildCount(); i++) {
    DOMNodeWrapper *mNode = mConfigurationRootNode->getChild(i);
    std::string mValue = mNode->getNodeName();
    if (mValue == "Map") {
      cMap = new Map(mNode, NULL, NULL, "Test.isorealms", false);
    }
  }
  cMap->initRuntime();
  cRunExitCommands = false;
}

void Runtime::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_ESCAPE: {
      cRunExitCommands = true;
    }

    default: {
      // Do nothing.
    }
  }
}

void Runtime::input(SDL_Event& event) {
  cMap->input(event);
  switch (event.type) {
    case SDL_KEYDOWN: {
      keyDown(event.key.keysym.sym);
    }
  }
}

// TODO: Should be unsigned int
void Runtime::execute(int milliseconds) {
  cMap->executePreLoopCommands(milliseconds);
  cMap->updateRuntime(milliseconds);
  cMap->executePostLoopCommands(milliseconds);
  cMap->executePreLoopRenderers();
  cMap->render();
  cMap->executePostLoopRenderers();
  if (cRunExitCommands) {
    for (unsigned int i = 0; i < cExitCommands.size(); i++) {
      cExitCommands[i]->execute();
    }
  }
}

extern "C" IControlLoop* create(DOMNodeWrapper* node) {
  return new Runtime(node);
}

extern "C" void destroy(IControlLoop* controlLoop) {
  delete controlLoop;
}

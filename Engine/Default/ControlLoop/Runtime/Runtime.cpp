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

Runtime::Runtime(DOMNodeWrapper* node, IEngineArguments* engineArguments) {
  ICommand* mCommand = CommandManager::getCommand("Pop");
  cExitCommands.push_back(mCommand);
  
  std::string mMapName = engineArguments->get();
  std::string mProjectFile = System::getProgramResource("Data/Projects/" + mMapName); // TODO: Should we hard code Data/Projects/ here?
  DOMNodeWrapper* mConfigurationRootNode = new DOMNodeWrapper(mProjectFile);
  for (int i = 0; i < mConfigurationRootNode->getChildCount(); i++) {
    DOMNodeWrapper *mNode = mConfigurationRootNode->getChild(i);
    std::string mValue = mNode->getNodeName();
    if (mValue == "Project") {
      cProject = new Project(mNode, NULL, NULL, mMapName, false);
    }
  }
  cProject->initRuntime();
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
  cProject->input(event);
  switch (event.type) {
    case SDL_KEYDOWN: {
      keyDown(event.key.keysym.sym);
    }
  }
}

// TODO: Should be unsigned int
void Runtime::execute(int milliseconds) {
  cProject->executePreLoopCommands(milliseconds);
  cProject->updateRuntime(milliseconds);
  cProject->executePostLoopCommands(milliseconds);
  cProject->executePreLoopRenderers();
  cProject->render();
  cProject->executePostLoopRenderers();
  if (cRunExitCommands) {
    for (unsigned int i = 0; i < cExitCommands.size(); i++) {
      cExitCommands[i]->execute();
    }
  }
}

extern "C" IControlLoop* create(DOMNodeWrapper* node, IEngineArguments* engineArguments) {
  return new Runtime(node, engineArguments);
}

extern "C" void destroy(IControlLoop* controlLoop) {
  delete controlLoop;
}

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

Runtime::Runtime(DOMNodeWrapper* node, const std::string& projectName) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValue = mNode->getNodeName();
    if (mValue == "Project") {
      cProject = new Project(mNode, projectName, NULL);
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

bool Runtime::terminated() {
  return cRunExitCommands;
}

void Runtime::input(SDL_Event& event) {
  cProject->input(event);
  switch (event.type) {
    case SDL_KEYDOWN: {
      keyDown(event.key.keysym.sym);
    }
  }
}

void Runtime::execute(unsigned int milliseconds) {
  cProject->updateRuntime(milliseconds);
  cProject->renderRuntime();
}

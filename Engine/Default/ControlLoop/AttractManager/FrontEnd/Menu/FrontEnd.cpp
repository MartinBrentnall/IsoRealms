/*
 * Copyright 2009 Martin Brentnall
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
#include "FrontEnd.h"

int FrontEnd::MAX_IDLE_TIME = 60000;

FrontEnd::FrontEnd(DOMNodeWrapper* node, IFont* font) {
  cExitCommand = CommandManager::getCommand("Pop");
  cFont = font;
  FrontEndMenu* mMainMenu = new FrontEndMenu(this, this, node);
  cActiveMenu.push_back(mMainMenu);
}

ICommand* FrontEnd::parseCommand(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ExecuteCommand") {
      std::string mCommandType = mNode->getAttribute("type");
      return CommandManager::getCommand(mCommandType);
    }
  }
  return NULL;
}

ICommand* FrontEnd::parseArgumentCommand(DOMNodeWrapper* node, const std::string& argument) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "LoadMapForEditing") {
      return new EditMapCommand(argument);
    }
  }
  return NULL;
}

void FrontEnd::update(int ticks) {
  switch (cStage) {
    case 0: {
      cFade += ticks / 1000.0;
      if (cFade >= 1.0) {
        cFade = 1.0;
        cStage = 1;
      }
      break;
    }

    case 1: {
      cIdleTime += ticks;
      if (cIdleTime >= MAX_IDLE_TIME) {
        cStage = 2;
        cFade = 1.0;
      }
      break;
    }

    case 2: {
      cFade -= ticks / 1000.0;
      if (cFade <= 0.0) {
        cFade = 0.0;
        cStage = 3;
      }
      break;
    }
  }
}

void FrontEnd::render() {
  cActiveMenu[cActiveMenu.size() - 1]->render(cFade, cFont);
}

void FrontEnd::setActive(bool active) {
  if (active) {
    cIdleTime = 0;
    cHasExited = false;
    cFade = 0.0f;
    cStage = 0;
  }
}

bool FrontEnd::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_ESCAPE: {
      if (cActiveMenu.size() == 1) {
        if (cExitCommand != NULL) {
          cExitCommand->execute();
        } else {
          std::cout << "Warning: No exit command has been defined!" << std::endl;
        } 
      } else {
        cActiveMenu.pop_back();
      }
      return true;
    }

    default: {
      // Nothing to do.
    }
  }
  return false;
}

void FrontEnd::input(SDL_Event& event) {  
  switch (event.type) {
    case SDL_KEYDOWN: {
      if (keyDown(event.key.keysym.sym)) {
        return;
      }
    }
  }
  
  if (cActiveMenu[cActiveMenu.size() - 1]->input(event)) {
    cIdleTime = 0;
  }
}

void FrontEnd::terminate() {
  cHasExited = true;
}

bool FrontEnd::hasIdled() {
  return cStage == 3;
}

bool FrontEnd::hasExited() {
  return cHasExited;
}

void FrontEnd::push(FrontEndMenu* menu) {
  cActiveMenu.push_back(menu);
}

extern "C" IFrontEnd* create(DOMNodeWrapper* node, IFont* font) {
  return new FrontEnd(node, font);
}

extern "C" void destroy(IFrontEnd* frontEnd) {
  delete frontEnd;
}

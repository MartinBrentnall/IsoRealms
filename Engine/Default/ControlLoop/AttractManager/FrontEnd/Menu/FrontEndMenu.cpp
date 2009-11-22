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
#include "FrontEndMenu.h"

int FrontEndMenu::MAX_IDLE_TIME = 60000;

FrontEndMenu::FrontEndMenu(DOMNodeWrapper* node) {
  cExitCommand = NULL;
  IFontEngine* mFontEngine = GlobalConfiguration::getFontEngine();

  // TODO: Get font from XML!
  cFont = mFontEngine->getFont("Menu");
  std::vector<std::string>* mMainMenu = new std::vector<std::string>;
  cActiveMenuItems = new std::vector<std::string>;
  parseMenu(node, "", mMainMenu);
  *cActiveMenuItems = *mMainMenu;
  cMenuItemCommands[""] = new EnterMenuCommand(*cActiveMenuItems, *cMenuItems[""], cSelectedItem, cNewLocation, "");
}

void FrontEndMenu::parseMenu(DOMNodeWrapper* node, std::string menuID, std::vector<std::string>* menu) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "MenuItem") {
      std::string mMenuItemLabel = mNode->getAttribute("name");
      std::string mMenuItemName = (menuID == "" ? "" : menuID + "/") + mMenuItemLabel;
      cMenuItemCommands[mMenuItemName] = parseEventCommand(mNode);
      menu->push_back(mMenuItemLabel);
    } else if (mValueAsString == "Menu") {
      std::string mMenuItemLabel = mNode->getAttribute("name");
      std::string mMenuItemName = (menuID == "" ? "" : menuID + "/") + mMenuItemLabel;
      cSubMenus.push_back(mMenuItemName);
      std::vector<std::string>* mMenu = new std::vector<std::string>;
      parseMenu(mNode, mMenuItemName, mMenu);
      cMenuItemCommands[mMenuItemName] = new EnterMenuCommand(*cActiveMenuItems, *cMenuItems[mMenuItemName], cSelectedItem, cNewLocation, mMenuItemName);
      menu->push_back(mMenuItemLabel);
    } else if (mValueAsString == "MenuItemList") {
      std::string mMenuItemLabel = mNode->getAttribute("name");
      std::string mMenuItemName = (menuID == "" ? "" : menuID + "/") + mMenuItemLabel;
      std::string mDirForSelection = mNode->getAttribute("fromdir");
      std::string mFileNameFilter = mNode->getAttribute("filter");
      cMenuItemCommands[mMenuItemName] = new SelectionList(*cActiveMenuItems, mDirForSelection, mFileNameFilter, cSelectedItem, cNewLocation, mMenuItemName);
      menu->push_back(mMenuItemLabel);
      cMenuItemArgumentedCommands[mMenuItemName] = parseEventCommand(mNode);
    } else if (mValueAsString == "ExitCommand") {
      cExitCommand = parseEventCommand(mNode);
    }
  }
  cMenuItems[menuID] = menu;
}

ICommand* FrontEndMenu::parseEventCommand(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ExecuteCommand") {
      std::string mCommandType = mNode->getAttribute("type");
      return CommandManager::getCommand(mCommandType);
    } else if (mValueAsString == "LoadMapForEditing") {
      return new EditMapCommand(cCommandArgument);
    }
  }
  return NULL;
}

void FrontEndMenu::update(int ticks) {
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

void FrontEndMenu::render() {
  glEnable(GL_BLEND);
  for (unsigned int i = 0; i < cActiveMenuItems->size(); i++) {
    glColor4f(0.0f, 0.0f, 0.0f, cFade);
    cFont->print(0.008f, (-0.10f * i) - 0.008f, 0.05f, 1, (*cActiveMenuItems)[i].c_str());
    if (i == cSelectedItem) {
      glColor4f(0.6f, 1.0f, 0.4f, cFade);
    } else {
      glColor4f(1.0f, 1.0f, 1.0f, cFade);
    }
    cFont->print(0.0f, -0.10f * i, 0.05f, 1, (*cActiveMenuItems)[i].c_str());
  }
  glDisable(GL_BLEND);
}

void FrontEndMenu::setActive(bool active) {
  if (active) {
    cIdleTime = 0;
    cHasExited = false;
    cFade = 0.0f;
    cStage = 0;
    cSelectedItem = 0;
  }
}

void FrontEndMenu::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_UP: {
      if (cSelectedItem > 0) {
        cSelectedItem--;
      }
      break;
    }

    case SDLK_DOWN: {
      if (cSelectedItem < cActiveMenuItems->size() - 1) {
        cSelectedItem++;
      }
      break;
    }

    case SDLK_RETURN: {
      std::string mDestination = (*cActiveMenuItems)[cSelectedItem];
      std::string mActionID = getActionID(mDestination);
      cNewLocation = NULL;
      executeAction(mActionID);
      if (cNewLocation != NULL) {
        cCurrentLocation.push_back(*cNewLocation);
      }
      break;
    }

    case SDLK_ESCAPE: {
      if (cCurrentLocation.size() == 0) {
        if (cExitCommand != NULL) {
          cExitCommand->execute();
        } else {
          std::cout << "Warning: No exit command has been defined!" << std::endl;
        } 
      } else {
        std::string mLastLocation = cCurrentLocation[cCurrentLocation.size() - 1];
        cCurrentLocation.pop_back();
        std::string mActionID = getActionID();
        ICommand* mCommand = cMenuItemCommands[mActionID];
        if (mCommand != NULL) {
          mCommand->execute();
        } else {
          std::cout << "Don't worry, there's no command defined for this menu item!" << std::endl;
        }
      }
      break;
    }

    default: {
      // Nothing to do.
    }
  }
}

void FrontEndMenu::executeAction(std::string id) {
  ICommand* mAction = cMenuItemCommands[id];
  if (mAction == NULL) {
    int mLastSeparator = id.find_last_of('/');
    std::string mArgumentedActionID = id.substr(0, mLastSeparator);
    cCommandArgument = id.substr(mLastSeparator + 1);
    mAction = cMenuItemArgumentedCommands[mArgumentedActionID];
  }

  if (mAction != NULL) {
    mAction->execute();
  } else {
    std::cout << "Warning: No action defined for item: " << id << std::endl;
  }
}

bool FrontEndMenu::isSubMenu(std::string id) {
  for (unsigned int i = 0; i < cSubMenus.size(); i++) {
    if (cSubMenus[i] == id) {
      return true;
    }
  }
  return false;
}

std::string FrontEndMenu::getActionID(std::string id) {
  return cCurrentLocation.size() == 0 ? id : cCurrentLocation[cCurrentLocation.size() - 1] + "/" + id;
}

std::string FrontEndMenu::getActionID() {
  std::string mLocation;
  for (unsigned int i = 0; i < cCurrentLocation.size(); i++) {
    if (i != 0) {
      mLocation = mLocation + "/";
    }
    mLocation = mLocation + cCurrentLocation[i];
  }
  return mLocation;
}

void FrontEndMenu::mouseButtonDown() {
  std::cout << "Mouse Button Down" << std::endl;
}

void FrontEndMenu::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      keyDown(event.key.keysym.sym);
      cIdleTime = 0;
      break;
    }

    case SDL_MOUSEBUTTONDOWN: {
      mouseButtonDown();
      cIdleTime = 0;
      break;
    }
  }
}

void FrontEndMenu::terminate() {
  cHasExited = true;
}

bool FrontEndMenu::hasIdled() {
  return cStage == 3;
}

bool FrontEndMenu::hasExited() {
  return cHasExited;
}

extern "C" IFrontEnd* create(DOMNodeWrapper* node) {
  return new FrontEndMenu(node);
}

extern "C" void destroy(IFrontEnd* frontEnd) {
  delete frontEnd;
}

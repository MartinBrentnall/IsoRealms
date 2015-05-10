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
#include "MenuPopup.h"

MenuPopup::MenuPopup(DOMNodeWrapper* node, IMenuContainer* parent, float x, float y, IComponentContainer* componentContainer, ICommandSource* commandSource, IResourceAccessor* resources) {
  cX = x;
  cY = y;
  cParent = parent;
  cSelectedItem = cMenuItems.size();
  cSubMenuShowing = NULL;

  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Menu") {
      std::string mSubMenuName = mNode->getAttribute("name");
      std::string mSubMenuText = mNode->getAttribute("text");
      MenuPopup* mMenuPopup = new MenuPopup(mNode, parent, x, y, componentContainer, commandSource, resources);
      PopupMenuCommand* mPopupMenuCommand = new PopupMenuCommand(this, mMenuPopup);
      addMenuItem(mSubMenuName, mSubMenuText, mPopupMenuCommand, true);
      cMenuPopups[mSubMenuName] = mMenuPopup;
    } else if (mValueAsString == "MenuItem") {
      std::string mPathElementName = mNode->getAttribute("name");
      ICommand* mCommand = parseCommand(mNode, commandSource, resources);
      std::string mPathElementText = mNode->getAttribute("text");
      addMenuItem(mPathElementName, mPathElementText, mCommand, false);
    }
  }
}

MenuPopup::MenuPopup(IMenuContainer* parent, float x, float y) {
  cX = x;
  cY = y;
  cParent = parent;
  cSelectedItem = cMenuItems.size();
}

void MenuPopup::addMenuItem(const std::string& name, const std::string& text, ICommand* command, bool subMenuCommand) {
  float mXDestination = getX() + 0.02f;
  float mYDestination = getY() + getHeight() - 0.04f;
  MenuItem* mMenuItem = new MenuItem(text, command, mXDestination, mYDestination, subMenuCommand);
  cMenuItemsByName[name] = mMenuItem;
  cMenuItems.push_back(mMenuItem);
}

void MenuPopup::addCommand(std::vector<std::string> path, ICommandInfo* command) {
//   std::string mPathElementName = path[path.size() - 1];
//   if (path.size() > 1) {
    // TODO: Allow sub-popup menus!
/*    std::string mPathElementName = path[path.size() - 1];
    MenuPopup* mPopupMenu = cMenuPopups[mPathElementName];
    if (mPopupMenu == NULL) {
      mPopupMenu = new MenuPopup(cParent, cWidth - 1.01f, 0.95f);
      PopupMenuCommand* mPopupMenuCommand = new PopupMenuCommand(cMenuPopupShowing, mPopupMenu);
      MenuItem* mMenuItem = new MenuItem(mPathElementName, mPopupMenuCommand, cWidth - 1.01f, 0.96f);
      cMenuItems.push_back(mMenuItem);
    }
    path.pop_back();
    mPopupMenu->addCommand(path, commandInfo);*/
//  } else {
//    ICommand* mCommand = command->getCommand();
//    addMenuItem(mPathElementName, mCommand);
    // TODO: Make a new menu item here!
//  }
}

void MenuPopup::setPosition(float x, float y) {
  cX = x;
  cY = y;
  float mX = getX() + 0.02f;
  float mY = getY() - 0.04f;
  for (unsigned int i = 0; i < cMenuItems.size(); i++) {
    cMenuItems[i]->setPosition(mX, mY);
    mY -= 0.05f;
  }
  for (std::map<std::string, MenuPopup*>::iterator i = cMenuPopups.begin(); i != cMenuPopups.end(); i++) {
    MenuItem* mMenuItem = cMenuItemsByName[i->first];
    float mY = mMenuItem->getY();
    i->second->setPosition(getX() + getWidth(), mY + 0.04f);
  }
  cSubMenuShowing = NULL;
}

// TODO: return (i.e. support) a vector
ICommand* MenuPopup::parseCommand(DOMNodeWrapper* node, ICommandSource* commandSource, IResourceAccessor* resources) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ExecuteEditorCommand") {
      std::string mCommandName = mNode->getAttribute("type");
      return commandSource->getCommand(mCommandName);
    } else if (mValueAsString == "Action") {
      return resources->getScriptCall(mNode);
    }
  }
  return NULL;
}

float MenuPopup::getX() {
  return cX;
}

float MenuPopup::getY() {
  return cY;
}

float MenuPopup::getWidth() {
  float mWidth = 0.0f;
  for (unsigned int i = 0; i < cMenuItems.size(); i++) {
    float mItemWidth = cMenuItems[i]->getWidth();
    if (mItemWidth > mWidth) {
      mWidth = mItemWidth;
    }
  }
  return mWidth + 0.04f;
}

float MenuPopup::getHeight() {
  float mHeight = 0.0f;
  for (unsigned int i = 0; i < cMenuItems.size(); i++) {
    mHeight += cMenuItems[i]->getHeight();
  }
  return -mHeight;
}

void MenuPopup::render() {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();
  glDisable(GL_DEPTH_TEST);

  float mHeight = getHeight();
  float mWidth = getWidth();

  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glColor4f(0.05f, 0.0f, 0.1f, 0.85f);
  glVertex2f(cX,          cY);
  glVertex2f(cX,          cY + mHeight);
  glVertex2f(cX + mWidth, cY + mHeight);
  glVertex2f(cX + mWidth, cY);
  glEnd();
  glDisable(GL_BLEND);
  
  glBegin(GL_LINE_LOOP);
  glColor3f(0.8f, 0.6f, 1.0f);
  glVertex2f(cX,          cY);
  glVertex2f(cX,          cY + mHeight);
  glVertex2f(cX + mWidth, cY + mHeight);
  glVertex2f(cX + mWidth, cY);
  glEnd();
  glLoadIdentity();  
  glEnable(GL_DEPTH_TEST);

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  glLoadIdentity();
  for (unsigned int i = 0; i < cMenuItems.size(); i++) {
    cMenuItems[i]->render(i == cSelectedItem);
  }
  if (cSubMenuShowing != NULL) {
    cSubMenuShowing->render();
  }
}

bool MenuPopup::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  for (unsigned int i = 0; i < cMenuItems.size(); i++) {
    // TODO: Test the width of the popup, not just the menu item text!
    if (cMenuItems[i]->testClick(mX, mY)) {
      if (!cMenuItems[i]->isSubMenuCommand()) {
        cParent->closeMenu(this);
      }
      cSelectedItem = cMenuItems.size();
      return true;
    }
  }
  return false;
}

bool MenuPopup::mouseMotion(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  for (unsigned int i = 0; i < cMenuItems.size(); i++) {
    if (cMenuItems[i]->contains(mX, mY)) {
      cSelectedItem = i;
      // TODO: Access sub-menu! (DO NOT EXECUTE MENU ITEM WITHOUT CHECKING FIRST THAT ITS A SUBMENU!)
      return false;
    }
  }
  return false;
}

bool MenuPopup::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_ESCAPE: {
      cParent->closeMenu(this);
      return true;
    }

    case SDLK_RETURN: {
      if (cSelectedItem != cMenuItems.size()) {
        cMenuItems[cSelectedItem]->execute();
        cParent->closeMenu(this);
        cSelectedItem = cMenuItems.size();
      }
      return true;
    }

    case SDLK_RIGHT: {
      // TODO: Access sub-menu! (DO NOT EXECUTE MENU ITEM WITHOUT CHECKING FIRST THAT ITS A SUBMENU!)
      return false;
    }

    case SDLK_DOWN: {
      cSelectedItem++;
      if (cSelectedItem >= cMenuItems.size()) {
        cSelectedItem = 0;
      }
      return true;
    }

    case SDLK_UP: {
      if (cSelectedItem > 0) {
        cSelectedItem--;
      } else {
        cSelectedItem = cMenuItems.size() - 1;
      }
      return true;
    }

    default: {
      return false;
    }
  }
}

bool MenuPopup::input(SDL_Event& event) {
  if (cSubMenuShowing != NULL) {
    if (cSubMenuShowing->input(event)) {
      return true;
    }
  }
  switch (event.type) {
    case SDL_KEYDOWN: {
      for (unsigned int i = 0; i < cMenuItems.size(); i++) {
        if (cMenuItems[i]->input(event)) {
          cParent->closeMenu(this);
          return true;
        }
      }
      return keyDown(event.key.keysym.sym);
    }

    case SDL_MOUSEBUTTONDOWN: {
      return mouseButtonDown(event);
    }

    case SDL_MOUSEMOTION: {
      return mouseMotion(event);
    }
  }
  return false;
}

bool MenuPopup::contains(float x, float y) {
  float mHeight = getHeight();
  float mWidth = getWidth();
  return x >= cX && x <= cX + mWidth && y <= cY && y >= cY + mHeight;
}

bool MenuPopup::setComponentAction(const std::string& name, ICommand* command) {
  std::map<std::string, MenuItem*>::iterator i = cMenuItemsByName.find(name);
  if (i != cMenuItemsByName.end()) {
    i->second->setCommand(command);
    return true;
  }
  return false;
}

bool MenuPopup::addComponentAction(const std::string& name, const std::string& actionDescription, ICommand* command) {
  std::map<std::string, MenuPopup*>::iterator i = cMenuPopups.find(name);
  if (i != cMenuPopups.end()) {
    i->second->addMenuItem("", actionDescription, command, false);
    return true;
  }
  return false;
}

void MenuPopup::popupMenu(IMenuPopup* popup) {
  for (std::map<std::string, MenuPopup*>::iterator i = cMenuPopups.begin(); i != cMenuPopups.end(); i++) {
    if (i->second == popup) {
      std::cout << "A SUBMENU IS ABOUT TO BE SHOWN!" << std::endl;
      cSubMenuShowing = i->second;
    }
  }
}


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

MenuPopup::MenuPopup(DOMNodeWrapper* node, IMenuContainer* parent, float x, float y, IPluginRegistryAccessor* pluginRegistryAccessor, IComponentContainer* componentContainer) {
  cX = x;
  cY = y;
  cParent = parent;
  cSelectedItem = 0;

  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
/*    if (mValueAsString == "Menu") {
      MenuItem* mMenuItem = new MenuItem(mNode->getAttribute("name"), NULL, -0.98f, mYOffset);
      cMenuItems.push_back(mMenuItem);
      mMenuItem->getWidth();
      mYOffset -= 0.05f;*/
    if (mValueAsString == "MenuItem") {
      ICommand* mCommand = parseCommand(mNode);
      std::string mPathElementName = mNode->getAttribute("name");
      addMenuItem(mPathElementName, mCommand);
    } else if (mValueAsString == "Plugins") {
      // TODO: Make this non-platform specific
      std::vector<std::string>* mPluginTypes = System::getFileList("/usr/share/IsoRealms/Plugins/");
      for (unsigned int i = 0; i < mPluginTypes->size(); i++) {
        ICommand* mPluginInstancesCommand = new ChoosePluginImplementationCommand(NULL, NULL, componentContainer, pluginRegistryAccessor, (*mPluginTypes)[i]);
        addMenuItem((*mPluginTypes)[i], mPluginInstancesCommand);
      }
    }
  }
}

MenuPopup::MenuPopup(IMenuContainer* parent, float x, float y) {
  cX = x;
  cY = y;
  cParent = parent;
  cSelectedItem = 0;
}

void MenuPopup::addMenuItem(std::string name, ICommand* command) {
  float mXDestination = getX() + 0.02f;
  float mYDestination = getY() + getHeight() - 0.04f;
  MenuItem* mMenuItem = new MenuItem(name, command, mXDestination, mYDestination);
  cMenuItems.push_back(mMenuItem);
}

void MenuPopup::addCommand(std::vector<std::string> path, ICommandInfo* command) {
  std::string mPathElementName = path[path.size() - 1];
  if (path.size() > 1) {
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
  } else {
    ICommand* mCommand = command->getCommand();
    addMenuItem(mPathElementName, mCommand);
    // TODO: Make a new menu item here!
  }
}

// TODO: return (i.e. support) a vector
ICommand* MenuPopup::parseCommand(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ExecuteEditorCommand") {
      std::string mCommandName = mNode->getAttribute("type");
      return EditorCommandManager::getCommand(mCommandName);
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
/*  if (cMenuPopupShowing != NULL) {
    cMenuPopupShowing->render();
  }*/
}

bool MenuPopup::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  for (unsigned int i = 0; i < cMenuItems.size(); i++) {
    // TODO: Test the width of the popup, not just the menu item text!
    if (cMenuItems[i]->testClick(mX, mY)) {
      cParent->closeMenu(this);
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
      cMenuItems[cSelectedItem]->execute();
      cParent->closeMenu(this);
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



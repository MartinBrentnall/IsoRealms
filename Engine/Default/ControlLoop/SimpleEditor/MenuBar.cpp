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
#include "MenuBar.h"

MenuBar::MenuBar(IComponentContainer* componentContainer, DOMNodeWrapper* node, IFont* font) {
  cComponentContainer = componentContainer;
  cFont = font;
  cMenuPopupShowing = NULL;
  cSelectedItem = 0;

  float mXOffset = getLeft() + 0.01f;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    
    if (mValueAsString == "Menu") {
      MenuPopup* mPopupMenu = new MenuPopup(mNode, this, mXOffset, 0.95f);
      PopupMenuCommand* mPopupMenuCommand = new PopupMenuCommand(cMenuPopupShowing, mPopupMenu);
      std::string mMenuItemName = mNode->getAttribute("name");
      MenuItem* mMenuItem = new MenuItem(mMenuItemName, mPopupMenuCommand, mXOffset, 0.96f);
      cMenuItems.push_back(mMenuItem);
      mXOffset += mMenuItem->getWidth() + 0.05f;
      unsigned int mAcceleratorIndex = mMenuItemName.find('^');
      if (mAcceleratorIndex != std::string::npos) {
        mMenuItemName = mMenuItemName.substr(0, mAcceleratorIndex) + mMenuItemName.substr(mAcceleratorIndex + 1);
      }
      cMenuPopups[mMenuItemName] = mPopupMenu;
    }
  }
}

void MenuBar::addCommand(ICommandInfo* commandInfo) {
  std::vector<std::string> mCommandPath = commandInfo->getCommandPath();
  std::string mPathElementName = mCommandPath[mCommandPath.size() - 1];
  if (mCommandPath.size() > 1) {
    std::map<std::string, MenuPopup*>::iterator mIterator = cMenuPopups.find(mPathElementName);
    MenuPopup* mPopupMenu;
    if (mIterator == cMenuPopups.end()) {
      mPopupMenu = new MenuPopup(this, getRight() - 0.01f, 0.95f);
      cMenuPopups[mPathElementName] = mPopupMenu;
      PopupMenuCommand* mPopupMenuCommand = new PopupMenuCommand(cMenuPopupShowing, mPopupMenu);
      MenuItem* mMenuItem = new MenuItem(mPathElementName, mPopupMenuCommand, getRight() - 0.01f, 0.96f);
      cMenuItems.push_back(mMenuItem);
    } else {
      mPopupMenu = mIterator->second;
    }
    mCommandPath.pop_back();
    mPopupMenu->addCommand(mCommandPath, commandInfo);
  } else {
    std::cout << "WARNING!  You probably don't want to add a menu item at the top level!" << std::endl;
    // TODO: Make a new menu item here!
  }
}

void MenuBar::removeCommand(ICommandInfo*) {
}

void MenuBar::update(int milliseconds) {
  // TODO: Maybe some nice animation?
}

void MenuBar::render() {

  // Render the bar
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();
  glDisable(GL_DEPTH_TEST);

  float mLeft = getLeft();
  float mBottom = getBottom();
  float mRight = getRight();
  float mTop = getTop();
  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glColor4f(0.05f, 0.0f, 0.1f, 0.85f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glEnd();
  glDisable(GL_BLEND);

  glBegin(GL_LINE_LOOP);
  glColor3f(0.8f, 0.6f, 1.0f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
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
  if (cMenuPopupShowing != NULL) {
    cMenuPopupShowing->render();
  }
}

bool MenuBar::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  for (unsigned int i = 0; i < cMenuItems.size(); i++) {
    if (cMenuItems[i]->testClick(mX, mY)) {
      cComponentContainer->bringComponentToFront(this);
      return true;
    }
  }
  cMenuPopupShowing = NULL;
  return false;
}

void MenuBar::closeMenu(IMenu* menu) {
  if (cMenuPopupShowing == menu) {
    cMenuPopupShowing = NULL;
  }
}

float MenuBar::getLeft() {
  return -1.0f;
}

float MenuBar::getBottom() {
  return 0.95f;
}

float MenuBar::getTop() {
  return 1.0f;
}

float MenuBar::getRight() {
  float mWidth = -0.98f;
  for (unsigned int i = 0; i < cMenuItems.size(); i++) {
    mWidth += cMenuItems[i]->getWidth() + 0.05f;
  }
  return mWidth;
}

bool MenuBar::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_RIGHT: {
      // TODO: Should ONLY excute if it's a sub-menu!
      if (cSelectedItem < cMenuItems.size() - 1) {
        cSelectedItem++;
      } else {
        cSelectedItem = 0;
      }
      return true;
    }

    case SDLK_LEFT: {
      // TODO: Close sub-popup-menu!
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

bool MenuBar::mouseMotion(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  for (unsigned int i = 0; i < cMenuItems.size(); i++) {
    if (cMenuItems[i]->contains(mX, mY)) {
      cSelectedItem = i;
      if (cMenuPopupShowing != NULL) {
        // TODO: Should ONLY excute if it's a sub-menu!
        cMenuItems[i]->execute();
      }
      return false;
    }
  }
  return false;
}

bool MenuBar::input(SDL_Event& event) {
  if (cMenuPopupShowing != NULL) {
    if (cMenuPopupShowing->input(event)) {
      return true;
    }
  }

  switch (event.type) {
    case SDL_KEYDOWN: {
      for (unsigned int i = 0; i < cMenuItems.size(); i++) {
        if (cMenuItems[i]->input(event)) {
          cComponentContainer->bringComponentToFront(this);
          cSelectedItem = i;
          return true;
        }
      }

      if (keyDown(event.key.keysym.sym)) {
        cComponentContainer->bringComponentToFront(this);
        return true;
      }
      return false;
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

bool MenuBar::contains(float x, float y) {
  return (x >= getLeft() && x <= getRight() && y >= getBottom() && y <= getTop())
      || (cMenuPopupShowing != NULL && cMenuPopupShowing->contains(x, y));
}


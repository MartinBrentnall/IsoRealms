/*
 * Copyright 2015 Martin Brentnall
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
#include "ScreenEdge.h"

const float ScreenEdge::TAB_SIZE = 0.08f;
const float ScreenEdge::TAB_WIDTH = 0.08f;

ScreenEdge::ScreenEdge() {
  cAnimation = 1.0f;
  cDragging = nullptr;
  cDragged = false;
  cExpandedDialog = nullptr;
}

void ScreenEdge::update(int milliseconds) {
  if (cAnimation < 1.0f) {
    cAnimation += milliseconds * 0.0025f;
    if (cAnimation > 1.0f) {
      cAnimation = 1.0f;
    }
    float mLocation = sine(1.0f, 1.0f - (cExpandedDialog->getPreferredSize() + getTabWidth(cExpandedDialog)), cAnimation);
    cExpandedDialog->setSize(mLocation, -1.0f, mLocation + cExpandedDialog->getPreferredSize(), 1.0f);
  }
  if (cExpandedDialog != nullptr) {
    cExpandedDialog->update(milliseconds);
  }
  for (std::map<DockedDialog*, float>::iterator i = cCollapsingDialogs.begin(); i != cCollapsingDialogs.end(); i++) {
    i->second += milliseconds * 0.0025f;
    if (i->second > 1.0f) {
      cCollapsingDialogs.erase(i);
    } else {
      float mLocation = sine(1.0f - (i->first->getPreferredSize() + getTabWidth(i->first)), 1.0f, i->second);
      i->first->setSize(mLocation, -1.0f, mLocation + i->first->getPreferredSize(), 1.0f);
    }
  }
}

void ScreenEdge::render() {
  for (std::map<DockedDialog*, float>::iterator i = cCollapsingDialogs.begin(); i != cCollapsingDialogs.end(); i++) {
    i->first->render();
  }
  if (cExpandedDialog != nullptr) {
    cExpandedDialog->render();
  }
  for (unsigned int i = 0; i < cDockedDialogs.size(); i++) {
    float mX = getTabX(cDockedDialogs[i]);
    float mY = getTabY(cDockedDialogs[i]);
    renderTab(cDockedDialogs[i], mX, mY);
    cDockedDialogs[i]->renderIcon();
  }
}

bool ScreenEdge::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  for (unsigned int i = 0; i < cDockedDialogs.size(); i++) {
    float mLeft   = getTabX(cDockedDialogs[i]);
    float mTop    = getTabY(cDockedDialogs[i]);
    float mRight  = mLeft + getTabWidth(cDockedDialogs[i]);
    float mBottom = mTop -  getTabHeight(cDockedDialogs[i]);
    if (mX >= mLeft && mX <= mRight && mY >= mBottom && mY <= mTop) {
      cDragging = cDockedDialogs[i];
      cDragged = false;
      return true;
    }
  }
  
  // This is supposed to make the expanded dialog collapse when clicking outside
  // it.  However, I'm not sure if this is what we really want.  Furthermore, it
  // is still very buggy, as it causes the dialog to collapse even when clicking
  // on things in the dialogs themselves.
//   if (cExpandedDialog != nullptr) {
//     cCollapsingDialogs[cExpandedDialog] = 0.0f;
//     cExpandedDialog = nullptr;
//   }
  return false;
}

bool ScreenEdge::mouseButtonUp(SDL_Event& event) {
  if (cDragging != nullptr) {
    if (!cDragged) {
      if (cDragging != cExpandedDialog) {
        if (cExpandedDialog != nullptr) {
          cCollapsingDialogs[cExpandedDialog] = 0.0f;
        }
        cExpandedDialog = cDragging;
        cAnimation = 0.0f;
      } else {
        cCollapsingDialogs[cExpandedDialog] = 0.0f;
        cExpandedDialog = nullptr;
      }
    }
    cDragging = nullptr;
    cDragged = false;
    return true;
  }
  return false;
}

bool ScreenEdge::mouseMotion(SDL_Event& event) {
  if (cDragging != nullptr) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(event.motion.xrel) + 1.0f;
    float mY = mScreen->getYLocation(event.motion.yrel) - 1.0f;
    moveTab(cDragging, mX, mY);
    cDragged = true;
    return true;
  }
  return false;
}

bool ScreenEdge::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      if (mouseButtonDown(event)) {
        return true;
      }
      break;
    }
    
    case SDL_MOUSEBUTTONUP: {
      if (mouseButtonUp(event)) {
        return true;
      }
      break;
    }
    
    case SDL_MOUSEMOTION: {
      if (mouseMotion(event)) {
        return true;
      }
      break;
    }
  }
  return cExpandedDialog != nullptr ? cExpandedDialog->input(event) : false;
}

bool ScreenEdge::contains(float x, float y) {
  for (unsigned int i = 0; i < cDockedDialogs.size(); i++) {
    float mLeft   = getTabX(cDockedDialogs[i]);
    float mTop    = getTabY(cDockedDialogs[i]);
    float mRight  = getTabWidth(cDockedDialogs[i]) + mLeft;
    float mBottom = getTabHeight(cDockedDialogs[i]) - mTop;
    if (x >= mLeft && x <= mRight && y >= mBottom && y <= mTop) {
      return true;
    }
    if (cDockedDialogs[i] == cExpandedDialog) {
      if (cExpandedDialog->contains(x, y)) {
        return true;
      }
    }
  }
  return false;
}

void ScreenEdge::add(Dialog* dialog, AbstractRectangularComponent* icon, float preferredSize) {
  dialog->setMovable(false);
  dialog->setClosable(false);
  DockedDialog* mDockedDialog = new DockedDialog(dialog, icon, preferredSize);
  cDockedDialogs.push_back(mDockedDialog);
  IComponentBoundsCalculator* mIconLayout = new TabIconLayout(this, mDockedDialog);
  icon->setBoundsCalculator(mIconLayout);
}

ScreenEdge::TabIconLayout::TabIconLayout(ScreenEdge* parent, DockedDialog* dialog) {
  cParent = parent;
  cDialog = dialog;
}
    
float ScreenEdge::TabIconLayout::getTop() {
  return cParent->getTabY(cDialog);
}

float ScreenEdge::TabIconLayout::getBottom() {
  return getTop() - cParent->getTabHeight(cDialog);
}

float ScreenEdge::TabIconLayout::getLeft() {
  return cParent->getTabX(cDialog);
}

float ScreenEdge::TabIconLayout::getRight() {
  return getLeft() + cParent->getTabWidth(cDialog);
}


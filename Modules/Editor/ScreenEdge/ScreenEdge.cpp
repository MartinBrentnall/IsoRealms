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
}

void ScreenEdge::update(int milliseconds) {
  if (cAnimation < 1.0f) {
    cAnimation += milliseconds * 0.0025f;
    if (cAnimation > 1.0f) {
      cAnimation = 1.0f;
    }
    float mLocation = sine(1.0f, 0.2f, cAnimation);
    cExpandedDialog->setSize(mLocation, -1.0f, mLocation + 0.8f, 1.0f);
  }
  if (cExpandedDialog != NULL) {
    cExpandedDialog->update(milliseconds);
  }
  for (std::map<DockedDialog*, float>::iterator i = cCollapsingDialogs.begin(); i != cCollapsingDialogs.end(); i++) {
    i->second += milliseconds * 0.0025f;
    if (i->second > 1.0f) {
      cCollapsingDialogs.erase(i);
    } else {
      float mLocation = sine(0.2f, 1.0f, cAnimation);
      i->first->setSize(mLocation, -1.0f, mLocation + 0.8f, 1.0f);
    }
  }
}

void ScreenEdge::render() {
  for (std::map<DockedDialog*, float>::iterator i = cCollapsingDialogs.begin(); i != cCollapsingDialogs.end(); i++) {
    i->first->render();
  }
  if (cExpandedDialog != NULL) {
    cExpandedDialog->render();
  }
  for (unsigned int i = 0; i < cDockedDialogs.size(); i++) {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();
    glDisable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D, 0);
    glLoadIdentity();
    float mX = getTabX(cDockedDialogs[i]);
    float mY = getTabY(cDockedDialogs[i]);
    renderTab(cDockedDialogs[i], mX, mY);
    cDockedDialogs[i]->renderIcon();
  
    glLoadIdentity();  
    glEnable(GL_DEPTH_TEST);

    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();  
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
      if (cDockedDialogs[i] != cExpandedDialog) {
        if (cExpandedDialog != NULL) {
          cCollapsingDialogs[cExpandedDialog] = 0.0f;
        }
        cExpandedDialog = cDockedDialogs[i];
        cAnimation = 0.0f;
      } else {
        cCollapsingDialogs[cExpandedDialog] = 0.0f;
        cExpandedDialog = NULL;
      }
      return true;
    }
  }
  return false;
}

bool ScreenEdge::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      if (mouseButtonDown(event)) {
        return true;
      }
    }
  }
  return cExpandedDialog != NULL ? cExpandedDialog->input(event) : false;
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

void ScreenEdge::add(Dialog* dialog, AbstractRectangularComponent* icon) {
  DockedDialog* mDockedDialog = new DockedDialog(dialog, icon);
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


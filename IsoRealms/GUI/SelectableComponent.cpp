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
#include "SelectableComponent.h"

SelectableComponent::SelectableComponent() {
  cHandler = NULL;
  cSelected = false;
}

SelectableComponent::SelectableComponent(ISelector* handler) {
  cSelected = false;
  setHandler(handler);
}

void SelectableComponent::setHandler(ISelector* handler) {
  cHandler = handler;
}

void SelectableComponent::update(unsigned int) {
}

void SelectableComponent::render() {
  float mLeft = getLeft();
  float mRight = getRight();
  float mTop = getTop();
  float mBottom = getBottom();
  glBindTexture(GL_TEXTURE_2D, 0);
  if (cHandler != NULL) {
    cHandler->render(this);
  }
  if (cSelected) {
    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(mLeft,  mBottom);
    glVertex2f(mRight, mBottom);
    glVertex2f(mRight, mTop);
    glVertex2f(mLeft,  mTop);
    glEnd();
    glLineWidth(1.0f);
  }  
}

bool SelectableComponent::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      Configuration* mConfiguration = Configuration::getInstance();
      ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
      float mX = mScreen->getXLocation(event.button.x);
      float mY = mScreen->getYLocation(event.button.y);
      if (contains(mX, mY)) {
//	gainedFocus();
        return true;
      }
    }
  }
  return false;
}

void SelectableComponent::gainedFocus() {
  if (!cSelected && cHandler != NULL) {
    cSelected = true;
    cHandler->selected();
  }
}

void SelectableComponent::lostFocus() {
  if (cSelected && cHandler != NULL) {
    cSelected = false;
    cHandler->deselected();
  }
}

float SelectableComponent::getWidth() {
  return 0.3f;
}

float SelectableComponent::getHeight() {
  return 0.05f;
}


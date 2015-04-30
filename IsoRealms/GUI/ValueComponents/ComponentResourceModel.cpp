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
#include "ComponentResourceModel.h"

ComponentResourceModel::ComponentResourceModel(float size, IResourceSelector* resourceSelector) {
  setValue(nullptr);
  cSize = size;
  cResourceSelector = resourceSelector;
  cHasFocus = false;
}

void ComponentResourceModel::setValue(I3DModelType* value) {
  cValue = value;
}

I3DModelType* ComponentResourceModel::getValue() {
  return cValue;
}

void ComponentResourceModel::addValueListener(IValueListener<I3DModelType*>* listener) {
  cListeners.push_back(listener);
}

float ComponentResourceModel::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  return cSize * mAspectRatio;
}

float ComponentResourceModel::getHeight() {
  return cSize;
}

void ComponentResourceModel::update(unsigned int milliseconds) {
  // TODO: Implement this
}

void ComponentResourceModel::render() {
  float mLeft = getLeft();
  float mRight = getRight();
  float mTop = getTop();
  float mBottom = getBottom();
  glBindTexture(GL_TEXTURE_2D, 0);
  if (cHasFocus) {
    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
  } else {
    glColor3f(0.45f, 0.0f, 0.9f);
  }
  glBegin(GL_LINE_LOOP);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glVertex2f(mLeft,  mTop);
  glEnd();
  glLineWidth(1.0f);
  
  glPushMatrix();
  float mScale = cSize / 2.0f;
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mXLocation = getLeft() + (getRight() - getLeft()) / 2.0f;
  float mYLocation = getBottom() + (getTop() - getBottom()) / 2.0f;
  glTranslatef(mXLocation, mYLocation, 0.0f);
  float mAspectRatio = mScreen->getAspectRatio();
  glScalef(mAspectRatio * mScale, mScale, mScale);
  glColor3f(1.0f, 1.0f, 1.0f);
  cValue->renderIcon();
  glPopMatrix();
}

bool ComponentResourceModel::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      // TODO:
    }
    
    case SDL_MOUSEBUTTONDOWN: {
      return true;
    }      
  }
  return false;
}

void ComponentResourceModel::gainedFocus() {
  cHasFocus = true;
  cResourceSelector->addResourceSelectionListener(this);
}

void ComponentResourceModel::lostFocus() {
  cResourceSelector->removeResourceSelectionListener(this);
  cHasFocus = false;
}

void ComponentResourceModel::fireChange() {
  std::cout << "There are " << cListeners.size() << " listeners for " << this << "!" << std::endl;
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->valueChanged(this, cValue);
  }
}

void ComponentResourceModel::resourceSelected(I3DModelType* value) {
  cValue = value;
  fireChange();
}

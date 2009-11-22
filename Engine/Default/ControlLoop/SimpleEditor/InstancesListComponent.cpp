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
#include "InstancesListComponent.h"

IFont* InstancesListComponent::cFont = NULL;

void InstancesListComponent::setFont(IFont* font) {
  cFont = font;
}

InstancesListComponent::InstancesListComponent(IMovableComponent* parent, ElementSetRegistry* elementSetRegistry) {
  cParent = parent;
  cElementSetRegistry = elementSetRegistry;
  cSelectedInstance = 0;
}

void InstancesListComponent::render() {
  glColor3f(1.0f, 1.0f, 1.0f);
  std::vector<std::string*> mElementSetInstances = cElementSetRegistry->getElementSets();
  float mXOffset = cParent->getX() + 0.02f;
  float mYOffset = (cParent->getY() + cParent->getHeight()) - 0.02f;
  float mLine = mYOffset - 0.05f;
  for (unsigned int i = 0; i < mElementSetInstances.size(); i++) {
    float mBrightness = i == cSelectedInstance ? 1.0f : 0.4f;
    glColor3f(1.0f * mBrightness, 1.0f * mBrightness, 1.0f * mBrightness);
    cFont->print(mXOffset, mLine, 0.02f, 0, mElementSetInstances[i]->c_str());
    mLine -= 0.05f;
  }
  float mX = getX();
  float mY = getY();
  float mWidth = getWidth();
  float mHeight = getHeight();
  glColor3f(1.0f, 1.0f, 1.0f);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINE_LOOP);
  glVertex2f(mX,          mY + mHeight);
  glVertex2f(mX + mWidth, mY + mHeight);
  glVertex2f(mX + mWidth, mY);
  glVertex2f(mX,          mY);
  glEnd();
}

IElementSet* InstancesListComponent::getSelectedElementSet() {
  std::vector<std::string*> mElementSetInstances = cElementSetRegistry->getElementSets();
  return cElementSetRegistry->getElementSet(mElementSetInstances[cSelectedInstance]);
}

void InstancesListComponent::update(int milliseconds) {
}

bool InstancesListComponent::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_DOWN: {
      std::vector<std::string*> mElementSetInstances = cElementSetRegistry->getElementSets();
      if (cSelectedInstance < mElementSetInstances.size() - 1) {
        cSelectedInstance++;
      }
      return true;
    }

    case SDLK_UP: {
      if (cSelectedInstance > 0) {
        cSelectedInstance--;
      }
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool InstancesListComponent::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }

    case SDL_MOUSEBUTTONDOWN: {
      // TODO: Implement mouse selection!
      return true;
    }

    default: {
      return false;
    }
  }
  return false;  
}

float InstancesListComponent::getX() {
  return cParent->getX() + 0.02f;
}

float InstancesListComponent::getY() {
  return (cParent->getY() + cParent->getHeight()) - (getHeight() + 0.02f);
}

float InstancesListComponent::getWidth() {
  return 0.6f;
}

float InstancesListComponent::getHeight() {
  // TODO: Dynamic, depending on number of element set instances.
  return 0.6f;
}

bool InstancesListComponent::contains(float x, float y) {
  return x >= getX() && x <= getX() + getWidth() && y >= getY() && y <= getY() + getHeight();
}



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

//TODO: CLASS REQUIRES REFACTOR FOR NEW COMPONENT FRAMEWORK!
void InstancesListComponent::setFont(IFont* font) {
  cFont = font;
}

InstancesListComponent::InstancesListComponent(IInstantiable* instantiator) {
  cInstantiator = instantiator;
  cSelectedInstance = 0;
}

void InstancesListComponent::render() {
  std::vector<std::string*> mInstances = cInstantiator->getInstances();

  float mLeft = getLeft();
  float mBottom = getBottom();
  float mRight = getRight();
  float mTop = getTop();

  float mXOffset = mLeft + 0.02f;
  float mLine = mTop - 0.05f;
  for (unsigned int i = 0; i < mInstances.size(); i++) {
    float mBrightness = i == cSelectedInstance ? 1.0f : 0.4f;
    glColor3f(1.0f * mBrightness, 1.0f * mBrightness, 1.0f * mBrightness);
    cFont->print(mXOffset, mLine, 0.02f, 0, mInstances[i]->c_str());
    mLine -= 0.05f;
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glColor3f(0.45f, 0.0f, 0.9f);
  glBegin(GL_LINE_LOOP);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mRight, mTop);
  glVertex2f(mRight, mBottom);
  glVertex2f(mLeft,  mBottom);
  glEnd();
}

std::string* InstancesListComponent::getSelectedInstance() {
  std::vector<std::string*> mInstances = cInstantiator->getInstances();
  return mInstances[cSelectedInstance];
}

void InstancesListComponent::update(int milliseconds) {
}

bool InstancesListComponent::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_DOWN: {
      std::vector<std::string*> mInstances = cInstantiator->getInstances();
      if (cSelectedInstance < mInstances.size() - 1) {
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



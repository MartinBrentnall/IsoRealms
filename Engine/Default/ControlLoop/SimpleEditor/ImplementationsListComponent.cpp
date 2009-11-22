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
#include "ImplementationsListComponent.h"

IFont* ImplementationsListComponent::cFont = NULL;

void ImplementationsListComponent::setFont(IFont* font) {
  cFont = font;
}

ImplementationsListComponent::ImplementationsListComponent(IRectangularComponent* parent, IRectangularComponent::Edge edge, float offset, std::vector<std::string> implementations) {
  cParent = parent;
  cEdge = edge;
  cOffset = offset;
  cImplementations = implementations;
  cSelectedImplementation = 0;
}

void ImplementationsListComponent::render() {
  float mX = getX();
  float mY = getY();
  float mWidth = getWidth();
  float mHeight = getHeight();

  float mXOffset = mX + 0.02f;
  float mLine = (mY + mHeight) - 0.05f;
  for (unsigned int i = 0; i < cImplementations.size(); i++) {
    float mBrightness = i == cSelectedImplementation ? 1.0f : 0.4f;
    glColor3f(1.0f * mBrightness, 1.0f * mBrightness, 1.0f * mBrightness);
    cFont->print(mXOffset, mLine, 0.02f, 0, cImplementations[i].c_str());
    mLine -= 0.05f;
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINE_LOOP);
  glVertex2f(mX,          mY + mHeight);
  glVertex2f(mX,          mY);
  glVertex2f(mX + mWidth, mY);
  glVertex2f(mX + mWidth, mY + mHeight);
  glEnd();  
}

void ImplementationsListComponent::update(int milliseconds) {
}

bool ImplementationsListComponent::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_DOWN: {
      cSelectedImplementation++;
      if (cSelectedImplementation >= cImplementations.size()) {
        cSelectedImplementation = cImplementations.size() - 1;
      }
      return true;
    }
 
    case SDLK_UP: {
      if (cSelectedImplementation != 0) {
        cSelectedImplementation--;
      }
      return true;
    }

    default: {
      return true;
    }
  }
  return true;
}

bool ImplementationsListComponent::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

std::string ImplementationsListComponent::getSelectedImplementation() {
  return cImplementations[cSelectedImplementation];
}

float ImplementationsListComponent::getX() {
  switch (cEdge) {
    case TOP:    return cParent->getX();
    case BOTTOM: return cParent->getX();
    case LEFT:   return (cParent->getX() - cOffset) - getWidth();
    case RIGHT:  return cParent->getX() + cParent->getWidth() + cOffset;
  }
  throw new IllegalStateException();
}

float ImplementationsListComponent::getY() {
  switch (cEdge) {
    case TOP:    return cParent->getY() + cParent->getHeight() + cOffset;
    case BOTTOM: return (cParent->getY() - cOffset) - getHeight();
    case LEFT:   return cParent->getY();
    case RIGHT:  return cParent->getY();
  }
  throw new IllegalStateException();
}

float ImplementationsListComponent::getWidth() {
  return 0.6f;
}

float ImplementationsListComponent::getHeight() {
  return 0.6f;
}

bool ImplementationsListComponent::contains(float x, float y) {
  return x >= getX() && x <= getX() + getWidth() && y >= getY() && y <= getY() + getHeight();
}

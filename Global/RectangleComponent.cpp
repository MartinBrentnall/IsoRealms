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
#include "RectangleComponent.h"

RectangleComponent::RectangleComponent(IComponentContainer* componentContainer, float x, float y, float width, float height) {
  cComponentContainer = componentContainer;
  cX = x;
  cY = y;
  cWidth = width;
  cHeight = height;
  cDragging = false;
}

float RectangleComponent::getLeft() {
  return cX;
}

float RectangleComponent::getBottom() {
  return cY;
}

float RectangleComponent::getRight() {
  return cX + cWidth;
}

float RectangleComponent::getTop() {
  return cY + cHeight;
}

bool RectangleComponent::contains(float x, float y) {
  return x >= cX && x <= cX + cWidth && y >= cY && y <= cY + cHeight;
}

void RectangleComponent::translate(float x, float y) {
  cX += x;
  cY += y;
}

void RectangleComponent::resize(float x, float y) {
  cWidth += x;
  cHeight += y;
}

void RectangleComponent::update(int milliseconds) {
  updateContent(milliseconds);
}

void RectangleComponent::render() {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();
  glDisable(GL_DEPTH_TEST);

  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glColor4f(0.05f, 0.0f, 0.1f, 0.85f);
  glVertex2f(cX,          cY + cHeight);
  glVertex2f(cX,          cY);
  glVertex2f(cX + cWidth, cY);
  glVertex2f(cX + cWidth, cY + cHeight);
  glEnd();
  glDisable(GL_BLEND);
  
  glBegin(GL_LINE_LOOP);
  glColor3f(0.8f, 0.6f, 1.0f);
  glVertex2f(cX,          cY + cHeight);
  glVertex2f(cX,          cY);
  glVertex2f(cX + cWidth, cY);
  glVertex2f(cX + cWidth, cY + cHeight);
  glEnd();

  renderContent();

  glLoadIdentity();  
  glEnable(GL_DEPTH_TEST);

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
}

bool RectangleComponent::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (contains(mX, mY)) {
    cComponentContainer->bringComponentToFront(this);
    cDragging = true;
    return true;
  }
  return false;
}

bool RectangleComponent::mouseMotion(SDL_Event& event) {
  if (cDragging) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(event.motion.xrel) + 1.0f;
    float mY = mScreen->getYLocation(event.motion.yrel) - 1.0f;
    translate(mX, mY);
    return true;
  }
  return false;
}

void RectangleComponent::close() {
  cComponentContainer->removeComponent(this);
}

bool RectangleComponent::input(SDL_Event& event) {
  if (inputContent(event)) {
    return true;
  }
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      return mouseButtonDown(event);
    }

    case SDL_MOUSEBUTTONUP: {
      cDragging = false;
      return false;
    }

    case SDL_MOUSEMOTION: {
      return mouseMotion(event);
    }
  }
  return false;
}


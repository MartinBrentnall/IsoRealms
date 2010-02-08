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
#include "Button.h"

IFont* Button::cFont = NULL;

void Button::setFont(IFont* font) {
  cFont = font;
}

Button::Button(IComponentBoundsCalculator* boundsCalculator, ICommand* command, std::string text) {
  setBoundsCalculator(boundsCalculator);
  cCommand = command;
  cText = text;
}

void Button::update(int milliseconds) {
}

void Button::render() {
  float mLeft = getLeft();
  float mBottom = getBottom();
  float mRight = getRight();
  float mTop = getTop();

  glColor3f(1.0f, 1.0f, 1.0f);
  cFont->print(mLeft + (mRight - mLeft) * 0.5f, mBottom + 0.01f, 0.02f, 1, cText.c_str());

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();
  glDisable(GL_DEPTH_TEST);

  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();

  glColor3f(0.45f, 0.0f, 0.9f);
  glBegin(GL_LINE_LOOP);
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

  // TODO: Button accelerator
/*  float mWidth = ;
  cFont->print(cXOffset + mWidth, cYOffset, 0.02f, 0, cText.substr(cAcceleratorIndex, 1).c_str());*/
}

float Button::getWidth() {
  return cFont->getWidth(0.02f, cText.c_str()) + 0.04f;
}

float Button::getHeight() {
  return 0.05f; // TODO: Make this correct
}

bool Button::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (contains(mX, mY)) {
    cCommand->execute();
    return true;
  }
  return false;
}

bool Button::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      return mouseButtonDown(event);
    }
  }
  return false;
}


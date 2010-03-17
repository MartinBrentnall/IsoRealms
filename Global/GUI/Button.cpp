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

Button::Button(IComponentBoundsCalculator* boundsCalculator, ICommand* command, const std::string& text) {
  setBoundsCalculator(boundsCalculator);
  cButtonPressed = false;
  cHovering = false;
  cCommand = command;
  cText = text;
}

Button::Button() {
  cButtonPressed = false;
  cHovering = false;
  cCommand = NULL;
  cText = "";
}

void Button::setText(const std::string& text) {
  cText = text;
}

void Button::update(int milliseconds) {
}

void Button::render() {
  float mLeft = getLeft();
  float mBottom = getBottom();
  float mRight = getRight();
  float mTop = getTop();

  if (cHovering) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.85f, 0.0f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(mLeft,  mTop);
    glVertex2f(mLeft,  mBottom);
    glVertex2f(mRight, mBottom);
    glVertex2f(mRight, mTop);
    glEnd();  
  }

  glColor3f(1.0f, 1.0f, 1.0f);
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  mFont->print(mLeft + (mRight - mLeft) * 0.5f, mBottom + 0.01f, mFontSize, 1, cText.c_str());

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

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

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  // TODO: Button accelerator
/*  float mWidth = ;
  cFont->print(cXOffset + mWidth, cYOffset, 0.02f, 0, cText.substr(cAcceleratorIndex, 1).c_str());*/
}

float Button::getWidth() {
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  return mFont->getWidth(mFontSize, cText.c_str()) + 0.04f;
}

float Button::getHeight() {
  return 0.05f; // TODO: Make this correct
}

bool Button::mouseMotion(SDL_Event& event) {
  if (cButtonPressed) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(event.button.x);
    float mY = mScreen->getYLocation(event.button.y);
    cHovering = contains(mX, mY);
  }
  return false;
}

bool Button::mouseButtonUp(SDL_Event& event) {
  if (cHovering) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(event.button.x);
    float mY = mScreen->getYLocation(event.button.y);
    cHovering = false;
    if (contains(mX, mY)) {
      cCommand->execute();
      return true;
    }
  }
  cButtonPressed = false;
  return false;
}

bool Button::mouseButtonDown(SDL_Event& event) {
  if (event.button.button == SDL_BUTTON_LEFT) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(event.button.x);
    float mY = mScreen->getYLocation(event.button.y);
    if (contains(mX, mY)) {
      cHovering = true;
      cButtonPressed = true;
      return true;
    }
  }
  return false;
}

bool Button::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      return mouseButtonDown(event);
    }

    case SDL_MOUSEBUTTONUP: {
      return mouseButtonUp(event);
    }

    case SDL_MOUSEMOTION: {
      return mouseMotion(event);
    }
  }
  return false;
}


/*
 * Copyright 2016 Martin Brentnall
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
#include "ComponentScriptCall.h"

ComponentScriptCall::ComponentScriptCall(IScriptCall* scriptCall, float size) {
  cScriptCall = scriptCall;
  cSize = size;
  cHasFocus = false;
}

float ComponentScriptCall::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  return cSize * mAspectRatio;
}

float ComponentScriptCall::getHeight() {
  return 0.05f;
}

void ComponentScriptCall::update(unsigned int milliseconds) {
  // TODO: Implement this
}

void ComponentScriptCall::render() {
  float mLeft   = getLeft();
  float mRight  = getRight();
  float mTop    = getTop();
  float mBottom = getBottom();
  glBindTexture(GL_TEXTURE_2D, 0);
  if (cHasFocus) {
    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
//   } else {
//     glColor3f(0.45f, 0.0f, 0.9f);
//   }
    glBegin(GL_LINE_LOOP);
    glVertex2f(mLeft,  mBottom);
    glVertex2f(mRight, mBottom);
    glVertex2f(mRight, mTop);
    glVertex2f(mLeft,  mTop);
    glEnd();
  }
  
  glColor3f(1.0f, 1.0f, 1.0f);
  if (cScriptCall != nullptr) {
    IFont* mFont = LookAndFeel::getDefaultFont();
    float mFontSize = LookAndFeel::getDefaultFontSize();
    std::string mScriptCallName = cScriptCall->getScriptName();
    mFont->print(mLeft + 0.01f, mBottom + 0.01f, mFontSize, IFont::LEFT, mScriptCallName.c_str());
  }
}

bool ComponentScriptCall::input(SDL_Event& event) {
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

void ComponentScriptCall::gainedFocus() {
  cHasFocus = true;
}

void ComponentScriptCall::lostFocus() {
  cHasFocus = false;
}

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
#include "ComponentDigitalInputMapping.h"

SDLKeyMap ComponentDigitalInputMapping::cSDLKeyMap;

ComponentDigitalInputMapping::ComponentDigitalInputMapping(std::vector<SDLKey> keys, float size) {
  cKeys = keys;
  cSize = size;
  cHasFocus = false;
}

float ComponentDigitalInputMapping::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  return cSize * mAspectRatio;
}

float ComponentDigitalInputMapping::getHeight() {
  return 0.05f;
}

void ComponentDigitalInputMapping::update(unsigned int milliseconds) {
  // TODO: Implement this
}

void ComponentDigitalInputMapping::render() {
  float mLeft   = getLeft();
  float mRight  = getRight();
  float mTop    = getTop();
  float mBottom = getBottom();
  glBindTexture(GL_TEXTURE_2D, 0);
  if (cHasFocus) {
    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(mLeft,  mBottom);
    glVertex2f(mRight, mBottom);
    glVertex2f(mRight, mTop);
    glVertex2f(mLeft,  mTop);
    glEnd();
  }
  
  glColor3f(1.0f, 1.0f, 1.0f);
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  std::string mKeysString;
  bool mFirst = true;
  for (SDLKey mKey : cKeys) {
    if (!mFirst) {
      mKeysString += ", ";
    }
    mKeysString += cSDLKeyMap.getName(mKey);
    mFirst = false;
  }
  mFont->print(mLeft + 0.01f, mBottom + 0.01f, mFontSize, IFont::LEFT, mKeysString.c_str());
}

bool ComponentDigitalInputMapping::input(SDL_Event& event) {
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

void ComponentDigitalInputMapping::gainedFocus() {
  cHasFocus = true;
}

void ComponentDigitalInputMapping::lostFocus() {
  cHasFocus = false;
}


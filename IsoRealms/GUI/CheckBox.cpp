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
#include "CheckBox.h"

CheckBox::CheckBox(const std::string& label) {
  setValue(false);
  cLabel = label;
}

void CheckBox::setValue(bool value) {
  cValue = value;
}

bool CheckBox::getValue() {
  return cValue;
}

void CheckBox::addValueListener(IValueListener<bool>* listener) {
  cListeners.push_back(listener);
}

float CheckBox::getWidth() {
  return 0.4f;
}

float CheckBox::getHeight() {
  return 0.05f;
}

void CheckBox::update(unsigned int milliseconds) {
  // Nothing to do
}

void CheckBox::render() {
  float mLeft = getLeft();
  float mBottom = getBottom();
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  glColor3f(1.0f, 1.0f, 1.0f);
  mFont->print(mLeft + 0.04f, mBottom + 0.01f, mFontSize, IFont::LEFT, cLabel.c_str());

  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  
  float mCheckBoxBottom = mBottom + 0.01f;
  float mCheckBoxLeft   = mLeft + 0.01f * mAspectRatio;
  float mCheckBoxHeight = 0.03f;
  float mCheckBoxWidth  = 0.03f * mAspectRatio;
  
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINE_LOOP);
  glVertex2f(mCheckBoxLeft,                  mCheckBoxBottom + mCheckBoxHeight);
  glVertex2f(mCheckBoxLeft,                  mCheckBoxBottom);
  glVertex2f(mCheckBoxLeft + mCheckBoxWidth, mCheckBoxBottom);
  glVertex2f(mCheckBoxLeft + mCheckBoxWidth, mCheckBoxBottom + mCheckBoxHeight);
  glEnd();
  
  if (cValue) {
    glBegin(GL_LINES);
    glVertex2f(mCheckBoxLeft,                  mCheckBoxBottom);
    glVertex2f(mCheckBoxLeft + mCheckBoxWidth, mCheckBoxBottom + mCheckBoxHeight);
    glVertex2f(mCheckBoxLeft,                  mCheckBoxBottom + mCheckBoxHeight);
    glVertex2f(mCheckBoxLeft + mCheckBoxWidth, mCheckBoxBottom);
    glEnd();
  }

  if (cHasFocus) {
    // dotted line.
  }
}

bool CheckBox::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      // return keyDown(event.key.keysym.sym, event.key.keysym.mod);
    }
    
    case SDL_MOUSEBUTTONDOWN: {
      cValue = !cValue;
      fireChange();
    }      
  }
  return false;
}

void CheckBox::gainedFocus() {
  cHasFocus = true;
}

void CheckBox::lostFocus() {
  cHasFocus = false;
}

void CheckBox::fireChange() {
  std::cout << "There are " << cListeners.size() << " listeners!" << std::endl;
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->valueChanged(nullptr, cValue);
  }
}

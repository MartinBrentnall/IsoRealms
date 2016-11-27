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
#include "IntegerField.h"

int IntegerField::cDelayUntilBlinkChange = BLINK_DELAY;
bool IntegerField::cBlinkShowing = true;

IntegerField::IntegerField(std::string initialText) {
  cInput = initialText;
  cCaret = 0;
  cUpdating = false;
  cHasFocus = false;
}

void IntegerField::render() {
  float mLeft = getLeft();
  float mBottom = getBottom();
  float mRight = getRight();
  float mTop = getTop();
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  glColor3f(1.0f, 1.0f, 1.0f);
  mFont->print(mLeft + 0.01f, mBottom + 0.01f, mFontSize, IFont::LEFT, cInput.c_str());

  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINE_LOOP);
  glColor3f(0.45f, 0.0f, 0.9f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glEnd();

  if (cHasFocus && cBlinkShowing) {
    float mCaretOffset = mFont->getWidth(mFontSize, cInput.substr(0, cCaret).c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(mLeft + 0.01f + mCaretOffset, mTop);
    glVertex2f(mLeft + 0.01f + mCaretOffset, mBottom);
    glEnd();
  }
}

void IntegerField::update(unsigned int milliseconds) {
  if (cHasFocus) {
    cDelayUntilBlinkChange -= milliseconds;
    if (cDelayUntilBlinkChange <= 0) {
      cDelayUntilBlinkChange += BLINK_DELAY;
      cBlinkShowing = !cBlinkShowing;
    }
  }
}

bool IntegerField::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym, event.key.keysym.mod);
    }
    
    case SDL_MOUSEBUTTONDOWN: {
      return mouseButtonDown(event);
    }      
  }
  return false;
}

void IntegerField::gainedFocus() {
  cHasFocus = true;
}

void IntegerField::lostFocus() {
  cHasFocus = false;
}

void IntegerField::validateValue() {
  if (cInput.empty() || cInput == "-") {
    return;
  }
  long mValue = atol(cInput.c_str());
  if (mValue > std::numeric_limits<int>::max()) {
    mValue = std::numeric_limits<int>::max();
  }
  if (mValue < std::numeric_limits<int>::lowest()) {
    mValue = std::numeric_limits<int>::lowest();
  }
  cInput = std::to_string(mValue);
  if (cCaret > cInput.length()) {
    cCaret = cInput.length();
  }
}

bool IntegerField::keyDown(SDLKey& key, SDLMod& mod) {
  switch (key) {
    case SDLK_LEFT: {
      if (cCaret != 0) {
        cCaret--;
      }
      return true;
    }

    case SDLK_RIGHT: {
      cCaret++;
      if (cCaret > cInput.size()) {
        cCaret = cInput.size();
      }
      return true;
    }

    case SDLK_HOME: {
      cCaret = 0;
      return true;
    }

    case SDLK_END: {
      cCaret = cInput.length();
      return true;
    }

    case SDLK_BACKSPACE: {
      if (cCaret > 0) {
        cInput = cInput.substr(0, cCaret - 1) + cInput.substr(cCaret);
        cCaret--;
        fireChange();
      }
      validateValue();
      return true;
    }

    case SDLK_DELETE: {
      if (cCaret < cInput.length()) {
        cInput = cInput.substr(0, cCaret) + cInput.substr(cCaret + 1);
        fireChange();
      }
      validateValue();
      return true;
    }

    default: {
      if ((key >= '0' && key <= '9') || (key == '-' && cCaret == 0)) {
        if (mod & KMOD_SHIFT) {
          cInput = cInput.substr(0, cCaret) + (char) toupper((char) key) + cInput.substr(cCaret);
        } else {
          cInput = cInput.substr(0, cCaret) + (char) key + cInput.substr(cCaret);
        }
        cCaret++;
        fireChange();
      }
      validateValue();
      return true;
    }
  }
  return false;
}

bool IntegerField::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (contains(mX, mY)) {
    float mLeft = getLeft() + 0.01f;
    IFont* mFont = LookAndFeel::getDefaultFont();
    float mFontSize = LookAndFeel::getDefaultFontSize();
    float mDifference = FLT_MAX;
    for (unsigned int i = 0; i <= cInput.length(); i++) {
      float mPosition = mLeft + mFont->getWidth(mFontSize, cInput.substr(0, i).c_str());
      float mNewDifference = mX - mPosition;
      if (mNewDifference < 0.0f) {
        mNewDifference = -mNewDifference;
      }
      if (mNewDifference > mDifference) {
        cCaret = mNewDifference > mDifference ? i - 1: i - 2;
        return true;
      }
      mDifference = mNewDifference;
    }
    cCaret = cInput.length();
    return true;
  }
  return false;
}

void IntegerField::fireChange() {
  cUpdating = true;
  int mValue = atoi(cInput.c_str());
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->valueChanged(nullptr, mValue);
  }
  cUpdating = false;
}

void IntegerField::setValue(int value) {
  if (!cUpdating) {
    cInput = std::to_string(value);
//    fireChange();
  } 
}

int IntegerField::getValue() {
  return atoi(cInput.c_str());
}

void IntegerField::addValueListener(IValueListener<int>* listener) {
  cListeners.push_back(listener);
}

float IntegerField::getWidth() {
  return 0.4f;
}

float IntegerField::getHeight() {
  return 0.05f;
}

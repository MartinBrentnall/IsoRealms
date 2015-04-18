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
#include "TextEditorComponent.h"

int TextEditorComponent::cDelayUntilBlinkChange = BLINK_DELAY;
bool TextEditorComponent::cBlinkShowing = true;
const double TextEditorComponent::CARET_X_NOT_ASSIGNED = DBL_MAX;

TextEditorComponent::TextEditorComponent(std::string initialText) {
  cCaretChar = 0;
  cCaretLine = 0;
  cCaretX = CARET_X_NOT_ASSIGNED;
  cUpdating = false;
  cHasFocus = false;
  setText(initialText);
}

void TextEditorComponent::render() {
  float mLeft = getLeft();
  float mTop = getTop();
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  glColor3f(1.0f, 1.0f, 1.0f);
  float mLineHeight = mFont->getHeight(mFontSize, "A");
  
  if (cCaretChar != cSelectionChar || cCaretLine != cSelectionLine) {
    if (cCaretLine == cSelectionLine) {
      unsigned int mStartSelection = std::min(cSelectionChar, cCaretChar);
      unsigned int mEndSelection = std::max(cSelectionChar, cCaretChar);
      float mStartOffset = mFont->getWidth(mFontSize, cInput[cCaretLine].substr(0, mStartSelection).c_str());
      float mEndOffset = mFont->getWidth(mFontSize, cInput[cCaretLine].substr(0, mEndSelection).c_str());
      float mCaretBase = (mTop - mLineHeight) - (mLineHeight * cCaretLine);
      glColor3f(1.0f, 0.0f, 0.0f);
      glBegin(GL_QUADS);
      glVertex2f(mLeft + 0.01f + mStartOffset, mCaretBase + mLineHeight);
      glVertex2f(mLeft + 0.01f + mStartOffset, mCaretBase);
      glVertex2f(mLeft + 0.01f + mEndOffset, mCaretBase);
      glVertex2f(mLeft + 0.01f + mEndOffset, mCaretBase + mLineHeight);
      glEnd();
    } else {
      unsigned int mStartLine;
      unsigned int mEndLine;
      unsigned int mStartSelection;
      unsigned int mEndSelection;
      if (cCaretLine > cSelectionLine) {
        mStartLine = cSelectionLine;
        mStartSelection = cSelectionChar;
        mEndLine = cCaretLine;
        mEndSelection = cCaretChar;
      } else {
        mStartLine = cCaretLine;
        mStartSelection = cCaretChar;
        mEndLine = cSelectionLine;
        mEndSelection = cSelectionChar;
      }
      glColor3f(1.0f, 0.0f, 0.0f);
      glBegin(GL_QUADS);
      for (unsigned int i = mStartLine; i <= mEndLine; i++) {
        unsigned int mStartChar = i == mStartLine ? mStartSelection : 0;
        unsigned int mEndChar = i == mEndLine ? mEndSelection : cInput[i].length();
        float mStartOffset = mFont->getWidth(mFontSize, cInput[i].substr(0, mStartChar).c_str());
        float mEndOffset = mFont->getWidth(mFontSize, cInput[i].substr(0, mEndChar).c_str());
        float mCaretBase = (mTop - mLineHeight) - (mLineHeight * i);
        glVertex2f(mLeft + 0.01f + mStartOffset, mCaretBase + mLineHeight);
        glVertex2f(mLeft + 0.01f + mStartOffset, mCaretBase);
        glVertex2f(mLeft + 0.01f + mEndOffset, mCaretBase);
        glVertex2f(mLeft + 0.01f + mEndOffset, mCaretBase + mLineHeight);
      }
      glEnd();
    }
    glColor3f(1.0f, 1.0f, 1.0f);
  }
  
  for (unsigned int i = 0; i < cInput.size(); i++) {
    mFont->print(mLeft + 0.01f, (mTop - 0.04f) - (mLineHeight * i), mFontSize, 0, cInput[i].c_str());
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  if (cHasFocus && cBlinkShowing) {
    float mCaretOffset = mFont->getWidth(mFontSize, cInput[cCaretLine].substr(0, cCaretChar).c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    
    float mCaretBase = (mTop - 0.04f) - (mLineHeight * cCaretLine);
    glVertex2f(mLeft + 0.01f + mCaretOffset, mCaretBase + mLineHeight);
    glVertex2f(mLeft + 0.01f + mCaretOffset, mCaretBase);
    glEnd();
  }
}

void TextEditorComponent::update(unsigned int milliseconds) {
  if (cHasFocus) {
    cDelayUntilBlinkChange -= milliseconds;
    if (cDelayUntilBlinkChange <= 0) {
      cDelayUntilBlinkChange += BLINK_DELAY;
      cBlinkShowing = !cBlinkShowing;
    }
  }
}

bool TextEditorComponent::input(SDL_Event& event) {
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

void TextEditorComponent::gainedFocus() {
  cHasFocus = true;
}

void TextEditorComponent::lostFocus() {
  cHasFocus = false;
}

void TextEditorComponent::updateCaretX() {
  if (cCaretX == CARET_X_NOT_ASSIGNED) {
    IFont* mFont = LookAndFeel::getDefaultFont();
    float mFontSize = LookAndFeel::getDefaultFontSize();
    cCaretX = mFont->getWidth(mFontSize, cInput[cCaretLine].substr(0, cCaretChar).c_str());
  }
}

void TextEditorComponent::calculateCaretChar() {
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  cCaretChar = mFont->getChar(cCaretX, mFontSize, cInput[cCaretLine].c_str());
}

void TextEditorComponent::updateSelection(SDLMod& mod) {
  if (!(mod & KMOD_SHIFT)) {
    cSelectionChar = cCaretChar;
    cSelectionLine = cCaretLine;
  }
}

bool TextEditorComponent::keyDown(SDLKey& key, SDLMod& mod) {
  switch (key) {
    case SDLK_LEFT: {
      if (cCaretChar != 0) {
        cCaretChar--;
      }
      cCaretX = CARET_X_NOT_ASSIGNED;
      updateSelection(mod);
      return true;
    }

    case SDLK_RIGHT: {
      cCaretChar++;
      if (cCaretChar > cInput[cCaretLine].length()) {
        cCaretChar = cInput[cCaretLine].length();
      }
      cCaretX = CARET_X_NOT_ASSIGNED;
      updateSelection(mod);
      return true;
    }
    
    case SDLK_UP: {
      updateCaretX();
      if (cCaretLine != 0) {
        cCaretLine--;
      }
      calculateCaretChar();
      updateSelection(mod);
      return true;
    }
    
    case SDLK_DOWN: {
      updateCaretX();
      cCaretLine++;
      if (cCaretLine >= cInput.size()) {
        cCaretLine = cInput.size() - 1;
      }
      calculateCaretChar();
      updateSelection(mod);
      return true;
    }

    case SDLK_HOME: {
      cCaretChar = 0;
      cCaretX = CARET_X_NOT_ASSIGNED;
      updateSelection(mod);
      return true;
    }

    case SDLK_END: {
      cCaretChar = cInput[cCaretLine].length();
      cCaretX = CARET_X_NOT_ASSIGNED;
      updateSelection(mod);
      return true;
    }

    case SDLK_BACKSPACE: {
      if (cCaretChar > 0) {
        cInput[cCaretLine] = cInput[cCaretLine].substr(0, cCaretChar - 1) + cInput[cCaretLine].substr(cCaretChar);
        cCaretChar--;
        fireChange();
      } else if (cCaretLine > 0) {
        cCaretLine--;
        cCaretChar = cInput[cCaretLine].length();
        cInput[cCaretLine] = cInput[cCaretLine] + cInput[cCaretLine + 1];
        cInput.erase(cInput.begin() + cCaretLine + 1);
      }
      cCaretX = CARET_X_NOT_ASSIGNED;
      return true;
    }

    case SDLK_DELETE: {
      if (cCaretChar < cInput[cCaretLine].length()) {
        cInput[cCaretLine] = cInput[cCaretLine].substr(0, cCaretChar) + cInput[cCaretLine].substr(cCaretChar + 1);
        fireChange();
      } else if (cCaretLine < cInput.size() - 1) {
        cInput[cCaretLine] = cInput[cCaretLine] + cInput[cCaretLine + 1];
        cInput.erase(cInput.begin() + cCaretLine + 1);
      }
      cCaretX = CARET_X_NOT_ASSIGNED;
      return true;
    }
    
    case SDLK_RETURN: {
      std::string mNewLine = cInput[cCaretLine].substr(cCaretChar);
      cInput[cCaretLine] = cInput[cCaretLine].substr(0, cCaretChar);
      cCaretLine++;
      cCaretChar = 0;
      cInput.insert(cInput.begin() + cCaretLine, mNewLine);
    }
    
    default: {
      if (key >= 32 && key <= 127) {
        char mChar;
        switch (key) {
          default:          mChar = (char) key; break;
        }
        if (mod & KMOD_SHIFT) {
          cInput[cCaretLine] = cInput[cCaretLine].substr(0, cCaretChar) + (char) toupper((char) mChar) + cInput[cCaretLine].substr(cCaretChar);
        } else {
          cInput[cCaretLine] = cInput[cCaretLine].substr(0, cCaretChar) + (char) mChar + cInput[cCaretLine].substr(cCaretChar);
        }
        cCaretChar++;
        fireChange();
      }
      cCaretX = CARET_X_NOT_ASSIGNED;
      return true;
    }
  }
  return false;
}

bool TextEditorComponent::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (contains(mX, mY)) {
    float mTop = getTop();
    float mLeft = getLeft();
    IFont* mFont = LookAndFeel::getDefaultFont();
    float mFontSize = LookAndFeel::getDefaultFontSize();
    float mLineHeight = mFont->getHeight(mFontSize, "A");
    cCaretLine = (mTop - mY) / mLineHeight;
    cCaretChar = mFont->getChar((mX - 0.01f) - mLeft, mFontSize, cInput[cCaretLine].c_str());
    updateSelection(event.key.keysym.mod);
    return true;
  }
  return false;
}

void TextEditorComponent::fireChange() {
  cUpdating = true;
  std::string mText = getText();
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->valueChanged(mText);
  }
  cUpdating = false;
}

void TextEditorComponent::setText(std::string text) {
  if (!cUpdating) {
    cInput = Utils::split(text, '\n');
  } 
}

std::string TextEditorComponent::getText() {
  std::string mResult;
  for (unsigned int i = 0; i < cInput.size(); i++) {
    mResult = mResult + cInput[i];
    if (i != cInput.size() - 1) {
      mResult = mResult + "\n";
    }
  }
  return mResult;
}

void TextEditorComponent::addStringListener(IStringListener* listener) {
  cListeners.push_back(listener);
}

float TextEditorComponent::getWidth() {
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  return mFont->getWidth(mFontSize, getText().c_str()) + 0.04f;
}

float TextEditorComponent::getHeight() {
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  return mFont->getHeight(mFontSize, getText().c_str());
}

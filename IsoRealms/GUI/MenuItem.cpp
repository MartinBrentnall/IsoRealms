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
#include "MenuItem.h"

MenuItem::MenuItem(std::string text, ICommand* command, bool isSubMenuCommand) {
  cText = text;
  cAcceleratorIndex = cText.find('^');
  if (cAcceleratorIndex != std::string::npos) {
    cAccelerator = tolower(cText[cAcceleratorIndex + 1]);
    cText = cText.substr(0, cAcceleratorIndex) + cText.substr(cAcceleratorIndex + 1);
  } else {
    cText = text;
  }
  cCommand = command;
  cIsSubMenuCommand = isSubMenuCommand;
}

void MenuItem::setCommand(ICommand* command) {
  if (cCommand != NULL) {
    std::cout << "WARNING: Menu item action has already been set." << std::endl;
    exit(1);
  }
  cCommand = command;
}

void MenuItem::render(bool selected, float x, float y) {
  float mMultiplier = selected ? 1.0f : 0.3f;
  glColor3f(1.0f * mMultiplier, 1.0f * mMultiplier, 1.0f * mMultiplier);
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  mFont->print(x, y, mFontSize, IFont::LEFT, cText.c_str());
  if (cAcceleratorIndex != std::string::npos) {
    float mWidth = mFont->getWidth(mFontSize, cText.substr(0, cAcceleratorIndex).c_str());
    glColor3f(0.0f * mMultiplier, 1.0f * mMultiplier, 1.0f * mMultiplier);
    mFont->print(x + mWidth, y, mFontSize, IFont::LEFT, cText.substr(cAcceleratorIndex, 1).c_str());
  }
  glBindTexture(GL_TEXTURE_2D, 0);
}

float MenuItem::getWidth() {
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  return mFont->getWidth(mFontSize, cText.c_str());
}

float MenuItem::getHeight() {
  return 0.05f;
}

bool MenuItem::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      // TODO: Change to read this from event instead!
      if ((KeyStates::isKeyDown(SDLK_LALT) || KeyStates::isKeyDown(SDLK_RALT)) && event.key.keysym.sym == cAccelerator) {
        cCommand->execute();
        return true;
      }
    }
  }
  return false;
}

void MenuItem::execute() {
  cCommand->execute();
}

bool MenuItem::contains(float cXOffset, float cYOffset, float x, float y) {
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  float mWidth = mFont->getWidth(mFontSize, cText.c_str());
  if (x >= cXOffset && x <= cXOffset + mWidth && y >= cYOffset - 0.02f * 0.5f && y <= cYOffset + 0.02f * 2.0f) {
    return true;
  }
  return false;
}

bool MenuItem::testClick(float cXOffset, float cYOffset, float x, float y) {
  if (contains(cXOffset, cYOffset, x, y)) {
    if (cCommand != NULL) {
      cCommand->execute();
    }
    return true;
  }
  return false;
}

bool MenuItem::isSubMenuCommand() {
  return cIsSubMenuCommand;
}


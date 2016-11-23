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
#include "DynamicMenuItems.h"

DynamicMenuItems::DynamicMenuItems() {
  cSelected = 0;
}

void DynamicMenuItems::addItem(std::string text, ICommand* command) {
  cItems.push_back(std::pair<std::string, ICommand*>(text, command));
}

void DynamicMenuItems::render(bool selected, float x, float y) {
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  unsigned int mIndex = 0;
  for (std::pair<std::string, ICommand*> mMenuItem : cItems) {
    if (mIndex == cSelected && selected) {
      glColor3f(1.0f, 1.0f, 1.0f);
    } else {
      glColor3f(0.3f, 0.3f, 0.3f);
    }
    mFont->print(x, y, mFontSize, 0, mMenuItem.first.c_str());
    y -= 0.05f;
    mIndex++;
  }
}

float DynamicMenuItems::getWidth() {
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  float mWidest = 0.0f;
  for (std::pair<std::string, ICommand*> mMenuItem : cItems) {
    mWidest = std::max(mFont->getWidth(mFontSize, mMenuItem.first.c_str()), mWidest);
  }  
  return mWidest;
}

float DynamicMenuItems::getHeight() {
  return 0.05f * cItems.size();
}

bool DynamicMenuItems::input(SDL_Event& event) {
  return false;
}

void DynamicMenuItems::execute() {
  if (cItems[cSelected].second != nullptr) {
    cItems[cSelected].second->execute();
  }
}

bool DynamicMenuItems::contains(float cXOffset, float cYOffset, float x, float y) {
  float mWidth = getWidth();
  unsigned int mIndex = 0;
  for (std::pair<std::string, ICommand*> mMenuItem : cItems) {
    if (x >= cXOffset && x <= cXOffset + mWidth && y >= (cYOffset + 0.04f) - (mIndex + 1) * 0.05f && y <= (cYOffset + 0.04f) - mIndex * 0.05f) {
      cSelected = mIndex;
      return true;
    }
    mIndex++;
  }
  return false;
}

bool DynamicMenuItems::testClick(float cXOffset, float cYOffset, float x, float y) {
  if (contains(cXOffset, cYOffset, x, y)) {
    execute();
    return true;
  }
  return false;
}

bool DynamicMenuItems::isSubMenuCommand() {
  return false;
}

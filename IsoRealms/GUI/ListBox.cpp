/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "ListBox.h"

ListBox::ListBox() {
  cSelectedItem = 0;
}

std::string& ListBox::getSelectedItem() {
  return cItems[cSelectedItem];
}

void ListBox::addItem(const std::string& item) {
  // TODO: Check if it already exists
  cItems.push_back(item);
}

void ListBox::addListener(IListBoxListener* listener) {
  cListeners.push_back(listener);
}

void ListBox::removeItem(const std::string& item) {
  for (unsigned int i = 0; i < cItems.size(); i++) {
    if (cItems[i] == item) {
      cItems.erase(cItems.begin() + i);
      return;
    }
  }
}

void ListBox::clear() {
  cItems.clear();
  cSelectedItem = 0;
}

void ListBox::render() {
  float mLeft = getLeft();
  float mTop = getTop();

  float mXOffset = mLeft + 0.02f;
  float mLine = mTop - 0.05f;
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  for (unsigned int i = 0; i < cItems.size(); i++) {
    float mBrightness = i == cSelectedItem ? 1.0f : 0.4f;
    glColor3f(1.0f * mBrightness, 1.0f * mBrightness, 1.0f * mBrightness);
    mFont->print(mXOffset, mLine, mFontSize, IFont::LEFT, cItems[i].c_str());
    mLine -= 0.05f;
  }
}

void ListBox::update(unsigned int milliseconds) {
  cMultipleClickDetector.update(milliseconds);
}

float ListBox::getWidth() {
  float mLargestWidth = 0.04f;
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  for (unsigned int i = 0; i < cItems.size(); i++) {
    float mTextWidth = mFont->getWidth(mFontSize, cItems[i].c_str()) + 0.04f;
    if (mTextWidth > mLargestWidth) {
      mLargestWidth = mTextWidth;
    }
  }
  return mLargestWidth;
}

float ListBox::getHeight() {
  return cItems.size() * 0.05f + 0.02f;
}

bool ListBox::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_DOWN: {
      cSelectedItem++;
      if (cSelectedItem >= cItems.size()) {
        cSelectedItem = cItems.size() - 1;
      }
      return true;
    }
 
    case SDLK_UP: {
      if (cSelectedItem != 0) {
        cSelectedItem--;
      }
      return true;
    }
    
    case SDLK_RETURN: {
      for (unsigned int i = 0; i < cListeners.size(); i++) {
	cListeners[i]->assertSelection(this, cItems[cSelectedItem]);
      }
    }

    default: {
      return true;
    }
  }
  return true;
}

bool ListBox::mouseButtonPressed(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (contains(mX, mY)) {
    cMultipleClickDetector.input(event);
    float mLine = getTop() - 0.05f;
    if (event.button.button == SDL_BUTTON_LEFT) {
      for (unsigned int i = 0; i < cItems.size(); i++) {
	if (mY > mLine && mY < mLine + 0.05f) {
	  cSelectedItem = i;
	  if (cMultipleClickDetector.getClicks() == MultipleClickDetector::DOUBLE_CLICK) {
	    for (unsigned int i = 0; i < cListeners.size(); i++) {
	      cListeners[i]->assertSelection(this, cItems[cSelectedItem]);
	    }
	  }
	  break;
	}
	mLine -= 0.05f;
      }
    }
    return true;
  }
  return false;
}

bool ListBox::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }

    case SDL_MOUSEBUTTONDOWN: {
      return mouseButtonPressed(event);
    }
  }
  return false;
}

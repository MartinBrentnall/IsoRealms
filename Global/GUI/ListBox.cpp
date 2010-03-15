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

void ListBox::removeItem(const std::string& item) {
  for (unsigned int i = 0; i < cItems.size(); i++) {
    if (cItems[i] == item) {
      cItems.erase(cItems.begin() + i);
      return;
    }
  }
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
    mFont->print(mXOffset, mLine, mFontSize, 0, cItems[i].c_str());
    mLine -= 0.05f;
  }
}

void ListBox::update(int milliseconds) {
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

    default: {
      return true;
    }
  }
  return true;
}

bool ListBox::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }

    // TODO: Support mouse selection
  }
  return false;
}

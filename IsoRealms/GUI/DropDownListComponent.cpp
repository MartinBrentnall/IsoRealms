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
#include "DropDownListComponent.h"

DropDownListComponent::DropDownListComponent(std::vector<std::string> items, unsigned int selected, IComponentContainer* windowWorkspace) {
  cUpdating = false;
  cHasFocus = false;
  cItems = items;
  cSelected = selected;
  cWindowWorkspace = windowWorkspace;
}

void DropDownListComponent::render() {
  float mLeft   = getLeft();
  float mBottom = getBottom();
  float mRight  = getRight();
  float mTop    = getTop();
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  glColor3f(1.0f, 1.0f, 1.0f);
  mFont->print(mLeft + 0.01f, mTop - 0.04f, mFontSize, IFont::LEFT, cItems[cSelected].c_str());

  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINE_LOOP);
  glColor3f(0.45f, 0.0f, 0.9f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glEnd();
}

void DropDownListComponent::update(unsigned int milliseconds) {
  // TODO: Implement
}

bool DropDownListComponent::input(SDL_Event& event) {
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

void DropDownListComponent::gainedFocus() {
  cHasFocus = true;
}

void DropDownListComponent::lostFocus() {
  cHasFocus = false;
}

void DropDownListComponent::setValue(unsigned int value) {
  cSelected = std::max(static_cast<unsigned int>(0), std::min(value, static_cast<unsigned int>(cItems.size()) - 1));
}

unsigned int DropDownListComponent::getValue() {
  return cSelected;
}

void DropDownListComponent::addValueListener(IValueListener<unsigned int>* listener) {
  cValueChangeListeners.push_back(listener);
}

void DropDownListComponent::fireValueChange() {
  for (IValueListener<unsigned int>* mValueListener : cValueChangeListeners) {
    mValueListener->valueChanged(this, cSelected);
  }
}

bool DropDownListComponent::keyDown(SDLKey& key, SDLMod& mod) {
//  switch (key) {
//  }
  return false;
}

bool DropDownListComponent::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (contains(mX, mY)) {
    ListPopupBounds* mBounds = new ListPopupBounds(this);
    ListPopup* mListComponent = new ListPopup(this, mBounds);
    cWindowWorkspace->addComponent(mListComponent);
    return true;
  }
  return false;
}

float DropDownListComponent::getWidth() {
  return 0.4f;
}

float DropDownListComponent::getHeight() {
  return 0.05f;
}

DropDownListComponent::ListPopup::ListPopup(DropDownListComponent* parent, IComponentBoundsCalculator* bounds) : AbstractRectangularComponent(bounds) {
  cParent = parent;
}

void DropDownListComponent::ListPopup::update(unsigned int milliseconds) {
  cParent->cMultipleClickDetector.update(milliseconds);
}

void DropDownListComponent::ListPopup::render() {
  float mLeft   = getLeft();
  float mBottom = getBottom();
  float mRight  = getRight();
  float mTop    = getTop();

  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();

  glBindTexture(GL_TEXTURE_2D, 0);
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glColor4f(0.05f, 0.0f, 0.1f, 0.85f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);

  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
  glVertex2f(mLeft,  mTop -  cParent->cSelected      * 0.05f);
  glVertex2f(mLeft,  mTop - (cParent->cSelected + 1) * 0.05f);
  glVertex2f(mRight, mTop - (cParent->cSelected + 1) * 0.05f);
  glVertex2f(mRight, mTop -  cParent->cSelected      * 0.05f);
  glEnd();
  glDisable(GL_BLEND);

  glBegin(GL_LINE_LOOP);
  glColor3f(0.8f, 0.6f, 1.0f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glEnd();

  glEnable(GL_SCISSOR_TEST);
  int mLeftPixels   = mScreen->convertToXPixels(mLeft);
  int mRightPixels  = mScreen->convertToXPixels(mRight);
  int mTopPixels    = mScreen->convertToYPixels(mTop);
  int mBottomPixels = mScreen->convertToYPixels(mBottom);
  glScissor(mLeftPixels + 1, mBottomPixels + 1, (mRightPixels - mLeftPixels) - 1, (mTopPixels - mBottomPixels) - 1);

  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  glColor3f(1.0f, 1.0f, 1.0f);
  for (unsigned int i = 0; i < cParent->cItems.size(); i++) {
    mFont->print(mLeft + 0.01f, mTop - (0.04f + i * 0.05f), mFontSize, IFont::LEFT, cParent->cItems[i].c_str());
  }
  glBindTexture(GL_TEXTURE_2D, 0);

  glDisable(GL_SCISSOR_TEST);
  glLoadIdentity();  
  glEnable(GL_DEPTH_TEST);

  glBindTexture(GL_TEXTURE_2D, 0);
  glColor3f(1.0f, 1.0f, 1.0f);  
}

bool DropDownListComponent::ListPopup::keyDown(SDLKey& key, SDLMod& mod) {
  switch (key) {
    case SDLK_UP: {
      if (cParent->cSelected > 0) {
        cParent->cSelected--;
      }
      return true;
    }

    case SDLK_DOWN: {
      if (cParent->cSelected < cParent->cItems.size() - 1) {
        cParent->cSelected++;
      }
      return true;
    }
    
    case SDLK_ESCAPE:
    case SDLK_RETURN: {
      // If clicked outside the popup, we close it.
      // TODO: deallocate the popup
      cParent->cWindowWorkspace->removeComponent(this);
      cParent->fireValueChange();
      return true;
    }
    
    default: {
      return true;
    }
  }
  return true;
}

bool DropDownListComponent::ListPopup::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (mX >= getLeft() && mX <= getRight() && mY >= getBottom() && mY <= getTop()) {
    float mTop = getTop();
    for (unsigned int i = 0; i < cParent->cItems.size(); i++) {
      if (mY < mTop - i * 0.05f && mY >= mTop - (i + 1) * 0.05f) {
        cParent->cSelected = i;
        if (cParent->cMultipleClickDetector.getClicks() == MultipleClickDetector::DOUBLE_CLICK) {
          cParent->cWindowWorkspace->removeComponent(this);
        }
        cParent->fireValueChange();
        return true;
      }
    }
    return true;
  }
  
  // If clicked outside the popup, we close it.
  // TODO: deallocate the popup
  cParent->cWindowWorkspace->removeComponent(this);
  cParent->fireValueChange();
  return true;
}

bool DropDownListComponent::ListPopup::input(SDL_Event& event) {
  cParent->cMultipleClickDetector.input(event);
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

bool DropDownListComponent::ListPopup::contains(float x, float y) {
  return true;  
}

DropDownListComponent::ListPopupBounds::ListPopupBounds(DropDownListComponent* parent) {
  cParent = parent;
}

float DropDownListComponent::ListPopupBounds::getLeft() {
  return cParent->getLeft();
}

float DropDownListComponent::ListPopupBounds::getRight() {
  return cParent->getRight();  
}

float DropDownListComponent::ListPopupBounds::getBottom() {
  return cParent->getBottom() - cParent->cItems.size() * 0.05f;  
}

float DropDownListComponent::ListPopupBounds::getTop() {
  return cParent->getBottom();
}

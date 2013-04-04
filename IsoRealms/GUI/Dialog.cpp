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
#include "Dialog.h"

const float Dialog::TITLE_BAR_HEIGHT = 0.06f;

Dialog::Dialog(IComponentContainer* componentContainer, const std::string& dialogDescriptionFile, IResourceAccessor* resourceAccessor) {
  std::string mFullPath = System::getProgramResource(dialogDescriptionFile) + ".dialog";
  DOMNodeWrapper* mFileNode = new DOMNodeWrapper(mFullPath);
  for (int i = 0; i < mFileNode->getChildCount(); i++) {
    DOMNodeWrapper *mNode = mFileNode->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Dialog") {
      cTitle = mNode->getAttribute("title");
      loadDialog(mNode, this, 0.02f, resourceAccessor);
    }
  }
  cComponentContainer = componentContainer;
  cX = 0.0f;
  cY = 0.0f;
  cWidth = 0.75f;
  cHeight = 0.75;
  cDragging = false;
}

Dialog::Dialog(IComponentContainer* componentContainer, const std::string& title, float x, float y, float width, float height) {
  cComponentContainer = componentContainer;
  cTitle = title;
  cX = x;
  cY = y;
  cWidth = width;
  cHeight = height;
  cDragging = false;
}

float Dialog::getLeft() {
  return cX;
}

float Dialog::getBottom() {
  return cY;
}

float Dialog::getRight() {
  return cX + cWidth;
}

float Dialog::getTop() {
  return cY + cHeight;
}

bool Dialog::contains(float x, float y) {
  return x >= cX && x <= cX + cWidth && y >= cY && y <= cY + cHeight + TITLE_BAR_HEIGHT;
}

void Dialog::translate(float x, float y) {
  cX += x;
  cY += y;
}

void Dialog::resize(float x, float y) {
  cWidth += x;
  cHeight += y;
}

void Dialog::setSize(float left, float bottom, float right, float top) {
  cX = left;
  cY = bottom;
  cWidth = right - left;
  cHeight = (top - bottom) - 0.05f;
}

void Dialog::renderContent() {
  float mLeft = getLeft();
  float mTop = getTop();
  float mRight = getRight();
  float mBottom = getBottom();
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();

  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glColor4f(0.05f, 0.0f, 0.1f, 0.85f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);

  glColor4f(0.85f, 0.0f, 0.7f, 0.85f); glVertex2f(mRight, mTop + TITLE_BAR_HEIGHT);
                                       glVertex2f(mLeft, mTop + TITLE_BAR_HEIGHT);
  glColor4f(0.05f, 0.0f, 0.1f, 0.85f); glVertex2f(mLeft, mTop);
                                       glVertex2f(mRight, mTop);
  glEnd();
  glDisable(GL_BLEND);

  glBegin(GL_LINE_LOOP);
  glColor3f(0.8f, 0.6f, 1.0f);
  glVertex2f(mLeft,  mTop + TITLE_BAR_HEIGHT);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop + TITLE_BAR_HEIGHT);
  glEnd();

  glEnable(GL_SCISSOR_TEST);
  int mLeftPixels = mScreen->convertToXPixels(mLeft);
  int mRightPixels = mScreen->convertToXPixels(mRight);
  int mTopPixels = mScreen->convertToYPixels(mTop + TITLE_BAR_HEIGHT);
  int mBottomPixels = mScreen->convertToYPixels(mBottom);
  glScissor(mLeftPixels + 1, mBottomPixels + 1, (mRightPixels - mLeftPixels) - 1, (mTopPixels - mBottomPixels) - 1);

  // Close button
  glEnd();
  float mButtonPadding = TITLE_BAR_HEIGHT * 0.25f;
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f((mRight - mAspectRatio * TITLE_BAR_HEIGHT) + mAspectRatio * mButtonPadding,  mTop + mButtonPadding);
  glVertex2f( mRight - mAspectRatio * mButtonPadding,                                    (mTop + TITLE_BAR_HEIGHT) - mButtonPadding);
  glVertex2f((mRight - mAspectRatio * TITLE_BAR_HEIGHT) + mAspectRatio * mButtonPadding, (mTop + TITLE_BAR_HEIGHT) - mButtonPadding);
  glVertex2f( mRight - mAspectRatio * mButtonPadding,                                     mTop + mButtonPadding);
  glEnd();
  glLineWidth(1.0f);

  // Title bar text
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  mFont->print(mLeft + 0.01f, mTop + 0.01f, mFontSize, 0, cTitle.c_str());
  
//   for (unsigned int i = 0; i < cChildren.size(); i++) {
//     cChildren[i]->render();
//   }
//   renderContent();
  glDisable(GL_SCISSOR_TEST);
  renderDialogContent();
}

bool Dialog::mouseButtonDown(SDL_Event& event) {
  float mTop = getTop();
  float mRight = getRight();
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (mX >= mRight - mAspectRatio * TITLE_BAR_HEIGHT && mX <= mRight && mY >= mTop && mY <= mTop + TITLE_BAR_HEIGHT) {
    close();
    return true;
  }
  if (contains(mX, mY)) {
    cComponentContainer->bringComponentToFront(this);
    cDragging = true;
    return true;
  }
  return false;
}

bool Dialog::mouseMotion(SDL_Event& event) {
  if (cDragging) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(event.motion.xrel) + 1.0f;
    float mY = mScreen->getYLocation(event.motion.yrel) - 1.0f;
    translate(mX, mY);
    return true;
  }
  return false;
}

void Dialog::relinquishFocus() {
  cComponentContainer->relinquishFocus(this);
}

void Dialog::close() {
  for (unsigned int i = 0; i < cCloseListeners.size(); i++) {
    cCloseListeners[i]->componentClosed(this);
  }
  cComponentContainer->removeComponent(this);
}

void Dialog::addCloseListener(IComponentCloseListener* listener) {
  cCloseListeners.push_back(listener);
}

void Dialog::updateContent(int milliseconds) {
  updateDialogContent(milliseconds);
}

bool Dialog::inputContent(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      if (mouseButtonDown(event)) {
        return true;
      }
      break;
    }

    case SDL_MOUSEBUTTONUP: {
      cDragging = false;
      break;
    }

    case SDL_MOUSEMOTION: {
      if (mouseMotion(event)) {
	return true;
      }
      break;
    }

    default: {
      break;
    }
  }
  return inputDialogContent(event);
}

Dialog::~Dialog() {
}

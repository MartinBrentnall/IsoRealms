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
#include "ScrollableContainer.h"

ScrollableContainer::ScrollableContainer() {
  mouseButtonUp();
  cScrollHorizontal = 0.0f;
  cScrollVertical = 0.0f;
}

void ScrollableContainer::setRootComponent(ISizedComponent* component) {
  IComponentBoundsCalculator* mBoundsCalculator = new RootComponentLayout(this, component);
  component->setBoundsCalculator(mBoundsCalculator);
  cComponents.push_back(component);
  cFocusedComponent = NULL;
}

void ScrollableContainer::testFocusChange(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      switch (event.button.button) {
        case SDL_BUTTON_WHEELUP: {
          if (isVerticalBarVisible()) {
            cScrollVertical -= 0.04f;
          } else if (isHorizontalBarVisible()) {
            cScrollHorizontal += 0.04f;
          }
          break;
        }
    
        case SDL_BUTTON_WHEELDOWN: {
          if (isVerticalBarVisible()) {
            cScrollVertical += 0.04f;
          } else if (isHorizontalBarVisible()) {
            cScrollHorizontal -= 0.04f;
          }
          break;
        }

        default: {
          Configuration* mConfiguration = Configuration::getInstance();
          ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
          float mX = mScreen->getXLocation(event.button.x);
          float mY = mScreen->getYLocation(event.button.y);
          if (contains(mX, mY)) {
            for (unsigned int i = 0; i < cComponents.size(); i++) {
              if (cComponents[i]->contains(mX, mY)) {
                cFocusedComponent = cComponents[i];
                return;
              }
            }
            cFocusedComponent = NULL;
          }
          break;
        }
      }
    }

    case SDL_MOUSEBUTTONUP: {
      mouseButtonUp();
    }
  }
}

bool ScrollableContainer::mouseButtonUp() {
  cScrollDirection = SCROLL_NONE;
  cScrollHorizontalActive = false;
  cScrollVerticalActive = false;
  return false;
}

bool ScrollableContainer::mouseButtonVerticalBar(SDL_Event& event) {
  if (isVerticalBarVisible()) {
    float mScrollBarLeft = getViewRight();
    float mScrollBarRight = getRight();
    float mScrollBarTop = getTop();
    float mScrollBarBottom = getViewBottom();
    float mTopButtonBottom = mScrollBarTop - 0.04f;
    float mBottomButtonTop = mScrollBarBottom + 0.04f;
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(event.button.x);
    float mY = mScreen->getYLocation(event.button.y);
    if (mX >= mScrollBarLeft && mX <= mScrollBarRight) {
      if (mY >= mTopButtonBottom && mY <= mScrollBarTop) {
        cScrollDirection = SCROLL_UP;
        return true;
      } else if (mY >= mScrollBarBottom && mY <= mBottomButtonTop) {
        cScrollDirection = SCROLL_DOWN;
        return true;
      } else if (mY >= getVerticalBottom() && mY <= getVerticalTop()) {
        std::cout << "Dragging vertical!" << std::endl;
        cScrollVerticalActive = true;
        return true;
      }
      return true;
    }
  }
  return false;
}

bool ScrollableContainer::mouseButtonHorizontalBar(SDL_Event& event) {
  if (isHorizontalBarVisible()) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mAspectRatio = getAspectRatio();
    float mScrollBarLeft = getLeft();
    float mScrollBarRight = getViewRight();
    float mScrollBarTop = getViewBottom();
    float mScrollBarBottom = getBottom();
    float mLeftButtonRight = mScrollBarLeft + 0.04f * mAspectRatio;
    float mRightButtonLeft = mScrollBarRight - 0.04f * mAspectRatio;
    float mX = mScreen->getXLocation(event.button.x);
    float mY = mScreen->getYLocation(event.button.y);
    if (mY >= mScrollBarBottom && mY <= mScrollBarTop) {
      if (mX >= mScrollBarLeft && mX <= mLeftButtonRight) {
        cScrollDirection = SCROLL_LEFT;
        return true;
      } else if (mX >= mRightButtonLeft && mX <= mScrollBarRight) {
        cScrollDirection = SCROLL_RIGHT;
        return true;
      } else if (mX >= getHorizontalLeft() && mX <= getHorizontalRight()) {
        cScrollHorizontalActive = true;
        return true;
      }
      return true;
    }
  }
  return false;
}

bool ScrollableContainer::mouseButtonDown(SDL_Event& event) {
  switch (event.button.button) {
    default: {
      return mouseButtonVerticalBar(event) || mouseButtonHorizontalBar(event);
    }
  }
}

bool ScrollableContainer::mouseMotion(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  if (cScrollHorizontalActive) {
    float mX = mScreen->getXLocation(event.motion.xrel) + 1.0f;
    cScrollHorizontal -= mX;
    return true;
  } else if (cScrollVerticalActive) {
    float mY = mScreen->getYLocation(event.motion.yrel) - 1.0f;
    cScrollVertical -= mY;
    return true;
  }
  return false;
}

bool ScrollableContainer::input(SDL_Event& event) {
  testFocusChange(event);
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      if (mouseButtonDown(event)) {
        return true;
      }
    }

    case SDL_MOUSEMOTION: {
      if (mouseMotion(event)) {
        return true;
      }
    }
  }
  return cFocusedComponent != NULL ? cFocusedComponent->input(event) : false;
}

void ScrollableContainer::update(int ticks) {
  float mAmount = 0.0005f * ticks;
  switch (cScrollDirection) {
    case SCROLL_UP: cScrollVertical -= mAmount; break;
    case SCROLL_DOWN: cScrollVertical += mAmount; break;
    case SCROLL_LEFT: cScrollHorizontal += mAmount; break;
    case SCROLL_RIGHT: cScrollHorizontal -= mAmount; break;
    default: /* Nothing to do. */ break;
  }
  float mContentLeft = getContentLeft();
  float mContentRight = getContentRight();
  float mContentTop = getContentTop();
  float mContentBottom = getContentBottom();
  float mViewLeft = getLeft();
  float mViewRight = getViewRight();
  float mViewTop = getTop();
  float mViewBottom = getViewBottom();

  if (mViewTop > mContentTop) {
    cScrollVertical -= mContentTop - mViewTop;
  }
  if (!isAllContentVisibleVertically()) {
    if (mContentBottom > mViewBottom) {
      cScrollVertical += mViewBottom - mContentBottom;
    }
  }
  if (mContentLeft > mViewLeft) {
    cScrollHorizontal += mViewLeft - mContentLeft;
  }
  if (!isAllContentVisibleHorizontally()) {
    if (mViewRight > mContentRight) {
      cScrollHorizontal -= mContentRight - mViewRight;
    }
  }
}

float ScrollableContainer::getViewRight() {
  float mAspectRatio = getAspectRatio();
  return getRight() - (isVerticalBarVisible() ? 0.04f * mAspectRatio : 0.0f);
}

float ScrollableContainer::getViewBottom() {
  return getBottom() + (isHorizontalBarVisible() ? 0.04f : 0.0f);
}

float ScrollableContainer::getHorizontalLeft() {
  float mViewLeft = getLeft();
  float mViewRight = getViewRight();
  float mContentLeft = fminf(getContentLeft(), mViewLeft);
  float mContentRight = fmaxf(getContentRight(), mViewRight);
  float mRatio = (mViewLeft - mContentLeft) / (mContentRight - mContentLeft);
  float mAspectRatio = getAspectRatio();
  float mLeftButtonOffset = mViewLeft + 0.04f * mAspectRatio;
  float mRightButtonOffset = mViewRight - 0.04f * mAspectRatio;
  return mLeftButtonOffset + (mRightButtonOffset - mLeftButtonOffset) * mRatio;
}

float ScrollableContainer::getHorizontalRight() {
  float mViewLeft = getLeft();
  float mViewRight = getViewRight();
  float mContentLeft = fminf(getContentLeft(), mViewLeft);
  float mContentRight = fmaxf(getContentRight(), mViewRight);
  float mRatio = (mViewRight - mContentLeft) / (mContentRight - mContentLeft);
  float mAspectRatio = getAspectRatio();
  float mLeftButtonOffset = mViewLeft + 0.04f * mAspectRatio;
  float mRightButtonOffset = mViewRight - 0.04f * mAspectRatio;
  return mLeftButtonOffset + (mRightButtonOffset - mLeftButtonOffset) * mRatio;
}

float ScrollableContainer::getVerticalBottom() {
  float mViewTop = getTop();
  float mViewBottom = getViewBottom();
  float mContentBottom = fminf(getContentBottom(), mViewBottom);
  float mContentTop = fmaxf(getContentTop(), mViewTop);
  float mRatio = (mViewBottom - mContentBottom) / (mContentTop - mContentBottom);
  float mBottomButtonOffset = mViewBottom + 0.04f;
  float mTopButtonOffset = mViewTop - 0.04f;
  return mBottomButtonOffset + (mTopButtonOffset - mBottomButtonOffset) * mRatio;
}

float ScrollableContainer::getVerticalTop() {
  float mViewTop = getTop();
  float mViewBottom = getViewBottom();
  float mContentBottom = fminf(getContentBottom(), mViewBottom);
  float mContentTop = fmaxf(getContentTop(), mViewTop);
  float mRatio = (mViewTop - mContentBottom) / (mContentTop - mContentBottom);
  float mBottomButtonOffset = mViewBottom + 0.04f;
  float mTopButtonOffset = mViewTop - 0.04f;
  return mBottomButtonOffset + (mTopButtonOffset - mBottomButtonOffset) * mRatio;
}

float ScrollableContainer::getContentLeft() {
  float mMostLeft = FLT_MAX;
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    float mComponentLeft = cComponents[i]->getLeft();
    if (mComponentLeft < mMostLeft) {
      mMostLeft = mComponentLeft;
    }
  }
  return mMostLeft;
}

float ScrollableContainer::getContentRight() {
  float mMostRight = -200.0f; // TODO: Largest negative float?
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    float mComponentRight = cComponents[i]->getRight();
    if (mComponentRight > mMostRight) {
      mMostRight = mComponentRight;
    }
  }
  return mMostRight;
}

float ScrollableContainer::getContentTop() {
  float mMostTop = -200.0f; // TODO: Largest negative float?
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    float mComponentTop = cComponents[i]->getTop();
    if (mComponentTop > mMostTop) {
      mMostTop = mComponentTop;
    }
  }
  return mMostTop;
}

float ScrollableContainer::getContentBottom() {
  float mMostBottom = FLT_MAX;
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    float mComponentBottom = cComponents[i]->getBottom();
    if (mComponentBottom < mMostBottom) {
      mMostBottom = mComponentBottom;
    }
  }
  return mMostBottom;
}

bool ScrollableContainer::isAllContentVisibleVertically() {
  float mContainerTop = getTop();
  float mContainerBottom = getBottom();
  float mContentTop = getContentTop();
  float mContentBottom = getContentBottom();
  return mContainerTop >= mContentTop && mContainerBottom <= mContentBottom;
}

bool ScrollableContainer::isAllContentVisibleHorizontally() {
  float mContainerLeft = getLeft();
  float mContainerRight = getRight();
  float mContentLeft = getContentLeft();
  float mContentRight = getContentRight();
  return mContainerLeft <= mContentLeft && mContainerRight >= mContentRight;
}

bool ScrollableContainer::isAllContentVisible() {
  return isAllContentVisibleVertically() && isAllContentVisibleHorizontally();
}

float ScrollableContainer::getAspectRatio() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  return mScreen->getAspectRatio();
}

bool ScrollableContainer::isHorizontalBarVisible() {
  if (isAllContentVisible()) {
    return false;
  }
  float mAspectRatio = getAspectRatio();
  float mContentLeft = getContentLeft();
  float mContentRight = getContentRight();
  float mContainerLeft = getLeft();
  float mContainerRight = getRight() - 0.04f * mAspectRatio;
  return mContentLeft < mContainerLeft || mContentRight > mContainerRight;
}

bool ScrollableContainer::isVerticalBarVisible() {
  if (isAllContentVisible()) {
    return false;
  }
  double mContentBottom = getContentBottom();
  double mContentTop = getContentTop();
  double mContainerBottom = getBottom() + 0.04f;
  double mContainerTop = getTop();
  return mContentBottom < mContainerBottom || mContentTop > mContainerTop;
}

void ScrollableContainer::render() {
  float mLeft = getLeft();
  float mTop = getTop();
  float mRight = getRight();
  float mBottom = getBottom();

  glBindTexture(GL_TEXTURE_2D, 0);

  float mAspectRatio = getAspectRatio();

  glBegin(GL_LINE_LOOP);
  glColor3f(0.45f, 0.0f, 0.9f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glEnd();

  float mViewRight = getViewRight();
  float mViewBottom = getViewBottom();
  if (isVerticalBarVisible()) {
    glBegin(GL_LINES);
    glVertex2f(mViewRight, mBottom);
    glVertex2f(mViewRight, mTop);
    float mBottomButtonOffset;
    float mTopButtonOffset;
    if (mTop - mViewBottom <= 0.08) {
      mBottomButtonOffset = mViewBottom + (mTop - mViewBottom) / 2.0f;
      mTopButtonOffset = mBottomButtonOffset;
      glVertex2f(mViewRight, mBottomButtonOffset);
      glVertex2f(mRight, mBottomButtonOffset);
      glEnd();
    } else {
      mBottomButtonOffset = mViewBottom + 0.04f;
      mTopButtonOffset = mTop - 0.04f;
      glVertex2f(mViewRight, mBottomButtonOffset);
      glVertex2f(mRight,     mBottomButtonOffset);
      glVertex2f(mViewRight, mTopButtonOffset);
      glVertex2f(mRight,     mTopButtonOffset);

      float mVerticalTop    = getVerticalTop();
      float mVerticalBottom = getVerticalBottom();
      glEnd();
      glBegin(GL_QUADS);
      glColor3f(0.9f, 0.0f, 0.45f);
      glVertex2f(mViewRight, mVerticalTop);
      glVertex2f(mViewRight, mVerticalBottom);
      glColor3f(0.15f, 0.0f, 0.3f);
      glVertex2f(mRight, mVerticalBottom);
      glVertex2f(mRight, mVerticalTop);
      glEnd();
    }

    if (cScrollDirection == SCROLL_UP) {
      glBegin(GL_QUADS);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex2f(mRight,     mTopButtonOffset);
      glVertex2f(mRight,     mTop);
      glVertex2f(mViewRight, mTop);
      glVertex2f(mViewRight, mTopButtonOffset);
      glEnd();
    } else if (cScrollDirection == SCROLL_DOWN) {
      glBegin(GL_QUADS);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex2f(mRight,     mViewBottom);
      glVertex2f(mRight,     mBottomButtonOffset);
      glVertex2f(mViewRight, mBottomButtonOffset);
      glVertex2f(mViewRight, mViewBottom);
      glEnd();
    }

    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(mRight - 0.01f * mAspectRatio, mTop - 0.03f);
    glVertex2f(mRight - 0.02f * mAspectRatio, mTop - 0.01f);
    glVertex2f(mRight - 0.02f * mAspectRatio, mTop - 0.01f);
    glVertex2f(mRight - 0.03f * mAspectRatio, mTop - 0.03f);

    glVertex2f(mRight - 0.01f * mAspectRatio, mViewBottom + 0.03f);
    glVertex2f(mRight - 0.02f * mAspectRatio, mViewBottom + 0.01f);
    glVertex2f(mRight - 0.02f * mAspectRatio, mViewBottom + 0.01f);
    glVertex2f(mRight - 0.03f * mAspectRatio, mViewBottom + 0.03f);
    glEnd();

    glColor3f(0.45f, 0.0f, 0.9f);
  }

  if (isHorizontalBarVisible()) {
    glBegin(GL_LINES);
    glVertex2f(mLeft,  mViewBottom);
    glVertex2f(mRight, mViewBottom);
    float mLeftButtonOffset;
    float mRightButtonOffset;
    if (mViewRight - mLeft <= 0.08 * mAspectRatio) {
      mLeftButtonOffset = mLeft + (mViewRight - mLeft) / 2.0f;
      mRightButtonOffset = mLeftButtonOffset;
      glVertex2f(mLeftButtonOffset, mViewBottom);
      glVertex2f(mLeftButtonOffset, mBottom);
    } else {
      mLeftButtonOffset  = mLeft      + 0.04f * mAspectRatio;
      mRightButtonOffset = mViewRight - 0.04f * mAspectRatio;
      glVertex2f(mLeftButtonOffset,  mViewBottom);
      glVertex2f(mLeftButtonOffset,  mBottom);
      glVertex2f(mRightButtonOffset, mViewBottom);
      glVertex2f(mRightButtonOffset, mBottom);

      float mHorizontalRight = getHorizontalRight();
      float mHorizontalLeft  = getHorizontalLeft();
      glEnd();
      glBegin(GL_QUADS);
      glColor3f(0.9f, 0.0f, 0.45f);
      glVertex2f(mHorizontalRight, mViewBottom);
      glVertex2f(mHorizontalLeft,  mViewBottom);
      glColor3f(0.15f, 0.0f, 0.3f);
      glVertex2f(mHorizontalLeft,  mBottom);
      glVertex2f(mHorizontalRight, mBottom);
      glEnd();
    }

    if (cScrollDirection == SCROLL_LEFT) {
      glBegin(GL_QUADS);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex2f(mLeftButtonOffset, mBottom);
      glVertex2f(mLeftButtonOffset, mViewBottom);
      glVertex2f(mLeft,             mViewBottom);
      glVertex2f(mLeft,             mBottom);
      glEnd();
    } else if (cScrollDirection == SCROLL_RIGHT) {
      glBegin(GL_QUADS);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex2f(mViewRight,         mBottom);
      glVertex2f(mViewRight,         mViewBottom);
      glVertex2f(mRightButtonOffset, mViewBottom);
      glVertex2f(mRightButtonOffset, mBottom);
      glEnd();
    }

    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(mLeft + 0.03f * mAspectRatio, mBottom + 0.01f);
    glVertex2f(mLeft + 0.01f * mAspectRatio, mBottom + 0.02f);
    glVertex2f(mLeft + 0.01f * mAspectRatio, mBottom + 0.02f);
    glVertex2f(mLeft + 0.03f * mAspectRatio, mBottom + 0.03f);

    glVertex2f(mViewRight - 0.03f * mAspectRatio, mBottom + 0.01f);
    glVertex2f(mViewRight - 0.01f * mAspectRatio, mBottom + 0.02f);
    glVertex2f(mViewRight - 0.01f * mAspectRatio, mBottom + 0.02f);
    glVertex2f(mViewRight - 0.03f * mAspectRatio, mBottom + 0.03f);
    glColor3f(0.45f, 0.0f, 0.9f);
    glEnd();
  }

  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  int mLeftPixels = mScreen->convertToXPixels(mLeft);
  int mRightPixels = mScreen->convertToXPixels(mViewRight);
  int mTopPixels = mScreen->convertToYPixels(mTop);
  int mBottomPixels = mScreen->convertToYPixels(mViewBottom);

  int mOldValues[4];
  glGetIntegerv(GL_SCISSOR_BOX, mOldValues);
  glScissor(mLeftPixels + 1, mBottomPixels + 1, (mRightPixels - mLeftPixels) - 1, (mTopPixels - mBottomPixels) - 1);
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    cComponents[i]->render();
  }
  glScissor(mOldValues[0], mOldValues[1], mOldValues[2], mOldValues[3]);
}

ScrollableContainer::RootComponentLayout::RootComponentLayout(ScrollableContainer* parent, IComponentSizeCalculator* sizeCalculator) {
  cParent = parent;
  cComponentSizeCalculator = sizeCalculator;
}

float ScrollableContainer::RootComponentLayout::getTop() {
  return cParent->getTop() + cParent->cScrollVertical;
}

float ScrollableContainer::RootComponentLayout::getLeft() {
  return cParent->getLeft() + cParent->cScrollHorizontal;
}

float ScrollableContainer::RootComponentLayout::getBottom() {
  return getTop() - cComponentSizeCalculator->getHeight();
}

float ScrollableContainer::RootComponentLayout::getRight() {
  return getLeft() + cComponentSizeCalculator->getWidth();
}


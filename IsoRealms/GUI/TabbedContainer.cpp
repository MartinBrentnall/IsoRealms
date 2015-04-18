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
#include "TabbedContainer.h"

TabbedContainer::Tab::Tab(const std::string& actualName, const std::string& name, IRectangularComponent* component) {
  cActualName = actualName;
  cName = name;
  cComponent = component;
}

TabbedContainer::TabbedContainer() {
  cActiveTab = 0;
  cTabLayout = new TabLayout(this);
}

void TabbedContainer::addTab(const std::string& actualName, const std::string& name, IRectangularComponent* component) {
  cTabs.push_back(new Tab(actualName, name, component));
}

IComponentBoundsCalculator* TabbedContainer::getTabLayout() {
  return cTabLayout;
}

std::string TabbedContainer::getSelectedTab() {
  return cTabs[cActiveTab]->cActualName;
}

void TabbedContainer::setSelectedTab(const std::string& tab) {
  for (unsigned int i = 0; i < cTabs.size(); i++) {
    if (cTabs[i]->cActualName == tab) {
      cActiveTab = i;
      return;
    }
  }
}

void TabbedContainer::update(unsigned int milliseconds) {
  cTabs[cActiveTab]->cComponent->update(milliseconds);
}

void TabbedContainer::render() {
  float mLeft = getLeft();
  float mBottom = getBottom();
  float mRight = getRight();
  float mTop = getTop();
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  float mX = mLeft;
  float mFontBase = mTop - (mFontSize * 2.0f + 0.01f);
  float mTabBase = mFontBase - 0.02f;
  for (unsigned int i = 0; i < cTabs.size(); i++) {
    if (cActiveTab == i) {
      cTabs[i]->cComponent->render();
      glBindTexture(GL_TEXTURE_2D, 0);
      glColor3f(1.0f, 1.0f, 1.0f);
    } else {
      glColor3f(0.3f, 0.3f, 0.3f);
    }
    mFont->print(mX + 0.01f, mFontBase, mFontSize, 0, cTabs[i]->cName.c_str());
    float mNewX = mX + mFont->getWidth(mFontSize, cTabs[i]->cName.c_str()) + 0.02f;
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.45f, 0.0f, 0.9f);
    glVertex2f(mX, mTabBase);
    glVertex2f(mX, mTop);
    glVertex2f(mNewX, mTop);
    glVertex2f(mNewX, mTabBase);
    if (cActiveTab != i) {
      glVertex2f(mX, mTabBase);
    }
    glEnd();
    mX = mNewX;
  }
  glBegin(GL_LINE_STRIP);
  glVertex2f(mX, mTabBase);
  glVertex2f(mRight, mTabBase);
  glVertex2f(mRight, mBottom);
  glVertex2f(mLeft, mBottom);
  glVertex2f(mLeft, mTabBase);
  glEnd();
}

bool TabbedContainer::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  float mLeft = getLeft();
  float mTop = getTop();
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  float mTabLeft = mLeft;
  float mTabBottom = (mTop - (mFontSize * 2.0f + 0.01f)) - 0.02f;
  for (unsigned int i = 0; i < cTabs.size(); i++) {
    float mTabRight = mTabLeft + mFont->getWidth(mFontSize, cTabs[i]->cName.c_str()) + 0.02f;
    if (mX > mTabLeft && mX < mTabRight && mY > mTabBottom && mY < mTop) {
      cActiveTab = i;
      return true;
    }
    mTabLeft = mTabRight;
  }
  return false;
}

bool TabbedContainer::input(SDL_Event& event) {
  if (cTabs[cActiveTab]->cComponent->input(event)) {
    return true;
  }
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      return mouseButtonDown(event);
    }
  }
  return false;
}

TabbedContainer::TabLayout::TabLayout(TabbedContainer* parent) {
  cParent = parent;
}

float TabbedContainer::TabLayout::getBottom() {
  return cParent->getBottom();
}

float TabbedContainer::TabLayout::getLeft() {
  return cParent->getLeft();
}

float TabbedContainer::TabLayout::getTop() {
  float mTop = cParent->getTop();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  float mFontBase = mTop - (mFontSize * 2.0f + 0.01f);
  float mTabBase = mFontBase - 0.02f;
  return mTabBase;
}

float TabbedContainer::TabLayout::getRight() {
  return cParent->getRight();
}

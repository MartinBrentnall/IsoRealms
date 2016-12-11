
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
#include "ComponentTable.h"

ComponentTable::ComponentTable(unsigned int columns, float padding) {
  cColumns = columns;
  cFocusedComponent = nullptr;
  cPadding = padding;
}

ComponentTable::ComponentTable(std::vector<std::string> headings, float padding) {
  cColumns = headings.size();
  cHeadings = headings;
  cFocusedComponent = nullptr;
  cPadding = padding;
}

void ComponentTable::addRow(std::vector<ISizedComponent*> row) {
  if (row.size() != cColumns) {
    return; // TODO: Throw
  }
  
  std::vector<ISizedComponent*> mRow;
  for (unsigned int i = 0; i < row.size(); i++) {
    mRow.push_back(row[i]);
    IComponentBoundsCalculator* mCellCalculator = new CellLayout(this, cGridComponents.size(), i);
    row[i]->setBoundsCalculator(mCellCalculator);
  }
  cGridComponents.push_back(mRow);
}

void ComponentTable::clear() {
  cGridComponents.clear();
}

float ComponentTable::getWidth() {
  float mGridWidth = 0.0f;
  if (!cGridComponents.empty()) {
    for (unsigned int x = 0; x < cGridComponents[0].size(); x++) {
      mGridWidth += getColumnWidth(x);
    }
    mGridWidth += cGridComponents[0].size() * cPadding;
  } else {
    mGridWidth = cPadding;
  }
  return mGridWidth;
}

float ComponentTable::getHeight() {
  float mGridHeight = 0.0f;
  for (unsigned int y = 0; y < cGridComponents.size(); y++) {
    mGridHeight += getRowHeight(y);
  }
  if (!cHeadings.empty()) {
    mGridHeight += 0.05f;
  }
  return mGridHeight + cGridComponents.size() * cPadding;
}

float ComponentTable::getRowHeight(unsigned int row) {
  float mLargestHeight = 0.0f;
  for (unsigned int x = 0; x < cGridComponents[row].size(); x++) {
    ISizedComponent* mCellComponent = cGridComponents[row][x];
    if (mCellComponent != nullptr) {
      float mCellHeight = mCellComponent->getHeight();
      if (mCellHeight > mLargestHeight) {
        mLargestHeight = mCellHeight;
      }
    }
  }
  return mLargestHeight;
}

float ComponentTable::getColumnWidth(unsigned int column) {
  float mLargestWidth = 0.0f;
  for (unsigned int y = 0; y < cGridComponents.size(); y++) {
    ISizedComponent* mCellComponent = cGridComponents[y][column];
    if (mCellComponent != nullptr) {
      float mCellWidth = mCellComponent->getWidth();
      if (mCellWidth > mLargestWidth) {
        mLargestWidth = mCellWidth;
      }
    }
  }
  return mLargestWidth;
}

void ComponentTable::update(unsigned int ticks) {
  // Nothing to do
}

void ComponentTable::render() {
  float mTop    = getTop();
  float mLeft   = getLeft();
  float mRight  = getRight();
  float mBottom = mTop - 0.05f; // TODO: Heading height
  
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  if (!cHeadings.empty()) {
    glColor4f(0.15f, 0.0f, 0.3f, 1.0f);
    glVertex2f(mLeft,  mBottom);
    glVertex2f(mRight, mBottom);
    glColor4f(0.9f, 0.0f, 0.45f, 1.0f);
    glVertex2f(mRight, mTop);
    glVertex2f(mLeft,  mTop);
    mTop = mBottom;
  }
  
  glColor4f(0.45f, 0.0f, 0.9f, 0.3f);
  for (unsigned int y = 0; y < cGridComponents.size(); y++) {
    float mRowHeight = getRowHeight(y) + cPadding;
    mBottom          = mTop - mRowHeight;
    if (y % 2 == 1) {
      glVertex2f(mLeft,  mBottom);
      glVertex2f(mRight, mBottom);
      glVertex2f(mRight, mTop);
      glVertex2f(mLeft,  mTop);
    }
    mTop = mBottom;
  }
  glEnd();
  glDisable(GL_BLEND);
  glColor3f(1.0f, 1.0f, 1.0f);

  float mColumnLeft = mLeft;
  unsigned int mColumn = 0;  
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  glColor3f(1.0f, 1.0f, 1.0f);
  
  mTop = getTop();
  for (std::string mHeading : cHeadings) {
    mFont->print(mColumnLeft + 0.01f, mTop - 0.04f, mFontSize, IFont::LEFT, mHeading.c_str());
    mColumnLeft += getColumnWidth(mColumn++);
  }
  
  for (unsigned int y = 0; y < cGridComponents.size(); y++) {
    for (unsigned int x = 0; x < cGridComponents[y].size(); x++) {
      ISizedComponent* mComponent = cGridComponents[y][x];
      if (mComponent != nullptr) {
        mComponent->render();
      }
    }
  }
}

void ComponentTable::testFocusChange(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      if (event.button.button == SDL_BUTTON_LEFT) {
        Configuration* mConfiguration = Configuration::getInstance();
        ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
        float mX = mScreen->getXLocation(event.button.x);
        float mY = mScreen->getYLocation(event.button.y);
        for (unsigned int y = 0; y < cGridComponents.size(); y++) {
          for (unsigned int x = 0; x < cGridComponents[y].size(); x++) {
            ISizedComponent* mComponent = cGridComponents[y][x];
            if (mComponent != nullptr && mComponent->contains(mX, mY)) {
              if (cFocusedComponent != nullptr) {
                cFocusedComponent->lostFocus();
              }
              cFocusedComponent = mComponent;
              cFocusedComponent->gainedFocus();
              return;
            }
          }
        }
        if (cFocusedComponent != nullptr) {
          cFocusedComponent->lostFocus();
          cFocusedComponent = nullptr;
        }
      }
    }
  }
}

bool ComponentTable::input(SDL_Event& event) {
  testFocusChange(event);
  return cFocusedComponent != nullptr ? cFocusedComponent->input(event) : false;
}

ComponentTable::CellLayout::CellLayout(ComponentTable* parent, unsigned int row, unsigned int column) {
  cParent = parent;
  cRow = row;
  cColumn = column;
}

float ComponentTable::CellLayout::getLeft() {
  float cLeftOffset = cParent->getLeft();
  for (unsigned int x = 0; x < cColumn; x++) {
    cLeftOffset += cParent->getColumnWidth(x) + cParent->cPadding;
  }
  return cLeftOffset + cParent->cPadding / 2.0f;
}

float ComponentTable::CellLayout::getRight() {
  float cLeftOffset = cParent->getLeft();
  for (unsigned int x = 0; x <= cColumn; x++) {
    cLeftOffset += cParent->getColumnWidth(x) + cParent->cPadding;
  }
  return cLeftOffset - cParent->cPadding / 2.0f;
}

float ComponentTable::CellLayout::getTop() {
  float cTopOffset = cParent->getTop();
  if (!cParent->cHeadings.empty()) {
    cTopOffset -= 0.05f;
  }
  for (unsigned int y = 0; y < cRow; y++) {
    cTopOffset -= cParent->getRowHeight(y) + cParent->cPadding;
  }
  return cTopOffset - cParent->cPadding / 2.0f;
}

float ComponentTable::CellLayout::getBottom() {
  float cTopOffset = cParent->getTop();
  if (!cParent->cHeadings.empty()) {
    cTopOffset -= 0.05f;
  }
  for (unsigned int y = 0; y <= cRow; y++) {
    cTopOffset -= cParent->getRowHeight(y) + cParent->cPadding;
  }
  return cTopOffset + cParent->cPadding / 2.0f;
}




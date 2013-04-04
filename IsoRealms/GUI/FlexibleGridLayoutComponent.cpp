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
#include "FlexibleGridLayoutComponent.h"

FlexibleGridLayoutComponent::FlexibleGridLayoutComponent(unsigned int columns, unsigned int rows) {
  // TODO: Throw IllegalArgumentException
  for (unsigned int i = 0; i < rows; i++) {
    std::vector<ISizedComponent*> mRow;
    for (unsigned int j = 0; j < columns; j++) {
      mRow.push_back(NULL);
    }
    cGridComponents.push_back(mRow);
  }
  cFocusedComponent = NULL;
}

void FlexibleGridLayoutComponent::addComponent(ISizedComponent* component, unsigned int column, unsigned int row) {
  cGridComponents[row][column] = component;
  // TODO: Who cleans up all these cell calculators?
  IComponentBoundsCalculator* mCellCalculator = new CellLayout(this, row, column);
  component->setBoundsCalculator(mCellCalculator);
}

float FlexibleGridLayoutComponent::getWidth() {
  float mGridWidth = 0.0f;
  if (!cGridComponents.empty()) {
    for (unsigned int x = 0; x < cGridComponents[0].size(); x++) {
      mGridWidth += getColumnWidth(x);
    }
    mGridWidth += cGridComponents[0].size() * 0.02f;
  } else {
    mGridWidth = 0.02f;
  }
  return mGridWidth;
}

float FlexibleGridLayoutComponent::getHeight() {
  float mGridHeight = 0.0f;
  for (unsigned int y = 0; y < cGridComponents.size(); y++) {
    mGridHeight += getRowHeight(y);
  }
  return mGridHeight + cGridComponents.size() * 0.02f;
}

float FlexibleGridLayoutComponent::getRowHeight(unsigned int row) {
  float mLargestHeight = 0.0f;
  for (unsigned int x = 0; x < cGridComponents[row].size(); x++) {
    ISizedComponent* mCellComponent = cGridComponents[row][x];
    if (mCellComponent != NULL) {
      float mCellHeight = mCellComponent->getHeight();
      if (mCellHeight > mLargestHeight) {
        mLargestHeight = mCellHeight;
      }
    }
  }
  return mLargestHeight;
}

float FlexibleGridLayoutComponent::getColumnWidth(unsigned int column) {
  float mLargestWidth = 0.0f;
  for (unsigned int y = 0; y < cGridComponents.size(); y++) {
    ISizedComponent* mCellComponent = cGridComponents[y][column];
    if (mCellComponent != NULL) {
      float mCellWidth = mCellComponent->getWidth();
      if (mCellWidth > mLargestWidth) {
        mLargestWidth = mCellWidth;
      }
    }
  }
  return mLargestWidth;
}

void FlexibleGridLayoutComponent::update(unsigned int ticks) {
  // Nothing to do
}

void FlexibleGridLayoutComponent::render() {
  for (unsigned int y = 0; y < cGridComponents.size(); y++) {
    for (unsigned int x = 0; x < cGridComponents[y].size(); x++) {
      ISizedComponent* mComponent = cGridComponents[y][x];
      if (mComponent != NULL) {
        mComponent->render();
      }
    }
  }
}

void FlexibleGridLayoutComponent::testFocusChange(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      Configuration* mConfiguration = Configuration::getInstance();
      ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
      float mX = mScreen->getXLocation(event.button.x);
      float mY = mScreen->getYLocation(event.button.y);
      for (unsigned int y = 0; y < cGridComponents.size(); y++) {
        for (unsigned int x = 0; x < cGridComponents[y].size(); x++) {
          ISizedComponent* mComponent = cGridComponents[y][x];
          if (mComponent != NULL && mComponent->contains(mX, mY)) {
            cFocusedComponent = mComponent;
            return;
          }
        }
      }
    }
    cFocusedComponent = NULL;
  }
}

bool FlexibleGridLayoutComponent::input(SDL_Event& event) {
  testFocusChange(event);
  return cFocusedComponent != NULL ? cFocusedComponent->input(event) : false;
}

FlexibleGridLayoutComponent::CellLayout::CellLayout(FlexibleGridLayoutComponent* parent, unsigned int row, unsigned int column) {
  cParent = parent;
  cRow = row;
  cColumn = column;
}

float FlexibleGridLayoutComponent::CellLayout::getLeft() {
  float cLeftOffset = cParent->getLeft();
  for (unsigned int x = 0; x < cColumn; x++) {
    cLeftOffset += cParent->getColumnWidth(x) + 0.02f;
  }
  return cLeftOffset + 0.01f;
}

float FlexibleGridLayoutComponent::CellLayout::getRight() {
  float cLeftOffset = cParent->getLeft();
  for (unsigned int x = 0; x <= cColumn; x++) {
    cLeftOffset += cParent->getColumnWidth(x) + 0.02f;
  }
  return cLeftOffset - 0.01f;
}

float FlexibleGridLayoutComponent::CellLayout::getTop() {
  float cBottomOffset = cParent->getBottom();
  for (unsigned int y = 0; y <= cRow; y++) {
    cBottomOffset += cParent->getRowHeight(y) + 0.02f;
  }
  return cBottomOffset - 0.01f;
}

float FlexibleGridLayoutComponent::CellLayout::getBottom() {
  float cBottomOffset = cParent->getBottom();
  for (unsigned int y = 0; y < cRow; y++) {
    cBottomOffset += cParent->getRowHeight(y) + 0.02f;
  }
  return cBottomOffset + 0.01f;
}




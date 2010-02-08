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
#include "GridLayoutComponent.h"

GridLayoutComponent::GridLayoutComponent(unsigned int horizontalCells, unsigned int verticalCells, IComponentBoundsCalculator* componentBoundsCalculator) {
  if (horizontalCells == 0 || verticalCells == 0) {
    std::cout << "Grid must have one or more cells" << std::endl;
    // TODO: Throw wobbly instead of exiting.
    exit(1);
  }
  cComponentBoundsCalculator = componentBoundsCalculator;
  cHorizontalCells = horizontalCells;
  cVerticalCells = verticalCells;
  cCellPadding = 0.02f;
}

IComponentBoundsCalculator* GridLayoutComponent::getCellLayout(unsigned int x, unsigned int y) {
  if (x < 0 || x >= cHorizontalCells || y < 0 || y >= cVerticalCells) {
    std::cout << "Specified cell is outside range of the grid" << std::endl;
    // TODO: Throw wobbly instead of exiting.
    exit(1);
  }
  std::map<unsigned int, std::map<unsigned int, CellBoundsCalculator*> >::iterator mRow = cCellCalculators.find(x);
  if (mRow != cCellCalculators.end()) {
    std::map<unsigned int, CellBoundsCalculator*>::iterator mCell = mRow->second.find(y);
    if (mCell != mRow->second.end()) {
      return mCell->second;
    }
  }
  CellBoundsCalculator* mCellBoundsCalculator = new CellBoundsCalculator(this, x, y);
  cCellCalculators[x][y] = mCellBoundsCalculator;
  return mCellBoundsCalculator;
}

float GridLayoutComponent::getCellWidth() {
  int mSeparators = cHorizontalCells - 1;
  float mTotalSeparatorSpace = cCellPadding * mSeparators;
  float mTotalWidth = cComponentBoundsCalculator->getRight() - cComponentBoundsCalculator->getLeft();
  float mTotalCellSpace = mTotalWidth - mTotalSeparatorSpace;
  return mTotalCellSpace / cHorizontalCells;
}

float GridLayoutComponent::getCellHeight() {
  int mSeparators = cVerticalCells - 1;
  float mTotalSeparatorSpace = cCellPadding * mSeparators;
  float mTotalWidth = cComponentBoundsCalculator->getTop() - cComponentBoundsCalculator->getBottom();
  float mTotalCellSpace = mTotalWidth - mTotalSeparatorSpace;
  return mTotalCellSpace / cVerticalCells;
}

float GridLayoutComponent::getCellLeft(unsigned int x) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  return cComponentBoundsCalculator->getLeft() + x * (getCellWidth() + cCellPadding);
}

float GridLayoutComponent::getCellRight(unsigned int x) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  return cComponentBoundsCalculator->getLeft() + ((x + 1) * getCellWidth()) + x * cCellPadding;
}

float GridLayoutComponent::getCellTop(unsigned int y) {
  return cComponentBoundsCalculator->getBottom() + ((y + 1) * getCellHeight()) + y * cCellPadding;
}

float GridLayoutComponent::getCellBottom(unsigned int y) {
  return cComponentBoundsCalculator->getBottom() + y * (getCellHeight() + cCellPadding);
}

GridLayoutComponent::CellBoundsCalculator::CellBoundsCalculator(GridLayoutComponent* parent, unsigned int x, unsigned int y) {
  cParent = parent;
  cX = x;
  cY = y;
}

float GridLayoutComponent::CellBoundsCalculator::getLeft() {
  return cParent->getCellLeft(cX);
}

float GridLayoutComponent::CellBoundsCalculator::getRight() {
  return cParent->getCellRight(cX);
}

float GridLayoutComponent::CellBoundsCalculator::getTop() {
  return cParent->getCellTop(cY);
}

float GridLayoutComponent::CellBoundsCalculator::getBottom() {
  return cParent->getCellBottom(cY);
}

GridLayoutComponent::~GridLayoutComponent() {
  for (std::map<unsigned int, std::map<unsigned int, CellBoundsCalculator*> >::iterator x = cCellCalculators.begin(); x != cCellCalculators.end(); ++x) {
    for (std::map<unsigned int, CellBoundsCalculator*>::iterator y = x->second.begin(); y != x->second.end(); ++y) {
      delete y->second;
    }
  }
}

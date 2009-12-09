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

void GridLayoutComponent::setComponentInCell(unsigned int x, unsigned int y, AbstractRectangularComponent* component) {
  if (x < 0 || x >= cHorizontalCells || y < 0 || y >= cVerticalCells) {
    std::cout << "Specified cell is outside range of the grid" << std::endl;
    // TODO: Throw wobbly instead of exiting.
    exit(1);
  }
  std::map<unsigned int, std::map<unsigned int, CellBoundsCalculator*> >::iterator mRow = cCellCalculators.find(x);
  if (mRow != cCellCalculators.end()) {
    std::map<unsigned int, CellBoundsCalculator*>::iterator mCell = mRow->second.find(y);
    if (mCell != mRow->second.end()) {
      std::cout << "Specified cell already contains a component" << std::endl;
      // TODO: Throw wobbly instead of exiting.
      exit(1);
    }
  }
  CellBoundsCalculator* mCellBoundsCalculator = new CellBoundsCalculator(this, x, y);
  cCellCalculators[x][y] = mCellBoundsCalculator;
  component->setBoundsCalculator(mCellBoundsCalculator);
}

float GridLayoutComponent::getCellWidth() {
  return (cComponentBoundsCalculator->getRight() - cComponentBoundsCalculator->getLeft()) / cHorizontalCells;
}

float GridLayoutComponent::getCellHeight() {
  return (cComponentBoundsCalculator->getTop() - cComponentBoundsCalculator->getBottom()) / cVerticalCells;
}

float GridLayoutComponent::getCellLeft(unsigned int x) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  return cComponentBoundsCalculator->getLeft() + x * getCellWidth() + cCellPadding * mAspectRatio;
}

float GridLayoutComponent::getCellRight(unsigned int x) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  return (cComponentBoundsCalculator->getLeft() + (x + 1) * getCellWidth()) - cCellPadding * mAspectRatio;
}

float GridLayoutComponent::getCellTop(unsigned int y) {
  return (cComponentBoundsCalculator->getBottom() + (y + 1) * getCellHeight()) - cCellPadding;
}

float GridLayoutComponent::getCellBottom(unsigned int y) {
  return cComponentBoundsCalculator->getBottom() + y * getCellHeight() + cCellPadding;
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

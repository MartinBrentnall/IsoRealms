#include "WrappingGridComponent.h"

WrappingGridComponent::CellLayout::CellLayout(WrappingGridComponent* parent, ISizedComponent* component) {
  cParent = parent;
  cComponent = component;
}

float WrappingGridComponent::CellLayout::getLeft() {
  float mGridLeft = cParent->getLeft();
  float mGridRight = cParent->getRight();
  float mAvailableWidth = mGridRight - mGridLeft;
  float mUsedWidth = 0.0f;
  for (unsigned int i = 0; i < cParent->cComponents.size(); i++) {
    float mComponentWidth = cParent->cComponents[i]->getWidth();
    if (cComponent == cParent->cComponents[i]) {
      float mTotalWidth = mUsedWidth + mComponentWidth;
      return mGridLeft + (mTotalWidth > mAvailableWidth ? 0 : mUsedWidth);
    }
    mUsedWidth += mComponentWidth + 0.02f;
    if (mUsedWidth - 0.02f  > mAvailableWidth) {
      mUsedWidth = mComponentWidth + 0.02f;
    }
  }
  std::cout << "Warning: failed to calculate left edge correctly" << std::endl;
  return 0.0f;
}

float WrappingGridComponent::CellLayout::getRight() {
  return getLeft() + cComponent->getWidth();
}

float WrappingGridComponent::CellLayout::getTop() {
  return getBottom() + cComponent->getHeight();
}

float WrappingGridComponent::CellLayout::getBottom() {
  float mGridLeft = cParent->getLeft();
  float mGridRight = cParent->getRight();
  float mGridTop = cParent->getTop();
  float mAvailableWidth = mGridRight - mGridLeft;
  float mUsedWidth = 0.0f;
  float mUsedHeight = 0.0f;
  float mLargestHeight = 0.0f;
  for (unsigned int i = 0; i < cParent->cComponents.size(); i++) {
    float mComponentWidth = cParent->cComponents[i]->getWidth();
    float mComponentHeight = cParent->cComponents[i]->getHeight();
    if (mComponentHeight > mLargestHeight) {
      mLargestHeight = mComponentHeight;
    }
    mUsedWidth += mComponentWidth + 0.02f;
    if (mUsedWidth - 0.02f > mAvailableWidth && i != 0) {
      mUsedWidth = mComponentWidth + 0.02f;
      mUsedHeight += mLargestHeight + 0.02f;
      mLargestHeight = mComponentHeight;
    }
    if (cComponent == cParent->cComponents[i]) {
      return (mGridTop - mUsedHeight) - mComponentHeight;
    }
  }
  std::cout << "Warning: failed to calculate right edge correctly" << std::endl;
  return 0.0f;
}

WrappingGridComponent::WrappingGridComponent() {
}

unsigned int WrappingGridComponent::getRow(ISizedComponent* component) {
  float mGridLeft = getLeft();
  float mGridRight = getRight();
  float mAvailableWidth = mGridRight - mGridLeft;
  float mUsedWidth = 0.0f;
  unsigned int mRowCount = 0;
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    float mComponentWidth = cComponents[i]->getWidth();
    mUsedWidth += mComponentWidth + 0.02f;
    if (mUsedWidth - 0.02f > mAvailableWidth && i != 0) {
      mRowCount++;
    }
    if (cComponents[i] == component) {
      return mRowCount;
    }
  }
  return mRowCount;
}

float WrappingGridComponent::getHeight() {
  float mGridLeft = getLeft();
  float mGridRight = getRight();
  float mAvailableWidth = mGridRight - mGridLeft;
  float mUsedWidth = 0.0f;
  float mUsedHeight = 0.0f;
  float mLargestHeight = 0.0f;
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    float mComponentWidth = cComponents[i]->getWidth();
    float mComponentHeight = cComponents[i]->getHeight();
    if (mComponentHeight > mLargestHeight) {
      mLargestHeight = mComponentHeight;
    }
    mUsedWidth += mComponentWidth + 0.02f;
    if (mUsedWidth - 0.02f > mAvailableWidth && i != 0) {
      mUsedWidth = mComponentWidth + 0.02f;
      mUsedHeight += mLargestHeight + 0.02f;
      mLargestHeight = mComponentHeight + 0.02f;
    }
  }
  return mUsedHeight + mLargestHeight;
}

void WrappingGridComponent::addComponent(ISizedComponent* component) {
  IComponentBoundsCalculator* mCellLayout = new CellLayout(this, component);
  component->setBoundsCalculator(mCellLayout);
  cComponents.push_back(component);
}

void WrappingGridComponent::update(int ticks) {
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    cComponents[i]->update(ticks);
  }
}

void WrappingGridComponent::render() {
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    cComponents[i]->render();
  }
}

bool WrappingGridComponent::input(SDL_Event& event) {
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    if (cComponents[i]->input(event)) {
      return true;
    }
  }
  return false;
}

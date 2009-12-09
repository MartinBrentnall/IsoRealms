#include "AbstractRectangularComponent.h"

AbstractRectangularComponent::AbstractRectangularComponent() {
  setBoundsCalculator(NULL);
}

AbstractRectangularComponent::AbstractRectangularComponent(IComponentBoundsCalculator* componentBoundsCalculator) {
  setBoundsCalculator(componentBoundsCalculator);
}

void AbstractRectangularComponent::setBoundsCalculator(IComponentBoundsCalculator* componentBoundsCalculator) {
  cComponentBoundsCalculator = componentBoundsCalculator;
}

float AbstractRectangularComponent::getLeft() {
  return cComponentBoundsCalculator != NULL ? cComponentBoundsCalculator->getLeft() : 0.0f;
}

float AbstractRectangularComponent::getTop() {
  return cComponentBoundsCalculator != NULL ? cComponentBoundsCalculator->getTop() : 0.0f;
}

float AbstractRectangularComponent::getRight() {
  return cComponentBoundsCalculator != NULL ? cComponentBoundsCalculator->getRight() : 0.0f;
}

float AbstractRectangularComponent::getBottom() {
  return cComponentBoundsCalculator != NULL ? cComponentBoundsCalculator->getBottom() : 0.0f;
}

bool AbstractRectangularComponent::contains(float x, float y) {
  return x >= getLeft() && x <= getRight() && y >= getBottom() && y <= getTop();
}

AbstractRectangularComponent::~AbstractRectangularComponent() {
  delete cComponentBoundsCalculator;
}

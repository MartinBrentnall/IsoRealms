#include "RectangleBoundsCalculator.h"

RectangleBoundsCalculator::RectangleBoundsCalculator(IRectangularComponent* rectangle) {
  cRectangle = rectangle;
}

float RectangleBoundsCalculator::getLeft() {
  return cRectangle->getLeft();
}

float RectangleBoundsCalculator::getRight() {
  return cRectangle->getRight();
}

float RectangleBoundsCalculator::getTop() {
  return cRectangle->getTop();
}

float RectangleBoundsCalculator::getBottom() {
  return cRectangle->getBottom();
}


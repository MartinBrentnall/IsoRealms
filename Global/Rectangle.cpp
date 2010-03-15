#include "Rectangle.h"

Rectangle::Rectangle(float top, float left, float bottom, float right) {
  cTop = top;
  cLeft = left;
  cBottom = bottom;
  cRight = right;
}

float Rectangle::getTop() {
  return cTop;
}

float Rectangle::getLeft() {
  return cLeft;
}

float Rectangle::getBottom() {
  return cBottom;
}

float Rectangle::getRight() {
  return cRight;
}

bool Rectangle::contains(Rectangle& rectangle) {
  return cTop    >= rectangle.cTop
      && cLeft   <= rectangle.cLeft
      && cBottom <= rectangle.cBottom
      && cRight  >= rectangle.cRight;
}

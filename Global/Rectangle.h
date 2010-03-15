#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "IRectangle.h"

class Rectangle:public IRectangle {
  private:
  float cTop;
  float cLeft;
  float cBottom;
  float cRight;

  public:
  Rectangle(float, float, float, float);

  bool contains(Rectangle&);

  /*************************\
   * Implements IRectangle *
  \*************************/
  float getTop();
  float getLeft();
  float getBottom();
  float getRight();
};

#endif

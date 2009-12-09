#ifndef RECTANGLE_BOUNDS_CALCULATOR_H
#define RECTANGLE_BOUNDS_CALCULATOR_H

#include "IComponentBoundsCalculator.h"
#include "IRectangularComponent.h"

class RectangleBoundsCalculator:public IComponentBoundsCalculator {
  private:
  IRectangularComponent* cRectangle;

  public:
  RectangleBoundsCalculator(IRectangularComponent* rectangle);

  /*****************************************\
   * Implements IComponentBoundsCalculator *
  \*****************************************/
  float getLeft();
  float getRight();
  float getTop();
  float getBottom();
};

#endif

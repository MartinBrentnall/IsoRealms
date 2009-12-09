#ifndef I_COMPONENT_BOUNDS_CALCULATOR_H
#define I_COMPONENT_BOUNDS_CALCULATOR_H

class IComponentBoundsCalculator {
  public:
  virtual float getLeft() = 0;
  virtual float getRight() = 0;
  virtual float getTop() = 0;
  virtual float getBottom() = 0;
};

#endif

#ifndef I_COMPONENT_HEIGHT_CALCULATOR_H
#define I_COMPONENT_HEIGHT_CALCULATOR_H

#include "AbstractRectangularComponent.h"
#include "IComponentSizeCalculator.h"

/**
 * Width of the component can be controlled.  Height is determined automatically.
 */
class IComponentHeightCalculator {
  public:
  virtual float getHeight() = 0;
};

#endif

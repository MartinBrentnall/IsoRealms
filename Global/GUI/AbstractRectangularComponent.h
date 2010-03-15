#ifndef ABSTRACT_RECTANGULAR_COMPONENT_H
#define ABSTRACT_RECTANGULAR_COMPONENT_H

#include <iostream>
#include <vector>

#include "IComponentBoundsCalculator.h"
#include "IRectangularComponent.h"

/**
 * The abstract rectangular component is a component whose bounds are defined
 * via an instance of the IComponentBoundsCalculator interface.  The bounds
 * calculator must be set before functions implemented by IRectangularComponent
 * can be called.
 */
class AbstractRectangularComponent:public IRectangularComponent {
  private:
  IComponentBoundsCalculator* cComponentBoundsCalculator;

  public:
  AbstractRectangularComponent();
  AbstractRectangularComponent(IComponentBoundsCalculator*);
  void setBoundsCalculator(IComponentBoundsCalculator*);

  /****************************************************\
   * Implements IRectangle (in IRectangularComponent) *
  \****************************************************/
  float getLeft();
  float getTop();
  float getRight();
  float getBottom();

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  bool contains(float, float);

  virtual ~AbstractRectangularComponent();
};

#endif

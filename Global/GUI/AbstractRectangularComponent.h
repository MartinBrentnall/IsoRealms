/*
 * Copyright 2009,2010 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
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

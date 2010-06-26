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
#ifndef COMPONENT_EDGE_LAYOUT_H
#define COMPONENT_EDGE_LAYOUT_H

#include <cstdlib>
#include <iostream>

#include "EdgeRelation.h"
#include "IComponentBoundsCalculator.h"
#include "IComponentSizeCalculator.h"

/**
 * This implementation of component bounds calculator will place a component
 * such that it is pressed into a corner, either inside another component or
 * against the outside of two different components.
 */
class ComponentEdgeLayout:public IComponentBoundsCalculator {
  private:
  IComponentSizeCalculator* cSizeCalculator;
  EdgeRelation* cTop;
  EdgeRelation* cBottom;
  EdgeRelation* cLeft;
  EdgeRelation* cRight;

  public:
  ComponentEdgeLayout(EdgeRelation*, EdgeRelation*, EdgeRelation*, EdgeRelation*, IComponentSizeCalculator*);

  /*****************************************\
   * Implements IComponentBoundsCalculator *
  \*****************************************/
  float getLeft();
  float getRight();
  float getTop();
  float getBottom();

  virtual ~ComponentEdgeLayout();
};

#endif

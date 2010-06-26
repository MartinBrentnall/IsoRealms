/*
 * Copyright 2009 Martin Brentnall
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
#ifndef GRID_LAYOUT_COMPONENT_H
#define GRID_LAYOUT_COMPONENT_H

#include <iostream>
#include <map>

#include "../Configuration.h"
#include "../ScreenConfiguration.h"

#include "AbstractRectangularComponent.h"
#include "IComponentBoundsCalculator.h"

class GridLayoutComponent {
  private:
  class CellBoundsCalculator:public IComponentBoundsCalculator {
    private:
    GridLayoutComponent* cParent;
    unsigned int cX;
    unsigned int cY;
    
    public:
    CellBoundsCalculator(GridLayoutComponent*, unsigned int, unsigned int);

    /*****************************************\
     * Implements IComponentBoundsCalculator *
    \*****************************************/
    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
  };

  IComponentBoundsCalculator* cComponentBoundsCalculator;
  std::map<unsigned int, std::map<unsigned int, CellBoundsCalculator*> > cCellCalculators;
  unsigned int cHorizontalCells;
  unsigned int cVerticalCells;
  float cCellPadding;

  float getCellWidth();
  float getCellHeight();

  float getCellLeft(unsigned int);
  float getCellRight(unsigned int);
  float getCellTop(unsigned int);
  float getCellBottom(unsigned int);

  public:
  GridLayoutComponent(unsigned int, unsigned int, IComponentBoundsCalculator*);

  /**
   * Set the component to be displayed in the specified cell.
   * 
   * @param int  The X cell position.
   * @param int  The Y cell position.
   * @throws ? TODO: OutOfBoundsException
   */
  IComponentBoundsCalculator* getCellLayout(unsigned int, unsigned int);

  virtual ~GridLayoutComponent();
};

#endif

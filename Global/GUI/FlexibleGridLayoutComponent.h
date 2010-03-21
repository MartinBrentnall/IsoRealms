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
#ifndef FLEXIBLE_GRID_LAYOUT_COMPONENT_H
#define FLEXIBLE_GRID_LAYOUT_COMPONENT_H

#include "../Configuration.h"
#include "../ScreenConfiguration.h"

#include "AbstractRectangularComponent.h"
#include "IComponentBoundsCalculator.h"
#include "IComponentSizeCalculator.h"
#include "ISizedComponent.h"

class FlexibleGridLayoutComponent:public ISizedComponent {
  private:
  std::vector<std::vector<ISizedComponent*> > cGridComponents;
  ISizedComponent* cFocusedComponent;

  float getColumnWidth(unsigned int);
  float getRowHeight(unsigned int);

  void testFocusChange(SDL_Event& event);

  class CellLayout:public IComponentBoundsCalculator {
    private:
    FlexibleGridLayoutComponent* cParent;
    unsigned int cRow;
    unsigned int cColumn;

    public:
    CellLayout(FlexibleGridLayoutComponent*, unsigned int, unsigned int);

    /*****************************************\
     * Implements IComponentBoundsCalculator *
    \*****************************************/
    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
  };

  public:
  FlexibleGridLayoutComponent(unsigned int, unsigned int);

  /**
   * Set the component to be displayed in the specified cell.
   * 
   * 
   * @param int  The X cell position.
   * @param int  The Y cell position.
   * @throws ? TODO: OutOfBoundsException
   */
  void addComponent(ISizedComponent*, unsigned int, unsigned int);

  virtual ~FlexibleGridLayoutComponent() {};

  /******************************\
   * Implements ISizedComponent *
  \******************************/
  float getWidth();
  float getHeight();
  void update(int);
  void render();
  bool input(SDL_Event&);
};

#endif

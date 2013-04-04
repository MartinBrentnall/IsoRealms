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
#ifndef WRAPPING_GRID_COMPONENT_H
#define WRAPPING_GRID_COMPONENT_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>

#include "AbstractRectangularComponent.h"
#include "../Configuration.h"
#include "IComponentHolder.h"
#include "IComponentBoundsCalculator.h"
#include "ISizedComponent.h"
#include "AbstractVerticalComponent.h"

class WrappingGridComponent:public AbstractVerticalComponent,
                            public IComponentHolder {
  private:
  std::vector<ISizedComponent*> cComponents;
  float cCellPaddingX;
  float cCellPaddingY;

  class CellLayout:public IComponentBoundsCalculator {
    private:
    WrappingGridComponent* cParent;
    ISizedComponent* cComponent;

    public:
    CellLayout(WrappingGridComponent*, ISizedComponent*);

    /*****************************************\
     * Implements IComponentBoundsCalculator *
    \*****************************************/
    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
  };

  unsigned int getRow(ISizedComponent*);

  public:
  WrappingGridComponent(float);
  void setPadding(float);

  void addComponent(const std::string&, ISizedComponent* component);
  void removeComponent(ISizedComponent* component);

  /*******************************************\
   * Implements AbstractRectangularComponent *
  \*******************************************/
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
  
  /****************************************\
   * Implements IWidthControlledComponent *
  \****************************************/
  float getHeight();
  float getWidth();
};

#endif

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
#ifndef SCROLLABLE_CONTAINER_H
#define SCROLLABLE_CONTAINER_H

#include <cfloat>
#include <cmath>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <vector>

#include "../Configuration.h"
#include "../ScreenConfiguration.h"

#include "AbstractRectangularComponent.h"
#include "AbstractVerticalComponent.h"
#include "IComponentBoundsCalculator.h"
#include "IComponentHeightCalculator.h"
#include "IComponentHolder.h"
#include "IComponentSizeCalculator.h"
#include "ISizedComponent.h"
#include "IRectangularComponent.h"

class ScrollableContainer:public AbstractRectangularComponent,
                          public IComponentHolder {
  private:
  enum ScrollDirection {
    SCROLL_NONE,
    SCROLL_LEFT,
    SCROLL_RIGHT,
    SCROLL_UP,
    SCROLL_DOWN
  };
  ScrollDirection cScrollDirection;
  bool cScrollHorizontalActive;
  bool cScrollVerticalActive;
  std::vector<IRectangularComponent*> cComponents;
  IRectangularComponent* cFocusedComponent;
  float cScrollHorizontal;
  float cScrollVertical;

  class RootComponentLayout:public IComponentBoundsCalculator {
    private:
    ScrollableContainer* cParent;
    IComponentSizeCalculator* cComponentSizeCalculator;
  
    public:
    RootComponentLayout(ScrollableContainer*, IComponentSizeCalculator*);
  
    /*****************************************\
    * Implements IComponentBoundsCalculator *
    \*****************************************/
    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
  };

  class ControlledWidthLayout:public IComponentBoundsCalculator {
    private:
    ScrollableContainer* cParent;
    IComponentHeightCalculator* cComponentHeightCalculator;
    bool cObtainingRight;
  
    public:
    ControlledWidthLayout(ScrollableContainer*, IComponentHeightCalculator*);
  
    /*****************************************\
    * Implements IComponentBoundsCalculator *
    \*****************************************/
    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
  };

  bool mouseButtonVerticalBar(SDL_Event&);
  bool mouseButtonHorizontalBar(SDL_Event&);
  bool mouseButtonDown(SDL_Event&);
  bool mouseButtonUp();
  bool mouseMotion(SDL_Event&);

  void testFocusChange(SDL_Event&);
  bool isAllContentVisibleVertically();
  bool isAllContentVisibleHorizontally();
  bool isAllContentVisible();
  bool isVerticalBarVisible();
  bool isHorizontalBarVisible();
  float getContentLeft();
  float getContentRight();
  float getContentTop();
  float getContentBottom();

  float getViewRight();
  float getViewBottom();
  float getHorizontalLeft();
  float getHorizontalRight();
  float getVerticalBottom();
  float getVerticalTop();
  float getAspectRatio(); // TODO: Move to IHUDComponent?

  public:
  ScrollableContainer();
  void addComponent(const std::string&, ISizedComponent*);
  void removeComponent(ISizedComponent*);
  void setRootComponent(ISizedComponent*);
  void setRootComponent(AbstractVerticalComponent*);

  /*******************************************\
   * Implements AbstractRectangularComponent *
  \*******************************************/
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
};

#endif

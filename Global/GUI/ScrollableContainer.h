#ifndef SCROLLABLE_CONTAINER_H
#define SCROLLABLE_CONTAINER_H

#include <cfloat>
#include <cmath>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <vector>

#include "../Configuration.h"
#include "../ScreenConfiguration.h"

#include "AbstractRectangularComponent.h"
#include "IComponentBoundsCalculator.h"
#include "IComponentSizeCalculator.h"
#include "ISizedComponent.h"
#include "IRectangularComponent.h"

class ScrollableContainer:public AbstractRectangularComponent {
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
  void setRootComponent(ISizedComponent*);

  /*******************************************\
   * Implements AbstractRectangularComponent *
  \*******************************************/
  void update(int);
  void render();
  bool input(SDL_Event&);
};

#endif

#ifndef WRAPPING_GRID_COMPONENT_H
#define WRAPPING_GRID_COMPONENT_H

#include <vector>

#include "AbstractRectangularComponent.h"
#include "IComponentBoundsCalculator.h"
#include "ISizedComponent.h"

class WrappingGridComponent:public AbstractRectangularComponent {
  private:
  std::vector<ISizedComponent*> cComponents;

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
  float getHeight();

  public:
  WrappingGridComponent();

  void addComponent(ISizedComponent* component);

  /*******************************************\
   * Implements AbstractRectangularComponent *
  \*******************************************/
  void update(int);
  void render();
  bool input(SDL_Event&);
};

#endif

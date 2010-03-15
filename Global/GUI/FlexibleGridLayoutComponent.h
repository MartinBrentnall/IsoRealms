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

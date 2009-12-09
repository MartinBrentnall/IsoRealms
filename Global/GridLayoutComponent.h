#ifndef GRID_LAYOUT_COMPONENT_H
#define GRID_LAYOUT_COMPONENT_H

#include <iostream>
#include <map>

#include "AbstractRectangularComponent.h"
#include "Configuration.h"
#include "IComponentBoundsCalculator.h"
#include "IHUDComponent.h"
#include "ScreenConfiguration.h"

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
  void setComponentInCell(unsigned int, unsigned int, AbstractRectangularComponent*);

  virtual ~GridLayoutComponent();
};

#endif

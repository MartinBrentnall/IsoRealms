#ifndef SCREEN_EDGE_BOTTOM_H
#define SCREEN_EDGE_BOTTOM_H

#include "ScreenEdgeHorizontal.h"

class ScreenEdgeBottom:public ScreenEdgeHorizontal {
  public:
  float getTabY(DockedDialog*);
  void renderTab(DockedDialog*, float, float);
};

#endif

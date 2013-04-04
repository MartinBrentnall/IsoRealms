#ifndef SCREEN_EDGE_LEFT_H
#define SCREEN_EDGE_LEFT_H

#include "ScreenEdgeVertical.h"

class ScreenEdgeLeft:public ScreenEdgeVertical {
  public:
  float getTabX(DockedDialog*);
  void renderTab(DockedDialog*, float, float);
};

#endif

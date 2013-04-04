#ifndef SCREEN_EDGE_RIGHT_H
#define SCREEN_EDGE_RIGHT_H

#include "ScreenEdgeVertical.h"

class ScreenEdgeRight:public ScreenEdgeVertical {
  public:
  float getTabX(DockedDialog*);
  void renderTab(DockedDialog*, float, float);    
};

#endif

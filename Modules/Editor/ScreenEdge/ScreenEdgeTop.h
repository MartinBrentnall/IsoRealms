#ifndef SCREEN_EDGE_TOP_H
#define SCREEN_EDGE_TOP_H

#include "ScreenEdgeHorizontal.h"

class ScreenEdgeTop:public ScreenEdgeHorizontal {
  public:
  float getTabY(DockedDialog*);
  void renderTab(DockedDialog*, float, float);
};

#endif

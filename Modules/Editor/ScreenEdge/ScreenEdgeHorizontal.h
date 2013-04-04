#ifndef SCREEN_EDGE_HORIZONTAL_H
#define SCREEN_EDGE_HORIZONTAL_H

#include "ScreenEdge.h"

class ScreenEdgeHorizontal:public ScreenEdge {
  public:
  float getTabWidth(DockedDialog*);
  float getTabHeight(DockedDialog*);
  float getTabX(DockedDialog*);    
};

#endif

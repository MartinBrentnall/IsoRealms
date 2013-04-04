#ifndef SCREEN_EDGE_VERTICAL_H
#define SCREEN_EDGE_VERTICAL_H

#include "ScreenEdge.h"

class ScreenEdgeVertical:public ScreenEdge {
  public:
  float getTabHeight(DockedDialog*);
  float getTabWidth(DockedDialog*);
  float getTabY(DockedDialog*);
};

#endif

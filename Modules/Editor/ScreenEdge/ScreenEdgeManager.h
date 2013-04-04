#ifndef SCREEN_EDGE_MANAGER_H
#define SCREEN_EDGE_MANAGER_H

#include <IsoRealms/IHUDComponent.h>

#include "ScreenEdgeBottom.h"
#include "ScreenEdgeLeft.h"
#include "ScreenEdgeRight.h"
#include "ScreenEdgeTop.h"

class ScreenEdgeManager:public IHUDComponent {
  private:
  ScreenEdgeBottom cScreenEdgeBottom;
  ScreenEdgeLeft cScreenEdgeLeft;
  ScreenEdgeRight cScreenEdgeRight;
  ScreenEdgeTop cScreenEdgeTop;
  
  public:
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
  bool contains(float, float);
  void add(Dialog*);
};

#endif

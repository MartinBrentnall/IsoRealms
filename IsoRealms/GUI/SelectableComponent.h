#ifndef SELECTABLE_COMPONENT_H
#define SELECTABLE_COMPONENT_H

#include "../Configuration.h"
#include "ISelector.h"
#include "ISizedComponent.h"

class SelectableComponent:public ISizedComponent {
  private:
  ISelector* cHandler;
  bool cSelected;
    
  public:
  SelectableComponent();
    
  void setHandler(ISelector*);
  
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
  float getWidth();
  float getHeight();
  void gainedFocus();
  void lostFocus();
};

#endif

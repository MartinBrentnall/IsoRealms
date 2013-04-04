#ifndef PANEL_COMPONENT_H
#define PANEL_COMPONENT_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "IPanelRenderer.h"
#include "ISizedComponent.h"

class PanelComponent:public ISizedComponent {
  private:
  IPanelRenderer* cRenderer;
  
  public:
  PanelComponent();
  
  void setRenderer(IPanelRenderer*);
    
  /***************************************\
   * Implements IComponentSizeCalculator *
  \***************************************/
  float getWidth();
  float getHeight();  

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);    
};

#endif

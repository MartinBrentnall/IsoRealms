#ifndef SLIDER_COMPONENT_H
#define SLIDER_COMPONENT_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "../Configuration.h"
#include "../MiscFunctions.h"
#include "IFloatListener.h"
#include "ISizedComponent.h"
#include "ISliderRenderer.h"

class SliderComponent:public ISizedComponent {
  private:
  float cValue;
  bool cSliderUpdating;
  bool cDragging;
  ISliderRenderer* cRenderer;
  std::vector<IFloatListener*> cValueListeners;
  
  bool mouseButtonDown(SDL_Event&);
  bool mouseMotion(Uint16);
  bool keyDown(SDLKey, SDLMod);
  void fireChange();
  
  public:
  SliderComponent();
  
  void setRenderer(ISliderRenderer*);
    
  void setValue(float);
  float getValue();
  
  void addFloatListener(IFloatListener*);
  
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

#ifndef TEXT_LABEL_COMPONENT_H
#define TEXT_LABEL_COMPONENT_H

#include <GL/gl.h>
#include <string>

#include "../IFont.h"

#include "ISizedComponent.h"
#include "LookAndFeel.h"

class TextLabelComponent:public ISizedComponent {
  private:
  std::string cLabel;

  public:
  TextLabelComponent(std::string);
  
  /***************************************\
   * Implements IComponentSizeCalculator *
  \***************************************/
  float getWidth();
  float getHeight();  

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void update(int);
  void render();
  bool input(SDL_Event&);
  bool contains(float, float);
};

#endif

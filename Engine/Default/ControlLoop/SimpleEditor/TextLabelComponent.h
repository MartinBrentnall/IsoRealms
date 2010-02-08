#ifndef TEXT_LABEL_COMPONENT_H
#define TEXT_LABEL_COMPONENT_H

#include <GL/gl.h>
#include <string>

#include "../../../../Global/ISizedComponent.h"
#include "../../../../Global/IFont.h"

class TextLabelComponent:public ISizedComponent {
  private:
  static IFont* cFont;

  std::string cLabel;

  public:
  void static setFont(IFont*);

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

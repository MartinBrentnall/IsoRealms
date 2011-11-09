#ifndef HUD_ROUNDED_RECTANGLE_H
#define HUD_ROUNDED_RECTANGLE_H

#include <GL/gl.h>

#include <IsoRealms/Colour.h>
#include <IsoRealms/Configuration.h>
#include <IsoRealms/Image.h>

#include "../../HUD/IHUDGameComponent.h"

class HUDRoundedRectangle:public IHUDGameComponent {
  private:
  static GLuint cCornerTexture;
  static unsigned int cInstanceCount;

  float cCornerSize;

  void renderCorner(float, float, float, float, float, float);
  void renderRectangle(float, float, float, float);

  public:
  HUDRoundedRectangle();
  
  /********************************\
   * Implements IHUDGameComponent *
  \********************************/
  void render(float, float);
  void update(int);
  float getAspectRatio();
  
  ~HUDRoundedRectangle();  
};

#endif

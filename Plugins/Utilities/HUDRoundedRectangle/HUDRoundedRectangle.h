#ifndef HUD_ROUNDED_RECTANGLE_H
#define HUD_ROUNDED_RECTANGLE_H

#include <cmath>
#include <GL/glew.h>

#include <IsoRealms/Colour.h>
#include <IsoRealms/Configuration.h>
#include <IsoRealms/Texture.h>

#include "../../HUD/IHUDGameComponent.h"

class HUDRoundedRectangle:public IHUDGameComponent {
  private:
  static Texture* cCornerTexture;
  static unsigned int cInstanceCount;
  static const float CIRCLE_RESOLUTION;

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

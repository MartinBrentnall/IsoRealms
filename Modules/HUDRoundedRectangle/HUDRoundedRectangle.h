#ifndef HUD_ROUNDED_RECTANGLE_H
#define HUD_ROUNDED_RECTANGLE_H

#include <cmath>
#include <GL/glew.h>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Resources/HUDComponents/IHUDComponentFactory.h>
#include <IsoRealms/Resources/HUDComponents/IHUDGameComponent.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/Texture/Texture.h>
#include <IsoRealms/SingleResourceTypeModule.h>

class HUDRoundedRectangle:public IHUDComponentFactory,
                          public IHUDGameComponent {
  private:
  static Texture* cCornerTexture;
  static unsigned int cInstanceCount;
  static const float CIRCLE_RESOLUTION;

  float cCornerSize;

  void renderCorner(float, float, float, float, float, float);
  void renderRectangle(float, float, float, float);

  public:
  HUDRoundedRectangle();
  
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /***********************************\
   * Implements IHUDComponentFactory *
  \***********************************/
  IHUDGameComponent* getHUDComponent();

  /********************************\
   * Implements IHUDGameComponent *
  \********************************/
  void render(float, float);
  void update(unsigned int);
  float getAspectRatio();
  
  void save(DOMNodeWriter*, IResourceLocator*);
  
  ~HUDRoundedRectangle();  
};

#endif

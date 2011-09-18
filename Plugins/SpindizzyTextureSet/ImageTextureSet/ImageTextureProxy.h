#ifndef IMAGE_TEXTURE_PROXY_H
#define IMAGE_TEXTURE_PROXY_H

#include "../ISpindizzyTexture.h"

class ImageTextureProxy:public ISpindizzyTexture {
  private:
  ISpindizzyTexture* cTexture;

  public:
  void setTexture(ISpindizzyTexture*);
  
  /***********************\
   * Implements ITexture *
  \***********************/
  void set();
  void texCoord2f(float, float);
  Colour* getColour(float, float);
  Mapping getMapping();
  
  ~ImageTextureProxy();
};

#endif

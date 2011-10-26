#ifndef IMAGE_TEXTURE_PROXY_H
#define IMAGE_TEXTURE_PROXY_H

#include "../ISpindizzyTexture.h"

class ImageTextureProxy:public ISpindizzyTexture {
  private:
  ISpindizzyTexture* cTexture;
  std::string cFilename;

  public:
  void setTexture(const std::string&, ISpindizzyTexture*);
  
  /***********************\
   * Implements ITexture *
  \***********************/
  void set();
  void texCoord2f(float, float);
  Colour* getColour(float, float);
  Mapping getMapping();
  void save(DOMNodeWriter*, const std::string&);
  
  ~ImageTextureProxy();
};

#endif

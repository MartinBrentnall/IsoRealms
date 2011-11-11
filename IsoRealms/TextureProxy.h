#ifndef TEXTURE_PROXY_H
#define TEXTURE_PROXY_H

#include <GL/gl.h>

#include "Colour.h"
#include "ITexture.h"

class TextureProxy:public ITexture {
  private:
  ITexture* cTexture;
  
  public:
  TextureProxy();
    
  void setImplementation(ITexture*);
    
  /***********************\
   * Implements ITexture *
  \***********************/
  void set();
  void texCoord2f(float, float);
  IColour* getColour(float, float);
  void save(DOMNodeWriter*, const std::string&);
  ~TextureProxy();
};

#endif

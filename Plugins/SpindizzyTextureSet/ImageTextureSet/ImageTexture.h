#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include "../ISpindizzyTexture.h"

class ImageTexture:public ISpindizzyTexture {
  private:
  GLuint cTextureID;

  public:
  ImageTexture(GLuint);
  
  /***********************\
   * Implements ITexture *
  \***********************/
  void set();
  void texCoord2f(float, float);
  Colour* getColour(float, float);
  Mapping getMapping();
  
  ~ImageTexture();
};

#endif

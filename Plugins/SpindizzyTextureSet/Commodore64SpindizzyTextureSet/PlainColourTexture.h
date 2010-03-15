#ifndef PLAIN_COLOUR_TEXTURE_H
#define PLAIN_COLOUR_TEXTURE_H

#include <GL/gl.h>

#include "../ISpindizzyTexture.h"

class PlainColourTexture:public ISpindizzyTexture {
  private:
  Colour* cColour;

  public:
  PlainColourTexture(Colour* colour);

  /***********************************\
   * Implements ISpindizzyTextureSet *
  \***********************************/
  void set();
  void texCoord2f(float, float);
  Colour* getColour(float, float);
  Mapping getMapping();
};

#endif

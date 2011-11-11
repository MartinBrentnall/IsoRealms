#ifndef I_TEXTURE_SOURCE_H
#define I_TEXTURE_SOURCE_H

#include "DOMNodeWrapper.h"
#include "ITexture.h"

class ITextureSource {
  public:
  virtual ITexture* getTexture(DOMNodeWrapper*) = 0;
};

#endif

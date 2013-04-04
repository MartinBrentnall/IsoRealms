#ifndef TEXTURE_REGISTRY_H
#define TEXTURE_REGISTRY_H

#include <IsoRealms/Resources/ResourceRegistry.h>

#include "Texture.h"

class TextureRegistry:public ResourceRegistry<ITexture> {
  public:
  ITexture* createDummyResource();
};

#endif

#include "ThemeTexture.h" 

void ThemeTexture::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  // TODO
}

void ThemeTexture::set(ITexture* texture) {
  cTexture = texture;
}

void ThemeTexture::set() {
  cTexture->set();
}

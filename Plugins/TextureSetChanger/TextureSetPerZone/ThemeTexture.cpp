#include "ThemeTexture.h" 

void ThemeTexture::set(ITexture* texture) {
  cTexture = texture;
}

void ThemeTexture::set() {
  cTexture->set();
}

void ThemeTexture::save(DOMNodeWriter* node, const std::string& name) {
  // TODO: Implement this
}

#include "ThemeTexture.h" 

void ThemeTexture::set(ITexture* texture) {
  cTexture = texture;
}

void ThemeTexture::set() {
  cTexture->set();
}

void ThemeTexture::texCoord2f(float x, float y) {
  cTexture->texCoord2f(x, y);
}

IColour* ThemeTexture::getColour(float x, float y) {
  return cTexture->getColour(x, y);
}

void ThemeTexture::save(DOMNodeWriter* node, const std::string& name) {
  // TODO: Implement this
}

#include "TextureProxy.h"

TextureProxy::TextureProxy() {
  cTexture = NULL;
}

void TextureProxy::setImplementation(ITexture* texture) {
  cTexture = texture;
}

void TextureProxy::set() {
  if (cTexture == NULL) {
    glBindTexture(GL_TEXTURE_2D, 0);
  } else {
    cTexture->set();
  }
}

void TextureProxy::texCoord2f(float x, float y) {
  if (cTexture == NULL) {
    glTexCoord2f(x,  y);
  } else {
    cTexture->texCoord2f(x, y);
  }
}

IColour* TextureProxy::getColour(float x, float y) {
  if (cTexture == NULL) {
    return new Colour(1.0f, 0.0f, 1.0f, 1.0f);
  } else {
    return cTexture->getColour(x, y);
  }
}

void TextureProxy::save(DOMNodeWriter* node, const std::string& name) {
  if (cTexture == NULL) {
    // Nothing to do
  } else {
    cTexture->save(node, name);
  }
}

TextureProxy::~TextureProxy() {
}

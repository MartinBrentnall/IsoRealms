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

void TextureProxy::save(DOMNodeWriter* node, const std::string& name) {
  if (cTexture == NULL) {
    // Nothing to do
  } else {
    cTexture->save(node, name);
  }
}

TextureProxy::~TextureProxy() {
}

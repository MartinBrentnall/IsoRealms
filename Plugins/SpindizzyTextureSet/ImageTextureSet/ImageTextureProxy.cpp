#include "ImageTextureProxy.h"

void ImageTextureProxy::setTexture(const std::string& filename, ISpindizzyTexture* texture) {
  cFilename = filename;
  cTexture = texture;
}

void ImageTextureProxy::set() {
  if (cTexture != NULL) {
    cTexture->set();
  }
}

void ImageTextureProxy::texCoord2f(float x, float y) {
  if (cTexture != NULL) {
    cTexture->texCoord2f(x, y);
  }
}

Colour* ImageTextureProxy::getColour(float x, float y) {
  if (cTexture != NULL) {
    return cTexture->getColour(x, y);
  }
  return NULL;
}

ISpindizzyTexture::Mapping ImageTextureProxy::getMapping() {
  if (cTexture != NULL) {
    return cTexture->getMapping();
  }
  return TILED;
}

void ImageTextureProxy::save(DOMNodeWriter* node, const std::string& id) {
  if (cTexture != NULL) {
    DOMNodeWriter* mTextureNode = node->addBranch("Texture");
    mTextureNode->addAttribute("file", cFilename);
    mTextureNode->addAttribute("id", id);
  }
}

ImageTextureProxy::~ImageTextureProxy() {
  delete cTexture;
}

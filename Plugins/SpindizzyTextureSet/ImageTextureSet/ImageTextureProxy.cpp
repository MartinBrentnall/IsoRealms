#include "ImageTextureProxy.h"

void ImageTextureProxy::setTexture(ISpindizzyTexture* texture) {
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

ImageTextureProxy::~ImageTextureProxy() {
  delete cTexture;
}

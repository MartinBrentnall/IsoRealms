#include "ImageTexture.h"

ImageTexture::ImageTexture(GLuint id) {
  cTextureID = id;
}

void ImageTexture::set() {
  glBindTexture(GL_TEXTURE_2D, cTextureID);
}

void ImageTexture::texCoord2f(float x, float y) {
  glTexCoord2f(x, y);
}

Colour* ImageTexture::getColour(float x, float y) {
  std::cout << "WARNING: Colour not supported here yet!" << std::endl;
  return NULL;
}

ISpindizzyTexture::Mapping ImageTexture::getMapping() {
  return TILED;
}

ImageTexture::~ImageTexture() {
  glDeleteTextures(1, &cTextureID);
}
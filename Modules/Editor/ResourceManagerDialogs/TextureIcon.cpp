#include "TextureIcon.h"

TextureIcon::TextureIcon(IResourceBrowser<ITexture>* browser, ITexture* texture) : Icon<ITexture>(browser, texture) {
}

void TextureIcon::renderIcon() {
  ITexture* mTexture = getResource();
  mTexture->set();
  float mLeft = getLeft();
  float mRight = getRight();
  float mTop = getTop();
  float mBottom = getBottom();
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f); glVertex2f(mLeft,  mBottom);
  glTexCoord2f(1.0f, 1.0f); glVertex2f(mRight, mBottom);
  glTexCoord2f(1.0f, 0.0f); glVertex2f(mRight, mTop);
  glTexCoord2f(0.0f, 0.0f); glVertex2f(mLeft,  mTop);
  glEnd();
}

float TextureIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.1f * mAspectRatio;
}

float TextureIcon::getHeight() {
  return 0.1f;
}

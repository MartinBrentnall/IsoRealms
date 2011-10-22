#include "HUDRoundedRectangle.h"

GLuint HUDRoundedRectangle::cCornerTexture = 0;
unsigned int HUDRoundedRectangle::cInstanceCount = 0;

HUDRoundedRectangle::HUDRoundedRectangle() {
  if (cInstanceCount++ == 0) {
    Image* mImage = new Image(64, 64, true);
    mImage->drawSquare(new Colour(1.0f, 1.0f, 1.0f, 0.0f), 0, 64, 0, 64);
    mImage->drawCircle(64, 64, new Colour(1.0f, 1.0f, 1.0f, 1.0f), 64);
    cCornerTexture = mImage->generateTexture();
    delete mImage;
  }
  cCornerSize = 0.02f;
}

void HUDRoundedRectangle::renderCorner(float xStart, float yStart, float xEnd, float yEnd, float xTextureEnd, float yTextureEnd) {
  glTexCoord2f(0.0f,        0.0f);        glVertex2f(xStart, yStart);
  glTexCoord2f(xTextureEnd, 0.0f);        glVertex2f(xEnd,   yStart);
  glTexCoord2f(xTextureEnd, yTextureEnd); glVertex2f(xEnd,   yEnd);
  glTexCoord2f(0.0f,        yTextureEnd); glVertex2f(xStart, yEnd);
}

void HUDRoundedRectangle::renderRectangle(float xStart, float yStart, float xEnd, float yEnd) {
  glVertex2f(xStart, yStart);
  glVertex2f(xEnd,   yStart);
  glVertex2f(xEnd,   yEnd);
  glVertex2f(xStart, yEnd);
}

void HUDRoundedRectangle::render(float xZoom, float yZoom) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mScreenAspectRatio = mScreen->getAspectRatio();
  float mYCornerSize = cCornerSize / yZoom;
  float mXCornerSize = cCornerSize / (xZoom / mScreenAspectRatio);
  float mXStartCorner = -1.0f + mXCornerSize / mScreenAspectRatio;
  float mYStartCorner = -1.0f + mYCornerSize;
  float mXEndCorner = 1.0f - mXCornerSize / mScreenAspectRatio;
  float mYEndCorner = 1.0f - mYCornerSize;
  glBindTexture(GL_TEXTURE_2D, cCornerTexture);
  glEnable(GL_BLEND);
  glColor4f(0.2f, 0.2f, 0.2f, 0.5f);
  glBegin(GL_QUADS);
  renderCorner(-1.0f,       -1.0f,        mXStartCorner, mYStartCorner,  1.0f,  1.0f);
  renderCorner(-1.0f,        mYEndCorner, mXStartCorner, 1.0f,           1.0f, -1.0f);
  renderCorner(mXEndCorner, -1.0f,        1.0f,          mYStartCorner, -1.0f,  1.0f);
  renderCorner(mXEndCorner,  mYEndCorner, 1.0f,          1.0f,          -1.0f, -1.0f);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_QUADS);
  renderRectangle(-1.0f,          mYStartCorner, mXStartCorner, mYEndCorner);
  renderRectangle(mXStartCorner, -1.0f,          mXEndCorner,   1.0f);
  renderRectangle(mXEndCorner,    mYStartCorner, 1.0f,          mYEndCorner);
  glEnd();
  glDisable(GL_BLEND);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void HUDRoundedRectangle::update(int) {
  // Nothing to do
}

float HUDRoundedRectangle::getAspectRatio() {
  return 1.0f;
}

HUDRoundedRectangle::~HUDRoundedRectangle() {
  // TODO: Unbind the texture, etc.
}

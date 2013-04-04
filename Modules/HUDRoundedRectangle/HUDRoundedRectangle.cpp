#include "HUDRoundedRectangle.h"

Texture* HUDRoundedRectangle::cCornerTexture = NULL;
unsigned int HUDRoundedRectangle::cInstanceCount = 0;
const float HUDRoundedRectangle::CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);

void HUDRoundedRectangle::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  // TODO: Initialise.
}

HUDRoundedRectangle::HUDRoundedRectangle() {
  if (cInstanceCount++ == 0) {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();

    cCornerTexture = new Texture();
    cCornerTexture->setRenderTarget();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(1.0f, 1.0f);
    float mStartAngle = 0.0f * (M_PI / 180.0f);
    float mEndAngle = 360.0f * (M_PI / 180.0f);
    float mRadius = 2.0f;
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
      glVertex2f(1.0f + sin(angle) * mRadius, 1.0f + cos(angle) * mRadius);
    }
    glEnd();

    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    Configuration* mConfiguration = Configuration::getInstance();
    mConfiguration->setViewPort();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
  }
  cCornerSize = 0.02f;
}

IHUDGameComponent* HUDRoundedRectangle::getHUDComponent() {
  return this;
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
  cCornerTexture->set();
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

void HUDRoundedRectangle::update(unsigned int) {
  // Nothing to do
}

float HUDRoundedRectangle::getAspectRatio() {
  return 1.0f;
}

void HUDRoundedRectangle::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  // TODO: Save colour and roundiness
}

HUDRoundedRectangle::~HUDRoundedRectangle() {
  // TODO: Unbind the texture, etc.
}

extern "C" IPlugin* create() {
  return new SingleResourceTypeModule<HUDRoundedRectangle>("Component");
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

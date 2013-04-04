#include "TextureRegistry.h"

ITexture* TextureRegistry::createDummyResource() {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

  Texture* mDummyTexture = new Texture();
  mDummyTexture->setRenderTarget();
  glBegin(GL_QUADS);
  glColor3f(1.0f, 0.0, 0.0f); glVertex2f(-1.0f, -1.0f);
  glColor3f(1.0f, 1.0, 0.0f); glVertex2f( 1.0f, -1.0f);
  glColor3f(0.0f, 1.0, 0.0f); glVertex2f( 1.0f,  1.0f);
  glColor3f(0.0f, 0.0, 1.0f); glVertex2f(-1.0f,  1.0f);
  glEnd();
  
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->setViewPort();
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  return mDummyTexture;
}

/*
 * Copyright 2015 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ResourceHUDRoundedRectangle.h"

Texture* ResourceHUDRoundedRectangle::cCornerTexture = NULL;
unsigned int ResourceHUDRoundedRectangle::cInstanceCount = 0;
const float ResourceHUDRoundedRectangle::CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);

void ResourceHUDRoundedRectangle::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  // TODO: Initialise.
}

ResourceHUDRoundedRectangle::ResourceHUDRoundedRectangle(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
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

IHUDGameComponent* ResourceHUDRoundedRectangle::getHUDComponent() {
  return this;
}

void ResourceHUDRoundedRectangle::renderCorner(float xStart, float yStart, float xEnd, float yEnd, float xTextureEnd, float yTextureEnd) {
  glTexCoord2f(0.0f,        0.0f);        glVertex2f(xStart, yStart);
  glTexCoord2f(xTextureEnd, 0.0f);        glVertex2f(xEnd,   yStart);
  glTexCoord2f(xTextureEnd, yTextureEnd); glVertex2f(xEnd,   yEnd);
  glTexCoord2f(0.0f,        yTextureEnd); glVertex2f(xStart, yEnd);
}

void ResourceHUDRoundedRectangle::renderRectangle(float xStart, float yStart, float xEnd, float yEnd) {
  glVertex2f(xStart, yStart);
  glVertex2f(xEnd,   yStart);
  glVertex2f(xEnd,   yEnd);
  glVertex2f(xStart, yEnd);
}

void ResourceHUDRoundedRectangle::render(float xZoom, float yZoom) {
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

void ResourceHUDRoundedRectangle::update(unsigned int) {
  // Nothing to do
}

float ResourceHUDRoundedRectangle::getAspectRatio() {
  return 1.0f;
}

void ResourceHUDRoundedRectangle::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  // TODO: Save colour and roundiness
}

ResourceHUDRoundedRectangle::~ResourceHUDRoundedRectangle() {
  // TODO: Unbind the texture, etc.
}

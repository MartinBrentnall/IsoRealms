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
#include "ResourceTextureSpindizzyCraftBall.h"

const float ResourceTextureSpindizzyCraftBall::CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);

ResourceTextureSpindizzyCraftBall::ResourceTextureSpindizzyCraftBall(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cTexture = new Texture();
}

void ResourceTextureSpindizzyCraftBall::renderCircle(float radius, IColour* colour) {
  glBegin(GL_TRIANGLE_FAN);
  colour->set();
  glVertex2f(0.0f, 0.0f);
  float mStartAngle = 0.0f * (M_PI / 180.0f);
  float mEndAngle = 360.0f * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * radius, cos(angle) * radius);
  }
  glEnd();
}

void ResourceTextureSpindizzyCraftBall::updateTexture() {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

  cTexture->setRenderTarget();
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  renderCircle(1.0f, cColourOutline);
  renderCircle(0.85f, cColourBall);
  renderCircle(0.7f, cColourShine);
  glTranslatef(-0.1f, -0.1f, 0.0f);
  renderCircle(0.75f, cColourBall);
  glTranslatef(0.1f, 0.1f, 0.0f);
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

void ResourceTextureSpindizzyCraftBall::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mPathColourOutline = node->getAttribute("outline");
  std::string mPathColourBall    = node->getAttribute("fill");
  std::string mPathColourShine   = node->getAttribute("shine");
  cColourOutline = resourceAccessor->getColour(mPathColourOutline);
  cColourBall    = resourceAccessor->getColour(mPathColourBall);
  cColourShine   = resourceAccessor->getColour(mPathColourShine);
  updateTexture();
}

void ResourceTextureSpindizzyCraftBall::save(DOMNodeWriter* node, IResourceLocator* resources) {
  // TODO
}

void ResourceTextureSpindizzyCraftBall::set() {
  cTexture->set();
}

void ResourceTextureSpindizzyCraftBall::save(DOMNodeWriter* node) {
  // TODO
}

ResourceTextureSpindizzyCraftBall::~ResourceTextureSpindizzyCraftBall() {
  // TODO
}

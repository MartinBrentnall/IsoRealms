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

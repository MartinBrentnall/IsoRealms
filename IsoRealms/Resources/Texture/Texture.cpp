/*
 * Copyright 2009 Martin Brentnall
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
#include "Texture.h"

Texture::Texture(bool clamp) {
  glGenFramebuffersEXT(1, &cFrameBuffer);
  glGenTextures(1, &cTexture);

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, cFrameBuffer);

  glBindTexture(GL_TEXTURE_2D, cTexture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if (clamp) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 128, 128, 0,GL_RGBA, GL_INT, NULL);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, cTexture, 0);

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void Texture::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  // Nothing to do
}

void Texture::setRenderTarget() {
  glViewport(0, 0, 128, 128);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, cFrameBuffer);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Texture::set() {
  glBindTexture(GL_TEXTURE_2D, cTexture);
}

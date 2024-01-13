/*
 * Copyright 2023 Martin Brentnall
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
#include "LiteralTexture.h"

namespace IsoRealms {
  LiteralTexture::LiteralTexture() :
            cTexture(0),
            cFrameBuffer(0) {
//    std::cout << "LiteralTexture::LiteralTexture: TODO: This constructor should not be here!" << std::endl;
  }

  LiteralTexture::LiteralTexture(IProject* project, bool clampX, bool clampY) :
            cTexture(0),
            cFrameBuffer(0) {
    project->mainThreadAlloc([this]() {
      glGenTextures(1, &cTexture);
    });
    
    project->mainThreadInit([this, clampX, clampY]() {
      glGenFramebuffersEXT(1, &cFrameBuffer);

      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, cFrameBuffer);

      glBindTexture(GL_TEXTURE_2D, cTexture);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      if (clampX) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      }
      if (clampY) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 128, 128, 0,GL_RGBA, GL_INT, nullptr);
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, cTexture, 0);

      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    });
  }

  void LiteralTexture::setRenderTarget() {
    glViewport(0, 0, 128, 128);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, cFrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void LiteralTexture::addUseListener(ITextureUseListener* listener) {
    cUseListeners.push_back(listener);
  }

  ITexture* LiteralTexture::getTexture() {
    return this;
  }

  void LiteralTexture::set() const {
    glBindTexture(GL_TEXTURE_2D, cTexture);
  }

  void LiteralTexture::hintTextureInUse(bool inUse) {
    for (ITextureUseListener* mListener : cUseListeners) {
      mListener->hintTextureUsed(this, inUse);
    }
  }
  
  void LiteralTexture::coord(float x, float y) const {
    glTexCoord2f(x, y);
  }
  
  LiteralTexture::~LiteralTexture() {
    if (cTexture != 0) {
      glDeleteTextures(1, &cTexture);
      glDeleteFramebuffers(1, &cFrameBuffer);
    }
  }
}

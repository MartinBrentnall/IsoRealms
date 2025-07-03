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

#include "IsoRealms/Assets/Type/ITextureUseListener.h"
#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/Persistence.h"

namespace IsoRealms {
  LiteralTexture::LiteralTexture(IProject& project, int width, int height, bool clampX, bool clampY, bool depth) :
            cProject(project),
            cWidth(width),
            cHeight(height),
            cTexture(0),
            cFrameBuffer(0) {
    project.mainThreadAlloc([this]() {
      glGenTextures(1, &cTexture);
    });
    
    project.mainThreadInit([this, clampX, clampY, depth]() {
      glGenFramebuffersEXT(1, &cFrameBuffer);
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, cFrameBuffer);

      if (depth) {
        GLuint depthBuffer;
        glGenRenderbuffersEXT(1, &depthBuffer);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, cWidth, cHeight);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
      }

      glBindTexture(GL_TEXTURE_2D, cTexture);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      if (clampX) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      }
      if (clampY) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, cWidth, cHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, cTexture, 0);

      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    });
  }

  void LiteralTexture::setRenderTarget() const {
    glViewport(0, 0, cWidth, cHeight);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, cFrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

  void LiteralTexture::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> LiteralTexture::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool LiteralTexture::isDefaultConfiguration() const {
    return true;
  }

  LiteralTexture::~LiteralTexture() {
    if (cTexture != 0) {
      cProject.mainThreadCleanUp([mTexture = cTexture, mFrameBuffer = cFrameBuffer]() {
        glDeleteTextures(1, &mTexture);
        glDeleteFramebuffers(1, &mFrameBuffer);
      });
    }
  }
}

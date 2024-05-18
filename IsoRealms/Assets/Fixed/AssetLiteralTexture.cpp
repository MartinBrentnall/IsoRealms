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
#include "TextureRegistry.h"

#include "IsoRealms/Project.h"

namespace IsoRealms {
  TextureRegistry::TextureRegistry(Project* project) :
            AssetRegistry<ITexture>(project),
            cProject(project) {
  }

  std::unique_ptr<ITexture> TextureRegistry::createLiteralAsset(const std::string& value) {
    if (value == "") {
      std::unique_ptr<LiteralTexture> mTexture = std::make_unique<LiteralTexture>(cProject);

//       cProject->mainThreadInit([this, mTexture]() { // TODO: Do I need to make sure this is only executed once?
//         glPushAttrib(GL_TRANSFORM_BIT);
//         glMatrixMode(GL_PROJECTION);
//         glPushMatrix();
//         glLoadIdentity();
//         glPopAttrib();
//
//         // Render an arrow indicating the dummy texture orientation
//         mTexture->setRenderTarget();
//         glBegin(GL_QUADS);
//         glColor3f(0.95f, 0.95f, 1.0f);
//         glVertex2f( 1.0f,  1.0f);
//         glVertex2f(-1.0f,  1.0f);
//         glColor3f(0.70f, 0.70f, 0.75f);
//         glVertex2f(-1.0f, -1.0f);
//         glVertex2f( 1.0f, -1.0f);
//
//         glColor3f(0.45f, 0.0f, 0.9f);
//         Utils::renderRectangle(-1.0f, -1.0f, -0.80f, -0.3f);
//         Utils::renderRectangle(-1.0f,  0.3f, -0.80f,  1.0f);
//         Utils::renderRectangle( 0.80f, -1.0f,  1.0f, -0.3f);
//         Utils::renderRectangle( 0.80f,  0.3f,  1.0f,  1.0f);
//
//         Utils::renderRectangle(-1.0f, -1.0f, -0.3f, -0.80f);
//         Utils::renderRectangle( 0.3f, -1.0f,  1.0f, -0.80f);
//         Utils::renderRectangle(-1.0f,  0.80f, -0.3f,  1.0f);
//         Utils::renderRectangle( 0.3f,  0.80f,  1.0f,  1.0f);
//
//         glColor3f(1.0f, 0.0f, 0.2f);
//         Utils::renderRectangle(-0.2f,  0.0f,  0.2f,  0.5f);
//
//         glEnd();
//         glBegin(GL_TRIANGLES);
//         glVertex2f( 0.5f,  0.0f);
//         glVertex2f(-0.5f,  0.0f);
//         glVertex2f( 0.0f, -0.5f);
//         glEnd();
//
//         glPushAttrib(GL_TRANSFORM_BIT);
//         glMatrixMode(GL_PROJECTION);
//         glPopMatrix();
//         glPopAttrib();
//
//         glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
//         glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
//         glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//         IApplication* mApplication = project->getApplication();
//         mApplication->setViewPort();
//       });
      return mTexture;
    }
    return nullptr;
  }

  std::unique_ptr<ITexture> TextureRegistry::createLiteralAsset(JSONObject object) {
    std::unique_ptr<LiteralTexture> mTexture = std::make_unique<LiteralTexture>(cProject);

//       cProject->mainThreadInit([this, mTexture]() { // TODO: Do I need to make sure this is only executed once?
//         glPushAttrib(GL_TRANSFORM_BIT);
//         glMatrixMode(GL_PROJECTION);
//         glPushMatrix();
//         glLoadIdentity();
//         glPopAttrib();
//
//         // Render an arrow indicating the dummy texture orientation
//         mTexture->setRenderTarget();
//         glBegin(GL_QUADS);
//         glColor3f(0.95f, 0.95f, 1.0f);
//         glVertex2f( 1.0f,  1.0f);
//         glVertex2f(-1.0f,  1.0f);
//         glColor3f(0.70f, 0.70f, 0.75f);
//         glVertex2f(-1.0f, -1.0f);
//         glVertex2f( 1.0f, -1.0f);
//
//         glColor3f(0.45f, 0.0f, 0.9f);
//         Utils::renderRectangle(-1.0f, -1.0f, -0.80f, -0.3f);
//         Utils::renderRectangle(-1.0f,  0.3f, -0.80f,  1.0f);
//         Utils::renderRectangle( 0.80f, -1.0f,  1.0f, -0.3f);
//         Utils::renderRectangle( 0.80f,  0.3f,  1.0f,  1.0f);
//
//         Utils::renderRectangle(-1.0f, -1.0f, -0.3f, -0.80f);
//         Utils::renderRectangle( 0.3f, -1.0f,  1.0f, -0.80f);
//         Utils::renderRectangle(-1.0f,  0.80f, -0.3f,  1.0f);
//         Utils::renderRectangle( 0.3f,  0.80f,  1.0f,  1.0f);
//
//         glColor3f(1.0f, 0.0f, 0.2f);
//         Utils::renderRectangle(-0.2f,  0.0f,  0.2f,  0.5f);
//
//         glEnd();
//         glBegin(GL_TRIANGLES);
//         glVertex2f( 0.5f,  0.0f);
//         glVertex2f(-0.5f,  0.0f);
//         glVertex2f( 0.0f, -0.5f);
//         glEnd();
//
//         glPushAttrib(GL_TRANSFORM_BIT);
//         glMatrixMode(GL_PROJECTION);
//         glPopMatrix();
//         glPopAttrib();
//
//         glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
//         glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
//         glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//         IApplication* mApplication = project->getApplication();
//         mApplication->setViewPort();
//       });
      return mTexture;
    }
    return nullptr;
  }
}


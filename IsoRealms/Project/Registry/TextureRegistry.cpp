 /*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "TextureRegistry.h"

#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  TextureRegistry::TextureRegistry(Project& project) :
            AssetClientManager(&cLiteral),
            cLiteral(project) {
  }

  TextureRegistry::Dummy::Dummy(Project& project) :
            cDummyTexture(project, 128, 128) {
    project.getApplication().mainThreadInit([this, &project]() { // TODO: Do I need to make sure this is only executed once?
      glPushAttrib(GL_TRANSFORM_BIT);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      glPopAttrib();

      // Render an arrow indicating the dummy texture orientation
      cDummyTexture.setRenderTarget();
      glBegin(GL_QUADS);
      glColor3f(0.95f, 0.95f, 1.0f);
      glVertex2f( 1.0f,  1.0f);
      glVertex2f(-1.0f,  1.0f);
      glColor3f(0.70f, 0.70f, 0.75f);
      glVertex2f(-1.0f, -1.0f);
      glVertex2f( 1.0f, -1.0f);

      glColor3f(0.45f, 0.0f, 0.9f);
      Utils::renderRectangle(-1.0f, -1.0f, -0.80f, -0.3f);
      Utils::renderRectangle(-1.0f,  0.3f, -0.80f,  1.0f);
      Utils::renderRectangle( 0.80f, -1.0f,  1.0f, -0.3f);
      Utils::renderRectangle( 0.80f,  0.3f,  1.0f,  1.0f);

      Utils::renderRectangle(-1.0f, -1.0f, -0.3f, -0.80f);
      Utils::renderRectangle( 0.3f, -1.0f,  1.0f, -0.80f);
      Utils::renderRectangle(-1.0f,  0.80f, -0.3f,  1.0f);
      Utils::renderRectangle( 0.3f,  0.80f,  1.0f,  1.0f);

      glColor3f(1.0f, 0.0f, 0.2f);
      Utils::renderRectangle(-0.2f,  0.0f,  0.2f,  0.5f);

      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex2f( 0.5f,  0.0f);
      glVertex2f(-0.5f,  0.0f);
      glVertex2f( 0.0f, -0.5f);
      glEnd();

      glPushAttrib(GL_TRANSFORM_BIT);
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glPopAttrib();

      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
      glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

      Application& mApplication = project.getApplication();
      mApplication.setViewPort();
    });
  }

  ITexture* TextureRegistry::Dummy::getTexture() {
    return this;
  }

  void TextureRegistry::Dummy::set() const {
    cDummyTexture.set();
  }

  void TextureRegistry::Dummy::hintTextureInUse(bool inUse) {
    cDummyTexture.hintTextureInUse(inUse);
  }

  void TextureRegistry::Dummy::coord(float x, float y) const {
    cDummyTexture.coord(x, y);
  }

  void TextureRegistry::Dummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void TextureRegistry::Dummy::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool TextureRegistry::Dummy::isDefaultConfiguration() const {
    return true;
  }
}

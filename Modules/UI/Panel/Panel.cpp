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
#include "Panel.h"

#include "IsoRealms/IApplication.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"
#include "IsoRealms/System.h"

namespace IsoRealms::UI {
  const float Panel::CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);

  const std::string Panel::TAG_COLOUR = "Colour";

  const std::string Panel::ATTRIBUTE_CORNER_SIZE = "cornerSize";

  std::unique_ptr<LiteralTexture> Panel::cGlobalCornerTexture = nullptr;
  unsigned int Panel::cGlobalInstanceCount = 0;
  
  Panel::Panel(IProject* project, UI* ui) :
            cDefColour(project, 0.0f, 0.0f, 1.0f),
            cDefCornerSize(0.0f) {
    initTextures(project);
  }

  Panel::Panel(IProject* project, UI* ui, DOMNode& node, IOptions* options, IResourceData* data) :
            Panel(project, ui) {
    cDefColour.init(node.getNode(TAG_COLOUR));
    cDefCornerSize = node.getFloatAttribute(ATTRIBUTE_CORNER_SIZE, 0.0f);
  }

  void Panel::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Panels");
  }
  
  void Panel::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }
  
  void Panel::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefColour.save(node, TAG_COLOUR);
    node->addAttribute(ATTRIBUTE_CORNER_SIZE, cDefCornerSize);
  }

  void Panel::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool Panel::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> Panel::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  void Panel::renderScreen(float scale, float aspectRatio) const {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    cDefColour.set();
    if (cDefCornerSize > 0.0f) {
      float mXStartCorner = std::min(-aspectRatio + cDefCornerSize / scale, 0.0f);
      float mXEndCorner   = std::max( aspectRatio - cDefCornerSize / scale, 0.0f);
      float mYStartCorner = std::min(-1.0f        + cDefCornerSize / scale, 0.0f);
      float mYEndCorner   = std::max( 1.0f        - cDefCornerSize / scale, 0.0f);
      cGlobalCornerTexture->set();
      glBegin(GL_QUADS);
      renderCorner(-aspectRatio, mXStartCorner, false, -1.0f,        mYStartCorner, false);
      renderCorner(-aspectRatio, mXStartCorner, false,  mYEndCorner, 1.0f,          true);
      renderCorner( mXEndCorner, aspectRatio,   true,  -1.0f,        mYStartCorner, false);
      renderCorner( mXEndCorner, aspectRatio,   true,   mYEndCorner, 1.0f,          true);
      glEnd();
      glBindTexture(GL_TEXTURE_2D, 0);
      glBegin(GL_QUADS);
      renderRectangle(-aspectRatio,   mXStartCorner, mYStartCorner, mYEndCorner);
      renderRectangle( mXStartCorner, mXEndCorner,  -1.0f,          1.0f);
      renderRectangle( mXEndCorner,   aspectRatio,   mYStartCorner, mYEndCorner);
      glEnd();
    } else {
      glBindTexture(GL_TEXTURE_2D, 0);
      glBegin(GL_QUADS);
      renderRectangle(-aspectRatio, aspectRatio, -1.0f, 1.0f);
      glEnd();
    }
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
  }

  bool Panel::renderAssetIcon() const {
    return renderIcon();
  }
 
  void Panel::initTextures(IProject* project) {
    if (cGlobalInstanceCount ++ == 0) {
      cGlobalCornerTexture = std::make_unique<LiteralTexture>(project, true, true);
      project->mainThreadInit([project]() {
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glPopAttrib();
        cGlobalCornerTexture->setRenderTarget();
        
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(1.0f, 1.0f);
        float mStartAngle = 0.0f * (M_PI / 180.0f);
        float mEndAngle = 360.0f * (M_PI / 180.0f);
        float mRadius = 2.0f;
        for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
          glVertex2f(1.0f + std::sin(angle) * mRadius, 1.0f + std::cos(angle) * mRadius);
        }
        glEnd();

        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glPopAttrib();

        IApplication* mApplication = project->getApplication();
        mApplication->setViewPort();
      });
    }
  }

  void Panel::renderCorner(float xStart, float xEnd, bool xFlip, float yStart, float yEnd, bool yFlip) {
    float mXTextureStart = xFlip ? 1.0f : 0.0f;
    float mXTextureEnd   = xFlip ? 0.0f : 1.0f;
    float mYTextureStart = yFlip ? 1.0f : 0.0f;
    float mYTextureEnd   = yFlip ? 0.0f : 1.0f;
    glTexCoord2f(mXTextureStart, mYTextureStart); glVertex2f(xStart, yStart);
    glTexCoord2f(mXTextureEnd,   mYTextureStart); glVertex2f(xEnd,   yStart);
    glTexCoord2f(mXTextureEnd,   mYTextureEnd);   glVertex2f(xEnd,   yEnd);
    glTexCoord2f(mXTextureStart, mYTextureEnd);   glVertex2f(xStart, yEnd);
  }

  void Panel::renderRectangle(float xStart, float xEnd, float yStart, float yEnd) {
    glVertex2f(xStart, yStart);
    glVertex2f(xEnd,   yStart);
    glVertex2f(xEnd,   yEnd);
    glVertex2f(xStart, yEnd);
  }
}

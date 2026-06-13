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
#include "ScreenPanel.h"

namespace IsoRealms::UI {
  ScreenPanel::ScreenPanel(const Metadata& /*metadata*/, IComponentData& owner) :
            cDefColour(owner, 0.0f, 0.0f, 1.0f),
            cDefCornerSize(0.0f) {
    initTextures(owner.getProject());
  }
  
  ScreenPanel::ScreenPanel(const Metadata& metadata, IComponentData& owner, JSONObject object) :
            ScreenPanel(metadata, owner) {
    cDefColour.set(object, JSON_COLOUR);
    cDefCornerSize = object.getFloat(JSON_CORNER_SIZE);
  }

  void ScreenPanel::renderScreen(float scale, float aspectRatio) const {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    cDefColour->set();
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

  bool ScreenPanel::renderAssetIcon() const {
    return false;
  }
  
  void ScreenPanel::saveAsset(JSONObject object) const {
    cDefColour.save(object, JSON_COLOUR);
    object.addFloat(JSON_CORNER_SIZE, cDefCornerSize);
  }

  void ScreenPanel::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyTreeSelector(JSON_COLOUR,      cDefColour);
    owner.createPropertyNativeFloat( JSON_CORNER_SIZE, [this]() {return cDefCornerSize;}, [this](float value) {cDefCornerSize = value;});
  }
  
  bool ScreenPanel::isDefaultConfiguration() const {
    return cDefColour.isDefaultConfigured() && cDefCornerSize == 0.0f;
  }

  void ScreenPanel::initTextures(Project& project) {
    if (cGlobalInstanceCount++ == 0) {
      cGlobalCornerTexture = std::make_unique<LiteralTexture>(project, 128, 128, true, true);
      project.getApplication().mainThreadInit([&project]() {
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

        Application& mApplication = project.getApplication();
        mApplication.setViewPort();
      });
    }
  }

  void ScreenPanel::renderCorner(float xStart, float xEnd, bool xFlip, float yStart, float yEnd, bool yFlip) {
    float mXTextureStart = xFlip ? 1.0f : 0.0f;
    float mXTextureEnd   = xFlip ? 0.0f : 1.0f;
    float mYTextureStart = yFlip ? 1.0f : 0.0f;
    float mYTextureEnd   = yFlip ? 0.0f : 1.0f;
    glTexCoord2f(mXTextureStart, mYTextureStart); glVertex2f(xStart, yStart);
    glTexCoord2f(mXTextureEnd,   mYTextureStart); glVertex2f(xEnd,   yStart);
    glTexCoord2f(mXTextureEnd,   mYTextureEnd);   glVertex2f(xEnd,   yEnd);
    glTexCoord2f(mXTextureStart, mYTextureEnd);   glVertex2f(xStart, yEnd);
  }

  void ScreenPanel::renderRectangle(float xStart, float xEnd, float yStart, float yEnd) {
    glVertex2f(xStart, yStart);
    glVertex2f(xEnd,   yStart);
    glVertex2f(xEnd,   yEnd);
    glVertex2f(xStart, yEnd);
  }

  std::unique_ptr<LiteralTexture> ScreenPanel::cGlobalCornerTexture = nullptr;
  unsigned int ScreenPanel::cGlobalInstanceCount = 0;
}

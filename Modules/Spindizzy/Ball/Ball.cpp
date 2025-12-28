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
#include "Ball.h"

namespace IsoRealms::Spindizzy {
  const std::string Ball::JSON_FILL    = "fill";
  const std::string Ball::JSON_OUTLINE = "outline";
  const std::string Ball::JSON_SHINE   = "shine";

  const float Ball::CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);

  Ball::Ball(Spindizzy& spindizzy, IResourceData& data) :
            cProject(data.getProject()),
            cDefFill(data, 1.0f, 0.0f, 1.0f, 0.0f, [this]() {setNeedsRedrawing();}),
            cDefOutline(data, 0.0f, 0.0f, 0.0f, 0.0f, [this]() {setNeedsRedrawing();}),
            cDefShine(data, 1.0f, 1.0f, 0.0f, 0.0f, [this]() {setNeedsRedrawing();}),
            cTexture(data.getProject(), 128, 128),
            cNeedsRedrawing(false) {
    setNeedsRedrawing();
  }
  
  Ball::Ball(Spindizzy& spindizzy, IResourceData& data, JSONObject object) :
            Ball(spindizzy, data) {
    cDefFill.init(object, JSON_FILL);
    cDefOutline.init(object, JSON_OUTLINE);
    cDefShine.init(object, JSON_SHINE);
  }

  void Ball::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<ITexture>(this, "", "Spindizzy Ball Craft Textures");
  }
  
  void Ball::save(JSONObject object) const {
    cDefFill.save(object, JSON_FILL);
    cDefOutline.save(object, JSON_OUTLINE);
    cDefShine.save(object, JSON_SHINE);
  }

  void Ball::hintInUse(bool inUse) {
    cTexture.hintTextureInUse(inUse);
  }

  bool Ball::renderIcon() const {
    glEnable(GL_ALPHA_TEST);
    return cTexture.renderAssetIcon();
  }

  void Ball::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyAsset<Colour>(metadata.getPropertyData("Fill"),    cDefFill);
    owner.createPropertyAsset<Colour>(metadata.getPropertyData("Shine"),   cDefShine);
    owner.createPropertyAsset<Colour>(metadata.getPropertyData("Outline"), cDefOutline);
  }

  void Ball::removed() {
    // Nothing to do.
  }

  void Ball::set() const {
    cTexture.set();
  }

  void Ball::hintTextureInUse(bool inUse) {
    cTexture.hintTextureInUse(inUse);
  }

  ITexture* Ball::getTexture() {
    return this;
  }

  void Ball::coord(float x, float y) const {
    glTexCoord2f(x, y);
  }

  void Ball::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Ball::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool Ball::isDefaultConfiguration() const {
    return true;
  }

  void Ball::updateTexture() {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();

    cTexture.setRenderTarget();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    renderCircle(1.0f,  cDefOutline);
    renderCircle(0.85f, cDefFill);
    renderCircle(0.7f,  cDefShine);
    glTranslatef(-0.1f, 0.1f, 0.0f);
    renderCircle(0.75f, cDefFill);
    glTranslatef(0.1f, -0.1f, 0.0f);
    glEnd();

    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    Application& mApplication = cProject.getApplication();
    mApplication.setViewPort();
  }

  void Ball::renderCircle(float radius, Colour& colour) {
    glBegin(GL_TRIANGLE_FAN);
    colour->set();
    glVertex2f(0.0f, 0.0f);
    float mStartAngle = 0.0f * (M_PI / 180.0f);
    float mEndAngle = 360.0f * (M_PI / 180.0f);
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
      glVertex2f(std::sin(angle) * radius, std::cos(angle) * radius);
    }
    glEnd();
  }

  void Ball::setNeedsRedrawing() {
    if (!cNeedsRedrawing) {
      cProject.updateLater([this]() {
        updateTexture();
        cNeedsRedrawing = false;
      });
    }
  }
}

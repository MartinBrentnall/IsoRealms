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
#include "Ball.h"

#include "IsoRealms/IApplication.h"

namespace IsoRealms::Spindizzy {
  const std::string Ball::TAG_FILL    = "Fill";
  const std::string Ball::TAG_OUTLINE = "Outline";
  const std::string Ball::TAG_SHINE   = "Shine";

  const float Ball::CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);

  Ball::Ball(IProject* project, Spindizzy* spindizzy) :
            cProject(project),
            cDefTexture(project),
            cDefFill(project, 1.0f, 0.0f, 1.0f, 0.0f, [this]() {updateTexture();}),
            cDefOutline(project, 0.0f, 0.0f, 0.0f, 0.0f, [this]() {updateTexture();}),
            cDefShine(project, 1.0f, 1.0f, 0.0f, 0.0f, [this]() {updateTexture();}) {
    project->mainThreadInit([this]() {
      updateTexture();
    });
  }
  
  Ball::Ball(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            Ball(project, spindizzy) {
    cDefFill.init(node, TAG_FILL);
    cDefOutline.init(node, TAG_OUTLINE);
    cDefShine.init(node, TAG_SHINE);
  }

  void Ball::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Spindizzy Ball Craft Textures");
  }
  
  void Ball::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }
  
  void Ball::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefFill.save(node, TAG_FILL);
    cDefOutline.save(node, TAG_OUTLINE);
    cDefShine.save(node, TAG_SHINE);
  }

  void Ball::hintInUse(bool inUse) {
    cDefTexture.hintTextureInUse(inUse);
  }

  bool Ball::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> Ball::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  void Ball::set() const {
    cDefTexture.set();
  }

  void Ball::hintTextureInUse(bool inUse) {
    cDefTexture.hintTextureInUse(inUse);
  }

  ITexture* Ball::getTexture() {
    return this;
  }

  void Ball::coord(float x, float y) const {
    glTexCoord2f(x, y);
  }
  
  void Ball::updateTexture() {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();

    cDefTexture.setRenderTarget();
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

    IApplication* mApplication = cProject->getApplication();
    mApplication->setViewPort();
  }

  void Ball::renderCircle(float radius, Colour& colour) {
    glBegin(GL_TRIANGLE_FAN);
    colour.set();
    glVertex2f(0.0f, 0.0f);
    float mStartAngle = 0.0f * (M_PI / 180.0f);
    float mEndAngle = 360.0f * (M_PI / 180.0f);
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
      glVertex2f(std::sin(angle) * radius, std::cos(angle) * radius);
    }
    glEnd();
  }
}

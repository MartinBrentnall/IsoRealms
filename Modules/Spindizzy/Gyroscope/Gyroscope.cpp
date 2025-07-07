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
#include "Gyroscope.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const std::string Gyroscope::JSON_COLOUR_1 = "colour1";
  const std::string Gyroscope::JSON_COLOUR_2 = "colour2";
  const std::string Gyroscope::JSON_COLOUR_3 = "colour3";
  const std::string Gyroscope::JSON_COLOUR_4 = "colour4";
  const std::string Gyroscope::JSON_OUTLINE  = "outline";

  const float Gyroscope::CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);
  const float Gyroscope::SPINDLE_WIDTH     = 0.03f;
  const float Gyroscope::WIDTH             = 0.33f;
  const float Gyroscope::HEIGHT            = 0.9f;

  Gyroscope::Gyroscope(IProject& project, Spindizzy& spindizzy, IResourceData& data) :
            cProject(project),
            cDefQuadrant{Colour(data, 1.0f, 1.0f, 0.0f, 0.0f, [this]() {setNeedsRedrawing();}),
                         Colour(data, 1.0f, 0.0f, 0.0f, 0.0f, [this]() {setNeedsRedrawing();}),
                         Colour(data, 0.0f, 1.0f, 0.0f, 0.0f, [this]() {setNeedsRedrawing();}),
                         Colour(data, 0.0f, 0.0f, 1.0f, 0.0f, [this]() {setNeedsRedrawing();})},
            cDefOutline(data, 1.0f, 0.0f, 1.0f),
            cTexture(project, 128, 128),
            cNeedsRedrawing(false),
            cEditingIconRotation(0.0f) {
    setNeedsRedrawing();
  }

  Gyroscope::Gyroscope(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options) :
            Gyroscope(project, spindizzy, data) {
    cDefQuadrant[0].init(object, JSON_COLOUR_1);
    cDefQuadrant[1].init(object, JSON_COLOUR_2);
    cDefQuadrant[2].init(object, JSON_COLOUR_3);
    cDefQuadrant[3].init(object, JSON_COLOUR_4);
    cDefOutline.init(object, JSON_OUTLINE);
  }

  void Gyroscope::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IModel>(this, "", "Spindizzy Gyroscope Models");
  }
    
  void Gyroscope::save(JSONObject object) const {
    cDefQuadrant[0].save(object, JSON_COLOUR_1);
    cDefQuadrant[1].save(object, JSON_COLOUR_2);
    cDefQuadrant[2].save(object, JSON_COLOUR_3);
    cDefQuadrant[3].save(object, JSON_COLOUR_4);
    cDefOutline.save(object, JSON_OUTLINE);
  }

  void Gyroscope::hintInUse(bool inUse) {
    cTexture.hintTextureInUse(inUse);
  }

  bool Gyroscope::renderIcon() const {
    glTranslatef(0.0f, -1.0f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(cEditingIconRotation,  0.0f, 0.0f, 1.0f);
    glScalef(3.0f, 3.0f, 3.0f);
    return renderPreview();
  }

  std::vector<std::unique_ptr<IProperty>> Gyroscope::getProperties(IResourceData& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>("Quadrant 1 Colour", "TODO", cDefQuadrant[0]));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>("Quadrant 2 Colour", "TODO", cDefQuadrant[1]));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>("Quadrant 3 Colour", "TODO", cDefQuadrant[2]));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>("Quadrant 4 Colour", "TODO", cDefQuadrant[3]));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>("Outline Colour",    "TODO", cDefOutline));
    return mProperties;
  }
  
  void Gyroscope::updateEditing(unsigned int milliseconds) {
    cEditingIconRotation -= 0.5f * milliseconds;
  }

  IModelInstance* Gyroscope::createModel() {
    return this;
  }

  bool Gyroscope::renderPreview() const {
    render();
    return true;
  }

  bool Gyroscope::renderAssetIcon() const {
    return renderIcon();
  }

  void Gyroscope::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Gyroscope::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Gyroscope::isDefaultConfiguration() const {
    return true;
  }

  void Gyroscope::update(unsigned int milliseconds) {
    // Nothing to do
  }

  void Gyroscope::render() const {
    cTexture.set();
    glDisable(GL_CULL_FACE);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(1.0f, 0.5f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -SPINDLE_WIDTH, HEIGHT / 2.0f);
    glVertex3f(0.0f,  SPINDLE_WIDTH, HEIGHT / 2.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f( SPINDLE_WIDTH, 0.0f, HEIGHT / 2.0f);
    glVertex3f(-SPINDLE_WIDTH, 0.0f, HEIGHT / 2.0f);

    glVertex3f(0.0f, 0.0f, HEIGHT);
    glVertex3f(0.0f, -SPINDLE_WIDTH, HEIGHT / 2.0f);
    glVertex3f(0.0f,  SPINDLE_WIDTH, HEIGHT / 2.0f);
    glVertex3f(0.0f, 0.0f, HEIGHT);
    glVertex3f( SPINDLE_WIDTH, 0.0f, HEIGHT / 2.0f);
    glVertex3f(-SPINDLE_WIDTH, 0.0f, HEIGHT / 2.0f);
    glEnd();

    glAlphaFunc(GL_GREATER, 0.1f);
    glEnable(GL_ALPHA_TEST);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( WIDTH,  WIDTH, HEIGHT / 2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-WIDTH,  WIDTH, HEIGHT / 2.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-WIDTH, -WIDTH, HEIGHT / 2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( WIDTH, -WIDTH, HEIGHT / 2.0f);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    
    glEnable(GL_CULL_FACE);  
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void Gyroscope::updateTexture() {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();

    cTexture.setRenderTarget();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    renderCircle(1.0f,  0.88f,          cDefOutline);
    renderCircle(0.88f, 0.0f,   90.0f,  cDefQuadrant[0]);
    renderCircle(0.88f, 90.0f,  180.0f, cDefQuadrant[1]);
    renderCircle(0.88f, 180.0f, 270.0f, cDefQuadrant[2]);
    renderCircle(0.88f, 270.0f, 360.0f, cDefQuadrant[3]);
      
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    IApplication& mApplication = cProject.getApplication();
    mApplication.setViewPort();
  }
    
  void Gyroscope::renderCircle(float radius, float startAngle, float endAngle, Colour& colour) {
    glBegin(GL_TRIANGLE_FAN);
    colour->set();
    glVertex2f(0.0f, 0.0f);
    float mStartAngle = startAngle * (M_PI / 180.0f);
    float mEndAngle   = endAngle   * (M_PI / 180.0f);
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
      glVertex2f(std::sin(angle) * radius, std::cos(angle) * radius);
    }
    glEnd();
  }

  void Gyroscope::renderCircle(float outerRadius, float innerRadius, Colour& colour) {
    glBegin(GL_TRIANGLE_STRIP);
    glEnable(GL_BLEND);
    colour->set();
    float mStartAngle =   0.0f * (M_PI / 180.0f);
    float mEndAngle   = 360.0f * (M_PI / 180.0f);
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
      glVertex2f(std::sin(angle) * innerRadius, std::cos(angle) * innerRadius);
      glVertex2f(std::sin(angle) * outerRadius, std::cos(angle) * outerRadius);
    }
    glEnd();
  }

  void Gyroscope::setNeedsRedrawing() {
    if (!cNeedsRedrawing) {
      cProject.updateLater([this]() {
        updateTexture();
        cNeedsRedrawing = false;
      });
    }
  }
}

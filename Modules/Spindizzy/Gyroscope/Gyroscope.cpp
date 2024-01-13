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
#include "Gyroscope.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const std::string Gyroscope::TAG_COLOUR_1 = "Colour1";
  const std::string Gyroscope::TAG_COLOUR_2 = "Colour2";
  const std::string Gyroscope::TAG_COLOUR_3 = "Colour3";
  const std::string Gyroscope::TAG_COLOUR_4 = "Colour4";
  const std::string Gyroscope::TAG_OUTLINE  = "Outline";

  const float Gyroscope::CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);
  const float Gyroscope::SPINDLE_WIDTH     = 0.03f;
  const float Gyroscope::WIDTH             = 0.33f;
  const float Gyroscope::HEIGHT            = 0.9f;

  Gyroscope::Gyroscope(IProject* project, Spindizzy* spindizzy) :
            cProject(project),
            cDefQuadrant{Colour(project, 1.0f, 1.0f, 0.0f, 0.0f, [this]() {updateTexture();}),
                         Colour(project, 1.0f, 0.0f, 0.0f, 0.0f, [this]() {updateTexture();}),
                         Colour(project, 0.0f, 1.0f, 0.0f, 0.0f, [this]() {updateTexture();}),
                         Colour(project, 0.0f, 0.0f, 1.0f, 0.0f, [this]() {updateTexture();})},
            cDefOutline(project, 1.0f, 0.0f, 1.0f),
            cDefTexture(project),
            cEditingIconRotation(0.0f) {
    project->updateEditing([this](unsigned int milliseconds) {
      cEditingIconRotation -= 0.5f * milliseconds;
    });

    project->mainThreadInit([this]() {
      updateTexture();
    });
  }

  Gyroscope::Gyroscope(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            Gyroscope(project, spindizzy) {
    cDefQuadrant[0].init(node.getNode(TAG_COLOUR_1));
    cDefQuadrant[1].init(node.getNode(TAG_COLOUR_2));
    cDefQuadrant[2].init(node.getNode(TAG_COLOUR_3));
    cDefQuadrant[3].init(node.getNode(TAG_COLOUR_4));
    cDefOutline.init(node.getNode(TAG_OUTLINE));
  }

  void Gyroscope::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Spindizzy Gyroscope Models");
  }
    
  void Gyroscope::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }

  void Gyroscope::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefQuadrant[0].save(node, TAG_COLOUR_1);
    cDefQuadrant[1].save(node, TAG_COLOUR_1);
    cDefQuadrant[2].save(node, TAG_COLOUR_3);
    cDefQuadrant[3].save(node, TAG_COLOUR_4);
    cDefOutline.save(node, TAG_OUTLINE);
  }

  void Gyroscope::hintInUse(bool inUse) {
    cDefTexture.hintTextureInUse(inUse);
  }

  bool Gyroscope::renderIcon() const {
    glTranslatef(0.0f, -1.0f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(cEditingIconRotation,  0.0f, 0.0f, 1.0f);
    glScalef(3.0f, 3.0f, 3.0f);
    render();
    return true;
  }

  std::vector<IProperty*> Gyroscope::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  I3DModel* Gyroscope::createModel() {
    return this;
  }

  bool Gyroscope::renderAssetIcon() const {
    return renderIcon();
  }

  void Gyroscope::update(unsigned int milliseconds) {
    // Nothing to do
  }

  void Gyroscope::render() const {
    cDefTexture.set();
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

    cDefTexture.setRenderTarget();
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

    IApplication* mApplication = cProject->getApplication();
    mApplication->setViewPort();
  }
    
  void Gyroscope::renderCircle(float radius, float startAngle, float endAngle, Colour& colour) {
    glBegin(GL_TRIANGLE_FAN);
    colour.set();
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
    colour.set();
    float mStartAngle =   0.0f * (M_PI / 180.0f);
    float mEndAngle   = 360.0f * (M_PI / 180.0f);
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
      glVertex2f(std::sin(angle) * innerRadius, std::cos(angle) * innerRadius);
      glVertex2f(std::sin(angle) * outerRadius, std::cos(angle) * outerRadius);
    }
    glEnd();
  }
}

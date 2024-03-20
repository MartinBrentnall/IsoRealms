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
#include "Top.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const std::string Top::TAG_OUTLINE = "Outline";
  const std::string Top::TAG_SIDE    = "Side";
  const std::string Top::TAG_TOP     = "Top";

  const float Top::OUTLINE        = 0.82f;
  const float Top::SPINDLE_WIDTH  = 0.03f;
  const float Top::SPINDLE_HEIGHT = 0.25f;
  const float Top::WIDTH          = 0.3f;
  const float Top::HEIGHT         = 0.75f;

  Top::Top(IProject* project, Spindizzy* spindizzy) :
            cProject(project),
            cDefColourTop(    project, 1.0f, 1.0f, 0.0f, 0.0f, [this]() {updateTextures();}),
            cDefColourSide(   project, 1.0f, 0.0f, 0.0f, 0.0f, [this]() {updateTextures();}),
            cDefColourOutline(project, 0.0f, 0.0f, 0.0f, 0.0f, [this]() {updateTextures();}),
            cRuntimeTextureTop(project),
            cRuntimeTextureSide(project),
            cEditingIconAngle(0.0f) {
    project->updateEditing([this](unsigned int milliseconds) {
      cEditingIconAngle -= 0.25f * milliseconds;
    });
    
    project->mainThreadInit([this]() {
      updateTextures();
    });
  }
            
  Top::Top(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            Top(project, spindizzy) {
    cDefColourTop.init(node.getNode(TAG_TOP));
    cDefColourSide.init(node.getNode(TAG_SIDE));
    cDefColourOutline.init(node.getNode(TAG_OUTLINE));
  }

  void Top::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Spindizzy Top Models");
  }
    
  void Top::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }
  
  void Top::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefColourTop.save(node, TAG_TOP);
    cDefColourSide.save(node, TAG_SIDE);
    cDefColourSide.save(node, TAG_OUTLINE);
  }

  void Top::hintInUse(bool inUse) {
    cRuntimeTextureSide.hintTextureInUse(inUse);
    cRuntimeTextureTop.hintTextureInUse(inUse);
  }

  bool Top::renderIcon() const {
    glTranslatef(0.0f, -1.0f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(cEditingIconAngle, 0.0f, 0.0f, 1.0f);
    glScalef(2.5f, 2.5f, 2.5f);
    return renderPreview();
  }

  std::vector<IProperty*> Top::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  I3DModel* Top::createModel() {
    return this;
  }

  bool Top::renderPreview() const {
    render();
    return true;
  }

  bool Top::renderAssetIcon() const {
    return renderIcon();
  }

  void Top::update(unsigned int milliseconds) {
    // Nothing to do
  }

  void Top::render() const {
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    cRuntimeTextureSide.set();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -SPINDLE_WIDTH, SPINDLE_HEIGHT * 1.2f);
    glVertex3f(0.0f,  SPINDLE_WIDTH, SPINDLE_HEIGHT * 1.2f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f( SPINDLE_WIDTH, 0.0f, SPINDLE_HEIGHT * 1.2f);
    glVertex3f(-SPINDLE_WIDTH, 0.0f, SPINDLE_HEIGHT * 1.2f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
    glEnable(GL_CULL_FACE);  

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0, 0.0); glVertex3f( WIDTH, -WIDTH, HEIGHT);
    glTexCoord2f(1.0, 0.0); glVertex3f(-WIDTH, -WIDTH, HEIGHT);
    glTexCoord2f(0.5, 1.0); glVertex3f( 0.0,    0.0,   SPINDLE_HEIGHT);

    glTexCoord2f(0.0, 0.0); glVertex3f( WIDTH,  WIDTH, HEIGHT);
    glTexCoord2f(0.5, 1.0); glVertex3f( 0.0,    0.0,   SPINDLE_HEIGHT);
    glTexCoord2f(1.0, 0.0); glVertex3f(-WIDTH,  WIDTH, HEIGHT);

    glTexCoord2f(0.0, 0.0); glVertex3f( WIDTH,  WIDTH, HEIGHT);
    glTexCoord2f(1.0, 0.0); glVertex3f( WIDTH, -WIDTH, HEIGHT);
    glTexCoord2f(0.5, 1.0); glVertex3f( 0.0,    0.0,   SPINDLE_HEIGHT);

    glTexCoord2f(0.0, 0.0); glVertex3f(-WIDTH,  WIDTH, HEIGHT);
    glTexCoord2f(0.5, 1.0); glVertex3f( 0.0,    0.0,   SPINDLE_HEIGHT);
    glTexCoord2f(1.0, 0.0); glVertex3f(-WIDTH, -WIDTH, HEIGHT);
    glEnd();

    cRuntimeTextureTop.set();
    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 1.0); glVertex3f( WIDTH,  WIDTH, HEIGHT);
    glTexCoord2f(0.0, 1.0); glVertex3f(-WIDTH,  WIDTH, HEIGHT);
    glTexCoord2f(0.0, 0.0); glVertex3f(-WIDTH, -WIDTH, HEIGHT);
    glTexCoord2f(1.0, 0.0); glVertex3f( WIDTH, -WIDTH, HEIGHT);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void Top::updateTextures() {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();
    cRuntimeTextureTop.setRenderTarget();  generateTextureTop();
    cRuntimeTextureSide.setRenderTarget(); generateTextureSide();
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
    IApplication* mApplication = cProject->getApplication();
    mApplication->setViewPort();
  }

  void Top::generateTextureTop() {
    glClearColor(cDefColourOutline.getRed(), cDefColourOutline.getGreen(), cDefColourOutline.getBlue(), 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    cDefColourTop.set();
    glVertex2f(-OUTLINE, -OUTLINE);
    glVertex2f( OUTLINE, -OUTLINE);
    glVertex2f( OUTLINE,  OUTLINE);
    glVertex2f(-OUTLINE,  OUTLINE);
    glEnd();
  }

  void Top::generateTextureSide() {
    glClearColor(cDefColourOutline.getRed(), cDefColourOutline.getGreen(), cDefColourOutline.getBlue(), 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    cDefColourSide.set();
    glVertex2f(-0.74, -OUTLINE);
    glVertex2f( 0.74, -OUTLINE);
    glVertex2f( 0.0f,  0.62f);
    glEnd();
  }
}

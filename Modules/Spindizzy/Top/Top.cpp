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
  const std::string Top::JSON_OUTLINE = "outline";
  const std::string Top::JSON_SIDES   = "sides";
  const std::string Top::JSON_TOP     = "top";

  const float Top::OUTLINE        = 0.82f;
  const float Top::SPINDLE_WIDTH  = 0.03f;
  const float Top::SPINDLE_HEIGHT = 0.25f;
  const float Top::WIDTH          = 0.3f;
  const float Top::HEIGHT         = 0.75f;

  Top::Top(IProject* project, Spindizzy* spindizzy) :
            cProject(project),
            cDefColourTop(    project, 1.0f, 1.0f, 0.0f, 0.0f, [this]() {setNeedsRedrawing();}),
            cDefColourSide(   project, 1.0f, 0.0f, 0.0f, 0.0f, [this]() {setNeedsRedrawing();}),
            cDefColourOutline(project, 0.0f, 0.0f, 0.0f, 0.0f, [this]() {setNeedsRedrawing();}),
            cRuntimeTextureTop(project),
            cRuntimeTextureSide(project),
            cNeedsRedrawing(false),
            cEditingIconAngle(0.0f) {
    project->updateEditing([this](unsigned int milliseconds) {
      cEditingIconAngle -= 0.25f * milliseconds;
    });
    setNeedsRedrawing();
  }
            
  Top::Top(IProject* project, Spindizzy* spindizzy, JSONObject object, IOptions* options, IResourceData* data) :
            Top(project, spindizzy) {
    cDefColourTop.init(object, JSON_TOP);
    cDefColourSide.init(object, JSON_SIDES);
    cDefColourOutline.init(object, JSON_OUTLINE);
  }

  void Top::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Spindizzy Top Models");
  }
    
  void Top::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }
  
  void Top::save(JSONObject object, IAssetIdentifier* identifier) const {
    cDefColourTop.save(object, JSON_TOP);
    cDefColourSide.save(object, JSON_SIDES);
    cDefColourOutline.save(object, JSON_OUTLINE);
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

  void Top::saveAsset(JSONObject object) const {
    // Nothing to do.
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

  void Top::setNeedsRedrawing() {
    if (!cNeedsRedrawing) {
      cProject->updateLater([this]() {
        updateTextures();
        cNeedsRedrawing = false;
      });
    }
  }
}

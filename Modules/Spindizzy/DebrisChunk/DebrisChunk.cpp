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
#include "DebrisChunk.h"

namespace IsoRealms::Spindizzy {
  DebrisChunk::DebrisChunk(Spindizzy& spindizzy, IComponentData& data) :
            cProject(data.getProject()),
            cDefSide{Colour(data, 1.0f, 0.0f, 0.0f, 1.0f, [this]() {setNeedsRedrawing();}),
                     Colour(data, 1.0f, 1.0f, 0.0f, 1.0f, [this]() {setNeedsRedrawing();}),
                     Colour(data, 0.0f, 1.0f, 0.0f, 1.0f, [this]() {setNeedsRedrawing();}),
                     Colour(data, 0.0f, 0.0f, 1.0f, 1.0f, [this]() {setNeedsRedrawing();})},
            cDefOutline(data, 0.0f, 0.0f, 0.0f, 1.0f, [this]() {setNeedsRedrawing();}),
            cDefOutlineWidth(DEFAULT_OUTLINE_WIDTH),
            cTextures{{data.getProject(), 128, 128},
                      {data.getProject(), 128, 128},
                      {data.getProject(), 128, 128},
                      {data.getProject(), 128, 128}},
            cNeedsRedrawing(false),
            cEditingIconRotation(0.0f) {
    setNeedsRedrawing();
  }

  void DebrisChunk::registerAssets(ComponentAssetRegistry& assets) {
    assets.add<IModel>(this, "", "Spindizzy Debris Chunks");
  }

  void DebrisChunk::hintInUse(bool inUse) {
    for (ITexture& mTexture : cTextures) {
      mTexture.hintTextureInUse(inUse);
    }
  }

  bool DebrisChunk::renderIcon() const {
    glRotatef(cEditingIconRotation,        1.0f, 0.0f, 0.0f);
    glRotatef(cEditingIconRotation * 0.2f, 0.0f, 1.0f, 0.0f);
    glScalef(1.25f, 1.25f, 1.25f);
    render();
    return true;
  }

  void DebrisChunk::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector("side1",   cDefSide[0]);
    owner.createPropertyTreeSelector("side2",   cDefSide[1]);
    owner.createPropertyTreeSelector("side3",   cDefSide[2]);
    owner.createPropertyTreeSelector("side4",   cDefSide[3]);
    owner.createPropertyTreeSelector("outline", cDefOutline);
  }

  void DebrisChunk::removed() {
    // Nothing to do.
  }

  void DebrisChunk::updateEditing(unsigned int milliseconds) {
    cEditingIconRotation += 0.1f * milliseconds;
  }
  
  IModelInstance* DebrisChunk::createModel() {
    return this;
  }

  bool DebrisChunk::renderPreview() const {
    return renderIcon();
  }

  bool DebrisChunk::renderAssetIcon() const {
    return renderIcon();
  }

  void DebrisChunk::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void DebrisChunk::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool DebrisChunk::isDefaultConfiguration() const {
    return true;
  }

  void DebrisChunk::update(unsigned int milliseconds) {
    // Nothing to do
  }

  void DebrisChunk::render() const {
    LiteralVertex mVerteA( sqrt(8.0 / 9.0),  0.0,             -1.0 / 3.0);
    LiteralVertex mVerteB(-sqrt(2.0 / 9.0),  sqrt(2.0 / 3.0), -1.0 / 3.0);
    LiteralVertex mVerteC(-sqrt(2.0 / 9.0), -sqrt(2.0 / 3.0), -1.0 / 3.0);
    LiteralVertex mVerteD( 0.0,              0.0,              1.0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);

    glColor3f(1.0f, 1.0f, 1.0f);
    cTextures[0].set();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0, 0.0); glVertex3f(mVerteA.x, mVerteA.y, mVerteA.z);
    glTexCoord2f(0.5, 1.0); glVertex3f(mVerteC.x, mVerteC.y, mVerteC.z);
    glTexCoord2f(1.0, 0.0); glVertex3f(mVerteB.x, mVerteB.y, mVerteB.z);
    glEnd();

    cTextures[1].set();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0, 0.0); glVertex3f(mVerteA.x, mVerteA.y, mVerteA.z);
    glTexCoord2f(1.0, 0.0); glVertex3f(mVerteB.x, mVerteB.y, mVerteB.z);
    glTexCoord2f(0.5, 1.0); glVertex3f(mVerteD.x, mVerteD.y, mVerteD.z);
    glEnd();

    cTextures[2].set();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0, 0.0); glVertex3f(mVerteB.x, mVerteB.y, mVerteB.z);
    glTexCoord2f(1.0, 0.0); glVertex3f(mVerteC.x, mVerteC.y, mVerteC.z);
    glTexCoord2f(0.5, 1.0); glVertex3f(mVerteD.x, mVerteD.y, mVerteD.z);
    glEnd();

    cTextures[3].set();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0, 0.0); glVertex3f(mVerteC.x, mVerteC.y, mVerteC.z);
    glTexCoord2f(1.0, 0.0); glVertex3f(mVerteA.x, mVerteA.y, mVerteA.z);
    glTexCoord2f(0.5, 1.0); glVertex3f(mVerteD.x, mVerteD.y, mVerteD.z);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void DebrisChunk::regenerateTextures() {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();

    cTextures[0].setRenderTarget(); generateTexture(cDefSide[0]);
    cTextures[1].setRenderTarget(); generateTexture(cDefSide[1]);
    cTextures[2].setRenderTarget(); generateTexture(cDefSide[2]);
    cTextures[3].setRenderTarget(); generateTexture(cDefSide[3]);

    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    Application& mApplication = cProject.getApplication();
    mApplication.setViewPort();
  }

  void DebrisChunk::generateTexture(Colour& colour) {
    glClearColor(cDefOutline->getRed(), cDefOutline->getGreen(), cDefOutline->getBlue(), 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    colour->set();
    LiteralVertex mInputA(-1.0f, -1.0f, 0.0f);
    LiteralVertex mInputB( 1.0f, -1.0f, 0.0f);
    LiteralVertex mInputC( 0.0f,  1.0f, 0.0f);
    LiteralVertex mOutputA;
    LiteralVertex mOutputB;
    LiteralVertex mOutputC;
    Utils::calculateTriangle(mInputA, mInputB, mInputC, mOutputA, mOutputB, mOutputC, cDefOutlineWidth);
    glVertex2f(mOutputA.getX(), mOutputA.getY());
    glVertex2f(mOutputB.getX(), mOutputB.getY());
    glVertex2f(mOutputC.getX(), mOutputC.getY());
    glEnd();
  }

  void DebrisChunk::setNeedsRedrawing() {
    if (!cNeedsRedrawing) {
      cProject.updateLater([this]() {
        regenerateTextures();
        cNeedsRedrawing = false;
      });
    }
  }
}

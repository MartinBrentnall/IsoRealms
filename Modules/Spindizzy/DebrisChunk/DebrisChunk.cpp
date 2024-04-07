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
#include "DebrisChunk.h"

#include "IsoRealms/IApplication.h"

namespace IsoRealms::Spindizzy {
  const std::string DebrisChunk::TAG_OUTLINE       = "Outline";
  const std::string DebrisChunk::TAG_SIDE_1        = "Side1";
  const std::string DebrisChunk::TAG_SIDE_2        = "Side2";
  const std::string DebrisChunk::TAG_SIDE_3        = "Side3";
  const std::string DebrisChunk::TAG_SIDE_4        = "Side4";

  const std::string DebrisChunk::ATTRIBUTE_OUTLINE_WIDTH = "outlineWidth";
  
  const float DebrisChunk::DEFAULT_OUTLINE_WIDTH = 0.18f;

  DebrisChunk::DebrisChunk(IProject* project, Spindizzy* spindizzy) :
            cProject(project),
            cDefSide{Colour(project, 1.0f, 1.0f, 0.0f, 0.0f, [this]() {regenerateTextures();}),
                     Colour(project, 1.0f, 0.0f, 0.0f, 0.0f, [this]() {regenerateTextures();}),
                     Colour(project, 0.0f, 1.0f, 0.0f, 0.0f, [this]() {regenerateTextures();}),
                     Colour(project, 0.0f, 0.0f, 1.0f, 0.0f, [this]() {regenerateTextures();})},
            cDefOutline(project, 1.0f, 0.0f, 1.0f, 0.0f, [this]() {regenerateTextures();}),
            cDefOutlineWidth(DEFAULT_OUTLINE_WIDTH),
            cDefTextures{project, project, project, project},
            cEditingIconRotation(0.0f) {
    project->updateEditing([this](unsigned int milliseconds) {
      cEditingIconRotation += 0.1f * milliseconds;
    });

    project->mainThreadInit([this]() {
      regenerateTextures();
    });
  }

  DebrisChunk::DebrisChunk(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            DebrisChunk(project, spindizzy) {
    cDefOutline.init(node, TAG_OUTLINE);
    cDefOutlineWidth = node.getFloatAttribute(ATTRIBUTE_OUTLINE_WIDTH, DEFAULT_OUTLINE_WIDTH);
    cDefSide[0].init(node, TAG_SIDE_1);
    cDefSide[1].init(node, TAG_SIDE_2);
    cDefSide[2].init(node, TAG_SIDE_3);
    cDefSide[3].init(node, TAG_SIDE_4);
  }

  void DebrisChunk::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Spindizzy Debris Chunks");
  }

  void DebrisChunk::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }

  void DebrisChunk::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefOutline.save(node, TAG_OUTLINE);
    node->addAttribute(ATTRIBUTE_OUTLINE_WIDTH, cDefOutlineWidth, DEFAULT_OUTLINE_WIDTH);
    cDefSide[0].save(node, TAG_SIDE_1);
    cDefSide[1].save(node, TAG_SIDE_2);
    cDefSide[2].save(node, TAG_SIDE_3);
    cDefSide[3].save(node, TAG_SIDE_4);
  }

  void DebrisChunk::hintInUse(bool inUse) {
    for (ITexture& mTexture : cDefTextures) {
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

  std::vector<IProperty*> DebrisChunk::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  I3DModel* DebrisChunk::createModel() {
    return this;
  }

  bool DebrisChunk::renderPreview() const {
    return renderIcon();
  }

  bool DebrisChunk::renderAssetIcon() const {
    return renderIcon();
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
    cDefTextures[0].set();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0, 0.0); glVertex3f(mVerteA.x, mVerteA.y, mVerteA.z);
    glTexCoord2f(0.5, 1.0); glVertex3f(mVerteC.x, mVerteC.y, mVerteC.z);
    glTexCoord2f(1.0, 0.0); glVertex3f(mVerteB.x, mVerteB.y, mVerteB.z);
    glEnd();

    cDefTextures[1].set();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0, 0.0); glVertex3f(mVerteA.x, mVerteA.y, mVerteA.z);
    glTexCoord2f(1.0, 0.0); glVertex3f(mVerteB.x, mVerteB.y, mVerteB.z);
    glTexCoord2f(0.5, 1.0); glVertex3f(mVerteD.x, mVerteD.y, mVerteD.z);
    glEnd();

    cDefTextures[2].set();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0, 0.0); glVertex3f(mVerteB.x, mVerteB.y, mVerteB.z);
    glTexCoord2f(1.0, 0.0); glVertex3f(mVerteC.x, mVerteC.y, mVerteC.z);
    glTexCoord2f(0.5, 1.0); glVertex3f(mVerteD.x, mVerteD.y, mVerteD.z);
    glEnd();

    cDefTextures[3].set();
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

    cDefTextures[0].setRenderTarget(); generateTexture(cDefSide[0]);
    cDefTextures[1].setRenderTarget(); generateTexture(cDefSide[1]);
    cDefTextures[2].setRenderTarget(); generateTexture(cDefSide[2]);
    cDefTextures[3].setRenderTarget(); generateTexture(cDefSide[3]);

    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    IApplication* mApplication = cProject->getApplication();
    mApplication->setViewPort();
  }

  void DebrisChunk::generateTexture(Colour& colour) {
    glClearColor(cDefOutline.getRed(), cDefOutline.getGreen(), cDefOutline.getBlue(), 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    colour.set();
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
}

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
#include "C64LiftGraphics.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const float C64LiftGraphics::CIRCLE_RESOLUTION            = 5.0f * (M_PI / 180.0);
  const float C64LiftGraphics::CIRCLE_OUTLINE_OUTER         = 1.0f;
  const float C64LiftGraphics::CIRCLE_OUTLINE_INNER         = 0.6f;
  const float C64LiftGraphics::CIRCLE_COLOUR_OUTER          = 0.9f;
  const float C64LiftGraphics::CIRCLE_COLOUR_INNER          = 0.7f;
  const float C64LiftGraphics::SQUARE_OUTLINE_OUTER         = 1.0f;
  const float C64LiftGraphics::SQUARE_OUTLINE_INNER         = 0.51f;
  const float C64LiftGraphics::SQUARE_COLOUR_OUTER          = 0.87f;
  const float C64LiftGraphics::SQUARE_COLOUR_INNER          = 0.64f;
  const float C64LiftGraphics::SQUARE_TRIANGLE_OUTER        = 0.2f;
  const float C64LiftGraphics::SQUARE_TRIANGLE_INNER        = 0.0f;
  const float C64LiftGraphics::DIAMOND_OUTLINE_OUTER        = 1.0f;
  const float C64LiftGraphics::DIAMOND_OUTLINE_INNER        = 0.6f;
  const float C64LiftGraphics::DIAMOND_COLOUR_OUTER         = 0.9f;
  const float C64LiftGraphics::DIAMOND_COLOUR_INNER         = 0.7f;
  const float C64LiftGraphics::DIAMOND_SQUARE_OUTLINE_OUTER = 0.70f;
  const float C64LiftGraphics::DIAMOND_SQUARE_OUTLINE_INNER = 0.40f;
  const float C64LiftGraphics::DIAMOND_SQUARE_COLOUR_OUTER  = 0.62f;
  const float C64LiftGraphics::DIAMOND_SQUARE_COLOUR_INNER  = 0.48f;

  const std::string C64LiftGraphics::CIRCLE_NONE  = "CircleEmpty";
  const std::string C64LiftGraphics::CIRCLE_HALF  = "CircleHalf";
  const std::string C64LiftGraphics::CIRCLE_BOTH  = "CircleFilled";
  const std::string C64LiftGraphics::SQUARE_NONE  = "SquareEmpty";
  const std::string C64LiftGraphics::SQUARE_HALF  = "SquareHalf";
  const std::string C64LiftGraphics::SQUARE_BOTH  = "SquareFilled";
  const std::string C64LiftGraphics::DIAMOND_NONE = "DiamondEmpty";
  const std::string C64LiftGraphics::DIAMOND_HALF = "DiamondHalf";
  const std::string C64LiftGraphics::DIAMOND_BOTH = "DiamondFilled";

  const std::string C64LiftGraphics::TAG_OUTLINE   = "Outline";
  const std::string C64LiftGraphics::TAG_PRIMARY   = "Primary";
  const std::string C64LiftGraphics::TAG_SECONDARY = "Secondary";

  C64LiftGraphics::C64LiftGraphics(IProject* project, Spindizzy* spindizzy) :
            cProject(project),
            cDefPrimary(project, 1.0f, 1.0f, 1.0f, 0.0f, [this]() {generateTextures();}),
            cDefSecondary(project, 0.7f, 0.7f, 0.7f, 0.0f, [this]() {generateTextures();}),
            cDefOutline(project, 0.0f, 0.0f, 0.0f, 0.0f, [this]() {generateTextures();}) {
    cDefTextures[CIRCLE_NONE]  = std::make_unique<LiteralTexture>(project);
    cDefTextures[CIRCLE_HALF]  = std::make_unique<LiteralTexture>(project);
    cDefTextures[CIRCLE_BOTH]  = std::make_unique<LiteralTexture>(project);
    cDefTextures[SQUARE_NONE]  = std::make_unique<LiteralTexture>(project);
    cDefTextures[SQUARE_HALF]  = std::make_unique<LiteralTexture>(project);
    cDefTextures[SQUARE_BOTH]  = std::make_unique<LiteralTexture>(project);
    cDefTextures[DIAMOND_NONE] = std::make_unique<LiteralTexture>(project);
    cDefTextures[DIAMOND_HALF] = std::make_unique<LiteralTexture>(project);
    cDefTextures[DIAMOND_BOTH] = std::make_unique<LiteralTexture>(project);
    project->updateLater([this]() {
      generateTextures();
    });
  }
  
  C64LiftGraphics::C64LiftGraphics(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            C64LiftGraphics(project, spindizzy) {
    cDefOutline.init(node, TAG_OUTLINE);
    cDefPrimary.init(node, TAG_PRIMARY);
    cDefSecondary.init(node, TAG_SECONDARY);
  }

  void C64LiftGraphics::registerAssets(IAssetRegistry* assets) {
    for (std::pair<const std::string, std::unique_ptr<LiteralTexture>>& mPair : cDefTextures) {
      assets->add(mPair.second.get(), mPair.first, "Spindizzy Lift Textures");
    }
  }
  
  void C64LiftGraphics::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    for (std::pair<const std::string, std::unique_ptr<LiteralTexture>>& mPair : cDefTextures) {
      assets->remove(mPair.second.get());
    }
  }
  
  void C64LiftGraphics::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefOutline.save(node, TAG_OUTLINE);
    cDefPrimary.save(node, TAG_PRIMARY);
    cDefSecondary.save(node, TAG_SECONDARY);
  }

  void C64LiftGraphics::hintInUse(bool inUse) {
    for (const std::pair<const std::string, std::unique_ptr<LiteralTexture>>& mTexture : cDefTextures) {
      mTexture.second->hintTextureInUse(inUse);
    }
  }
  
  bool C64LiftGraphics::renderIcon() {
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_YAW,   0.0f, 0.0f, 1.0f);
    glScalef(1.4f, 1.4f, 1.4f);
    cDefTextures[DIAMOND_HALF]->set();
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(-1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f( 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( 1.0f,  1.0f);
    glEnd();
    return true;
  }

  std::vector<IProperty*> C64LiftGraphics::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }
  
  void C64LiftGraphics::generateTextures() {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();

    cDefTextures[CIRCLE_HALF]->setRenderTarget();  renderLiftCircleHalf();
    cDefTextures[CIRCLE_NONE]->setRenderTarget();  renderLiftCircle();
    cDefTextures[CIRCLE_BOTH]->setRenderTarget();  renderLiftCircleBoth();
    cDefTextures[SQUARE_HALF]->setRenderTarget();  renderLiftSquareHalf();
    cDefTextures[SQUARE_NONE]->setRenderTarget();  renderLiftSquare();
    cDefTextures[SQUARE_BOTH]->setRenderTarget();  renderLiftSquareBoth();
    cDefTextures[DIAMOND_HALF]->setRenderTarget(); renderLiftDiamondHalf();
    cDefTextures[DIAMOND_NONE]->setRenderTarget(); renderLiftDiamond();
    cDefTextures[DIAMOND_BOTH]->setRenderTarget(); renderLiftDiamondBoth();

    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    IApplication* mApplication = cProject->getApplication();
    mApplication->setViewPort();
  }

  void C64LiftGraphics::clear() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void C64LiftGraphics::renderCircle(float radius, Colour& colour) {
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

  void C64LiftGraphics::renderCircularRing(float outer, float inner, Colour& colour) {
    glBegin(GL_TRIANGLE_STRIP);
    colour.set();
    float mStartAngle = 0.0f * (M_PI / 180.0f);
    float mEndAngle = 360.0f * (M_PI / 180.0f);
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
      glVertex2f(std::sin(angle) * inner, std::cos(angle) * inner);
      glVertex2f(std::sin(angle) * outer, std::cos(angle) * outer);
    }
    glEnd();
  }

  void C64LiftGraphics::renderSquareRing(float outer, float inner, Colour& colour) {
    colour.set();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(-outer, -outer);
    glVertex2f(-inner, -inner);
    glVertex2f(-outer,  outer);
    glVertex2f(-inner,  inner);
    glVertex2f( outer,  outer);
    glVertex2f( inner,  inner);
    glVertex2f( outer, -outer);
    glVertex2f( inner, -inner);
    glVertex2f(-outer, -outer);
    glVertex2f(-inner, -inner);
    glEnd();
  }

  void C64LiftGraphics::renderSquareRingHalf(float outer, float inner, Colour& colour) {
    colour.set();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(-outer, -outer);
    glVertex2f(-inner, -inner);
    glVertex2f(-outer,  outer);
    glVertex2f(-inner,  inner);
    glVertex2f( outer,  outer);
    glVertex2f( inner,  inner);
    glEnd();
  }

  void C64LiftGraphics::renderDiamondRing(float outer, float inner, Colour& colour) {
    colour.set();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(-inner,  0.0f);
    glVertex2f(-outer,  0.0f);
    glVertex2f(0.0f,   -inner);
    glVertex2f(0.0f,   -outer);
    glVertex2f( inner,  0.0f);
    glVertex2f( outer,  0.0f);
    glVertex2f(0.0f,    inner);
    glVertex2f(0.0f,    outer);
    glVertex2f(-inner,  0.0f);
    glVertex2f(-outer,  0.0f);
    glEnd();
  }

  void C64LiftGraphics::renderDiamondRingHalf(float outer, float inner, Colour& colour) {
    colour.set();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(-inner,  0.0f);
    glVertex2f(-outer,  0.0f);
    glVertex2f(0.0f,   -inner);
    glVertex2f(0.0f,   -outer);
    glVertex2f( inner,  0.0f);
    glVertex2f( outer,  0.0f);
    glVertex2f(0.0f,    inner);
    glVertex2f(0.0f,    outer);
    glEnd();
  }

  void C64LiftGraphics::renderDiamondRingEdges(float outer, float edge, Colour& colour) {
    colour.set();
    glBegin(GL_TRIANGLES);
    glVertex2f(-outer,         0.0f);
    glVertex2f(-edge,         -outer + edge);
    glVertex2f(-edge,          outer - edge);
    glVertex2f( 0.0f,         -outer);
    glVertex2f( outer - edge, -edge);
    glVertex2f(-outer + edge, -edge);
    glVertex2f( outer,         0.0f);
    glVertex2f( edge,          outer - edge);
    glVertex2f( edge,         -outer + edge);
    glVertex2f( 0.0f,          outer);
    glVertex2f(-outer + edge,  edge);
    glVertex2f( outer - edge,  edge);
    glEnd();
  }

  void C64LiftGraphics::renderLiftCircle() {
    clear();
    renderCircularRing(CIRCLE_OUTLINE_OUTER, CIRCLE_OUTLINE_INNER, cDefOutline);
    renderCircularRing(CIRCLE_COLOUR_OUTER,  CIRCLE_COLOUR_INNER,  cDefPrimary);
  }

  void C64LiftGraphics::renderLiftCircleHalf() {
    renderLiftCircle();
    glBegin(GL_TRIANGLE_FAN);
    cDefSecondary.set();
    glVertex2f(0.0f, 0.0f);
    float mStartAngle = 45.0f * (M_PI / 180.0f);
    float mEndAngle = 225.0f * (M_PI / 180.0f);
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
      glVertex2f(std::sin(angle) * CIRCLE_COLOUR_INNER, std::cos(angle) * CIRCLE_COLOUR_INNER);
    }
    glEnd();
  }

  void C64LiftGraphics::renderLiftCircleBoth() {
    clear();
    renderCircle(CIRCLE_OUTLINE_OUTER, cDefOutline);
    renderCircle(CIRCLE_COLOUR_OUTER,  cDefPrimary);
    renderCircle(CIRCLE_COLOUR_INNER,  cDefSecondary);
  }

  void C64LiftGraphics::renderLiftSquare() {
    clear();
    renderSquareRing(SQUARE_OUTLINE_OUTER, SQUARE_OUTLINE_INNER, cDefOutline);
    renderSquareRing(SQUARE_COLOUR_OUTER,  SQUARE_COLOUR_INNER,  cDefSecondary);
  }

  void C64LiftGraphics::renderLiftSquareHalf() {
    renderLiftSquare();
    glBegin(GL_TRIANGLES);
    cDefOutline.set();
    glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_COLOUR_INNER);
    glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_TRIANGLE_INNER);
    glVertex2f(-SQUARE_TRIANGLE_INNER,  SQUARE_COLOUR_INNER);
    cDefPrimary.set();
    glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_COLOUR_INNER);
    glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_TRIANGLE_OUTER);
    glVertex2f(-SQUARE_TRIANGLE_OUTER,  SQUARE_COLOUR_INNER);
    glEnd();
  }

  void C64LiftGraphics::renderLiftSquareBoth() {
    renderLiftSquareHalf();
    glBegin(GL_TRIANGLES);
    cDefOutline.set();
    glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_COLOUR_INNER);
    glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_TRIANGLE_INNER);
    glVertex2f(SQUARE_TRIANGLE_INNER, -SQUARE_COLOUR_INNER);
    cDefPrimary.set();
    glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_COLOUR_INNER);
    glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_TRIANGLE_OUTER);
    glVertex2f(SQUARE_TRIANGLE_OUTER, -SQUARE_COLOUR_INNER);
    glEnd();
  }

  void C64LiftGraphics::renderLiftDiamondBoth() {
    clear();
    renderSquareRing(DIAMOND_SQUARE_OUTLINE_OUTER, DIAMOND_SQUARE_OUTLINE_INNER, cDefOutline);
    renderSquareRing(DIAMOND_SQUARE_COLOUR_OUTER,  DIAMOND_SQUARE_COLOUR_INNER,  cDefPrimary);
    renderDiamondRing(DIAMOND_OUTLINE_OUTER,       DIAMOND_OUTLINE_INNER,        cDefOutline);
    renderDiamondRing(DIAMOND_COLOUR_OUTER,        DIAMOND_COLOUR_INNER,         cDefSecondary);
  }

  void C64LiftGraphics::renderLiftDiamondHalf() {
    renderLiftDiamond();
    renderDiamondRingHalf(DIAMOND_OUTLINE_OUTER,       DIAMOND_OUTLINE_INNER,        cDefOutline);
    renderDiamondRingHalf(DIAMOND_COLOUR_OUTER,        DIAMOND_COLOUR_INNER,         cDefSecondary);
    renderSquareRingHalf(DIAMOND_SQUARE_OUTLINE_OUTER, DIAMOND_SQUARE_OUTLINE_INNER, cDefOutline);
    renderSquareRingHalf(DIAMOND_SQUARE_COLOUR_OUTER,  DIAMOND_SQUARE_COLOUR_INNER,  cDefPrimary);
  }

  void C64LiftGraphics::renderLiftDiamond() {
    clear();
    renderDiamondRingEdges(DIAMOND_OUTLINE_OUTER,  DIAMOND_SQUARE_COLOUR_OUTER,  cDefOutline);
    renderDiamondRingEdges(DIAMOND_COLOUR_OUTER,   DIAMOND_SQUARE_COLOUR_OUTER,  cDefSecondary);
    renderSquareRing(DIAMOND_SQUARE_OUTLINE_OUTER, DIAMOND_SQUARE_OUTLINE_INNER, cDefOutline);
    renderSquareRing(DIAMOND_SQUARE_COLOUR_OUTER,  DIAMOND_SQUARE_COLOUR_INNER,  cDefPrimary);
  }
}

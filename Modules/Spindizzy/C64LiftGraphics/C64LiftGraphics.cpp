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

  const std::string C64LiftGraphics::JSON_OUTLINE   = "outline";
  const std::string C64LiftGraphics::JSON_PRIMARY   = "primary";
  const std::string C64LiftGraphics::JSON_SECONDARY = "secondary";

  C64LiftGraphics::C64LiftGraphics(Spindizzy& spindizzy, IResourceData& data) :
            cProject(data.getProject()),
            cDefPrimary(data, 1.0f, 1.0f, 1.0f, 0.0f, [this]() {setNeedsRedrawing();}),
            cDefSecondary(data, 0.7f, 0.7f, 0.7f, 0.0f, [this]() {setNeedsRedrawing();}),
            cDefOutline(data, 0.0f, 0.0f, 0.0f, 0.0f, [this]() {setNeedsRedrawing();}),
            cNeedsRedrawing(false) {
    cTextures[CIRCLE_NONE]  = std::make_unique<LiteralTexture>(data.getProject(), 128, 128);
    cTextures[CIRCLE_HALF]  = std::make_unique<LiteralTexture>(data.getProject(), 128, 128);
    cTextures[CIRCLE_BOTH]  = std::make_unique<LiteralTexture>(data.getProject(), 128, 128);
    cTextures[SQUARE_NONE]  = std::make_unique<LiteralTexture>(data.getProject(), 128, 128);
    cTextures[SQUARE_HALF]  = std::make_unique<LiteralTexture>(data.getProject(), 128, 128);
    cTextures[SQUARE_BOTH]  = std::make_unique<LiteralTexture>(data.getProject(), 128, 128);
    cTextures[DIAMOND_NONE] = std::make_unique<LiteralTexture>(data.getProject(), 128, 128);
    cTextures[DIAMOND_HALF] = std::make_unique<LiteralTexture>(data.getProject(), 128, 128);
    cTextures[DIAMOND_BOTH] = std::make_unique<LiteralTexture>(data.getProject(), 128, 128);
    setNeedsRedrawing();
  }
  
  C64LiftGraphics::C64LiftGraphics(Spindizzy& spindizzy, IResourceData& data, JSONObject object) :
            C64LiftGraphics(spindizzy, data) {
    cDefOutline.init(object, JSON_OUTLINE);
    cDefPrimary.init(object, JSON_PRIMARY);
    cDefSecondary.init(object, JSON_SECONDARY);
  }

  void C64LiftGraphics::registerAssets(ResourceAssetRegistry& assets) {
    for (std::pair<const std::string, std::unique_ptr<LiteralTexture>>& mPair : cTextures) {
      assets.add<ITexture>(mPair.second.get(), mPair.first, "Spindizzy Lift Textures");
    }
  }
  
  void C64LiftGraphics::save(JSONObject object) const {
    cDefOutline.save(object, JSON_OUTLINE);
    cDefPrimary.save(object, JSON_PRIMARY);
    cDefSecondary.save(object, JSON_SECONDARY);
  }

  void C64LiftGraphics::hintInUse(bool inUse) {
    for (const std::pair<const std::string, std::unique_ptr<LiteralTexture>>& mTexture : cTextures) {
      mTexture.second->hintTextureInUse(inUse);
    }
  }
  
  bool C64LiftGraphics::renderIcon() {
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH,       1.0f, 0.0f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_YAW - 90.0f, 0.0f, 0.0f, 1.0f);
    glScalef(1.3f, 1.3f, 1.3f);
    cTextures[DIAMOND_HALF]->set();
    glEnable(GL_ALPHA_TEST);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(-1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f( 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( 1.0f,  1.0f);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    return true;
  }

  void C64LiftGraphics::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyAsset<Colour>(metadata.getPropertyData("Primary"),   cDefPrimary);
    owner.createPropertyAsset<Colour>(metadata.getPropertyData("Secondary"), cDefSecondary);
    owner.createPropertyAsset<Colour>(metadata.getPropertyData("Outline"),   cDefOutline);
  }
  
  void C64LiftGraphics::generateTextures() {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();

    cTextures[CIRCLE_HALF]->setRenderTarget();  renderLiftCircleHalf();
    cTextures[CIRCLE_NONE]->setRenderTarget();  renderLiftCircle();
    cTextures[CIRCLE_BOTH]->setRenderTarget();  renderLiftCircleBoth();
    cTextures[SQUARE_HALF]->setRenderTarget();  renderLiftSquareHalf();
    cTextures[SQUARE_NONE]->setRenderTarget();  renderLiftSquare();
    cTextures[SQUARE_BOTH]->setRenderTarget();  renderLiftSquareBoth();
    cTextures[DIAMOND_HALF]->setRenderTarget(); renderLiftDiamondHalf();
    cTextures[DIAMOND_NONE]->setRenderTarget(); renderLiftDiamond();
    cTextures[DIAMOND_BOTH]->setRenderTarget(); renderLiftDiamondBoth();

    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    IApplication& mApplication = cProject.getApplication();
    mApplication.setViewPort();
  }

  void C64LiftGraphics::clear() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void C64LiftGraphics::renderCircle(float radius, Colour& colour) {
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

  void C64LiftGraphics::renderCircularRing(float outer, float inner, Colour& colour) {
    glBegin(GL_TRIANGLE_STRIP);
    colour->set();
    float mStartAngle = 0.0f * (M_PI / 180.0f);
    float mEndAngle = 360.0f * (M_PI / 180.0f);
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
      glVertex2f(std::sin(angle) * inner, std::cos(angle) * inner);
      glVertex2f(std::sin(angle) * outer, std::cos(angle) * outer);
    }
    glEnd();
  }

  void C64LiftGraphics::renderSquareRing(float outer, float inner, Colour& colour) {
    colour->set();
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
    colour->set();
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
    colour->set();
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
    colour->set();
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
    colour->set();
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
    cDefSecondary->set();
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
    cDefOutline->set();
    glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_COLOUR_INNER);
    glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_TRIANGLE_INNER);
    glVertex2f(-SQUARE_TRIANGLE_INNER,  SQUARE_COLOUR_INNER);
    cDefPrimary->set();
    glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_COLOUR_INNER);
    glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_TRIANGLE_OUTER);
    glVertex2f(-SQUARE_TRIANGLE_OUTER,  SQUARE_COLOUR_INNER);
    glEnd();
  }

  void C64LiftGraphics::renderLiftSquareBoth() {
    renderLiftSquareHalf();
    glBegin(GL_TRIANGLES);
    cDefOutline->set();
    glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_COLOUR_INNER);
    glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_TRIANGLE_INNER);
    glVertex2f(SQUARE_TRIANGLE_INNER, -SQUARE_COLOUR_INNER);
    cDefPrimary->set();
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

  void C64LiftGraphics::setNeedsRedrawing() {
    if (!cNeedsRedrawing) {
      cProject.updateLater([this]() {
        generateTextures();
        cNeedsRedrawing = false;
      });
    }
  }
}

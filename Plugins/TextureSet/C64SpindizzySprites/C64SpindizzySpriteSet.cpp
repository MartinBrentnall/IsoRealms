/*
 * Copyright 2009 Martin Brentnall
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
#include "C64SpindizzySpriteSet.h"

const float C64SpindizzySpriteSet::CIRCLE_RESOLUTION            = 5.0f * (M_PI / 180.0);
const float C64SpindizzySpriteSet::CIRCLE_OUTLINE_OUTER         = 1.0f;
const float C64SpindizzySpriteSet::CIRCLE_OUTLINE_INNER         = 0.6f;
const float C64SpindizzySpriteSet::CIRCLE_COLOUR_OUTER          = 0.9f;
const float C64SpindizzySpriteSet::CIRCLE_COLOUR_INNER          = 0.7f;
const float C64SpindizzySpriteSet::SQUARE_OUTLINE_OUTER         = 1.0f;
const float C64SpindizzySpriteSet::SQUARE_OUTLINE_INNER         = 0.51f;
const float C64SpindizzySpriteSet::SQUARE_COLOUR_OUTER          = 0.87f;
const float C64SpindizzySpriteSet::SQUARE_COLOUR_INNER          = 0.64f;
const float C64SpindizzySpriteSet::SQUARE_TRIANGLE_OUTER        = 0.2f;
const float C64SpindizzySpriteSet::SQUARE_TRIANGLE_INNER        = 0.0f;
const float C64SpindizzySpriteSet::DIAMOND_OUTLINE_OUTER        = 1.0f;
const float C64SpindizzySpriteSet::DIAMOND_OUTLINE_INNER        = 0.6f;
const float C64SpindizzySpriteSet::DIAMOND_COLOUR_OUTER         = 0.9f;
const float C64SpindizzySpriteSet::DIAMOND_COLOUR_INNER         = 0.7f;
const float C64SpindizzySpriteSet::DIAMOND_SQUARE_OUTLINE_OUTER = 0.70f;
const float C64SpindizzySpriteSet::DIAMOND_SQUARE_OUTLINE_INNER = 0.40f;
const float C64SpindizzySpriteSet::DIAMOND_SQUARE_COLOUR_OUTER  = 0.62f;
const float C64SpindizzySpriteSet::DIAMOND_SQUARE_COLOUR_INNER  = 0.48f;


const std::string C64SpindizzySpriteSet::LIFT_CIRCLE_BOTH  = "LiftCircleFilled";
const std::string C64SpindizzySpriteSet::LIFT_CIRCLE_ONE   = "LiftCircleOne";
const std::string C64SpindizzySpriteSet::LIFT_CIRCLE_NONE  = "LiftCircleEmpty";
const std::string C64SpindizzySpriteSet::LIFT_SQUARE_BOTH  = "LiftSquareFilled";
const std::string C64SpindizzySpriteSet::LIFT_SQUARE_ONE   = "LiftSquareOne";
const std::string C64SpindizzySpriteSet::LIFT_SQUARE_NONE  = "LiftSquareEmpty";
const std::string C64SpindizzySpriteSet::LIFT_DIAMOND_BOTH = "LiftDiamondFilled";
const std::string C64SpindizzySpriteSet::LIFT_DIAMOND_ONE  = "LiftDiamondOne";
const std::string C64SpindizzySpriteSet::LIFT_DIAMOND_NONE = "LiftDiamondEmpty";

C64SpindizzySpriteSet::C64SpindizzySpriteSet(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  cColour1 = new Colour(1.0f, 1.0f, 1.0f);
  cColour2 = new Colour(0.7f, 0.7f, 0.7f);
  cColour3 = new Colour(0.3f, 0.3f, 0.3f);
  cOutlineColour = new Colour(0.0f, 0.0f, 0.0f);

  TRANSPARENT = new Colour(0.0, 0.0, 0.0, 0.0);

  cTextures[LIFT_CIRCLE_ONE]   = new Texture();
  cTextures[LIFT_CIRCLE_NONE]  = new Texture();
  cTextures[LIFT_CIRCLE_BOTH]  = new Texture();
  cTextures[LIFT_SQUARE_ONE]   = new Texture();
  cTextures[LIFT_SQUARE_NONE]  = new Texture();
  cTextures[LIFT_SQUARE_BOTH]  = new Texture();
  cTextures[LIFT_DIAMOND_ONE]  = new Texture();
  cTextures[LIFT_DIAMOND_NONE] = new Texture();
  cTextures[LIFT_DIAMOND_BOTH] = new Texture();
  
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    cRuntimeContext->add(i->second, i->first);
  }
  generateTextures();
}

/* Generation functions */
void C64SpindizzySpriteSet::clear(IColour* colour) {
  glClearColor(colour->getRed(), colour->getGreen(), colour->getBlue(), colour->getAlpha());
  glClear(GL_COLOR_BUFFER_BIT);
}

void C64SpindizzySpriteSet::renderCircle(float radius, IColour* colour) {
  glBegin(GL_TRIANGLE_FAN);
  colour->set();
  glVertex2f(0.0f, 0.0f);
  float mStartAngle = 0.0f * (M_PI / 180.0f);
  float mEndAngle = 360.0f * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * radius, cos(angle) * radius);
  }
  glEnd();
}

void C64SpindizzySpriteSet::renderCircle(float outerRadius, float innerRadius, IColour* colour) {
  glBegin(GL_TRIANGLE_STRIP);
  colour->set();
  float mStartAngle = 0.0f * (M_PI / 180.0f);
  float mEndAngle = 360.0f * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * innerRadius, cos(angle) * innerRadius);
    glVertex2f(sin(angle) * outerRadius, cos(angle) * outerRadius);
  }
  glEnd();
}

void C64SpindizzySpriteSet::renderSquare(float outer, float inner, IColour* colour) {
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

void C64SpindizzySpriteSet::renderSquareHalf(float outer, float inner, IColour* colour) {
  colour->set();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex2f(-outer, -outer);
  glVertex2f(-inner, -inner);
  glVertex2f(-outer,  outer);
  glVertex2f(-inner,  inner);
  glVertex2f( outer,  outer);
  glVertex2f( inner,  inner);
/*  glVertex2f( outer, -outer);
  glVertex2f( inner, -inner);
  glVertex2f(-outer, -outer);
  glVertex2f(-inner, -inner);*/
  glEnd();
}

void C64SpindizzySpriteSet::renderDiamond(float outer, float inner, IColour* colour) {
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

void C64SpindizzySpriteSet::renderDiamondHalf(float outer, float inner, IColour* colour) {
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

void C64SpindizzySpriteSet::renderDiamondEdges(float outer, float edge, IColour* colour) {
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

void C64SpindizzySpriteSet::renderLiftSquare() {
  clear(TRANSPARENT);
  renderSquare(SQUARE_OUTLINE_OUTER, SQUARE_OUTLINE_INNER, cOutlineColour);
  renderSquare(SQUARE_COLOUR_OUTER, SQUARE_COLOUR_INNER, cColour2);
}

void C64SpindizzySpriteSet::renderLiftSquareHalf() {
  renderLiftSquare();
  glBegin(GL_TRIANGLES);
  cOutlineColour->set();
  glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_COLOUR_INNER);
  glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_TRIANGLE_INNER);
  glVertex2f(-SQUARE_TRIANGLE_INNER,  SQUARE_COLOUR_INNER);
  cColour1->set();
  glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_COLOUR_INNER);
  glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_TRIANGLE_OUTER);
  glVertex2f(-SQUARE_TRIANGLE_OUTER,  SQUARE_COLOUR_INNER);
  glEnd();
}

void C64SpindizzySpriteSet::renderLiftSquareBoth() {
  renderLiftSquareHalf();
  glBegin(GL_TRIANGLES);
  cOutlineColour->set();
  glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_COLOUR_INNER);
  glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_TRIANGLE_INNER);
  glVertex2f(SQUARE_TRIANGLE_INNER, -SQUARE_COLOUR_INNER);
  cColour1->set();
  glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_COLOUR_INNER);
  glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_TRIANGLE_OUTER);
  glVertex2f(SQUARE_TRIANGLE_OUTER, -SQUARE_COLOUR_INNER);
  glEnd();
}

void C64SpindizzySpriteSet::renderLiftDiamondBoth() {
  clear(TRANSPARENT);
  renderSquare(DIAMOND_SQUARE_OUTLINE_OUTER, DIAMOND_SQUARE_OUTLINE_INNER, cOutlineColour);
  renderSquare(DIAMOND_SQUARE_COLOUR_OUTER,  DIAMOND_SQUARE_COLOUR_INNER,  cColour1);
  renderDiamond(DIAMOND_OUTLINE_OUTER,       DIAMOND_OUTLINE_INNER,        cOutlineColour);
  renderDiamond(DIAMOND_COLOUR_OUTER,        DIAMOND_COLOUR_INNER,         cColour2);
}

void C64SpindizzySpriteSet::renderLiftDiamondHalf() {
  renderLiftDiamond();
  renderDiamondHalf(DIAMOND_OUTLINE_OUTER,       DIAMOND_OUTLINE_INNER,        cOutlineColour);
  renderDiamondHalf(DIAMOND_COLOUR_OUTER,        DIAMOND_COLOUR_INNER,         cColour2);
  renderSquareHalf(DIAMOND_SQUARE_OUTLINE_OUTER, DIAMOND_SQUARE_OUTLINE_INNER, cOutlineColour);
  renderSquareHalf(DIAMOND_SQUARE_COLOUR_OUTER,  DIAMOND_SQUARE_COLOUR_INNER,  cColour1);
}

void C64SpindizzySpriteSet::renderLiftDiamond() {
  clear(TRANSPARENT);
  renderDiamondEdges(DIAMOND_OUTLINE_OUTER,  DIAMOND_SQUARE_COLOUR_OUTER,  cOutlineColour);
  renderDiamondEdges(DIAMOND_COLOUR_OUTER,   DIAMOND_SQUARE_COLOUR_OUTER,  cColour2);
  renderSquare(DIAMOND_SQUARE_OUTLINE_OUTER, DIAMOND_SQUARE_OUTLINE_INNER, cOutlineColour);
  renderSquare(DIAMOND_SQUARE_COLOUR_OUTER,  DIAMOND_SQUARE_COLOUR_INNER,  cColour1);
}

void C64SpindizzySpriteSet::renderLiftCircle() {
  clear(TRANSPARENT);
  renderCircle(CIRCLE_OUTLINE_OUTER, CIRCLE_OUTLINE_INNER, cOutlineColour);
  renderCircle(CIRCLE_COLOUR_OUTER, CIRCLE_COLOUR_INNER, cColour1);
}

void C64SpindizzySpriteSet::renderLiftCircleHalf() {
  renderLiftCircle();
  glBegin(GL_TRIANGLE_FAN);
  cColour2->set();
  glVertex2f(0.0f, 0.0f);
  float mStartAngle = 45.0f * (M_PI / 180.0f);
  float mEndAngle = 225.0f * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * CIRCLE_COLOUR_INNER, cos(angle) * CIRCLE_COLOUR_INNER);
  }
  glEnd();
}

void C64SpindizzySpriteSet::renderLiftCircleBoth() {
  clear(TRANSPARENT);
  renderCircle(CIRCLE_OUTLINE_OUTER, cOutlineColour);
  renderCircle(CIRCLE_COLOUR_OUTER, cColour1);
  renderCircle(CIRCLE_COLOUR_INNER, cColour2);
}

void C64SpindizzySpriteSet::generateTextures() {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

  cTextures[LIFT_CIRCLE_ONE]->setRenderTarget();   renderLiftCircleHalf();
  cTextures[LIFT_CIRCLE_NONE]->setRenderTarget();  renderLiftCircle();
  cTextures[LIFT_CIRCLE_BOTH]->setRenderTarget();  renderLiftCircleBoth();
  cTextures[LIFT_SQUARE_ONE]->setRenderTarget();   renderLiftSquareHalf();
  cTextures[LIFT_SQUARE_NONE]->setRenderTarget();  renderLiftSquare();
  cTextures[LIFT_SQUARE_BOTH]->setRenderTarget();  renderLiftSquareBoth();
  cTextures[LIFT_DIAMOND_ONE]->setRenderTarget();  renderLiftDiamondHalf();
  cTextures[LIFT_DIAMOND_NONE]->setRenderTarget(); renderLiftDiamond();
  cTextures[LIFT_DIAMOND_BOTH]->setRenderTarget(); renderLiftDiamondBoth();

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  glViewport(0, 0, 1024, 768);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}

void C64SpindizzySpriteSet::destroyTextures() {
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    delete i->second;
  }
  for (std::map<std::string, GLuint>::iterator i = cTextureIDs.begin(); i != cTextureIDs.end(); i++) {
    glDeleteTextures(1, &(i->second));
  }
  cTextureIDs.clear();
}

void C64SpindizzySpriteSet::paletteChanged(IPalette*, const std::string&) {
  generateTextures();
}

ITexture* C64SpindizzySpriteSet::getTexture(const std::string& name) {
  std::map<std::string, Texture*>::iterator i = cTextures.find(name);
  return i != cTextures.end() ? i->second : cTextures.begin()->second;
}

void C64SpindizzySpriteSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Colour1") {
      cColour1 = cRuntimeContext->getColour(mNode);
    } else if (mValueAsString == "Colour2") {
      cColour2 = cRuntimeContext->getColour(mNode);
    } else if (mValueAsString == "Colour3") {
      cColour3 = cRuntimeContext->getColour(mNode);
    } else if (mValueAsString == "Outline") {
      cOutlineColour = cRuntimeContext->getColour(mNode);
    } else {
      // TODO: Throw something!
    }
  }
  generateTextures();
}

void C64SpindizzySpriteSet::save(DOMNodeWriter* node) {
  cColour1->save(node, "Colour1");
  cColour2->save(node, "Colour2");
  cColour3->save(node, "Colour3");
  cOutlineColour->save(node, "Outline");
}

C64SpindizzySpriteSet::~C64SpindizzySpriteSet() {
  delete TRANSPARENT;
  destroyTextures();
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new C64SpindizzySpriteSet(runtimeContext);
}

extern "C" void destroy(IPlugin* textureSet) {
  delete textureSet;
}

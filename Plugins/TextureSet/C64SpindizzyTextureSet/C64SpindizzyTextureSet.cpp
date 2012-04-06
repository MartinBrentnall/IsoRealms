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
#include "C64SpindizzyTextureSet.h"

const float C64SpindizzyTextureSet::TILE_SIZE                    = 0.88f;
const float C64SpindizzyTextureSet::SWITCH_SQUARE_OUTER          = 0.63f;
const float C64SpindizzyTextureSet::SWITCH_SQUARE_INNER          = 0.44f;
const float C64SpindizzyTextureSet::SWITCH_SQUARE_TRIANGLE_INNER = 0.0f;

const float C64SpindizzyTextureSet::SWITCH_DIAMOND_OUTER         = 0.66f;
const float C64SpindizzyTextureSet::SWITCH_DIAMOND_INNER         = 0.41f;

const float C64SpindizzyTextureSet::SWITCH_CIRCLE_OUTER          = 0.7f;
const float C64SpindizzyTextureSet::SWITCH_CIRCLE_INNER          = 0.52f;
const float C64SpindizzyTextureSet::ICE_EDGE_WIDTH               = 0.9f;
const float C64SpindizzyTextureSet::ARROW_SIZE                   = 0.66f;
const float C64SpindizzyTextureSet::ARROW_LINE_WIDTH             = 0.25f;
const float C64SpindizzyTextureSet::CIRCLE_RESOLUTION            = 5.0f * (M_PI / 180.0);

const std::string C64SpindizzyTextureSet::PLAIN                = "Plain";
const std::string C64SpindizzyTextureSet::PLAIN_SPLIT          = "PlainSplit";
const std::string C64SpindizzyTextureSet::ICE_WATER            = "IceWater";
const std::string C64SpindizzyTextureSet::ARROW                = "Arrow";
const std::string C64SpindizzyTextureSet::SWITCH_CIRCLE_BOTH   = "SwitchCircleFilled";
const std::string C64SpindizzyTextureSet::SWITCH_CIRCLE_ONE    = "SwitchCircleOne";
const std::string C64SpindizzyTextureSet::SWITCH_CIRCLE_NONE   = "SwitchCircleEmpty";
const std::string C64SpindizzyTextureSet::SWITCH_SQUARE_BOTH   = "SwitchSquareFilled";
const std::string C64SpindizzyTextureSet::SWITCH_SQUARE_ONE    = "SwitchSquareOne";
const std::string C64SpindizzyTextureSet::SWITCH_SQUARE_NONE   = "SwitchSquareEmpty";
const std::string C64SpindizzyTextureSet::SWITCH_DIAMOND_BOTH  = "SwitchDiamondFilled";
const std::string C64SpindizzyTextureSet::SWITCH_DIAMOND_ONE   = "SwitchDiamondOne";
const std::string C64SpindizzyTextureSet::SWITCH_DIAMOND_NONE  = "SwitchDiamondEmpty";
const std::string C64SpindizzyTextureSet::TRAMPOLINE           = "Trampoline";
const std::string C64SpindizzyTextureSet::WALL_ICE             = "WallIce";
const std::string C64SpindizzyTextureSet::WALL_MIXED_CAP       = "WallMixedCap";
const std::string C64SpindizzyTextureSet::WALL_MIXED_MIDDLE    = "WallMixedMiddle";
const std::string C64SpindizzyTextureSet::WALL_PLAIN_CAP       = "WallPlainCap";
const std::string C64SpindizzyTextureSet::WALL_PLAIN_MIDDLE    = "WallPlainMiddle";

C64SpindizzyTextureSet::C64SpindizzyTextureSet(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  cFloorColour = new Colour(1.0f, 1.0f, 1.0f);
  cWallColour = new Colour(0.7f, 0.7f, 0.7f);
  cGridColour = new Colour(0.3f, 0.3f, 0.3f);
  cBackgroundColour = new Colour(0.0f, 0.0f, 0.0f);
  
  cTextures[SWITCH_CIRCLE_BOTH]  = new Texture();
  cTextures[SWITCH_CIRCLE_ONE]   = new Texture();
  cTextures[SWITCH_CIRCLE_NONE]  = new Texture();
  cTextures[SWITCH_SQUARE_BOTH]  = new Texture();
  cTextures[SWITCH_SQUARE_ONE]   = new Texture();
  cTextures[SWITCH_SQUARE_NONE]  = new Texture();
  cTextures[SWITCH_DIAMOND_NONE] = new Texture();
  cTextures[SWITCH_DIAMOND_ONE]  = new Texture();
  cTextures[SWITCH_DIAMOND_BOTH] = new Texture();
  cTextures[ARROW]               = new Texture();
  cTextures[TRAMPOLINE]          = new Texture();
  cTextures[ICE_WATER]           = new Texture();
  cTextures[PLAIN]               = new Texture();
  cTextures[PLAIN_SPLIT]         = new Texture();
  cTextures[WALL_MIXED_CAP]      = new Texture(true);
  cTextures[WALL_MIXED_MIDDLE]   = new Texture();
  cTextures[WALL_PLAIN_CAP]      = new Texture(true);
  cTextures[WALL_PLAIN_MIDDLE]   = new Texture();
  cTextures[WALL_ICE]            = new Texture();
  
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    cRuntimeContext->add(i->second, i->first);
  }
  
  cBackgroundTexture = new PlainColourTexture();

  generateTextures();
}

float C64SpindizzyTextureSet::getGridWallLuminanceAdjustment() {
  float mFloorLuminance = cFloorColour->luminance();
  float mWallLuminance = cWallColour->luminance();
  float mAdjustment = mWallLuminance / mFloorLuminance;
  return max(0.7f, min(mAdjustment, 1.3f)) - 1.0f;
}

void C64SpindizzyTextureSet::clear(IColour* colour) {
  glClearColor(colour->getRed(), colour->getGreen(), colour->getBlue(), colour->getAlpha());
  glClear(GL_COLOR_BUFFER_BIT);
}

void C64SpindizzyTextureSet::renderSquare(float size, IColour* colour) {
  glBegin(GL_QUADS);
  colour->set();
  glVertex2f(-size, -size);
  glVertex2f( size, -size);
  glVertex2f( size,  size);
  glVertex2f(-size,  size);
  glEnd();
}

void C64SpindizzyTextureSet::renderDiamond(float size, IColour* colour) {
  glBegin(GL_QUADS);
  colour->set();
  glVertex2f(-size,  0.0f);
  glVertex2f( 0.0f, -size);
  glVertex2f( size,  0.0f);
  glVertex2f( 0.0f,  size);
  glEnd();
}

void C64SpindizzyTextureSet::renderCircle(float radius, IColour* colour) {
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

void C64SpindizzyTextureSet::renderRectangle(float xs, float ys, float xe, float ye, IColour* colour) {
  colour->set();
  glBegin(GL_QUADS);
  glVertex2f(xe, ys);
  glVertex2f(xe, ye);
  glVertex2f(xs, ye);
  glVertex2f(xs, ys);
  glEnd();
}

void C64SpindizzyTextureSet::renderTile(IColour* colour) {
  clear(cGridColour);
  renderSquare(TILE_SIZE, colour);
}

void C64SpindizzyTextureSet::renderIce() {
  return clear(cGridColour);
}

void C64SpindizzyTextureSet::renderPlain() {
  renderTile(cFloorColour);
}

void C64SpindizzyTextureSet::renderSplitPlain() {
  clear(cGridColour);
  glBegin(GL_TRIANGLES);
  float mInnerTileSize = TILE_SIZE - (1.0f - TILE_SIZE);
  glVertex2f(-TILE_SIZE,      -TILE_SIZE);
  glVertex2f( mInnerTileSize, -TILE_SIZE);
  glVertex2f(-TILE_SIZE,       mInnerTileSize);
  glVertex2f( TILE_SIZE,       TILE_SIZE);
  glVertex2f(-mInnerTileSize,  TILE_SIZE);
  glVertex2f( TILE_SIZE,      -mInnerTileSize);
  glEnd();
}

void C64SpindizzyTextureSet::renderTrampoline() {
  renderTile(cBackgroundColour);
  renderSquare(TILE_SIZE - (1.0f - TILE_SIZE), cWallColour);
}

void C64SpindizzyTextureSet::renderSwitchSquare() {
  renderTile(cFloorColour);
  renderSquare(SWITCH_SQUARE_OUTER, cWallColour);
  renderSquare(SWITCH_SQUARE_INNER, cFloorColour);
}

void C64SpindizzyTextureSet::renderSwitchSquareHalf() {
  renderSwitchSquare();
  glBegin(GL_TRIANGLES);
  cGridColour->set();
  glVertex2f(SWITCH_SQUARE_INNER,          -SWITCH_SQUARE_INNER);
  glVertex2f(SWITCH_SQUARE_INNER,          -SWITCH_SQUARE_TRIANGLE_INNER);
  glVertex2f(SWITCH_SQUARE_TRIANGLE_INNER, -SWITCH_SQUARE_INNER);
  glEnd();
}

void C64SpindizzyTextureSet::renderSwitchSquareBoth() {
  renderSwitchSquareHalf();
  glBegin(GL_TRIANGLES);
  cGridColour->set();
  glVertex2f(-SWITCH_SQUARE_INNER,          SWITCH_SQUARE_INNER);
  glVertex2f(-SWITCH_SQUARE_INNER,          SWITCH_SQUARE_TRIANGLE_INNER);
  glVertex2f(-SWITCH_SQUARE_TRIANGLE_INNER, SWITCH_SQUARE_INNER);
  glEnd();
}

void C64SpindizzyTextureSet::renderSwitchDiamond() {
  renderTile(cFloorColour);
  renderSquare(SWITCH_DIAMOND_OUTER, cWallColour);
  renderSquare(SWITCH_DIAMOND_INNER, cFloorColour);
  renderDiamond(SWITCH_DIAMOND_OUTER, cGridColour);
  renderDiamond(SWITCH_DIAMOND_INNER, cFloorColour);
}

void C64SpindizzyTextureSet::renderSwitchDiamondHalf() {
  renderSwitchDiamond();
  renderRectangle( SWITCH_DIAMOND_INNER, -SWITCH_DIAMOND_OUTER, SWITCH_DIAMOND_OUTER,  SWITCH_DIAMOND_OUTER, cWallColour);
  renderRectangle(-SWITCH_DIAMOND_OUTER, -SWITCH_DIAMOND_OUTER,  SWITCH_DIAMOND_OUTER, -SWITCH_DIAMOND_INNER, cWallColour);
}

void C64SpindizzyTextureSet::renderSwitchDiamondBoth() {
  renderSwitchDiamondHalf();
  renderRectangle(-SWITCH_DIAMOND_OUTER, -SWITCH_DIAMOND_OUTER, -SWITCH_DIAMOND_INNER,  SWITCH_DIAMOND_OUTER, cWallColour);
  renderRectangle(-SWITCH_DIAMOND_OUTER,  SWITCH_DIAMOND_INNER, SWITCH_DIAMOND_OUTER,  SWITCH_DIAMOND_OUTER, cWallColour);
}

void C64SpindizzyTextureSet::renderSwitchCircle() {
  renderTile(cFloorColour);
  renderCircle(SWITCH_CIRCLE_OUTER, cGridColour);
  renderCircle(SWITCH_CIRCLE_INNER, cFloorColour);
}

void C64SpindizzyTextureSet::renderSwitchCircleBoth() {
  renderTile(cFloorColour);
  renderCircle(SWITCH_CIRCLE_OUTER, cGridColour);
  renderCircle(SWITCH_CIRCLE_INNER, cWallColour);
}

void C64SpindizzyTextureSet::renderSwitchCircleHalf() {
  renderSwitchCircle();
  glBegin(GL_TRIANGLE_FAN);
  cWallColour->set();
  glVertex2f(0.0f, 0.0f);
  float mStartAngle = 45.0f * (M_PI / 180.0f);
  float mEndAngle = 225.0f * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * SWITCH_CIRCLE_INNER, cos(angle) * SWITCH_CIRCLE_INNER);
  }
  glEnd();
}

void C64SpindizzyTextureSet::renderArrow() {
  renderTile(cFloorColour);
  renderRectangle(-ARROW_LINE_WIDTH, -ARROW_SIZE, ARROW_LINE_WIDTH, 0.0f, cWallColour);
  glBegin(GL_TRIANGLES);
  cWallColour->set();
  glVertex2f(-ARROW_SIZE, 0.0f);
  glVertex2f( ARROW_SIZE, 0.0f);
  glVertex2f( 0.0f,       ARROW_SIZE);
  glEnd();
}

void C64SpindizzyTextureSet::renderWallPlainMiddle() {
  Colour mGridColour(cGridColour, 1.0f + getGridWallLuminanceAdjustment());
  clear(&mGridColour);
  renderRectangle(-TILE_SIZE, -1.0f, TILE_SIZE, 1.0f, cWallColour);
}

void C64SpindizzyTextureSet::renderWallMixedMiddle() {
  Colour mGridColour(cGridColour, 1.0f + getGridWallLuminanceAdjustment() / 2.0f);
  Colour mWallFloorMix(cFloorColour, cWallColour);
  clear(&mGridColour);
  renderRectangle(-TILE_SIZE, -1.0f, TILE_SIZE, 1.0f, &mWallFloorMix);
}

void C64SpindizzyTextureSet::renderWallPlainCap() {
  Colour mGridColour(cGridColour, 1.0f + getGridWallLuminanceAdjustment());
  Colour mWallFloorMix(cFloorColour, cWallColour);
  clear(&mGridColour);
  renderRectangle(-TILE_SIZE, -TILE_SIZE / 2.0f, TILE_SIZE, 1.0f, cWallColour);
}

void C64SpindizzyTextureSet::renderWallMixedCap() {
  Colour mGridColour(cGridColour, 1.0f + getGridWallLuminanceAdjustment() / 2.0f);
  Colour mWallFloorMix(cFloorColour, cWallColour);
  clear(&mGridColour);
  renderRectangle(-TILE_SIZE, -TILE_SIZE / 2.0f, TILE_SIZE, 1.0f, &mWallFloorMix);
}

void C64SpindizzyTextureSet::renderIceWall() {
  clear(cGridColour);
  glBegin(GL_QUADS);
  cBackgroundColour->set();
  glVertex2f(-1.0f,           1.0f);
  glVertex2f(-ICE_EDGE_WIDTH, ICE_EDGE_WIDTH - (1.0f - ICE_EDGE_WIDTH));
  glVertex2f( ICE_EDGE_WIDTH, ICE_EDGE_WIDTH - (1.0f - ICE_EDGE_WIDTH));
  glVertex2f( 1.0f,           1.0f);
  glEnd();
}

void C64SpindizzyTextureSet::generateTextures() {
//   GLuint mFramebufferObject;
//   glGenFramebuffersEXT(1, &mFramebufferObject);
//   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFramebufferObject);

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

  cTextures[SWITCH_CIRCLE_BOTH]->setRenderTarget();  renderSwitchCircleBoth();
  cTextures[SWITCH_CIRCLE_ONE]->setRenderTarget();   renderSwitchCircleHalf();
  cTextures[SWITCH_CIRCLE_NONE]->setRenderTarget();  renderSwitchCircle();
  cTextures[SWITCH_SQUARE_BOTH]->setRenderTarget();  renderSwitchSquareBoth();
  cTextures[SWITCH_SQUARE_ONE]->setRenderTarget();   renderSwitchSquareHalf();
  cTextures[SWITCH_SQUARE_NONE]->setRenderTarget();  renderSwitchSquare();
  cTextures[SWITCH_DIAMOND_BOTH]->setRenderTarget(); renderSwitchDiamondBoth();
  cTextures[SWITCH_DIAMOND_ONE]->setRenderTarget();  renderSwitchDiamondHalf();
  cTextures[SWITCH_DIAMOND_NONE]->setRenderTarget(); renderSwitchDiamond();
  cTextures[ARROW]->setRenderTarget();               renderArrow();
  cTextures[TRAMPOLINE]->setRenderTarget();          renderTrampoline();
  cTextures[ICE_WATER]->setRenderTarget();           renderIce();
  cTextures[PLAIN]->setRenderTarget();               renderPlain();
  cTextures[PLAIN_SPLIT]->setRenderTarget();         renderSplitPlain();
  cTextures[WALL_MIXED_CAP]->setRenderTarget();      renderWallMixedCap();
  cTextures[WALL_MIXED_MIDDLE]->setRenderTarget();   renderWallMixedMiddle();
  cTextures[WALL_PLAIN_CAP]->setRenderTarget();      renderWallPlainCap();
  cTextures[WALL_PLAIN_MIDDLE]->setRenderTarget();   renderWallPlainMiddle();
  cTextures[WALL_ICE]->setRenderTarget();            renderIceWall();

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  cBackgroundTexture->setTexture(cBackgroundColour);
  glViewport(0, 0, 1024, 768);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}

void C64SpindizzyTextureSet::destroyTextures() {
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    delete i->second;
  }
  delete cBackgroundTexture;
  for (std::map<std::string, GLuint>::iterator i = cTextureIDs.begin(); i != cTextureIDs.end(); i++) {
    glDeleteTextures(1, &(i->second));
  }
  cTextureIDs.clear();
}

void C64SpindizzyTextureSet::paletteChanged(IPalette*, const std::string&) {
//  if (colour == cBackgroundColour || colour == cFloorColour || colour == cGridColour || colour == cWallColour) {
    generateTextures();
//  }
}

C64SpindizzyTextureSet::~C64SpindizzyTextureSet() {
  destroyTextures();
}

void C64SpindizzyTextureSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Floor") {
      cFloorColour = cRuntimeContext->getColour(mNode);
    } else if (mValueAsString == "Wall") {
      cWallColour = cRuntimeContext->getColour(mNode);
    } else if (mValueAsString == "Grid") {
      cGridColour = cRuntimeContext->getColour(mNode);
    } else if (mValueAsString == "Extra") {
      cBackgroundColour = cRuntimeContext->getColour(mNode);
    } else {
      // TODO: Throw something!
    }
  }
  generateTextures();
}

void C64SpindizzyTextureSet::save(DOMNodeWriter* node) {
  cFloorColour->save(node, "Floor");
  cWallColour->save(node, "Wall");
  cGridColour->save(node, "Grid");
  cBackgroundColour->save(node, "Extra");
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new C64SpindizzyTextureSet(runtimeContext);
}

extern "C" void destroy(IPlugin* textureSet) {
  delete textureSet;
}

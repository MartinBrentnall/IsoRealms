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
#include "ResourceTexturesSpindizzyBlocksC64.h"

const float ResourceTexturesSpindizzyBlocksC64::TILE_SIZE                    = 0.88f;
const float ResourceTexturesSpindizzyBlocksC64::SWITCH_SQUARE_OUTER          = 0.63f;
const float ResourceTexturesSpindizzyBlocksC64::SWITCH_SQUARE_INNER          = 0.44f;
const float ResourceTexturesSpindizzyBlocksC64::SWITCH_SQUARE_TRIANGLE_INNER = 0.0f;

const float ResourceTexturesSpindizzyBlocksC64::SWITCH_DIAMOND_OUTER         = 0.66f;
const float ResourceTexturesSpindizzyBlocksC64::SWITCH_DIAMOND_INNER         = 0.41f;

const float ResourceTexturesSpindizzyBlocksC64::SWITCH_CIRCLE_OUTER          = 0.7f;
const float ResourceTexturesSpindizzyBlocksC64::SWITCH_CIRCLE_INNER          = 0.52f;
const float ResourceTexturesSpindizzyBlocksC64::ICE_EDGE_WIDTH               = 0.1f;
const float ResourceTexturesSpindizzyBlocksC64::ARROW_SIZE                   = 0.66f;
const float ResourceTexturesSpindizzyBlocksC64::ARROW_LINE_WIDTH             = 0.25f;
const float ResourceTexturesSpindizzyBlocksC64::CIRCLE_RESOLUTION            = 5.0f * (M_PI / 180.0);

const std::string ResourceTexturesSpindizzyBlocksC64::PLAIN                = "Plain";
const std::string ResourceTexturesSpindizzyBlocksC64::PLAIN_SPLIT          = "PlainSplit";
const std::string ResourceTexturesSpindizzyBlocksC64::ICE_WATER            = "IceWater";
const std::string ResourceTexturesSpindizzyBlocksC64::ARROW                = "Arrow";
const std::string ResourceTexturesSpindizzyBlocksC64::SWITCH_CIRCLE_BOTH   = "SwitchCircleFilled";
const std::string ResourceTexturesSpindizzyBlocksC64::SWITCH_CIRCLE_ONE    = "SwitchCircleOne";
const std::string ResourceTexturesSpindizzyBlocksC64::SWITCH_CIRCLE_NONE   = "SwitchCircleEmpty";
const std::string ResourceTexturesSpindizzyBlocksC64::SWITCH_SQUARE_BOTH   = "SwitchSquareFilled";
const std::string ResourceTexturesSpindizzyBlocksC64::SWITCH_SQUARE_ONE    = "SwitchSquareOne";
const std::string ResourceTexturesSpindizzyBlocksC64::SWITCH_SQUARE_NONE   = "SwitchSquareEmpty";
const std::string ResourceTexturesSpindizzyBlocksC64::SWITCH_DIAMOND_BOTH  = "SwitchDiamondFilled";
const std::string ResourceTexturesSpindizzyBlocksC64::SWITCH_DIAMOND_ONE   = "SwitchDiamondOne";
const std::string ResourceTexturesSpindizzyBlocksC64::SWITCH_DIAMOND_NONE  = "SwitchDiamondEmpty";
const std::string ResourceTexturesSpindizzyBlocksC64::TRAMPOLINE           = "Trampoline";
const std::string ResourceTexturesSpindizzyBlocksC64::WALL_ICE             = "WallIce";
const std::string ResourceTexturesSpindizzyBlocksC64::WALL_MIXED_CAP       = "WallMixedCap";
const std::string ResourceTexturesSpindizzyBlocksC64::WALL_MIXED_MIDDLE    = "WallMixedMiddle";
const std::string ResourceTexturesSpindizzyBlocksC64::WALL_PLAIN_CAP       = "WallPlainCap";
const std::string ResourceTexturesSpindizzyBlocksC64::WALL_PLAIN_MIDDLE    = "WallPlainMiddle";

ResourceTexturesSpindizzyBlocksC64::ResourceTexturesSpindizzyBlocksC64(ICameraSupplier* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  initialise();
  generateTextures();
  module->addCameraAngleChangeListener(this);
}

std::string ResourceTexturesSpindizzyBlocksC64::stripMemberName(const std::string& name) {
  return name.substr(0, name.rfind('_'));
}

void ResourceTexturesSpindizzyBlocksC64::initialise() {
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
}

float ResourceTexturesSpindizzyBlocksC64::getGridWallLuminanceAdjustment() {
  float mFloorLuminance = cFloorColour->luminance();
  float mWallLuminance = cWallColour->luminance();
  float mAdjustment = mWallLuminance / mFloorLuminance;
  return max(0.7f, min(mAdjustment, 1.3f)) - 1.0f;
}

void ResourceTexturesSpindizzyBlocksC64::clear(IColour* colour) {
  glClearColor(colour->getRed(), colour->getGreen(), colour->getBlue(), colour->getAlpha());
  glClear(GL_COLOR_BUFFER_BIT);
}

void ResourceTexturesSpindizzyBlocksC64::renderSquare(float size, IColour* colour) {
  glBegin(GL_QUADS);
  colour->set();
  glVertex2f(-size, -size);
  glVertex2f( size, -size);
  glVertex2f( size,  size);
  glVertex2f(-size,  size);
  glEnd();
}

void ResourceTexturesSpindizzyBlocksC64::renderDiamond(float size, IColour* colour) {
  glBegin(GL_QUADS);
  colour->set();
  glVertex2f(-size,  0.0f);
  glVertex2f( 0.0f, -size);
  glVertex2f( size,  0.0f);
  glVertex2f( 0.0f,  size);
  glEnd();
}

void ResourceTexturesSpindizzyBlocksC64::renderCircle(float radius, IColour* colour) {
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

void ResourceTexturesSpindizzyBlocksC64::renderRectangle(float xs, float ys, float xe, float ye, IColour* colour) {
  colour->set();
  glBegin(GL_QUADS);
  glVertex2f(xe, ys);
  glVertex2f(xe, ye);
  glVertex2f(xs, ye);
  glVertex2f(xs, ys);
  glEnd();
}

void ResourceTexturesSpindizzyBlocksC64::renderTile(IColour* colour) {
  clear(cGridColour);
  renderSquare(TILE_SIZE, colour);
}

void ResourceTexturesSpindizzyBlocksC64::renderIce() {
  return clear(cGridColour);
}

void ResourceTexturesSpindizzyBlocksC64::renderPlain() {
  renderTile(cFloorColour);
}

void ResourceTexturesSpindizzyBlocksC64::renderSplitPlain() {
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

void ResourceTexturesSpindizzyBlocksC64::renderTrampoline() {
  renderTile(cBackgroundColour);
  renderSquare(TILE_SIZE - (1.0f - TILE_SIZE), cWallColour);
}

void ResourceTexturesSpindizzyBlocksC64::renderSwitchSquare(float angle) {
  renderTile(cFloorColour);
  glRotatef(angle, 0.0f, 0.0f, 1.0f);
  renderSquare(SWITCH_SQUARE_OUTER, cWallColour);
  renderSquare(SWITCH_SQUARE_INNER, cFloorColour);
}

void ResourceTexturesSpindizzyBlocksC64::renderSwitchSquareHalf(float angle) {
  renderSwitchSquare(angle);
  glBegin(GL_TRIANGLES);
  cGridColour->set();
  glVertex2f(SWITCH_SQUARE_INNER,          -SWITCH_SQUARE_INNER);
  glVertex2f(SWITCH_SQUARE_INNER,          -SWITCH_SQUARE_TRIANGLE_INNER);
  glVertex2f(SWITCH_SQUARE_TRIANGLE_INNER, -SWITCH_SQUARE_INNER);
  glEnd();
  glRotatef(-angle, 0.0f, 0.0f, 1.0f);
}

void ResourceTexturesSpindizzyBlocksC64::renderSwitchSquareBoth(float angle) {
  renderSwitchSquareHalf(angle);
  glRotatef(angle, 0.0f, 0.0f, 1.0f);
  glBegin(GL_TRIANGLES);
  cGridColour->set();
  glVertex2f(-SWITCH_SQUARE_INNER,          SWITCH_SQUARE_INNER);
  glVertex2f(-SWITCH_SQUARE_INNER,          SWITCH_SQUARE_TRIANGLE_INNER);
  glVertex2f(-SWITCH_SQUARE_TRIANGLE_INNER, SWITCH_SQUARE_INNER);
  glEnd();
  glRotatef(-angle, 0.0f, 0.0f, 1.0f);
}

void ResourceTexturesSpindizzyBlocksC64::renderSwitchDiamond(float angle) {
  renderTile(cFloorColour);
  glRotatef(angle, 0.0f, 0.0f, 1.0f);
  renderSquare(SWITCH_DIAMOND_OUTER, cWallColour);
  renderSquare(SWITCH_DIAMOND_INNER, cFloorColour);
  renderDiamond(SWITCH_DIAMOND_OUTER, cGridColour);
  renderDiamond(SWITCH_DIAMOND_INNER, cFloorColour);
}

void ResourceTexturesSpindizzyBlocksC64::renderSwitchDiamondHalf(float angle) {
  renderSwitchDiamond(angle);
  renderRectangle( SWITCH_DIAMOND_INNER, -SWITCH_DIAMOND_OUTER, SWITCH_DIAMOND_OUTER,  SWITCH_DIAMOND_OUTER, cWallColour);
  renderRectangle(-SWITCH_DIAMOND_OUTER, -SWITCH_DIAMOND_OUTER,  SWITCH_DIAMOND_OUTER, -SWITCH_DIAMOND_INNER, cWallColour);
  glRotatef(-angle, 0.0f, 0.0f, 1.0f);
}

void ResourceTexturesSpindizzyBlocksC64::renderSwitchDiamondBoth() {
  renderSwitchDiamondHalf(0.0f);
  renderRectangle(-SWITCH_DIAMOND_OUTER, -SWITCH_DIAMOND_OUTER, -SWITCH_DIAMOND_INNER,  SWITCH_DIAMOND_OUTER, cWallColour);
  renderRectangle(-SWITCH_DIAMOND_OUTER,  SWITCH_DIAMOND_INNER, SWITCH_DIAMOND_OUTER,  SWITCH_DIAMOND_OUTER, cWallColour);
}

void ResourceTexturesSpindizzyBlocksC64::renderSwitchCircle(float angle) {
  renderTile(cFloorColour);
  glRotatef(angle, 0.0f, 0.0f, 1.0f);
  renderCircle(SWITCH_CIRCLE_OUTER, cGridColour);
  renderCircle(SWITCH_CIRCLE_INNER, cFloorColour);
}

void ResourceTexturesSpindizzyBlocksC64::renderSwitchCircleBoth() {
  renderTile(cFloorColour);
  renderCircle(SWITCH_CIRCLE_OUTER, cGridColour);
  renderCircle(SWITCH_CIRCLE_INNER, cWallColour);
}

void ResourceTexturesSpindizzyBlocksC64::renderSwitchCircleHalf(float angle) {
  renderSwitchCircle(angle);
  glBegin(GL_TRIANGLE_FAN);
  cWallColour->set();
  glVertex2f(0.0f, 0.0f);
  float mStartAngle = 45.0f * (M_PI / 180.0f);
  float mEndAngle = 225.0f * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * SWITCH_CIRCLE_INNER, cos(angle) * SWITCH_CIRCLE_INNER);
  }
  glEnd();
  glRotatef(-angle, 0.0f, 0.0f, 1.0f);
}

void ResourceTexturesSpindizzyBlocksC64::renderArrow() {
  renderTile(cFloorColour);
  renderRectangle(-ARROW_LINE_WIDTH, 0.0f, ARROW_LINE_WIDTH, ARROW_SIZE, cWallColour);
  glBegin(GL_TRIANGLES);
  cWallColour->set();
  glVertex2f( ARROW_SIZE,  0.0f);
  glVertex2f(-ARROW_SIZE,  0.0f);
  glVertex2f( 0.0f,       -ARROW_SIZE);
  glEnd();
}

void ResourceTexturesSpindizzyBlocksC64::renderWallMiddle(float interpolation) {
  Colour mGridColour(cGridColour, 1.0f + getGridWallLuminanceAdjustment() / (interpolation + 1.0f));
  Colour mWallFloorMix(cWallColour, cFloorColour, interpolation * 0.5f);
  clear(&mGridColour);
  renderRectangle(-TILE_SIZE, -1.0f, TILE_SIZE, 1.0f, &mWallFloorMix);
}

void ResourceTexturesSpindizzyBlocksC64::renderWallCap(float interpolation) {
  Colour mGridColour(cGridColour, 1.0f + getGridWallLuminanceAdjustment() / (interpolation + 1.0f));
  Colour mWallFloorMix(cWallColour, cFloorColour, interpolation * 0.5f);
  clear(&mGridColour);
  renderRectangle(-TILE_SIZE, -TILE_SIZE / 2.0f, TILE_SIZE, 1.0f, &mWallFloorMix);
}

void ResourceTexturesSpindizzyBlocksC64::renderIceWall() {
  clear(cGridColour);
  glBegin(GL_QUADS);
  cBackgroundColour->set();
  glVertex2f( 1.0f,                  -1.0f);
  glVertex2f( 1.0f - ICE_EDGE_WIDTH, -1.0f + ICE_EDGE_WIDTH * 2.0f);
  glVertex2f(-1.0f + ICE_EDGE_WIDTH, -1.0f + ICE_EDGE_WIDTH * 2.0f);
  glVertex2f(-1.0f,                  -1.0f);
  glEnd();
}

void ResourceTexturesSpindizzyBlocksC64::generateAngledTextures(float angle) {
  if (angle > 180.0f) {
    angle -= 360.0f;
  }
  cTextures[SWITCH_CIRCLE_ONE]->setRenderTarget();   renderSwitchCircleHalf(angle);
  cTextures[SWITCH_SQUARE_ONE]->setRenderTarget();   renderSwitchSquareHalf(angle);
  cTextures[SWITCH_SQUARE_BOTH]->setRenderTarget();  renderSwitchSquareBoth(angle);
  cTextures[SWITCH_DIAMOND_ONE]->setRenderTarget();  renderSwitchDiamondHalf(angle);
  float mInterpolation = std::abs(angle / 90.0f);
  if (mInterpolation > 1.0f) {
    mInterpolation -= ((mInterpolation - 1.0f) * 2.0f);
  }
  cTextures[WALL_MIXED_CAP]->setRenderTarget();      renderWallCap(std::abs(mInterpolation - 1.0f));
  cTextures[WALL_MIXED_MIDDLE]->setRenderTarget();   renderWallMiddle(std::abs(mInterpolation - 1.0f));
  cTextures[WALL_PLAIN_CAP]->setRenderTarget();      renderWallCap(mInterpolation);
  cTextures[WALL_PLAIN_MIDDLE]->setRenderTarget();   renderWallMiddle(mInterpolation);
}

void ResourceTexturesSpindizzyBlocksC64::generateTextures() {
//   GLuint mFramebufferObject;
//   glGenFramebuffersEXT(1, &mFramebufferObject);
//   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFramebufferObject);

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

  generateAngledTextures(0.0f);
  
  cTextures[SWITCH_CIRCLE_BOTH]->setRenderTarget();  renderSwitchCircleBoth();
  cTextures[SWITCH_CIRCLE_NONE]->setRenderTarget();  renderSwitchCircle(0.0f);
  cTextures[SWITCH_SQUARE_NONE]->setRenderTarget();  renderSwitchSquare(0.0f);
  cTextures[SWITCH_DIAMOND_BOTH]->setRenderTarget(); renderSwitchDiamondBoth();
  cTextures[SWITCH_DIAMOND_NONE]->setRenderTarget(); renderSwitchDiamond(0.0f);
  cTextures[ARROW]->setRenderTarget();               renderArrow();
  cTextures[TRAMPOLINE]->setRenderTarget();          renderTrampoline();
  cTextures[ICE_WATER]->setRenderTarget();           renderIce();
  cTextures[PLAIN]->setRenderTarget();               renderPlain();
  cTextures[PLAIN_SPLIT]->setRenderTarget();         renderSplitPlain();
  cTextures[WALL_ICE]->setRenderTarget();            renderIceWall();

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->setViewPort();
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void ResourceTexturesSpindizzyBlocksC64::destroyTextures() {
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    delete i->second;
  }
  for (std::map<std::string, GLuint>::iterator i = cTextureIDs.begin(); i != cTextureIDs.end(); i++) {
    glDeleteTextures(1, &(i->second));
  }
  cTextureIDs.clear();
}

void ResourceTexturesSpindizzyBlocksC64::resourceChanged(IColour* colour) {
  if (colour == cBackgroundColour || colour == cFloorColour || colour == cGridColour || colour == cWallColour) {
    generateTextures();
  }
}

void ResourceTexturesSpindizzyBlocksC64::resourcePendingDestruction(IColour* destroyee, IColour* replacement) {
  bool mChanged = false;
  if (destroyee == cFloorColour)      {cFloorColour = replacement;      mChanged = true;}
  if (destroyee == cWallColour)       {cWallColour = replacement;       mChanged = true;}
  if (destroyee == cGridColour)       {cGridColour = replacement;       mChanged = true;}
  if (destroyee == cBackgroundColour) {cBackgroundColour = replacement; mChanged = true;}
  if (mChanged) {
    generateTextures();
  }
}

bool ResourceTexturesSpindizzyBlocksC64::contains(ITexture* texture) {
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    if (texture == i->second) {
      return true;
    }
  }
  return false;
}

void ResourceTexturesSpindizzyBlocksC64::deleteResources(IResourceInstanceListener<ITexture>* listener) {
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    listener->resourcePendingDestruction(i->second);
  }
}

void ResourceTexturesSpindizzyBlocksC64::addTextures(IResourceRegistry* runtimeContext, const std::string& name) {
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    runtimeContext->add(i->second, name + "_" + i->first);
  }
}

ResourceTexturesSpindizzyBlocksC64::~ResourceTexturesSpindizzyBlocksC64() {
  destroyTextures();
}

void ResourceTexturesSpindizzyBlocksC64::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resourceAccessor) {
  cFloorColour = resourceAccessor->getColour(node->getAttribute("floor"));
  cWallColour = resourceAccessor->getColour(node->getAttribute("wall"));
  cGridColour = resourceAccessor->getColour(node->getAttribute("grid"));
  cBackgroundColour = resourceAccessor->getColour(node->getAttribute("extra"));
  generateTextures();
}

  
IColour* ResourceTexturesSpindizzyBlocksC64::getFloorColour() {
  return cFloorColour;
}

IColour* ResourceTexturesSpindizzyBlocksC64::getWallColour() {
  return cWallColour;
}

IColour* ResourceTexturesSpindizzyBlocksC64::getGridColour() {
  return cGridColour;
}

IColour* ResourceTexturesSpindizzyBlocksC64::getDetailColour() {
  return cBackgroundColour;
}

void ResourceTexturesSpindizzyBlocksC64::setFloorColour(IColour* colour) {
  cFloorColour = colour;
  generateTextures();
}

void ResourceTexturesSpindizzyBlocksC64::setWallColour(IColour* colour) {
  cWallColour = colour;
  generateTextures();
}

void ResourceTexturesSpindizzyBlocksC64::setGridColour(IColour* colour) {
  cGridColour = colour;
  generateTextures();
}

void ResourceTexturesSpindizzyBlocksC64::setDetailColour(IColour* colour) {
  cBackgroundColour = colour;
  generateTextures();
}

std::vector<ITexture*> ResourceTexturesSpindizzyBlocksC64::getResources() {
  std::vector<ITexture*> mTextures;
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    mTextures.push_back(i->second);
  }
  return mTextures;
}

std::vector<std::string> ResourceTexturesSpindizzyBlocksC64::getResourceNames() {
  std::vector<std::string> mNames;
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    mNames.push_back(i->first);
  }
  return mNames;
}

ITexture* ResourceTexturesSpindizzyBlocksC64::getResource(const std::string& name) {
  return cTextures[name];
}

void ResourceTexturesSpindizzyBlocksC64::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator) {
  std::string mResourceName = resourceLocator->getPath(cTextures[PLAIN]);
  mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
  mResourceName = mResourceName.substr(0, mResourceName.find_last_of('_'));
  node->addAttribute("name", mResourceName);
  node->addAttribute("floor", resourceLocator->getPath(cFloorColour));
  node->addAttribute("wall", resourceLocator->getPath(cWallColour));
  node->addAttribute("grid", resourceLocator->getPath(cGridColour));
  node->addAttribute("extra", resourceLocator->getPath(cBackgroundColour));
}

void ResourceTexturesSpindizzyBlocksC64::cameraAngleChanged(float angle) {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

  generateAngledTextures(angle + 45.0f);

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();  

  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->setViewPort();
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  
}

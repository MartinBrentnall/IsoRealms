/*
 * Copyright 2016 Martin Brentnall
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
#include "WallPatternCap.h"

WallPatternCap::WallPatternCap(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mTopPath    = node->getAttribute("top");
  std::string mMiddlePath = node->getAttribute("middle");
  std::string mBottomPath = node->getAttribute("bottom");
  cTextureTop    = resourceAccessor->getTexture(mTopPath);
  cTexture       = resourceAccessor->getTexture(mMiddlePath);
  cTextureBottom = resourceAccessor->getTexture(mBottomPath);
  cFlipBottom    = node->getBooleanAttribute("bottomFlip");
}

std::vector<IVisualElement*> WallPatternCap::getStaticVisuals(IWallSurface* wallSurface) {
  std::vector<IVisualElement*> mVisuals;
  mVisuals.push_back(new SectionBottom(this, wallSurface));
  mVisuals.push_back(new SectionMiddle(this, wallSurface));
  mVisuals.push_back(new SectionTop(   this, wallSurface));
  return mVisuals;
}

void WallPatternCap::renderTop(int x, int y, int z, int length, int height, int topSlope, IWallSurface::FaceDirection facing) {
  float mBlockRadius = IsoRealmsConstants::BLOCK_RADIUS;
  float mEdgeWidth = IsoRealmsConstants::BLOCK_HEIGHT * 0.5f;
  double mFromX = x + (facing == IWallSurface::EAST ? mBlockRadius : -mBlockRadius);
  double mFromY = y + (facing == IWallSurface::NORTH ? mBlockRadius : -mBlockRadius); 
  double mToX = (facing == IWallSurface::WEST  || facing == IWallSurface::EAST)  ? (mFromX) : x - mBlockRadius + length;
  double mToY = (facing == IWallSurface::SOUTH || facing == IWallSurface::NORTH) ? (mFromY) : y - mBlockRadius + length;
  double mHighStartSlopeZ = (z + height) * IsoRealmsConstants::BLOCK_HEIGHT;
  double mHighEndSlopeZ =  ((z + height) + topSlope * length) * IsoRealmsConstants::BLOCK_HEIGHT;

  glBegin(GL_QUADS);
  if (facing == IWallSurface::EAST || facing == IWallSurface::SOUTH) {
    glTexCoord2f(x,          1.0f); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
    glTexCoord2f(x + length, 1.0f); glVertex3f(mToX,   mToY,   mHighEndSlopeZ   - mEdgeWidth);
    glTexCoord2f(x + length, 0.0f); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
    glTexCoord2f(x,          0.0f); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
  } else {
    glTexCoord2f(x + length, 0.0f); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
    glTexCoord2f(x,          0.0f); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
    glTexCoord2f(x,          1.0f); glVertex3f(mToX,   mToY,   mHighEndSlopeZ   - mEdgeWidth);
    glTexCoord2f(x + length, 1.0f); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
  }
  glEnd();
}

void WallPatternCap::renderMiddle(int x, int y, int z, int length, int height, int topSlope, IWallSurface::FaceDirection facing) {
  float mBlockRadius = IsoRealmsConstants::BLOCK_RADIUS;
  float mEdgeWidth = IsoRealmsConstants::BLOCK_HEIGHT * 0.5f;
  double mFromX = x + (facing == IWallSurface::EAST ? mBlockRadius : -mBlockRadius);
  double mFromY = y + (facing == IWallSurface::NORTH ? mBlockRadius : -mBlockRadius); 
  double mFromZ = z * IsoRealmsConstants::BLOCK_HEIGHT;
  double mToX = (facing == IWallSurface::WEST  || facing == IWallSurface::EAST)  ? (mFromX) : x - mBlockRadius + length;
  double mToY = (facing == IWallSurface::SOUTH || facing == IWallSurface::NORTH) ? (mFromY) : y - mBlockRadius + length;
  double mHighStartSlopeZ = (z + height) * IsoRealmsConstants::BLOCK_HEIGHT;
  double mHighEndSlopeZ =  ((z + height) + topSlope * length) * IsoRealmsConstants::BLOCK_HEIGHT;
  double mHighStartSlopeTexture = z + height;
  double mHighEndSlopeTexture = (z + height) + topSlope * length;

  glBegin(GL_QUADS);
  if (facing == IWallSurface::EAST || facing == IWallSurface::SOUTH) {
    if (mHighEndSlopeZ - mFromZ > 0.0f || mHighStartSlopeZ - mFromZ > 0.0f) {
      glTexCoord2f(x,          z);                          glVertex3f(mFromX, mFromY, mFromZ           + mEdgeWidth);
      glTexCoord2f(x + length, z);                          glVertex3f(mToX,   mToY,   mFromZ           + mEdgeWidth);
      glTexCoord2f(x + length, z - mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mHighEndSlopeZ   - mEdgeWidth);
      glTexCoord2f(x,          z - mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
    }
  } else {
    if (mHighEndSlopeZ - mFromZ > 0.0f || mHighStartSlopeZ - mFromZ > 0.0f) {
      glTexCoord2f(x + length, z - mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
      glTexCoord2f(x,          z - mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mHighEndSlopeZ   - mEdgeWidth);
      glTexCoord2f(x,          z);                          glVertex3f(mToX,   mToY,   mFromZ           + mEdgeWidth);
      glTexCoord2f(x + length, z);                          glVertex3f(mFromX, mFromY, mFromZ           + mEdgeWidth);
    }
  }
  glEnd();
}

void WallPatternCap::renderBottom(int x, int y, int z, int length, int height, int topSlope, IWallSurface::FaceDirection facing) {
  float mBlockRadius = IsoRealmsConstants::BLOCK_RADIUS;
  float mEdgeWidth = IsoRealmsConstants::BLOCK_HEIGHT * 0.5f;
  double mFromX = x + (facing == IWallSurface::EAST ? mBlockRadius : -mBlockRadius);
  double mFromY = y + (facing == IWallSurface::NORTH ? mBlockRadius : -mBlockRadius); 
  double mFromZ = z * IsoRealmsConstants::BLOCK_HEIGHT;
  double mToX = (facing == IWallSurface::WEST  || facing == IWallSurface::EAST)  ? (mFromX) : x - mBlockRadius + length;
  double mToY = (facing == IWallSurface::SOUTH || facing == IWallSurface::NORTH) ? (mFromY) : y - mBlockRadius + length;

  glBegin(GL_QUADS);
  if (facing == IWallSurface::EAST || facing == IWallSurface::SOUTH) {
    glTexCoord2f(x,          0.0f); glVertex3f(mFromX, mFromY, mFromZ);
    glTexCoord2f(x + length, 0.0f); glVertex3f(mToX,   mToY,   mFromZ);
    glTexCoord2f(x + length, 1.0f); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
    glTexCoord2f(x,          1.0f); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
  } else {
    glTexCoord2f(x + length, 1.0f); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
    glTexCoord2f(x,          1.0f); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
    glTexCoord2f(x,          0.0f); glVertex3f(mToX,   mToY,   mFromZ);
    glTexCoord2f(x + length, 0.0f); glVertex3f(mFromX, mFromY, mFromZ);
  }
  glEnd();
}

void WallPatternCap::render(IWallSurface* surface) {
  int mX                                     = surface->getX();
  int mY                                     = surface->getY();
  int mZ                                     = surface->getZ();
  int mLength                                = surface->getLength();
  int mHeight                                = surface->getHeight();
  int mTopSlope                              = surface->getTopSlope();
  IWallSurface::FaceDirection mFaceDirection = surface->getFaceDirection();
  cTextureBottom->set();
  renderBottom(mX, mY, mZ, mLength, mHeight, mTopSlope, mFaceDirection);
  cTexture->set();
  renderMiddle(mX, mY, mZ, mLength, mHeight, mTopSlope, mFaceDirection);
  cTextureTop->set();
  renderTop(   mX, mY, mZ, mLength, mHeight, mTopSlope, mFaceDirection);
}

bool WallPatternCap::contains(ITexture* texture) {
  return cTexture       == texture
      || cTextureBottom == texture
      || cTextureTop    == texture;
}

bool WallPatternCap::resourcePendingDestruction(ITexture* destroyee, ITexture* replacement) {
  bool mTexturesReplaced = false;
  mTexturesReplaced |= Utils::replaceTexture(cTexture,       destroyee, replacement);
  mTexturesReplaced |= Utils::replaceTexture(cTextureBottom, destroyee, replacement);
  mTexturesReplaced |= Utils::replaceTexture(cTextureTop,    destroyee, replacement);
  return mTexturesReplaced;
}

void WallPatternCap::save(DOMNodeWriter* node, IResourceLocator* resources) {
}

WallPatternCap::SectionBottom::SectionBottom(WallPatternCap* parent, IWallSurface* surface) {
  cParent = parent;
  cWallSurface = surface;
}

void WallPatternCap::SectionBottom::render() {
  int mX                                     = cWallSurface->getX();
  int mY                                     = cWallSurface->getY();
  int mZ                                     = cWallSurface->getZ();
  int mLength                                = cWallSurface->getLength();
  int mHeight                                = cWallSurface->getHeight();
  int mTopSlope                              = cWallSurface->getTopSlope();
  IWallSurface::FaceDirection mFaceDirection = cWallSurface->getFaceDirection();
  cParent->renderBottom(mX, mY, mZ, mLength, mHeight, mTopSlope, mFaceDirection);
}

ITexture* WallPatternCap::SectionBottom::getTexture() {
  return cParent->cTextureBottom->getTexture();
}

void WallPatternCap::SectionBottom::prepareVisual() {
  // Nothing to do
}

WallPatternCap::SectionMiddle::SectionMiddle(WallPatternCap* parent, IWallSurface* surface) {
  cParent = parent;
  cWallSurface = surface;
}

void WallPatternCap::SectionMiddle::render() {
  int mX                                     = cWallSurface->getX();
  int mY                                     = cWallSurface->getY();
  int mZ                                     = cWallSurface->getZ();
  int mLength                                = cWallSurface->getLength();
  int mHeight                                = cWallSurface->getHeight();
  int mTopSlope                              = cWallSurface->getTopSlope();
  IWallSurface::FaceDirection mFaceDirection = cWallSurface->getFaceDirection();
  cParent->renderMiddle(mX, mY, mZ, mLength, mHeight, mTopSlope, mFaceDirection);
}

ITexture* WallPatternCap::SectionMiddle::getTexture() {
  return cParent->cTexture->getTexture();
}

void WallPatternCap::SectionMiddle::prepareVisual() {
  // Nothing to do
}

WallPatternCap::SectionTop::SectionTop(WallPatternCap* parent, IWallSurface* surface) {
  cParent = parent;
  cWallSurface = surface;
}

void WallPatternCap::SectionTop::render() {
  int mX                                     = cWallSurface->getX();
  int mY                                     = cWallSurface->getY();
  int mZ                                     = cWallSurface->getZ();
  int mLength                                = cWallSurface->getLength();
  int mHeight                                = cWallSurface->getHeight();
  int mTopSlope                              = cWallSurface->getTopSlope();
  IWallSurface::FaceDirection mFaceDirection = cWallSurface->getFaceDirection();
  cParent->renderTop(mX, mY, mZ, mLength, mHeight, mTopSlope, mFaceDirection);
}

ITexture* WallPatternCap::SectionTop::getTexture() {
  return cParent->cTextureTop->getTexture();
}

void WallPatternCap::SectionTop::prepareVisual() {
  // Nothing to do
}

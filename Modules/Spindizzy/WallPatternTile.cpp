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

#include "WallPatternTile.h"

WallPatternTile::WallPatternTile(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mTexturePath = node->getAttribute("texture");
  cTexture = resourceAccessor->getTexture(mTexturePath);
}

std::vector<IVisualElement*> WallPatternTile::getStaticVisuals(IWallSurface* wallSurface) {
  std::vector<IVisualElement*> mVisuals;
  mVisuals.push_back(new WallPatternTileSurface(this, wallSurface));
  return mVisuals;
}

void WallPatternTile::render(IWallSurface* surface) {
  int mX                                     = surface->getX();
  int mY                                     = surface->getY();
  int mZ                                     = surface->getZ();
  int mLength                                = surface->getLength();
  int mHeight                                = surface->getHeight();
  int mTopSlope                              = surface->getTopSlope();
  IWallSurface::FaceDirection mFaceDirection = surface->getFaceDirection();
  render(mX, mY, mZ, mLength, mHeight, mTopSlope, mFaceDirection);
}

void WallPatternTile::render(int x, int y, int z, int length, int height, int topSlope, IWallSurface::FaceDirection facing) {
  float mBlockRadius = IsoRealmsConstants::BLOCK_RADIUS;
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
    glTexCoord2f(x,          mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mFromZ);
    glTexCoord2f(x + length, mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mFromZ);
    glTexCoord2f(x + length, z);                      glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
    glTexCoord2f(x,          z);                      glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
  } else {
    glTexCoord2f(x,          z);                      glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
    glTexCoord2f(x + length, z);                      glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
    glTexCoord2f(x + length, mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mFromZ);
    glTexCoord2f(x,          mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mFromZ);
  }
  glEnd();
}

bool WallPatternTile::contains(ITexture* texture) {
  return cTexture == texture;
}

bool WallPatternTile::resourcePendingDestruction(ITexture* destroyee, ITexture* replacement) {
  bool mTexturesReplaced = false;
  mTexturesReplaced |= Utils::replaceTexture(cTexture, destroyee, replacement);
  return mTexturesReplaced;
}

void WallPatternTile::save(DOMNodeWriter* node, IResourceLocator* resources) {
}

WallPatternTile::WallPatternTileSurface::WallPatternTileSurface(WallPatternTile* parent, IWallSurface* surface) {
  cParent = parent;
  cWallSurface = surface;
}

void WallPatternTile::WallPatternTileSurface::render() {
  int mX                                     = cWallSurface->getX();
  int mY                                     = cWallSurface->getY();
  int mZ                                     = cWallSurface->getZ();
  int mLength                                = cWallSurface->getLength();
  int mHeight                                = cWallSurface->getHeight();
  int mTopSlope                              = cWallSurface->getTopSlope();
  IWallSurface::FaceDirection mFaceDirection = cWallSurface->getFaceDirection();
  cParent->render(mX, mY, mZ, mLength, mHeight, mTopSlope, mFaceDirection);
}

ITexture* WallPatternTile::WallPatternTileSurface::getTexture() {
  return cParent->cTexture->getTexture();
}

void WallPatternTile::WallPatternTileSurface::prepareVisual() {
  // Nothing to do
}


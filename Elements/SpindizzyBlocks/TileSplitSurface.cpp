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
#include "TileSplitSurface.h"

TileSplitSurface::TileSplitSurface(bool splitDirection, BlockLocation& location, ISpindizzyTextureSet** textureSet, ISpindizzyTextureSet::TextureType textureType, int nw, int ne, int se, int sw) {
  cTextureSet = textureSet;
  cTextureType = textureType;
  cLocation = location;
  cCornerHeights[0][1] = nw;
  cCornerHeights[1][1] = ne;
  cCornerHeights[1][0] = se;
  cCornerHeights[0][0] = sw;
  cSplitDirection = splitDirection;
}

int TileSplitSurface::getSurfaceCellHeight(int x, int y) {
  return cLocation.z;
}

int TileSplitSurface::getSurfaceCellElevation(int x, int y) {
  int mHighestA = cCornerHeights[0][0] > cCornerHeights[0][1] ? cCornerHeights[0][0] : cCornerHeights[0][1];
  int mHighestB = cCornerHeights[1][0] > cCornerHeights[1][1] ? cCornerHeights[1][0] : cCornerHeights[1][1];
  return mHighestA > mHighestB ? mHighestA : mHighestB;
}

void TileSplitSurface::render() {
  // TODO: Change local variable names here
  float mNorthWest = cCornerHeights[0][1] * IsoRealmsConstants::BLOCK_HEIGHT;
  float mNorthEast = cCornerHeights[1][1] * IsoRealmsConstants::BLOCK_HEIGHT;
  float mSouthEast = cCornerHeights[1][0] * IsoRealmsConstants::BLOCK_HEIGHT;
  float mSouthWest = cCornerHeights[0][0] * IsoRealmsConstants::BLOCK_HEIGHT;
  float mZ = cLocation.z * IsoRealmsConstants::BLOCK_HEIGHT;
  int mX = cLocation.x;
  int mY = cLocation.y;

  ISpindizzyTexture* mTexture = (*cTextureSet)->getTexture(cTextureType);
  mTexture->set();
  glBegin(GL_TRIANGLES);
  if (cSplitDirection) {
    mTexture->texCoord2f(mX + 1.0f, mY + 1.0f); glVertex3f(-IsoRealmsConstants::BLOCK_RADIUS + mX,  IsoRealmsConstants::BLOCK_RADIUS + mY, mNorthWest + mZ);
    mTexture->texCoord2f(mX,        mY + 1.0f); glVertex3f(-IsoRealmsConstants::BLOCK_RADIUS + mX, -IsoRealmsConstants::BLOCK_RADIUS + mY, mSouthWest + mZ);
    mTexture->texCoord2f(mX + 1.0f, mY       ); glVertex3f( IsoRealmsConstants::BLOCK_RADIUS + mX,  IsoRealmsConstants::BLOCK_RADIUS + mY, mNorthEast + mZ);

    mTexture->texCoord2f(mX,        mY + 1.0f); glVertex3f(-IsoRealmsConstants::BLOCK_RADIUS + mX, -IsoRealmsConstants::BLOCK_RADIUS + mY, mSouthWest + mZ);
    mTexture->texCoord2f(mX,        mY       ); glVertex3f( IsoRealmsConstants::BLOCK_RADIUS + mX, -IsoRealmsConstants::BLOCK_RADIUS + mY, mSouthEast + mZ);
    mTexture->texCoord2f(mX + 1.0f, mY       ); glVertex3f( IsoRealmsConstants::BLOCK_RADIUS + mX,  IsoRealmsConstants::BLOCK_RADIUS + mY, mNorthEast + mZ);
  } else {
    mTexture->texCoord2f(mX,        mY + 1.0f); glVertex3f(-IsoRealmsConstants::BLOCK_RADIUS + mX,  IsoRealmsConstants::BLOCK_RADIUS + mY, mNorthWest + mZ);
    mTexture->texCoord2f(mX + 1.0f, mY       ); glVertex3f( IsoRealmsConstants::BLOCK_RADIUS + mX, -IsoRealmsConstants::BLOCK_RADIUS + mY, mSouthEast + mZ);
    mTexture->texCoord2f(mX + 1.0f, mY + 1.0f); glVertex3f( IsoRealmsConstants::BLOCK_RADIUS + mX,  IsoRealmsConstants::BLOCK_RADIUS + mY, mNorthEast + mZ);

    mTexture->texCoord2f(mX,        mY + 1.0f); glVertex3f(-IsoRealmsConstants::BLOCK_RADIUS + mX,  IsoRealmsConstants::BLOCK_RADIUS + mY, mNorthWest + mZ);
    mTexture->texCoord2f(mX,        mY       ); glVertex3f(-IsoRealmsConstants::BLOCK_RADIUS + mX, -IsoRealmsConstants::BLOCK_RADIUS + mY, mSouthWest + mZ);
    mTexture->texCoord2f(mX + 1.0f, mY       ); glVertex3f( IsoRealmsConstants::BLOCK_RADIUS + mX, -IsoRealmsConstants::BLOCK_RADIUS + mY, mSouthEast + mZ);
  }
  glEnd();
}

BlockArea* TileSplitSurface::getCoverage() {
  return new BlockArea(cLocation, cLocation);
}

bool TileSplitSurface::alligned(int x, int y) {
  return cLocation.x == x && cLocation.y == y;
}

bool TileSplitSurface::contains(Vertex& location) {
  // TODO: Implement
  return false;
}

float TileSplitSurface::getHeightAt(float x, float y) {
  // TODO: Implement
  return 0.0;
}

ICollisionData* TileSplitSurface::getCollision(Vertex& start, Vertex& end) {
  return NULL;
}

ICollisionData* TileSplitSurface::getRollingEvent(Vertex& start, Vertex& end) {
  return NULL;
}


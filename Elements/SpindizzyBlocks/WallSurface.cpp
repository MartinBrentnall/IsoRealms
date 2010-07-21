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
#include "WallSurface.h"

WallSurface::WallSurface(int x, int y, int z, int length, int height, int topSlope, FaceDirection facing) {
  cX = x;
  cY = y;
  cZ = z;
  cLength = length;
  if (topSlope < 0) {
    cHeight = height - cLength * topSlope;
  } else {
    cHeight = height;
  }
  cFacing = facing;
  cTopSlope = topSlope;
  cCondition = NULL;
}

WallSurface::WallSurface(int x, int y, int z, int length, int height, int topSlope, FaceDirection facing, ISpindizzyTextureSet** textureSet, ISpindizzyTextureSet::TextureType textureType, Condition* condition) {
  cX = x;
  cY = y;
  cZ = z;
  cLength = length;
  if (topSlope < 0) {
    cHeight = height - cLength * topSlope;
  } else {
    cHeight = height;
  }
  cFacing = facing;
  cTextureSet = textureSet;
  cTextureType = textureType;
  cTopSlope = topSlope;
  cCondition = condition;
}

IWallEdge* WallSurface::getTopEdge(int location) {
  location -= cFacing == IWallSurface::NORTH || cFacing == IWallSurface::SOUTH ? cX : cY;
  int mStartLocation = location * cTopSlope;
  int mEndLocation = (location + 1) * cTopSlope;
  return new WallEdge(cZ + cHeight + mStartLocation, cZ + cHeight + mEndLocation); // TODO: Sloped walls
}

IWallEdge* WallSurface::getBottomEdge(int location) {
  return new WallEdge(cZ, cZ); // TODO: Sloped walls
}

BlockArea* WallSurface::getCoverage() {
  BlockLocation mStartLocation(cX, cY, cZ);
  bool mFacingPole = cFacing == IWallSurface::NORTH || cFacing == IWallSurface::SOUTH;
  // TODO: TEST THIS!  Height may be wrong.
  int mTopHeight = cHeight + cLength * cTopSlope;
  BlockLocation mEndLocation(mFacingPole ? cX + (cLength - 1) : cX, mFacingPole ? cY : cY + (cLength - 1), mTopHeight); 
  return new BlockArea(mStartLocation, mEndLocation);
}

void WallSurface::render() {
  if (cCondition == NULL || cCondition->isTrue()) {
    float mBlockRadius = IsoRealmsConstants::BLOCK_RADIUS;
    double mFromX = cX + (cFacing == EAST ? mBlockRadius : -mBlockRadius);
    double mFromY = cY + (cFacing == NORTH ? mBlockRadius : -mBlockRadius); 
    double mFromZ = cZ * IsoRealmsConstants::BLOCK_HEIGHT;
    double mToX = (cFacing == WEST || cFacing == EAST) ? (mFromX) : cX - mBlockRadius + cLength;
    double mToY = (cFacing == SOUTH || cFacing == NORTH) ? (mFromY) : cY - mBlockRadius + cLength;
    double mHighStartSlopeZ = (cZ + cHeight) * IsoRealmsConstants::BLOCK_HEIGHT;
    double mHighEndSlopeZ =  ((cZ + cHeight) + cTopSlope * cLength) * IsoRealmsConstants::BLOCK_HEIGHT;
    double mHighStartSlopeTexture = cZ + cHeight;
    double mHighEndSlopeTexture = (cZ + cHeight) + cTopSlope * cLength;

    ISpindizzyTexture* mTexture = (*cTextureSet)->getTexture(cTextureType);
    mTexture->set();
    double mEdgeWidth = IsoRealmsConstants::BLOCK_HEIGHT * 0.5;
    glBegin(GL_QUADS);

    switch (mTexture->getMapping()) {
      case ISpindizzyTexture::TILED: {
        if (cFacing == EAST || cFacing == SOUTH) {
          mTexture->texCoord2f(cX,           cZ);                     glVertex3f(mFromX, mFromY, mFromZ);
          mTexture->texCoord2f(cX + cLength, cZ);                     glVertex3f(mToX,   mToY,   mFromZ);
          mTexture->texCoord2f(cX + cLength, mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
          mTexture->texCoord2f(cX,           mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
        } else {
          mTexture->texCoord2f(cX,           mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
          mTexture->texCoord2f(cX + cLength, mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
          mTexture->texCoord2f(cX + cLength, cZ);                     glVertex3f(mToX,   mToY,   mFromZ);
          mTexture->texCoord2f(cX,           cZ);                     glVertex3f(mFromX, mFromY, mFromZ);
        }
        break;
      }

      case ISpindizzyTexture::COLUMN_CAPPED: {
        if (cFacing == EAST || cFacing == SOUTH) {
          if (mHighEndSlopeZ - mFromZ > 1.0 || mHighStartSlopeZ - mFromZ > 1.0) {
            mTexture->texCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
            mTexture->texCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
            mTexture->texCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
            mTexture->texCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
          }
          mTexture->texCoord2f(cX,           0.0);  glVertex3f(mFromX, mFromY, mFromZ);
          mTexture->texCoord2f(cX + cLength, 0.0);  glVertex3f(mToX,   mToY,   mFromZ);
          mTexture->texCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
          mTexture->texCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
          mTexture->texCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
          mTexture->texCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
          mTexture->texCoord2f(cX + cLength, 1.00); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
          mTexture->texCoord2f(cX,           1.00); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
  /*        glColor3f(1.0f, 0.0, 0.0f);
          mTexture->texCoord2f(cX,           0.0);  glVertex3f(mFromX, mFromY, mFromZ);
          glColor3f(0.0f, 1.0, 0.0f);
          mTexture->texCoord2f(cX + cLength, 0.0);  glVertex3f(mToX,   mToY,   mFromZ);
          mTexture->texCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
          glColor3f(1.0f, 0.0, 0.0f);
          mTexture->texCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
          mTexture->texCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
          glColor3f(0.0f, 1.0, 0.0f);
          mTexture->texCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
          mTexture->texCoord2f(cX + cLength, 1.00); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
          glColor3f(1.0f, 0.0, 0.0f);
          mTexture->texCoord2f(cX,           1.00); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);*/
        } else {
          if (mHighEndSlopeZ - mFromZ > 1.0 || mHighStartSlopeZ - mFromZ > 1.0) {
            mTexture->texCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
            mTexture->texCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
            mTexture->texCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
            mTexture->texCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
          }
          mTexture->texCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
          mTexture->texCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
          mTexture->texCoord2f(cX + cLength, 0.0);  glVertex3f(mToX,   mToY,   mFromZ);
          mTexture->texCoord2f(cX,           0.0);  glVertex3f(mFromX, mFromY, mFromZ);
          mTexture->texCoord2f(cX,           1.00); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
          mTexture->texCoord2f(cX + cLength, 1.00); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
          mTexture->texCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
          mTexture->texCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
  /*        glColor3f(1.0f, 0.0, 0.0f);
          mTexture->texCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
          glColor3f(0.0f, 1.0, 0.0f);
          mTexture->texCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
          mTexture->texCoord2f(cX + cLength, 0.0);  glVertex3f(mToX,   mToY,   mFromZ);
          glColor3f(1.0f, 0.0, 0.0f);
          mTexture->texCoord2f(cX,           0.0);  glVertex3f(mFromX, mFromY, mFromZ);
          mTexture->texCoord2f(cX,           1.00); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
          glColor3f(0.0f, 1.0, 0.0f);
          mTexture->texCoord2f(cX + cLength, 1.00); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
          mTexture->texCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
          glColor3f(1.0f, 0.0, 0.0f);
          mTexture->texCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);*/
        }
        break;
      }

      default: {
        // TODO: Throw Unsupported map type exception
        break;
      }
    }
  }
  glEnd();
}

WallSurface::WallEdge::WallEdge(int startHeight, int endHeight) {
  cStartHeight = startHeight;
  cEndHeight = endHeight;
}

int WallSurface::WallEdge::getStartHeight() {
  return cStartHeight;
}

int WallSurface::WallEdge::getEndHeight() {
  return cEndHeight;
}

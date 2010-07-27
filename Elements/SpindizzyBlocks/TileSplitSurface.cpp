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

const float TileSplitSurface::SLOPE_ACCELERATION = 0.0005f;

TileSplitSurface::TileSplitSurface(bool splitDirection, BlockLocation& location, ISpindizzyTextureSet** textureSet, ISpindizzyTextureSet::TextureType textureType, int nw, int ne, int se, int sw, Condition* condition) {
  cTextureSet = textureSet;
  cTextureType = textureType;
  cLocation = location;
  cCornerHeights[0][1] = nw;
  cCornerHeights[1][1] = ne;
  cCornerHeights[1][0] = se;
  cCornerHeights[0][0] = sw;
  cSplitDirection = splitDirection;
  cCondition = condition;
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
  if (cCondition == NULL || cCondition->isTrue()) {
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
}

BlockArea* TileSplitSurface::getCoverage() {
  return new BlockArea(cLocation, cLocation);
}

bool TileSplitSurface::alligned(int x, int y) {
  return cLocation.x == x && cLocation.y == y;
}

bool TileSplitSurface::contains(Vertex& location) {
  float mSouthEdge  = cLocation.y - IsoRealmsConstants::BLOCK_RADIUS;
  float mWestEdge   = cLocation.x  - IsoRealmsConstants::BLOCK_RADIUS;
  float mNorthEdge  = cLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
  float mEastEdge   = cLocation.x  + IsoRealmsConstants::BLOCK_RADIUS;
  if (location.y > mSouthEdge  && location.y <= mNorthEdge && location.x > mWestEdge && location.x <= mEastEdge) {
    return location.z == getHeightAt(location.x, location.y);
  }
  return false;
}
/*
 3 - 4 = -1 * mX == -1 

-1    0.5   0
 3 +------+ 4
          |
      +   | 0
          |
          + 4      4 + 0
 
*/ 
float TileSplitSurface::getHeightAt(float x, float y) {
  float mX = x - (cLocation.x - IsoRealmsConstants::BLOCK_RADIUS);
  float mY = y - (cLocation.y - IsoRealmsConstants::BLOCK_RADIUS);
  if (cSplitDirection) {
    if (mX > mY) {
      return cCornerHeights[0][0] + (cCornerHeights[1][0] - cCornerHeights[0][0]) * mX + (cCornerHeights[1][1] - cCornerHeights[1][0]) * mY;
    } else {
      return cCornerHeights[0][0] + (cCornerHeights[1][1] - cCornerHeights[0][1]) * mX + (cCornerHeights[0][1] - cCornerHeights[0][0]) * mY;
    }
  } else {
    if (mX + mY > 1.0f) {
      return cCornerHeights[1][0] + (cCornerHeights[1][1] - cCornerHeights[1][0]) * mY + (cCornerHeights[0][1] - cCornerHeights[1][1]) * (1.0f - mX);
    } else {
      return cCornerHeights[0][0] + (cCornerHeights[1][0] - cCornerHeights[0][0]) * mX + (cCornerHeights[0][1] - cCornerHeights[0][0]) * mY;
    }
  }
}

float TileSplitSurface::getXAcceleration(float x, float y) {
  float mX = x - (cLocation.x - IsoRealmsConstants::BLOCK_RADIUS);
  float mY = y - (cLocation.y - IsoRealmsConstants::BLOCK_RADIUS);
  return cSplitDirection
       ? (mX > mY
          ? (cCornerHeights[0][0] - cCornerHeights[1][0]) * SLOPE_ACCELERATION
          : (cCornerHeights[0][1] - cCornerHeights[1][1]) * SLOPE_ACCELERATION)
       : (mX + mY > 1.0f
          ? (cCornerHeights[0][1] - cCornerHeights[1][1]) * SLOPE_ACCELERATION
          : (cCornerHeights[0][0] - cCornerHeights[1][0]) * SLOPE_ACCELERATION);
}

float TileSplitSurface::getYAcceleration(float x, float y) {
  float mX = x - (cLocation.x - IsoRealmsConstants::BLOCK_RADIUS);
  float mY = y - (cLocation.y - IsoRealmsConstants::BLOCK_RADIUS);
  return cSplitDirection
       ? (mX > mY
          ? (cCornerHeights[1][0] - cCornerHeights[1][1]) * SLOPE_ACCELERATION
          : (cCornerHeights[0][0] - cCornerHeights[0][1]) * SLOPE_ACCELERATION)
       : (mX + mY > 1.0f
          ? (cCornerHeights[1][0] - cCornerHeights[1][1]) * SLOPE_ACCELERATION
          : (cCornerHeights[0][0] - cCornerHeights[0][1]) * SLOPE_ACCELERATION);
}

Vertex* TileSplitSurface::getBoundaryCrossingPoint(Vertex& start, Vertex& end, float* mLowestGradient) {
  *mLowestGradient = 2.0f;
  float mXMovement = end.x - start.x;
  float mYMovement = end.y - start.y;
  float mNorth = cLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
  float mEast  = cLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
  float mSouth = cLocation.y - IsoRealmsConstants::BLOCK_RADIUS;
  float mWest  = cLocation.x - IsoRealmsConstants::BLOCK_RADIUS;
  float mImpactX;
  float mImpactY;
  bool mXKnown = false;
  bool mYKnown = false;

  float mTempGradient = (mWest - start.x) / mXMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mWestYLocation = start.y + mYMovement * mTempGradient;
    if (mWestYLocation >= mSouth && mWestYLocation <= mNorth) {
      *mLowestGradient = mTempGradient;
      mImpactX = mWest;
      mXKnown = true;
    }
  }

  mTempGradient = (mEast - start.x) / mXMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mEastYLocation = start.y + mYMovement * mTempGradient;
    if (mEastYLocation >= mSouth && mEastYLocation <= mNorth) {
      *mLowestGradient = mTempGradient;
      mImpactX = mEast;
      mXKnown = true;
    }
  }

  mTempGradient = (mNorth - start.y) / mYMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mNorthXLocation = start.x + mXMovement * mTempGradient;
    if (mNorthXLocation >= mWest && mNorthXLocation <= mEast) {
      *mLowestGradient = mTempGradient;
      mImpactY = mNorth;
      mXKnown = false;
      mYKnown = true;
    }
  }

  mTempGradient = (mSouth - start.y) / mYMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mSouthXLocation = start.x + mXMovement * mTempGradient;
    if (mSouthXLocation >= mWest && mSouthXLocation <= mEast) {
      *mLowestGradient = mTempGradient;
      mImpactY = mSouth;
      mXKnown = false;
      mYKnown = true;
    }
  }

  float mMovementZ = end.z - start.z;
  if (*mLowestGradient <= 1.0f) {
    if (!mXKnown) {
      mImpactX = start.x + mXMovement * *mLowestGradient;
    }
    if (!mYKnown) {
      mImpactY = start.y + mYMovement * *mLowestGradient;
    }
    float mImpactZ = start.z + mMovementZ * *mLowestGradient;
    return new Vertex(mImpactX, mImpactY, mImpactZ);
  }

  // Line doesn't cross boundary
  return NULL;
}

ICollisionData* TileSplitSurface::getCollision(Vertex& start, Vertex& end) {
  if (!contains(start)) {
    float mGradient;
    Vertex* mEnterPoint = getBoundaryCrossingPoint(start, end, &mGradient);
    if (mEnterPoint != NULL) {
      SurfaceCollisionEvent* mEvent = new SurfaceCollisionEvent(this);
//      mImpactPoint->setRelocationPoint(*mLeavePoint);
      return mEvent;
    }
  }
  
  // No event
  return NULL;
}

ICollisionData* TileSplitSurface::getRollingEvent(Vertex& start, Vertex& end) {
  float mGradient;
  Vertex* mLeavePoint = getBoundaryCrossingPoint(start, end, &mGradient);
  if (mLeavePoint != NULL) {
    SurfaceCollisionEvent* mEvent = new SurfaceCollisionEvent(this);
//    mImpactPoint->setRelocationPoint(*mLeavePoint);
    return mEvent;
  }

  // No event
  return NULL;
}


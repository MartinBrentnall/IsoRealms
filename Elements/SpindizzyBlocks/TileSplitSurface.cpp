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

TileSplitSurface::TileSplitSurface(bool splitDirection, BlockLocation& location, ISpindizzyTextureSet** textureSet, ISpindizzyTextureSet::TextureType textureType, int nw, int ne, int se, int sw, Condition* condition, BlockTypeProperties* blockTypeProperties) {
  cTextureSet = textureSet;
  cTextureType = textureType;
  cLocation = location;
  cCornerHeights[0][1] = nw;
  cCornerHeights[1][1] = ne;
  cCornerHeights[1][0] = se;
  cCornerHeights[0][0] = sw;
  cSplitDirection = splitDirection;
  cCondition = condition;
  cBlockTypeProperties = blockTypeProperties;
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
    float mSurfaceHeight = getHeightAt(location.x, location.y);
    return location.z <= mSurfaceHeight && location.z >= mSurfaceHeight - 0.5f;
  }
  return false;
}

float TileSplitSurface::getHeightAt(float x, float y, bool northSplit) {
  x -= cLocation.x - IsoRealmsConstants::BLOCK_RADIUS;
  y -= cLocation.y - IsoRealmsConstants::BLOCK_RADIUS;
  if (cSplitDirection) {
    if (northSplit) {
      return cLocation.z + (cCornerHeights[0][0] + (cCornerHeights[1][1] - cCornerHeights[0][1]) * x + (cCornerHeights[0][1] - cCornerHeights[0][0]) * y);
    } else {
      return cLocation.z + (cCornerHeights[0][0] + (cCornerHeights[1][0] - cCornerHeights[0][0]) * x + (cCornerHeights[1][1] - cCornerHeights[1][0]) * y);
    }
  } else {
    if (northSplit) {
      return cLocation.z + (cCornerHeights[1][0] + (cCornerHeights[1][1] - cCornerHeights[1][0]) * y + (cCornerHeights[0][1] - cCornerHeights[1][1]) * (1.0f - x));
    } else {
      return cLocation.z + (cCornerHeights[0][0] + (cCornerHeights[1][0] - cCornerHeights[0][0]) * x + (cCornerHeights[0][1] - cCornerHeights[0][0]) * y);
    }
  }
}

float TileSplitSurface::getHeightAt(float x, float y) {
  return getHeightAt(x, y, inNorthSplit(x, y));
}

bool TileSplitSurface::hasFlatSide() {
  return cSplitDirection
       ? (cCornerHeights[0][0] == cCornerHeights[1][0] && cCornerHeights[0][0] == cCornerHeights[1][1])
      || (cCornerHeights[0][0] == cCornerHeights[0][1] && cCornerHeights[0][0] == cCornerHeights[1][1])
       : (cCornerHeights[0][0] == cCornerHeights[1][0] && cCornerHeights[0][0] == cCornerHeights[0][1])
      || (cCornerHeights[1][1] == cCornerHeights[1][0] && cCornerHeights[1][1] == cCornerHeights[0][1]);
}

Vertex* TileSplitSurface::getSplitCrossingPoint(Vertex& start, Vertex& end, float* mGradient) {
  Vertex mStart(cSplitDirection ? -(start.x - cLocation.x) : start.x - cLocation.x, start.y - cLocation.y, start.z);
  Vertex mEnd(  cSplitDirection ? -(end.x   - cLocation.x) : end.x   - cLocation.x, end.y   - cLocation.y, end.z);
  float mDistanceToStart = mStart.x + mStart.y;
  float mDistanceToEnd   = mEnd.x   + mEnd.y;
  bool mStartAtNorth = mDistanceToStart > 0;
  if (mStartAtNorth != (mDistanceToEnd > 0)) {
    float mGradient = mDistanceToStart / (mDistanceToStart - mDistanceToEnd);
    float mYLocation = mStart.y + (mEnd.y - mStart.y) * mGradient;
    if (mYLocation >= -IsoRealmsConstants::BLOCK_RADIUS && mYLocation < IsoRealmsConstants::BLOCK_RADIUS) {
      float mXLocation = cSplitDirection ? mYLocation : -mYLocation;
      mXLocation += cLocation.x;
      mYLocation += cLocation.y;
      float mZLocation = getHeightAt(mXLocation, mYLocation);
      float mXDirection = mStartAtNorth == cSplitDirection ?  INFINITY : -INFINITY;
      float mYDirection = mStartAtNorth                    ? -INFINITY :  INFINITY;
      return new Vertex(nextafterf(mXLocation, mXDirection), nextafterf(mYLocation, mYDirection), mZLocation);
    }
  }
  return NULL;
}

float TileSplitSurface::getXAcceleration(float x, float y) {
  x -= cLocation.x;
  y -= cLocation.y;
  if (x == 0.0f && y == 0.0f && hasFlatSide()) {
    return 0.0f;
  }
  return cSplitDirection
       ? (x > y
          ? (cCornerHeights[0][0] - cCornerHeights[1][0])
          : (cCornerHeights[0][1] - cCornerHeights[1][1]))
       : (x + y > 0.0f
          ? (cCornerHeights[0][1] - cCornerHeights[1][1])
          : (cCornerHeights[0][0] - cCornerHeights[1][0]));
}

float TileSplitSurface::getYAcceleration(float x, float y) {
  x -= cLocation.x;
  y -= cLocation.y;
  if (x == 0.0f && y == 0.0f && hasFlatSide()) {
    return 0.0f;
  }
  return cSplitDirection
       ? (x > y
          ? (cCornerHeights[1][0] - cCornerHeights[1][1])
          : (cCornerHeights[0][0] - cCornerHeights[0][1]))
       : (x + y > 0.0f
          ? (cCornerHeights[1][0] - cCornerHeights[1][1])
          : (cCornerHeights[0][0] - cCornerHeights[0][1]));
}

void TileSplitSurface::notifyContact() {
  cBlockTypeProperties->executeContactScript();
}

void TileSplitSurface::notifyImpact() {
  cBlockTypeProperties->executeImpactScript();
}

float TileSplitSurface::getSurfaceFriction() {
  return cBlockTypeProperties->getSurfaceFriction();
}

float TileSplitSurface::getSurfaceGrip() {
  return cBlockTypeProperties->getSurfaceGrip();
}

float TileSplitSurface::getSurfaceBounce() {
  return cBlockTypeProperties->getSurfaceBounce();
}

IRollableSurface::RespawnPossibility TileSplitSurface::getRespawnPossibility() {
  return cBlockTypeProperties->isRespawnAllowed() 
       ? (cCondition != NULL ? IRollableSurface::CONDITIONAL : IRollableSurface::YES)
       : IRollableSurface::NO;
}

bool TileSplitSurface::isRespawnPossibleNow() {
  return cBlockTypeProperties->isRespawnAllowed() && (cCondition == NULL || cCondition->isTrue());
}

Vertex* TileSplitSurface::getBoundaryCrossingPoint(Vertex& start, Vertex& end, float* mLowestGradient, float infinity) {
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
      mImpactX = nextafterf(mWest, -infinity);
      mXKnown = true;
    }
  }

  mTempGradient = (mEast - start.x) / mXMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mEastYLocation = start.y + mYMovement * mTempGradient;
    if (mEastYLocation >= mSouth && mEastYLocation <= mNorth) {
      *mLowestGradient = mTempGradient;
      mImpactX = nextafterf(mEast, infinity);
      mXKnown = true;
    }
  }

  mTempGradient = (mNorth - start.y) / mYMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mNorthXLocation = start.x + mXMovement * mTempGradient;
    if (mNorthXLocation >= mWest && mNorthXLocation <= mEast) {
      *mLowestGradient = mTempGradient;
      mImpactY = nextafterf(mNorth, infinity);
      mXKnown = false;
      mYKnown = true;
    }
  }

  mTempGradient = (mSouth - start.y) / mYMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mSouthXLocation = start.x + mXMovement * mTempGradient;
    if (mSouthXLocation >= mWest && mSouthXLocation <= mEast) {
      *mLowestGradient = mTempGradient;
      mImpactY = nextafterf(mSouth, -infinity);
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

bool TileSplitSurface::inNorthSplit(float x, float y) {
  x -= cLocation.x;
  y -= cLocation.y;
  return cSplitDirection ? y > x : x + y > 0.0f;
}

ICollisionData* TileSplitSurface::getImpactCollision(Vertex& start, Vertex& end, bool northSplit) {
  float mStartHeight = getHeightAt(start.x, start.y, northSplit);
  float mEndHeight = getHeightAt(end.x, end.y, northSplit);
  if ((start.z > mStartHeight) != (end.z > mEndHeight) && start.z > mStartHeight) {
    float mEndHeightModified = mEndHeight - (start.z - end.z);
    float mGradient = (start.z - mStartHeight) / (mEndHeightModified - mStartHeight);
    double mXImpact = start.x + (end.x - start.x) * mGradient;
    double mYImpact = start.y + (end.y - start.y) * mGradient;
    if (inNorthSplit(mXImpact, mYImpact) == northSplit) {
      if (alligned(round(mXImpact), round(mYImpact))) {
        double mZImpact = getHeightAt(mXImpact, mYImpact);
        float mXSlope = getXAcceleration(start.x, start.y);
        float mYSlope = getYAcceleration(start.x, start.y);
        Vertex* mImpactLocation = new Vertex(mXImpact, mYImpact, mZImpact);
        return new SurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mImpactLocation, -mXSlope, -mYSlope, mGradient);
      }
    }
  }
  return NULL;
}

ICollisionData* TileSplitSurface::getCollision(Vertex& start, Vertex& end) {
  if (cCondition == NULL || cCondition->isTrue()) {
    if (contains(start)) {
      Vertex* mEnterPoint = new Vertex(start);
      return new SurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mEnterPoint, getXAcceleration(start.x, start.y), getYAcceleration(start.x, start.y), 0.0f);
    }
  
    float mGradient;
    Vertex* mEnterPoint = getBoundaryCrossingPoint(start, end, &mGradient, -INFINITY);
    if (mEnterPoint != NULL) {
      float mEnterHeight = getHeightAt(mEnterPoint->x, mEnterPoint->y);
      if (mEnterPoint->z <= mEnterHeight && mEnterPoint->z >= mEnterHeight - 0.5f) {
        SurfaceCollisionEvent* mEvent = new SurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mEnterPoint, getXAcceleration(start.x, start.y), getYAcceleration(start.x, start.y), mGradient);
        return mEvent;
      }
    }

// TODO: EXPERIMENTAL
    ICollisionData* mImpact = getImpactCollision(start, end, true);
    if (mImpact != NULL) {
      return mImpact;
    }
    return getImpactCollision(start, end, false);
// TODO: END EXPERIMENTAL
  }
  
  // No event
  return NULL;
}

ICollisionData* TileSplitSurface::getRollingEvent(Vertex& start, Vertex& end) {
  if (cCondition != NULL && !cCondition->isTrue()) {
    return new SurfaceCollisionEvent(this, ICollisionData::SURFACE_LEAVE, new Vertex(start), getXAcceleration(start.x, start.y), getYAcceleration(start.x, start.y), 0.0f);
  }
  
  float mGradient;
  Vertex* mLeavePoint = getSplitCrossingPoint(start, end, &mGradient);
  if (mLeavePoint != NULL) {
    return new SurfaceCollisionEvent(this, ICollisionData::SURFACE_LEAVE, mLeavePoint, getXAcceleration(start.x, start.y), getYAcceleration(start.x, start.y), mGradient);
  }

  mLeavePoint = getBoundaryCrossingPoint(start, end, &mGradient, INFINITY);
  if (mLeavePoint != NULL) {
    return new SurfaceCollisionEvent(this, ICollisionData::SURFACE_LEAVE, mLeavePoint, getXAcceleration(start.x, start.y), getYAcceleration(start.x, start.y), mGradient);
  }
  return NULL;
}

void TileSplitSurface::getRestingLocation(Vertex& location) {
  float mXSlope = -getXAcceleration(location.x, location.y);
  float mYSlope = -getYAcceleration(location.x, location.y);
  float mYEdgeLocation = mYSlope > 0.0f ? nextafterf(cLocation.y - IsoRealmsConstants::BLOCK_RADIUS,  INFINITY)
                       : mYSlope < 0.0f ? nextafterf(cLocation.y + IsoRealmsConstants::BLOCK_RADIUS, -INFINITY)
                       :                  location.y;
  float mXEdgeLocation = mXSlope > 0.0f ? nextafterf(cLocation.x - IsoRealmsConstants::BLOCK_RADIUS,  INFINITY)
                       : mXSlope < 0.0f ? nextafterf(cLocation.x + IsoRealmsConstants::BLOCK_RADIUS, -INFINITY)
                       :                  location.x;
  float mDistanceToY = mYEdgeLocation - location.y;
  float mDistanceToX = mXEdgeLocation - location.x;
  if (fabs(mDistanceToY / mYSlope) < fabs(mDistanceToX / mXSlope)) {
    location.y = mYEdgeLocation;
    // TODO: location.x
  } else {
    location.x = mXEdgeLocation;
    // TODO: location.y
  }
  // TODO: Need to consider the case where we cross over the split
}

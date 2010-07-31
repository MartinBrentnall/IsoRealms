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
#include "TileSurface.h"

TileSurface::TileSurface(ISpindizzyTextureSet** textureSet, ISpindizzyTextureSet::TextureType textureType, int north, int east, int south, int west, int height, int westEastSlope, int northSouthSlope, ITileSurface::FaceDirection facing, Condition* condition, Script* contactScript, float friction, float grip, bool respawnAllowed) {
  cTextureSet = textureSet;
  cTextureType = textureType;
  cNorth = north;
  cEast = east;
  cSouth = south;
  cWest = west;
  cHeight = height;
  cWestEastSlope = westEastSlope;
  cNorthSouthSlope = northSouthSlope;
  cFacing = facing;
  cCondition = condition;
  cContactScript = contactScript;
  cFriction = friction;
  cGrip = grip;
  cRespawnAllowed = respawnAllowed;
}

int TileSurface::getSurfaceCellHeight(int x, int y) {
  return abs(((cWestEastSlope   >= 0 ? cWest  : cEast)  - x) * cWestEastSlope)
       + abs(((cNorthSouthSlope >= 0 ? cSouth : cNorth) - y) * cNorthSouthSlope)
       + cHeight;
}

int TileSurface::getSurfaceCellElevation(int x, int y) {
  return abs(cWestEastSlope) + abs(cNorthSouthSlope);
}

void TileSurface::render() {
  if (cCondition == NULL || cCondition->isTrue()) {
    double xs = cWest - IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
    double ys = cSouth - IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
    double xe = cEast + IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
    double ye = cNorth + IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
    
    double xsys = cHeight * IsoRealmsConstants::BLOCK_HEIGHT; // TODO: Rename this
    double xsye = cHeight * IsoRealmsConstants::BLOCK_HEIGHT; // TODO: Rename this
    double xeye = cHeight * IsoRealmsConstants::BLOCK_HEIGHT; // TODO: Rename this
    double xeys = cHeight * IsoRealmsConstants::BLOCK_HEIGHT; // TODO: Rename this

    if (cWestEastSlope < 0) {
      xsys += (abs(cWestEastSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (xe - xs));
      xeys += (abs(cWestEastSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (xe - xs));
    } else {
      xsye += (abs(cWestEastSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (xe - xs));
      xeye += (abs(cWestEastSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (xe - xs));
    }

    if (cNorthSouthSlope < 0) {
      xsys += (abs(cNorthSouthSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (ye - ys));
      xsye += (abs(cNorthSouthSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (ye - ys));
    } else {
      xeys += (abs(cNorthSouthSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (ye - ys));
      xeye += (abs(cNorthSouthSlope) * IsoRealmsConstants::BLOCK_HEIGHT * (ye - ys));
    }
    ISpindizzyTexture* mTexture = (*cTextureSet)->getTexture(cTextureType);
    mTexture->set();
    glBegin(GL_QUADS);

    switch (cFacing) {
      case ITileSurface::UP: {
        mTexture->texCoord2f(cEast + 1, cNorth + 1); glVertex3f(xe, ys, xsye);
        mTexture->texCoord2f(cEast + 1, cSouth    ); glVertex3f(xe, ye, xeye);
        mTexture->texCoord2f(cWest,     cSouth    ); glVertex3f(xs, ye, xeys);
        mTexture->texCoord2f(cWest,     cNorth + 1); glVertex3f(xs, ys, xsys);
/*        glColor3f(1.0, 1.0, 0.0); mTexture->texCoord2f(cEast + 1, cNorth + 1); glVertex3f(xe, ys, xsye);
        glColor3f(0.0, 1.0, 1.0); mTexture->texCoord2f(cEast + 1, cSouth);     glVertex3f(xe, ye, xeye);
        glColor3f(1.0, 0.0, 1.0); mTexture->texCoord2f(cWest,     cSouth);     glVertex3f(xs, ye, xeys);
        glColor3f(1.0, 1.0, 1.0); mTexture->texCoord2f(cWest,     cNorth + 1);  glVertex3f(xs, ys, xsys);*/
        break;
      }

      case ITileSurface::DOWN: {
        mTexture->texCoord2f(cWest,     cNorth + 1); glVertex3f(xs, ye, xeys);
        mTexture->texCoord2f(cEast + 1, cNorth + 1); glVertex3f(xe, ye, xeye);
        mTexture->texCoord2f(cEast + 1, cSouth);     glVertex3f(xe, ys, xsye);
        mTexture->texCoord2f(cWest,     cSouth);     glVertex3f(xs, ys, xsys);
  /*      glColor3f(0.0, 1.0, 0.0); mTexture->texCoord2f(cWest,     cNorth + 1); glVertex3f(xs, ye, xeys);
        glColor3f(1.0, 1.0, 0.0); mTexture->texCoord2f(cEast + 1, cNorth + 1); glVertex3f(xe, ye, xeye);
        glColor3f(1.0, 0.0, 0.0); mTexture->texCoord2f(cEast + 1, cSouth);     glVertex3f(xe, ys, xsye);
        glColor3f(0.0, 0.0, 1.0); mTexture->texCoord2f(cWest,     cSouth);     glVertex3f(xs, ys, xsys);*/
        break;
      }
    }
    glEnd();
  }
}

float TileSurface::getHeightAt(float x, float y) {
  return cWestEastSlope   * ((cWestEastSlope   > 0 ? x - cWest  : -(cEast  + 1 - x)) + IsoRealmsConstants::BLOCK_RADIUS) +
         cNorthSouthSlope * ((cNorthSouthSlope > 0 ? y - cSouth : -(cNorth + 1 - y)) + IsoRealmsConstants::BLOCK_RADIUS) + cHeight;
}

Vertex* TileSurface::getBoundaryCrossingPoint(Vertex& start, Vertex& end, float* mLowestGradient) {
  *mLowestGradient = 2.0f;
  float mXMovement = end.x - start.x;
  float mYMovement = end.y - start.y;
  float mNorth = cNorth + IsoRealmsConstants::BLOCK_RADIUS;
  float mEast = cEast + IsoRealmsConstants::BLOCK_RADIUS;
  float mSouth = cSouth - IsoRealmsConstants::BLOCK_RADIUS;
  float mWest = cWest - IsoRealmsConstants::BLOCK_RADIUS;
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

bool TileSurface::contains(Vertex& location) {
  float mSouthEdge  = cSouth - IsoRealmsConstants::BLOCK_RADIUS;
  float mWestEdge   = cWest  - IsoRealmsConstants::BLOCK_RADIUS;
  float mNorthEdge  = cNorth + IsoRealmsConstants::BLOCK_RADIUS;
  float mEastEdge   = cEast  + IsoRealmsConstants::BLOCK_RADIUS;
  if (location.y > mSouthEdge  && location.y <= mNorthEdge && location.x > mWestEdge && location.x <= mEastEdge) {
    return location.z == getHeightAt(location.x, location.y);
  }
  return false;
}

ICollisionData* TileSurface::getRollingEvent(Vertex& start, Vertex& end) {
  if (cCondition != NULL && !cCondition->isTrue()) {
    std::cout << "Surface disappeared!" << std::endl;
    return new SurfaceCollisionEvent(this, ICollisionData::SURFACE_LEAVE, new Vertex(start), 0.0f);
  }
  
  float mGradient;
  Vertex* mLeavePoint = getBoundaryCrossingPoint(start, end, &mGradient);
  if (mLeavePoint != NULL) {
    SurfaceCollisionEvent* mEvent = new SurfaceCollisionEvent(this, ICollisionData::SURFACE_LEAVE, mLeavePoint, mGradient);
//    mImpactPoint->setRelocationPoint(*mLeavePoint);
    return mEvent;
  }

  // No event
  return NULL;
}

ICollisionData* TileSurface::getCollision(Vertex& start, Vertex& end) {
  if (!contains(start) && (cCondition == NULL || cCondition->isTrue())) {
    float mGradient;
    Vertex* mEnterPoint = getBoundaryCrossingPoint(start, end, &mGradient);
    if (mEnterPoint != NULL) {
      float mEnterHeight = getHeightAt(mEnterPoint->x, mEnterPoint->y);
      std::cout << "From " << start.z << " to " << end.z << "..." << std::endl;
      std::cout << "Height: " << mEnterPoint->z << " (" << (mEnterHeight - 0.5f) << " to " << mEnterHeight << ")" << std::endl;
      // TODO: The "0.01f" is a bit nasty magic number
      if (mEnterPoint->z <= mEnterHeight + 0.01f && mEnterPoint->z >= mEnterHeight - 0.5f) {
        SurfaceCollisionEvent* mEvent = new SurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mEnterPoint, mGradient);
//        mImpactPoint->setRelocationPoint(*mLeavePoint);
        return mEvent;
      }
    }
  }

  float mStartHeight = getHeightAt(start.x, start.y);
  float mEndHeight = getHeightAt(end.x, end.y);
  if ((start.z > mStartHeight) != (end.z > mEndHeight) && start.z > mStartHeight) {
    float mEndHeightModified = mEndHeight - (start.z - end.z);
    float mGradient = (start.z - mStartHeight) / (mEndHeightModified - mStartHeight);
    float mXImpact = start.x + (end.x - start.x) * mGradient;
    float mYImpact = start.y + (end.y - start.y) * mGradient;
    float mZImpact = start.z + (end.z - start.z) * mGradient;
    Vertex* mImpactLocation = new Vertex(mXImpact, mYImpact, mZImpact);
    if (alligned(round(mImpactLocation->x), round(mImpactLocation->y))) {
      SurfaceCollisionEvent *mEvent = new SurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mImpactLocation, mGradient);
      return mEvent;
    }
  }

  // No event
  return NULL;
}

float TileSurface::getXAcceleration(float, float) {
  return -cWestEastSlope;
}

float TileSurface::getYAcceleration(float, float) {
  return -cNorthSouthSlope;
}

void TileSurface::notifyContact() {
  if (cContactScript != NULL) {
    cContactScript->execute();
  }
}

float TileSurface::getSurfaceFriction() {
  return cFriction;
}

float TileSurface::getSurfaceGrip() {
  return cGrip;
}

IRollableSurface::RespawnPossibility TileSurface::getRespawnPossibility() {
  return cRespawnAllowed 
       ? (cCondition != NULL ? IRollableSurface::CONDITIONAL : IRollableSurface::YES)
       : IRollableSurface::NO;
}

bool TileSurface::isRespawnPossibleNow() {
  return cRespawnAllowed && (cCondition == NULL || cCondition->isTrue());
}

BlockArea* TileSurface::getCoverage() {
  BlockLocation mStartLocation(cWest, cSouth, cHeight);
  // TODO: Not sure if next line is correct
  int mTopHeight = cHeight + (((cEast - cWest) + 1) * cWestEastSlope) + (((cNorth - cSouth) + 1) * cNorthSouthSlope);
  BlockLocation mEndLocation(cEast, cNorth, mTopHeight);
  return new BlockArea(mStartLocation, mEndLocation);
}

bool TileSurface::alligned(int x, int y) {
  return y >= cSouth && y <= cNorth && x >= cWest && x <= cEast;
}

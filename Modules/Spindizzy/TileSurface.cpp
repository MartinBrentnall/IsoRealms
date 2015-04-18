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

TileSurface::TileSurface(ITexture** texture, TextureRotation rotation, int north, int east, int south, int west, int height, int westEastSlope, int northSouthSlope, ITileSurface::FaceDirection facing, Condition* condition, BlockTypeProperties* blockTypeProperties, IArgument* element) {
  cTexture = texture;
  cRotation = rotation;
  cNorth = north;
  cEast = east;
  cSouth = south;
  cWest = west;
  cHeight = height;
  cWestEastSlope = westEastSlope;
  cNorthSouthSlope = northSouthSlope;
  cFacing = facing;
  cCondition = condition;
  cBlockTypeProperties = blockTypeProperties;
  cElement = element;
}

int TileSurface::getSurfaceCellHeight(int x, int y) {
  return abs(((cWestEastSlope   >= 0 ? cWest  : cEast)  - x) * cWestEastSlope)
       + abs(((cNorthSouthSlope >= 0 ? cSouth : cNorth) - y) * cNorthSouthSlope)
       + cHeight;
}

int TileSurface::getSurfaceCellElevation(int x, int y) {
  return abs(cWestEastSlope) + abs(cNorthSouthSlope);
}

float TileSurface::getEastTextureCoord() {
  switch (cRotation) {
    case STRAIGHT: return   cEast  + 1;
    case RIGHT:    return -(cNorth + 1);
    case LEFT:     return   cNorth + 1;
    case REVERSE:  return -(cEast  + 1);
  }
  std::cout << "WARNING: Shouldn't happen" << std::endl; // TODO: Throw
  return 0.0f;
}

float TileSurface::getWestTextureCoord() {
  switch (cRotation) {
    case STRAIGHT: return   cWest;
    case RIGHT:    return  -cSouth;
    case LEFT:     return   cSouth;
    case REVERSE:  return  -cWest;
  }
  std::cout << "WARNING: Shouldn't happen" << std::endl; // TODO: Throw
  return 0.0f;
}

float TileSurface::getNorthTextureCoord() {
  switch (cRotation) {
    case STRAIGHT: return   cNorth + 1;
    case RIGHT:    return -(cEast  + 1);
    case LEFT:     return   cEast  + 1;    
    case REVERSE:  return -(cNorth + 1);
  }
  std::cout << "WARNING: Shouldn't happen" << std::endl; // TODO: Throw
  return 0.0f;
}

float TileSurface::getSouthTextureCoord() {
  switch (cRotation) {
    case STRAIGHT: return   cSouth;
    case RIGHT:    return  -cWest;
    case LEFT:     return   cWest;
    case REVERSE:  return  -cSouth;
  }
  std::cout << "WARNING: Shouldn't happen" << std::endl; // TODO: Throw
  return 0.0f;
}

void TileSurface::coord(float x, float y) {
  switch (cRotation) {
    case STRAIGHT: glTexCoord2f( x,  y); break;
    case RIGHT:    glTexCoord2f( y,  x); break;
    case REVERSE:  glTexCoord2f(-x, -y); break;
    case LEFT:     glTexCoord2f(-y, -x); break;
  }
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
    (*cTexture)->set();
    glBegin(GL_QUADS);

    switch (cFacing) {
      case ITileSurface::UP: {
/*        float mEast  = getEastTextureCoord();
        float mWest  = getWestTextureCoord();
        float mNorth = getNorthTextureCoord();
        float mSouth = getSouthTextureCoord();
        cTexture->texCoord2f(mEast, mNorth); glVertex3f(xe, ys, xsye);
        cTexture->texCoord2f(mEast, mSouth); glVertex3f(xe, ye, xeye);
        cTexture->texCoord2f(mWest, mSouth); glVertex3f(xs, ye, xeys);
        cTexture->texCoord2f(mWest, mNorth); glVertex3f(xs, ys, xsys);*/
        coord(cEast + 1, cNorth + 1); glVertex3f(xe, ys, xsye);
        coord(cEast + 1, cSouth);     glVertex3f(xe, ye, xeye);
        coord(cWest,     cSouth);     glVertex3f(xs, ye, xeys);
        coord(cWest,     cNorth + 1); glVertex3f(xs, ys, xsys);
/*        glColor3f(1.0, 1.0, 0.0); cTexture->texCoord2f(cEast + 1, cNorth + 1); glVertex3f(xe, ys, xsye);
        glColor3f(0.0, 1.0, 1.0); cTexture->texCoord2f(cEast + 1, cSouth);     glVertex3f(xe, ye, xeye);
        glColor3f(1.0, 0.0, 1.0); cTexture->texCoord2f(cWest,     cSouth);     glVertex3f(xs, ye, xeys);
        glColor3f(1.0, 1.0, 1.0); cTexture->texCoord2f(cWest,     cNorth + 1);  glVertex3f(xs, ys, xsys);*/
        break;
      }

      case ITileSurface::DOWN: {
/*        cTexture->texCoord2f(cWest,     cNorth + 1); glVertex3f(xs, ye, xeys);
        cTexture->texCoord2f(cEast + 1, cNorth + 1); glVertex3f(xe, ye, xeye);
        cTexture->texCoord2f(cEast + 1, cSouth);     glVertex3f(xe, ys, xsye);
        cTexture->texCoord2f(cWest,     cSouth);     glVertex3f(xs, ys, xsys);*/
  /*      glColor3f(0.0, 1.0, 0.0); cTexture->texCoord2f(cWest,     cNorth + 1); glVertex3f(xs, ye, xeys);
        glColor3f(1.0, 1.0, 0.0); cTexture->texCoord2f(cEast + 1, cNorth + 1); glVertex3f(xe, ye, xeye);
        glColor3f(1.0, 0.0, 0.0); cTexture->texCoord2f(cEast + 1, cSouth);     glVertex3f(xe, ys, xsye);
        glColor3f(0.0, 0.0, 1.0); cTexture->texCoord2f(cWest,     cSouth);     glVertex3f(xs, ys, xsys);*/
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

Vertex* TileSurface::getBoundaryCrossingPoint(Vertex& start, Vertex& end, float* mLowestGradient, float infinity) {
  *mLowestGradient = 2.0f;
  float mXMovement = end.x - start.x;
  float mYMovement = end.y - start.y;
  float mSouth = cSouth - IsoRealmsConstants::BLOCK_RADIUS;
  float mWest  = cWest  - IsoRealmsConstants::BLOCK_RADIUS;
  float mNorth = cNorth + IsoRealmsConstants::BLOCK_RADIUS;
  float mEast  = cEast  + IsoRealmsConstants::BLOCK_RADIUS;
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

bool TileSurface::contains(Vertex& location) {
  float mSouthEdge  = cSouth - IsoRealmsConstants::BLOCK_RADIUS;
  float mWestEdge   = cWest  - IsoRealmsConstants::BLOCK_RADIUS;
  float mNorthEdge  = cNorth + IsoRealmsConstants::BLOCK_RADIUS;
  float mEastEdge   = cEast  + IsoRealmsConstants::BLOCK_RADIUS;
  if (location.y >= mSouthEdge  && location.y < mNorthEdge && location.x >= mWestEdge && location.x < mEastEdge) {
    float mSurfaceHeight = getHeightAt(location.x, location.y);
    return location.z <= mSurfaceHeight && location.z >= mSurfaceHeight - 0.5f;
  }
  return false;
}

ICollisionData* TileSurface::getRollingEvent(Vertex& start, Vertex& end) {
  if (cCondition != NULL && !cCondition->isTrue()) {
    return new SurfaceCollisionEvent(this, ICollisionData::SURFACE_LEAVE, new Vertex(start), -cWestEastSlope, -cNorthSouthSlope, 0.0f);
  }
  
  float mGradient;
  Vertex* mLeavePoint = getBoundaryCrossingPoint(start, end, &mGradient, INFINITY);
  if (mLeavePoint != NULL) {
    return new SurfaceCollisionEvent(this, ICollisionData::SURFACE_LEAVE, mLeavePoint, -cWestEastSlope, -cNorthSouthSlope, mGradient);
  }
  return NULL;
}

ICollisionData* TileSurface::getCollision(Vertex& start, Vertex& end) {
  if (cCondition == NULL || cCondition->isTrue()) {
    if (contains(start)) {
      float mStartDifference = start.z - getHeightAt(start.x, start.y);
      float mEndDifference = end.z - getHeightAt(end.x, end.y);
      if (mStartDifference >= mEndDifference) {
        Vertex* mEnterPoint = new Vertex(start);
        mEnterPoint->x = max(min(mEnterPoint->x, nextafterf(cEast  + IsoRealmsConstants::BLOCK_RADIUS, -INFINITY)), nextafterf(cWest  - IsoRealmsConstants::BLOCK_RADIUS, INFINITY));
        mEnterPoint->y = max(min(mEnterPoint->y, nextafterf(cNorth + IsoRealmsConstants::BLOCK_RADIUS, -INFINITY)), nextafterf(cSouth - IsoRealmsConstants::BLOCK_RADIUS, INFINITY));
        return new SurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mEnterPoint, -cWestEastSlope, -cNorthSouthSlope, 0.0f);
      }
    } else {
      float mGradient;
      Vertex* mEnterPoint = getBoundaryCrossingPoint(start, end, &mGradient, -INFINITY);
      if (mEnterPoint != NULL) {
        float mEnterHeight = getHeightAt(mEnterPoint->x, mEnterPoint->y);
        if (mEnterPoint->z <= mEnterHeight && mEnterPoint->z >= mEnterHeight - 0.5f) {
          return new SurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mEnterPoint, -cWestEastSlope, -cNorthSouthSlope, mGradient);
        }
      }
    }

    float mStartHeight = getHeightAt(start.x, start.y);
    float mEndHeight = getHeightAt(end.x, end.y);
    if ((start.z > mStartHeight) != (end.z > mEndHeight) && start.z > mStartHeight) {
      float mGradient = Collision::getCrossingPoint(start.z, end.z, mStartHeight, mEndHeight);
      double mXImpact = start.x + (end.x - start.x) * mGradient;
      double mYImpact = start.y + (end.y - start.y) * mGradient;
      double mZImpact = getHeightAt(mXImpact, mYImpact);
      Vertex* mImpactLocation = new Vertex(mXImpact, mYImpact, mZImpact);
      if (alligned(round(mImpactLocation->x), round(mImpactLocation->y))) {
        SurfaceCollisionEvent *mEvent = new SurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mImpactLocation, -cWestEastSlope, -cNorthSouthSlope, mGradient);
        return mEvent;
      }
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
  cElement->setValue();
  cBlockTypeProperties->executeContactScript();
  cElement->unsetValue();
}

void TileSurface::notifyImpact() {
  cElement->setValue();
  cBlockTypeProperties->executeImpactScript();
  cElement->unsetValue();
}

float TileSurface::getSurfaceFriction() {
  return cBlockTypeProperties->getSurfaceFriction();
}

float TileSurface::getSurfaceGrip() {
  return cBlockTypeProperties->getSurfaceGrip();
}

float TileSurface::getSurfaceBounce() {
  return cBlockTypeProperties->getSurfaceBounce();
}

IRollableSurface::RespawnPossibility TileSurface::getRespawnPossibility() {
  return cBlockTypeProperties->isRespawnAllowed() 
       ? (cCondition != NULL ? IRollableSurface::CONDITIONAL : IRollableSurface::YES)
       : IRollableSurface::NO;
}

bool TileSurface::isRespawnPossibleNow() {
  return cBlockTypeProperties->isRespawnAllowed() && (cCondition == NULL || cCondition->isTrue());
}

void TileSurface::getRestingLocation(Vertex& location) {
  float mYEdgeLocation = cNorthSouthSlope > 0 ? nextafterf(cSouth - IsoRealmsConstants::BLOCK_RADIUS,  INFINITY)
                       : cNorthSouthSlope < 0 ? nextafterf(cNorth + IsoRealmsConstants::BLOCK_RADIUS, -INFINITY)
                       :                        location.y;
  float mXEdgeLocation = cWestEastSlope > 0 ? nextafterf(cWest - IsoRealmsConstants::BLOCK_RADIUS,  INFINITY)
                       : cWestEastSlope < 0 ? nextafterf(cEast + IsoRealmsConstants::BLOCK_RADIUS, -INFINITY)
                       :                      location.x;
  std::cout << "X EDGE: " << mXEdgeLocation << std::endl;
  float mDistanceToY = mYEdgeLocation - location.y;
  float mDistanceToX = mXEdgeLocation - location.x;
  std::cout << "Time to reach X: " << mDistanceToX << " / " << cWestEastSlope << " == " << fabs(mDistanceToX / cWestEastSlope) << std::endl;
  std::cout << "Time to reach Y: " << fabs(mDistanceToY / cNorthSouthSlope) << std::endl;
  if (fabs(mDistanceToY / cNorthSouthSlope) < fabs(mDistanceToX / cWestEastSlope) || isnan(fabs(mDistanceToX / cWestEastSlope))) {
    std::cout << "Closer to Y" << std::endl;
    // Reach Y edge first
    location.y = mYEdgeLocation;
    // TODO: location.x
  } else {
    std::cout << "Closer to X" << std::endl;
    // Reach X edge first
    location.x = mXEdgeLocation;
    // TODO: location.y
  }
}

BlockArea* TileSurface::getBounds() {
  BlockLocation* mStart = new BlockLocation(cWest, cSouth, 0);
  BlockLocation* mEnd = new BlockLocation(cEast, cNorth, 0);
  return new BlockArea(*mStart, *mEnd);
}

BlockArea* TileSurface::getCoverage() {
  BlockLocation mStartLocation(cWest, cSouth, cHeight);
  // TODO: Not sure if next line is correct
  int mTopHeight = cHeight + (((cEast - cWest) + 1) * cWestEastSlope) + (((cNorth - cSouth) + 1) * cNorthSouthSlope);
  BlockLocation mEndLocation(cEast, cNorth, mTopHeight);
  return new BlockArea(mStartLocation, mEndLocation);
}

void TileSurface::destroyCoverage(BlockArea* coverage) {
  delete coverage;
}

bool TileSurface::alligned(int x, int y) {
  return y >= cSouth && y <= cNorth && x >= cWest && x <= cEast;
}

TileSurface::~TileSurface() {
  if (cCondition != NULL) {
    delete cCondition; // TODO: The surface processor created this... should we really delete it?
  }
}

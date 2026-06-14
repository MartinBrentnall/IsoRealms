/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "InfiniteSurface.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/World.h"
#include "Modules/Equilibria/World/Object/Zone/Zone.h"

namespace IsoRealms::Equilibria {
  InfiniteSurface::InfiniteSurface(Zone& zone, float height) :
            cZone(zone),
            cDefHeight(height) {
  }
  
  std::unique_ptr<CollisionData> InfiniteSurface::getCollision(LiteralVertex& start, LiteralVertex& end, float stepHeight, double startTime, double endTime) {
    float mStartX = cZone.getStartX() - 0.5f;
    float mEndX   = cZone.getEndX()   + 0.5f;
    float mStartY = cZone.getStartY() - 0.5f;
    float mEndY   = cZone.getEndY()   + 0.5f;
    if (contains(start, stepHeight)) {

      // Starting point is already within the surface.
      // We ignore this if the craft is moving away from the surface.
      if (start.z >= end.z) {
        LiteralVertex mEnterPoint(start);

        // Make sure the collision point is really confined to the area of the surface
        float mDistanceFromStartX = mEnterPoint.x - mStartX;
        float mDistanceFromEndX   = mEndX - mEnterPoint.x;
        float mDistanceFromStartY = mEnterPoint.y - mStartY;
        float mDistanceFromEndY   = mEndY - mEnterPoint.y;
        if (mDistanceFromStartX < mDistanceFromEndX) {
          if (mDistanceFromStartX < mDistanceFromStartY) {
            if (mDistanceFromStartX < mDistanceFromEndY) {
              // Closest to west.
              mEnterPoint.x = std::min(static_cast<float>(mEnterPoint.x), static_cast<float>(nextafter(mStartX, -INFINITY)));
            } else {
              // Closest to north.
              mEnterPoint.y = std::max(static_cast<float>(mEnterPoint.y), static_cast<float>(nextafter(mEndY, INFINITY)));
            }
          }
        } else if (mDistanceFromEndX < mDistanceFromStartY) {
          if (mDistanceFromEndX < mDistanceFromEndY) {
            // Closest to east.
            mEnterPoint.x = std::max(static_cast<float>(mEnterPoint.x), static_cast<float>(nextafter(mEndX, INFINITY)));
          } else {
            // Closest to north.
            mEnterPoint.y = std::max(static_cast<float>(mEnterPoint.y), static_cast<float>(nextafter(mEndY, INFINITY)));
          }
        } else if (mDistanceFromStartY < mDistanceFromEndY) {
          // Closest to south.
          mEnterPoint.y = std::min(static_cast<float>(mEnterPoint.y), static_cast<float>(nextafter(mStartY, -INFINITY)));
        } else {
          // Closest to north.
          mEnterPoint.y = std::max(static_cast<float>(mEnterPoint.y), static_cast<float>(nextafter(mEndY, INFINITY)));
        }
        return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_MOUNT, mEnterPoint, 0.0f, 0.0f, 0.0f);
      }
    } else {

      // Test if the craft crossed the X/Y boundary of this surface.
      double mGradient;
      std::unique_ptr<LiteralVertex> mEnterPoint = getBoundaryCrossingPoint(start, end, &mGradient, INFINITY);
      if (mEnterPoint != nullptr) {
        if (mEnterPoint->getZ() <= cDefHeight && mEnterPoint->getZ() >= cDefHeight - stepHeight) {
          mEnterPoint->z = cDefHeight;
          return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_MOUNT, *mEnterPoint.get(), 0.0f, 0.0f, mGradient);
        }
      }
    }

    double mStartHeight = getHeightAt(start.x, start.y);
    double mEndHeight = getHeightAt(end.x, end.y);
    if ((start.z > mStartHeight) != (end.z > mEndHeight) && start.z > mStartHeight) {
      double mGradient = CollisionUtils::getCrossingPoint(start.z, end.z, mStartHeight, mEndHeight);
      double mXImpact = start.x + (end.x - start.x) * mGradient;
      double mYImpact = start.y + (end.y - start.y) * mGradient;
      double mZImpact = getHeightAt(mXImpact, mYImpact);
      LiteralVertex mImpactLocation(mXImpact, mYImpact, mZImpact);
      if (alligned(round(mImpactLocation.getX()), round(mImpactLocation.getY()))) {
        return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_MOUNT, mImpactLocation, 0.0f, 0.0f, mGradient);
      }
    }

    // No event
    return nullptr;
  }
    
  std::unique_ptr<CollisionData> InfiniteSurface::getRollingEvent(LiteralVertex& start, LiteralVertex& end, double startTime, double endTime) {

    // Check if the object has moved off the surface.
    double mGradient;
    std::unique_ptr<LiteralVertex> mLeavePoint = getBoundaryCrossingPoint(start, end, &mGradient, -INFINITY);
    if (mLeavePoint != nullptr) {
      std::cout << "Leaving infinite surface: " << start.y << " --> " << end.y << " (leave point: " << mLeavePoint->x << "," << mLeavePoint->y << "..." << std::endl;
      getBoundaryCrossingPoint(start, end, &mGradient, -INFINITY);
      return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_LEAVE, *mLeavePoint.get(), 0.0f, 0.0f, mGradient);
    }

    // Object is still on the surface.
    return nullptr;
  }
    
  float InfiniteSurface::getHeightAt(float x, float y) const {
    return cDefHeight;
  }
    
  bool InfiniteSurface::contains(LiteralVertex& location, float stepHeight) {
    double mSouthEdge  = cZone.getStartY() - 0.5f;
    double mWestEdge   = cZone.getStartX() - 0.5f;
    double mNorthEdge  = cZone.getEndY()   + 0.5f;
    double mEastEdge   = cZone.getEndX()   + 0.5f;
    if (location.y < mSouthEdge || location.y >= mNorthEdge || location.x < mWestEdge || location.x >= mEastEdge) {
      return location.z <= cDefHeight && location.z >= cDefHeight - stepHeight;
    }
    return false;
  }

  float InfiniteSurface::getXAcceleration(float, float) {
    return 0.0f;
  }

  float InfiniteSurface::getYAcceleration(float, float) {
    return 0.0f;
  }

  void InfiniteSurface::notifyContact() {
    // Nothing to do
  }

  void InfiniteSurface::notifyImpact() {
    // Nothing to do
  }

  float InfiniteSurface::getSurfaceFriction() {
    return 0.0015f;
  }

  float InfiniteSurface::getSurfaceGrip() {
    return 1.0f;
  }

  float InfiniteSurface::getSurfaceBounce() {
    return 0.0f;
  }

  InfiniteSurface::Respawn InfiniteSurface::isRespawnAllowed() {
    return Respawn::NO;
  }

  bool InfiniteSurface::isRespawnPossible() {
    return false;
  }

  void InfiniteSurface::getRestingLocation(LiteralVertex& location) {
    // Nothing to do
  }

  void InfiniteSurface::saveCache(std::ostream& cache, bool physical) {
    // Nothing to do
  }

  bool InfiniteSurface::isSolid() {
    return true;
  }
  
  void InfiniteSurface::adjustPosition(LiteralVertex& location, double milliseconds) {
    // Nothing to do.
  }

  IWorldObject* InfiniteSurface::getOwner() {
    return nullptr;
  }

  Zone& InfiniteSurface::getZone() {
    return cZone;
  }
  
  int InfiniteSurface::getXStart() const {
    return 0;
  }

  int InfiniteSurface::getXEnd() const {
    return 0;
  }

  int InfiniteSurface::getYStart() const {
    return 0;
  }

  int InfiniteSurface::getYEnd() const {
    return 0;
  }
  
  void InfiniteSurface::render() const {
    // Not supported.
  }
  
  void InfiniteSurface::renderOutline() const {
    // Not supported.
  }
  
  void InfiniteSurface::renderHighlight() const {
    // Not supported.
  }
  
  int InfiniteSurface::getSurfaceCellHeight(int x, int y) const {
    return cDefHeight;
  }
  
  int InfiniteSurface::getSurfaceCellElevation(int x, int y) const {
    return 0;
  }
  
  bool InfiniteSurface::alligned(int x, int y) const {
    return true;
  }
  
  std::vector<std::unique_ptr<IVisualElement>> InfiniteSurface::getStaticVisuals() {
    return std::vector<std::unique_ptr<IVisualElement>> {
    };
  }  

  std::unique_ptr<LiteralVertex> InfiniteSurface::getBoundaryCrossingPoint(LiteralVertex& start, LiteralVertex& end, double* lowestGradient, double infinity) {
    if (!contains(start, 0.5f) && !contains(end, 0.5f)) {
      return nullptr;
    }
    *lowestGradient = 2.0f;
    double mXMovement = end.x - start.x;
    double mYMovement = end.y - start.y;
    double mSouth = cZone.getStartY() - 0.5f;
    double mWest  = cZone.getStartX() - 0.5f;
    double mNorth = cZone.getEndY()   + 0.5f;
    double mEast  = cZone.getEndX()   + 0.5f;
    double mImpactX;
    double mImpactY;
    bool mXKnown = false;
    bool mYKnown = false;

    double mTempGradient = (mWest - start.x) / mXMovement;
    if (mTempGradient >= 0.0f && mTempGradient <= *lowestGradient) {
      double mWestYLocation = start.y + mYMovement * mTempGradient;
      if (mWestYLocation >= mSouth && mWestYLocation <= mNorth) {
        *lowestGradient = mTempGradient;
        mImpactX = nextafterf(mWest, -infinity);
        mXKnown = true;
      }
    }

    mTempGradient = (mEast - start.x) / mXMovement;
    if (mTempGradient >= 0.0f && mTempGradient <= *lowestGradient) {
      double mEastYLocation = start.y + mYMovement * mTempGradient;
      if (mEastYLocation >= mSouth && mEastYLocation <= mNorth) {
        *lowestGradient = mTempGradient;
        mImpactX = nextafterf(mEast, infinity);
        mXKnown = true;
      }
    }

    mTempGradient = (mNorth - start.y) / mYMovement;
    if (mTempGradient >= 0.0f && mTempGradient <= *lowestGradient) {
      double mNorthXLocation = start.x + mXMovement * mTempGradient;
      if (mNorthXLocation >= mWest && mNorthXLocation <= mEast) {
        *lowestGradient = mTempGradient;
        mImpactY = nextafterf(mNorth, infinity);
        mXKnown = false;
        mYKnown = true;
      }
    }

    mTempGradient = (mSouth - start.y) / mYMovement;
    if (mTempGradient >= 0.0f && mTempGradient <= *lowestGradient) {
      double mSouthXLocation = start.x + mXMovement * mTempGradient;
      if (mSouthXLocation >= mWest && mSouthXLocation <= mEast) {
        *lowestGradient = mTempGradient;
        mImpactY = nextafterf(mSouth, -infinity);
        mXKnown = false;
        mYKnown = true;
      }
    }

    double mMovementZ = end.z - start.z;
    if (*lowestGradient <= 1.0f) {
      if (!mXKnown) {
        mImpactX = start.x + mXMovement * *lowestGradient;
      }
      if (!mYKnown) {
        mImpactY = start.y + mYMovement * *lowestGradient;
      }
      double mImpactZ = start.z + mMovementZ * *lowestGradient;
      return std::make_unique<LiteralVertex>(mImpactX, mImpactY, mImpactZ);
    }

    // Line doesn't cross boundary
    return nullptr;
  }
}

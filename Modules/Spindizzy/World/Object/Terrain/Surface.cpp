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
#include "Surface.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/TerrainType/TerrainType.h"
#include "Modules/Spindizzy/World/Object/Zone/Zone.h"

#include "Terrain.h"

namespace IsoRealms::Spindizzy {
  Surface::Surface(int north, int east, int south, int west, int height, int westEastSlope, int northSouthSlope, Direction facing, const std::optional<Condition>& condition, TerrainType* terrainType, IWorldObject& owner) :
            cOwner(owner),
            cType(*terrainType),
            cDefStartX(west),
            cDefEndX(east),
            cDefStartY(south),
            cDefEndY(north),
            cDefZ(height),
            cDefSlopeX(westEastSlope),
            cDefSlopeY(northSouthSlope),
            cDefFacing(facing),
            cDefCondition(condition) {
  }

  Surface::Surface(std::ifstream& cache, std::vector<ConditionElement*> elements, TerrainType& terrainType, IWorldObject& owner) :
            cOwner(owner),
            cType(terrainType) {
    cache.read(reinterpret_cast<char*>(&cDefStartY), sizeof(cDefStartY));
    cache.read(reinterpret_cast<char*>(&cDefEndY),   sizeof(cDefEndY));
    cache.read(reinterpret_cast<char*>(&cDefStartX), sizeof(cDefStartX));
    cache.read(reinterpret_cast<char*>(&cDefEndX),   sizeof(cDefEndX));
    cache.read(reinterpret_cast<char*>(&cDefZ),      sizeof(cDefZ));
    cache.read(reinterpret_cast<char*>(&cDefSlopeX), sizeof(cDefSlopeX));
    cache.read(reinterpret_cast<char*>(&cDefSlopeY), sizeof(cDefSlopeY));
    cache.read(reinterpret_cast<char*>(&cDefFacing), sizeof(cDefFacing));

    char mNextEntity;
    cache.read(reinterpret_cast<char*>(&mNextEntity), sizeof(mNextEntity));
    if (mNextEntity == Terrain::CACHE_CONDITION) {
      cDefCondition = Condition(cache, elements, Terrain::CACHE_CONDITION, Terrain::CACHE_CONDITION_ELEMENT, Terrain::CACHE_CONDITION_END);
    }
  }

  std::optional<Condition>& Surface::getCondition() {
    return cDefCondition;
  }

  int Surface::getZ() const {
    return cDefZ;
  }

  int Surface::getXSlope() const {
    return cDefSlopeX;
  }

  int Surface::getYSlope() const {
    return cDefSlopeY;
  }

  ISurface::Direction Surface::getFacing() const {
    return cDefFacing;
  }

  int Surface::getSurfaceCellHeight(int x, int y) const {
    return abs(((cDefSlopeX   >= 0 ? cDefStartX  : cDefEndX)  - x) * cDefSlopeX)
         + abs(((cDefSlopeY >= 0 ? cDefStartY : cDefEndY) - y) * cDefSlopeY)
         + cDefZ;
  }

  int Surface::getSurfaceCellElevation(int x, int y) const {
    return abs(cDefSlopeX) + abs(cDefSlopeY);
  }

  void Surface::render() const {
    if (!cDefCondition.has_value() || cDefCondition->isTrue()) {
      cType.getSurfacePattern()->render(cDefStartX - 0.5f, cDefEndX + 0.5f, cDefStartY - 0.5f, cDefEndY + 0.5f, cDefZ, cDefSlopeX, cDefSlopeY, cDefFacing);
    }
  }

  void Surface::renderOutline() const {
    double mXStart = cDefStartX  - 0.5f;
    double mXEnd   = cDefEndX  + 0.5f;
    double mYStart = cDefStartY - 0.5f;
    double mYEnd   = cDefEndY + 0.5f;

    double mHeightSouthWest = cDefZ * 0.5f;
    double mHeightNorthWest = cDefZ * 0.5f;
    double mHeightNorthEast = cDefZ * 0.5f;
    double mHeightSouthEast = cDefZ * 0.5f;
    
    if (cDefSlopeX < 0) {
      mHeightSouthWest += (abs(cDefSlopeX) * 0.5f * (mXEnd - mXStart));
      mHeightSouthEast += (abs(cDefSlopeX) * 0.5f * (mXEnd - mXStart));
    } else {
      mHeightNorthWest += (abs(cDefSlopeX) * 0.5f * (mXEnd - mXStart));
      mHeightNorthEast += (abs(cDefSlopeX) * 0.5f * (mXEnd - mXStart));
    }

    if (cDefSlopeY < 0) {
      mHeightSouthWest += (abs(cDefSlopeY) * 0.5f * (mYEnd - mYStart));
      mHeightNorthWest += (abs(cDefSlopeY) * 0.5f * (mYEnd - mYStart));
    } else {
      mHeightSouthEast += (abs(cDefSlopeY) * 0.5f * (mYEnd - mYStart));
      mHeightNorthEast += (abs(cDefSlopeY) * 0.5f * (mYEnd - mYStart));
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.0f, 1.0f, 1.0f);
    glLineWidth(6.0f);
    glBegin(GL_LINE_LOOP);
    switch (cDefFacing) {
      case Direction::UP: {
        glVertex3f(mXEnd,   mYStart, mHeightNorthWest);
        glVertex3f(mXEnd,   mYEnd,   mHeightNorthEast);
        glVertex3f(mXStart, mYEnd,   mHeightSouthEast);
        glVertex3f(mXStart, mYStart, mHeightSouthWest);
        break;
      }

      case Direction::DOWN: {
        glVertex3f(mXStart, mYStart, mHeightSouthWest);
        glVertex3f(mXStart, mYEnd,   mHeightSouthEast);
        glVertex3f(mXEnd,   mYEnd,   mHeightNorthEast);
        glVertex3f(mXEnd,   mYStart, mHeightNorthWest);
        break;
      }
    }
    glEnd();
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
  }

  void Surface::renderHighlight() const {
    double mXStart = cDefStartX  - 0.5f;
    double mXEnd   = cDefEndX  + 0.5f;
    double mYStart = cDefStartY - 0.5f;
    double mYEnd   = cDefEndY + 0.5f;
    
    double mHeightSouthWest = cDefZ * 0.5f;
    double mHeightNorthWest = cDefZ * 0.5f;
    double mHeightNorthEast = cDefZ * 0.5f;
    double mHeightSouthEast = cDefZ * 0.5f;

    if (cDefSlopeX < 0) {
      mHeightSouthWest += (abs(cDefSlopeX) * 0.5f * (mXEnd - mXStart));
      mHeightSouthEast += (abs(cDefSlopeX) * 0.5f * (mXEnd - mXStart));
    } else {
      mHeightNorthWest += (abs(cDefSlopeX) * 0.5f * (mXEnd - mXStart));
      mHeightNorthEast += (abs(cDefSlopeX) * 0.5f * (mXEnd - mXStart));
    }

    if (cDefSlopeY < 0) {
      mHeightSouthWest += (abs(cDefSlopeY) * 0.5f * (mYEnd - mYStart));
      mHeightNorthWest += (abs(cDefSlopeY) * 0.5f * (mYEnd - mYStart));
    } else {
      mHeightSouthEast += (abs(cDefSlopeY) * 0.5f * (mYEnd - mYStart));
      mHeightNorthEast += (abs(cDefSlopeY) * 0.5f * (mYEnd - mYStart));
    }
    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    switch (cDefFacing) {
      case Direction::UP: {
        glVertex3f(mXEnd,   mYStart, mHeightNorthWest + 0.001f);
        glVertex3f(mXEnd,   mYEnd,   mHeightNorthEast + 0.001f);
        glVertex3f(mXStart, mYEnd,   mHeightSouthEast + 0.001f);
        glVertex3f(mXStart, mYStart, mHeightSouthWest + 0.001f);
        break;
      }

      case Direction::DOWN: {
        glVertex3f(mXStart, mYStart, mHeightSouthWest - 0.001f);
        glVertex3f(mXStart, mYEnd,   mHeightSouthEast - 0.001f);
        glVertex3f(mXEnd,   mYEnd,   mHeightNorthEast - 0.001f);
        glVertex3f(mXEnd,   mYStart, mHeightNorthWest - 0.001f);
        break;
      }
    }
    glEnd();
    glDisable(GL_BLEND);
  }

  float Surface::getHeightAt(float x, float y) const {
    return cDefSlopeX   * ((cDefSlopeX   > 0 ? x - cDefStartX  : -(cDefEndX  + 1 - x)) + 0.5f) +
           cDefSlopeY * ((cDefSlopeY > 0 ? y - cDefStartY : -(cDefEndY + 1 - y)) + 0.5f) + cDefZ;
  }

  std::unique_ptr<LiteralVertex> Surface::getBoundaryCrossingPoint(LiteralVertex& start, LiteralVertex& end, double* lowestGradient, double infinity) {
    if (contains(start, 0.5f) && contains(end, 0.5f)) {
      return nullptr;
    }
    *lowestGradient = 2.0f;
    double mXMovement = end.x - start.x;
    double mYMovement = end.y - start.y;
    double mSouth = cDefStartY - 0.5f;
    double mWest  = cDefStartX  - 0.5f;
    double mNorth = cDefEndY + 0.5f;
    double mEast  = cDefEndX  + 0.5f;
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

  bool Surface::contains(LiteralVertex& location, float stepHeight) {
    double mSouthEdge  = cDefStartY - 0.5f;
    double mWestEdge   = cDefStartX  - 0.5f;
    double mNorthEdge  = cDefEndY + 0.5f;
    double mEastEdge   = cDefEndX  + 0.5f;
    if (location.y >= mSouthEdge && location.y < mNorthEdge && location.x >= mWestEdge && location.x < mEastEdge) {
      double mSurfaceHeight = getHeightAt(location.x, location.y);
      return location.z <= mSurfaceHeight && location.z >= mSurfaceHeight - stepHeight;
    }
    return false;
  }

  std::unique_ptr<CollisionData> Surface::getRollingEvent(LiteralVertex& start, LiteralVertex& end, double startTime, double endTime) {
    
    // Check if surface has disappeared or object is no longer on it.
    if ((cDefCondition.has_value() && !cDefCondition->isTrue()) || !contains(start, 0.5)) {
      return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_LEAVE, LiteralVertex(start), -cDefSlopeX, -cDefSlopeY, 0.0);
    }
    
    // Check if the object has moved off the surface.
    double mGradient;
    std::unique_ptr<LiteralVertex> mLeavePoint = getBoundaryCrossingPoint(start, end, &mGradient, INFINITY);
    if (mLeavePoint != nullptr) {
      return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_LEAVE, *mLeavePoint.get(), -cDefSlopeX, -cDefSlopeY, mGradient);
    }
    
    // Object is still on the surface.
    return nullptr;
  }

  std::vector<std::unique_ptr<IVisualElement>> Surface::getStaticVisuals() {
    return cType.getSurfacePattern()->getStaticVisuals(this);
  }

  std::unique_ptr<CollisionData> Surface::getCollision(LiteralVertex& start, LiteralVertex& end, float stepHeight, double startTime, double endTime) {
    
    // Collisions only happen when surface is present
    if (!cDefCondition.has_value() || cDefCondition->isTrue()) {
      
      
      if (contains(start, stepHeight)) {
        // Starting point is already within the surface.
        // We ignore this if the craft is moving away from the surface.
        double mStartDifference = start.z - getHeightAt(start.x, start.y);
        double mEndDifference   = end.z   - getHeightAt(end.x, end.y);
        if (mStartDifference >= mEndDifference) {
          LiteralVertex mEnterPoint(start);
          
          // Make sure the collision point is really confined to the area of the surface
          mEnterPoint.x = std::clamp(mEnterPoint.x, static_cast<double>(nextafter(cDefStartX - 0.5f, INFINITY)), static_cast<double>(nextafter(cDefEndX + 0.5f, -INFINITY)));
          mEnterPoint.y = std::clamp(mEnterPoint.y, static_cast<double>(nextafter(cDefStartY - 0.5f, INFINITY)), static_cast<double>(nextafter(cDefEndY + 0.5f, -INFINITY)));
          return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_MOUNT, mEnterPoint, -cDefSlopeX, -cDefSlopeY, 0.0f);
        }
      } else {
        
        // Test if the craft crossed the X/Y boundary of this surface.
        double mGradient;
        std::unique_ptr<LiteralVertex> mEnterPoint = getBoundaryCrossingPoint(start, end, &mGradient, -INFINITY);
        if (mEnterPoint != nullptr) {
          double mEnterHeight = getHeightAt(mEnterPoint->getX(), mEnterPoint->getY());
          if (mEnterPoint->getZ() <= mEnterHeight && mEnterPoint->getZ() >= mEnterHeight - stepHeight) {
            mEnterPoint->z = mEnterHeight;
            return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_MOUNT, *mEnterPoint.get(), -cDefSlopeX, -cDefSlopeY, mGradient);
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
          return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_MOUNT, mImpactLocation, -cDefSlopeX, -cDefSlopeY, mGradient);
        }
      }
    }

    // No event
    return nullptr;
  }

  float Surface::getXAcceleration(float x, float y) {
    return -cDefSlopeX;
  }

  float Surface::getYAcceleration(float x, float y) {
    return -cDefSlopeY;
  }

  void Surface::notifyContact() {
    cType.executeContactScript();
  }

  void Surface::notifyImpact() {
    cType.executeImpactScript();
  }

  float Surface::getSurfaceFriction() {
    return cType.getSurfaceFriction();
  }

  float Surface::getSurfaceGrip() {
    return cType.getSurfaceGrip();
  }

  float Surface::getSurfaceBounce() {
    return cType.getSurfaceBounce();
  }

  ISurface::Respawn Surface::isRespawnAllowed() {
    return cType.isRespawnAllowed()
        ? (cDefCondition.has_value() ? Respawn::CONDITIONAL : Respawn::YES)
        : Respawn::NO;
  }

  bool Surface::isRespawnPossible() {
    return cType.isRespawnAllowed() && (!cDefCondition.has_value() || cDefCondition->isTrue());
  }

  void Surface::getRestingLocation(LiteralVertex& location) {
    double mYEdgeLocation = cDefSlopeY > 0 ? nextafter(cDefStartY - 0.5f,  INFINITY)
                          : cDefSlopeY < 0 ? nextafter(cDefEndY + 0.5f, -INFINITY)
                          :                        location.y;
    double mXEdgeLocation = cDefSlopeX > 0 ? nextafter(cDefStartX - 0.5f,  INFINITY)
                          : cDefSlopeX < 0 ? nextafter(cDefEndX + 0.5f, -INFINITY)
                          :                      location.x;
    double mDistanceToY = mYEdgeLocation - location.y;
    double mDistanceToX = mXEdgeLocation - location.x;
    if (fabs(mDistanceToY / cDefSlopeY) < fabs(mDistanceToX / cDefSlopeX) || std::isnan(fabs(mDistanceToX / cDefSlopeX))) {
      // Reach Y edge first
      location.y = mYEdgeLocation;
      // TODO: location.x
    } else {
      // Reach X edge first
      location.x = mXEdgeLocation;
      // TODO: location.y
    }
  }

  IWorldObject* Surface::getOwner() {
    return &cOwner;
  }

  Zone& Surface::getZone() {
    return cOwner.getObjectZone();
  }

  bool Surface::isSolid() {
    return cType.isSolid();
  }
  
  void Surface::adjustPosition(LiteralVertex& location, double milliseconds) {
    // Nothing to do.
  }

  int Surface::getYStart() const {
    return cDefStartY;
  }

  int Surface::getYEnd() const {
    return cDefEndY;
  }

  int Surface::getXStart() const {
    return cDefStartX;
  }

  int Surface::getXEnd() const {
    return cDefEndX;
  }

  bool Surface::alligned(int x, int y) const {
    return y >= cDefStartY && y <= cDefEndY && x >= cDefStartX && x <= cDefEndX;
  }

  void Surface::saveCache(std::ostream& cache, bool physical) {
    cache.write(reinterpret_cast<const char*>(&Terrain::CACHE_SURFACE), sizeof(Terrain::CACHE_SURFACE));
    cache.write(reinterpret_cast<const char*>(&physical),               sizeof(physical));
    cache.write(reinterpret_cast<const char*>(&cDefStartY),             sizeof(cDefStartY));
    cache.write(reinterpret_cast<const char*>(&cDefEndY),               sizeof(cDefEndY));
    cache.write(reinterpret_cast<const char*>(&cDefStartX),             sizeof(cDefStartX));
    cache.write(reinterpret_cast<const char*>(&cDefEndX),               sizeof(cDefEndX));
    cache.write(reinterpret_cast<const char*>(&cDefZ),                  sizeof(cDefZ));
    cache.write(reinterpret_cast<const char*>(&cDefSlopeX),             sizeof(cDefSlopeX));
    cache.write(reinterpret_cast<const char*>(&cDefSlopeY),             sizeof(cDefSlopeY));
    cache.write(reinterpret_cast<const char*>(&cDefFacing),             sizeof(cDefFacing));
    if (cDefCondition.has_value()) {
      cDefCondition->saveCache(cache, Terrain::CACHE_CONDITION, Terrain::CACHE_CONDITION_ELEMENT, Terrain::CACHE_CONDITION_END);
    } else {
      cache.write(reinterpret_cast<const char*>(&Terrain::CACHE_SURFACE_END), sizeof(Terrain::CACHE_SURFACE_END));
    }
  }
}

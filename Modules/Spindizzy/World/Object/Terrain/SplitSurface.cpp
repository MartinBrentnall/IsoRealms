/*
 * Copyright 2023 Martin Brentnall
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
#include "SplitSurface.h"

#include "Modules/Spindizzy/TerrainType/TerrainType.h"

#include "Terrain.h"

namespace IsoRealms::Spindizzy {
  SplitSurface::SplitSurface(bool splitDirection, int x, int y, int z, int southWestCorner, int southEastCorner, int northWestCorner, int northEastCorner, const std::optional<Condition>& condition, TerrainType& terrainType, IWorldObject& owner) :
            cType(terrainType),
            cOwner(owner),
            cDefX(x),
            cDefY(y),
            cDefZ(z),
            cDefCornerHeights{{southWestCorner, northWestCorner},
                              {southEastCorner, northEastCorner}},
            cDefAlternativeSplit(splitDirection),
            cDefCondition(condition) {
  }

  SplitSurface::SplitSurface(std::ifstream& cache, std::vector<ConditionElement*> elements, TerrainType& terrainType, IWorldObject& owner) :
            cType(terrainType),
            cOwner(owner) {
    cache.read(reinterpret_cast<char*>(&cDefX),                sizeof(cDefX));
    cache.read(reinterpret_cast<char*>(&cDefY),                sizeof(cDefY));
    cache.read(reinterpret_cast<char*>(&cDefZ),                sizeof(cDefZ));
    cache.read(reinterpret_cast<char*>(&cDefCornerHeights),    sizeof(cDefCornerHeights));
    cache.read(reinterpret_cast<char*>(&cDefAlternativeSplit), sizeof(cDefAlternativeSplit));

    char mNextEntity;
    cache.read(reinterpret_cast<char*>(&mNextEntity),      sizeof(mNextEntity));
    if (mNextEntity == Terrain::CACHE_CONDITION) {
      cDefCondition = std::make_optional<Condition>(cache, elements, Terrain::CACHE_CONDITION, Terrain::CACHE_CONDITION_ELEMENT, Terrain::CACHE_CONDITION_END);
    }
  }

  std::optional<Condition>& SplitSurface::getCondition() {
    return cDefCondition;
  }

  bool SplitSurface::isAlternativeSplit() const {
    return cDefAlternativeSplit;
  }

  int SplitSurface::getX() const {
    return cDefX;
  }

  int SplitSurface::getY() const {
    return cDefY;
  }

  int SplitSurface::getZ() const {
    return cDefZ;
  }

  int SplitSurface::getHeightSW() const {
    return cDefCornerHeights[0][0];
  }

  int SplitSurface::getHeightSE() const {
    return cDefCornerHeights[1][0];
  }

  int SplitSurface::getHeightNW() const {
    return cDefCornerHeights[0][1];
  }

  int SplitSurface::getHeightNE() const {
    return cDefCornerHeights[1][1];
  }

  int SplitSurface::getSurfaceCellHeight(int x, int y) const {
    return cDefZ;
  }

  int SplitSurface::getSurfaceCellElevation(int x, int y) const {
    int mHighestA = cDefCornerHeights[0][0] > cDefCornerHeights[0][1] ? cDefCornerHeights[0][0] : cDefCornerHeights[0][1];
    int mHighestB = cDefCornerHeights[1][0] > cDefCornerHeights[1][1] ? cDefCornerHeights[1][0] : cDefCornerHeights[1][1];
    return mHighestA > mHighestB ? mHighestA : mHighestB;
  }

  void SplitSurface::render() const {
    if (!cDefCondition.has_value() || cDefCondition->isTrue()) {
      cType.getSurfacePattern()->render(cDefX, cDefY, cDefZ, cDefCornerHeights[0][0], cDefCornerHeights[0][1], cDefCornerHeights[1][0], cDefCornerHeights[1][1], false);
    }
  }

  void SplitSurface::renderOutline() const {
    float mNorthWest = (cDefZ + cDefCornerHeights[0][1]) * 0.5f;
    float mNorthEast = (cDefZ + cDefCornerHeights[1][1]) * 0.5f;
    float mSouthEast = (cDefZ + cDefCornerHeights[1][0]) * 0.5f;
    float mSouthWest = (cDefZ + cDefCornerHeights[0][0]) * 0.5f;
    float mWest  = cDefX - 0.5f;
    float mEast  = cDefX + 0.5f;
    float mSouth = cDefY - 0.5f;
    float mNorth = cDefY + 0.5f;
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.0f, 1.0f, 1.0f);
    glLineWidth(6.0f);
    glBegin(GL_LINE_LOOP);
    if (cDefAlternativeSplit) {
      glVertex3f(mEast, mNorth, mNorthEast);
      glVertex3f(mWest, mNorth, mNorthWest);
      glVertex3f(mWest, mSouth, mSouthWest);

      glVertex3f(mEast, mNorth, mNorthEast);
      glVertex3f(mEast, mSouth, mSouthEast);
      glVertex3f(mWest, mSouth, mSouthWest);
    } else {
      glVertex3f(mWest, mNorth, mNorthWest);
      glVertex3f(mEast, mNorth, mNorthEast);
      glVertex3f(mEast, mSouth, mSouthEast);

      glVertex3f(mWest, mNorth, mNorthWest);
      glVertex3f(mWest, mSouth, mSouthWest);
      glVertex3f(mEast, mSouth, mSouthEast);
    }
    glEnd();
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
  }

  void SplitSurface::renderHighlight() const {
    float mNorthWest = (cDefZ + cDefCornerHeights[0][1]) * 0.5f;
    float mNorthEast = (cDefZ + cDefCornerHeights[1][1]) * 0.5f;
    float mSouthEast = (cDefZ + cDefCornerHeights[1][0]) * 0.5f;
    float mSouthWest = (cDefZ + cDefCornerHeights[0][0]) * 0.5f;
    float mWest  = cDefX - 0.5f;
    float mEast  = cDefX + 0.5f;
    float mSouth = cDefY - 0.5f;
    float mNorth = cDefY + 0.5f;
    glEnable(GL_BLEND);
    glBegin(GL_TRIANGLES);
    if (cDefAlternativeSplit) {
      glVertex3f(mWest, mNorth, mNorthWest + 0.001f);
      glVertex3f(mWest, mSouth, mSouthWest + 0.001f);
      glVertex3f(mEast, mNorth, mNorthEast + 0.001f);

      glVertex3f(mWest, mSouth, mSouthWest + 0.001f);
      glVertex3f(mEast, mSouth, mSouthEast + 0.001f);
      glVertex3f(mEast, mNorth, mNorthEast + 0.001f);
    } else {
      glVertex3f(mWest, mNorth, mNorthWest + 0.001f);
      glVertex3f(mEast, mSouth, mSouthEast + 0.001f);
      glVertex3f(mEast, mNorth, mNorthEast + 0.001f);

      glVertex3f(mWest, mNorth, mNorthWest + 0.001f);
      glVertex3f(mWest, mSouth, mSouthWest + 0.001f);
      glVertex3f(mEast, mSouth, mSouthEast + 0.001f);
    }
    glEnd();
    glDisable(GL_BLEND);
  }

  std::vector<std::unique_ptr<IVisualElement>> SplitSurface::getStaticVisuals() {
    return cType.getSurfacePattern()->getStaticVisuals(this);
  }

  bool SplitSurface::alligned(int x, int y) const {
    return cDefX == x && cDefY == y;
  }

  void SplitSurface::saveCache(std::ostream& cache, bool physical) {
    cache.write(reinterpret_cast<const char*>(&Terrain::CACHE_SPLIT_SURFACE), sizeof(Terrain::CACHE_SPLIT_SURFACE));
    cache.write(reinterpret_cast<const char*>(&physical),                     sizeof(physical));
    cache.write(reinterpret_cast<const char*>(&cDefX),                        sizeof(cDefX));
    cache.write(reinterpret_cast<const char*>(&cDefY),                        sizeof(cDefY));
    cache.write(reinterpret_cast<const char*>(&cDefZ),                        sizeof(cDefZ));
    cache.write(reinterpret_cast<const char*>(&cDefCornerHeights),            sizeof(cDefCornerHeights));
    cache.write(reinterpret_cast<const char*>(&cDefAlternativeSplit),         sizeof(cDefAlternativeSplit));
    if (cDefCondition.has_value()) {
      cDefCondition->saveCache(cache, Terrain::CACHE_CONDITION, Terrain::CACHE_CONDITION_ELEMENT, Terrain::CACHE_CONDITION_END);
    } else {
      cache.write(reinterpret_cast<const char*>(&Terrain::CACHE_SURFACE_END), sizeof(Terrain::CACHE_SURFACE_END));
    }
  }

  bool SplitSurface::contains(LiteralVertex& location, float stepHeight) {
    float mSouthEdge  = cDefY - 0.5f;
    float mWestEdge   = cDefX - 0.5f;
    float mNorthEdge  = cDefY + 0.5f;
    float mEastEdge   = cDefX + 0.5f;
    if (location.y >= mSouthEdge  && location.y < mNorthEdge && location.x >= mWestEdge && location.x < mEastEdge) {
      float mSurfaceHeight = getHeightAt(location.x, location.y);
      return location.z <= mSurfaceHeight && location.z >= mSurfaceHeight - stepHeight;
    }
    return false;
  }

  float SplitSurface::getHeightAt(float x, float y, bool northSplit) const {
    x -= cDefX - 0.5f;
    y -= cDefY - 0.5f;
    if (cDefAlternativeSplit) {
      if (northSplit) {
        return cDefZ + (cDefCornerHeights[0][0] + (cDefCornerHeights[1][1] - cDefCornerHeights[0][1]) * x + (cDefCornerHeights[0][1] - cDefCornerHeights[0][0]) * y);
      } else {
        return cDefZ + (cDefCornerHeights[0][0] + (cDefCornerHeights[1][0] - cDefCornerHeights[0][0]) * x + (cDefCornerHeights[1][1] - cDefCornerHeights[1][0]) * y);
      }
    } else {
      if (northSplit) {
        return cDefZ + (cDefCornerHeights[1][0] + (cDefCornerHeights[1][1] - cDefCornerHeights[1][0]) * y + (cDefCornerHeights[0][1] - cDefCornerHeights[1][1]) * (1.0f - x));
      } else {
        return cDefZ + (cDefCornerHeights[0][0] + (cDefCornerHeights[1][0] - cDefCornerHeights[0][0]) * x + (cDefCornerHeights[0][1] - cDefCornerHeights[0][0]) * y);
      }
    }
  }

  float SplitSurface::getHeightAt(float x, float y) const {
    return getHeightAt(x, y, inNorthSplit(x, y));
  }

  bool SplitSurface::hasFlatSide() {
    return cDefAlternativeSplit
        ?  (cDefCornerHeights[0][0] == cDefCornerHeights[1][0] && cDefCornerHeights[0][0] == cDefCornerHeights[1][1])
        || (cDefCornerHeights[0][0] == cDefCornerHeights[0][1] && cDefCornerHeights[0][0] == cDefCornerHeights[1][1])
        :  (cDefCornerHeights[0][0] == cDefCornerHeights[1][0] && cDefCornerHeights[0][0] == cDefCornerHeights[0][1])
        || (cDefCornerHeights[1][1] == cDefCornerHeights[1][0] && cDefCornerHeights[1][1] == cDefCornerHeights[0][1]);
  }

  std::unique_ptr<LiteralVertex> SplitSurface::getSplitCrossingPoint(LiteralVertex& start, LiteralVertex& end, float* gradient) {
    LiteralVertex mStart(cDefAlternativeSplit ? -(start.x - cDefX) : start.x - cDefX, start.y - cDefY, start.z);
    LiteralVertex mEnd(  cDefAlternativeSplit ? -(end.x   - cDefX) : end.x   - cDefX, end.y   - cDefY, end.z);
    float mDistanceToStart = mStart.x + mStart.y;
    float mDistanceToEnd   = mEnd.x   + mEnd.y;
    bool mStartAtNorth = mDistanceToStart > 0;
    if (mStartAtNorth != (mDistanceToEnd > 0)) {
      *gradient = mDistanceToStart / (mDistanceToStart - mDistanceToEnd);
      float mYLocation = mStart.y + (mEnd.y - mStart.y) * *gradient;
      if (mYLocation >= -0.5f && mYLocation < 0.5f) {
        float mXLocation = cDefAlternativeSplit ? mYLocation : -mYLocation;
        mXLocation += cDefX;
        mYLocation += cDefY;
        float mZLocation = getHeightAt(mXLocation, mYLocation);
        float mXDirection = mStartAtNorth == cDefAlternativeSplit ?  INFINITY : -INFINITY;
        float mYDirection = mStartAtNorth                         ? -INFINITY :  INFINITY;
        return std::make_unique<LiteralVertex>(nextafterf(mXLocation, mXDirection), nextafterf(mYLocation, mYDirection), mZLocation);
      }
    }
    return nullptr;
  }

  float SplitSurface::getXAcceleration(float x, float y) {
    x -= cDefX;
    y -= cDefY;
    if (x == 0.0f && y == 0.0f && hasFlatSide()) {
      return 0.0f;
    }
    return cDefAlternativeSplit
        ? (x > y
            ? (cDefCornerHeights[0][0] - cDefCornerHeights[1][0])
            : (cDefCornerHeights[0][1] - cDefCornerHeights[1][1]))
        : (x + y > 0.0f
            ? (cDefCornerHeights[0][1] - cDefCornerHeights[1][1])
            : (cDefCornerHeights[0][0] - cDefCornerHeights[1][0]));
  }

  float SplitSurface::getYAcceleration(float x, float y) {
    x -= cDefX;
    y -= cDefY;
    if (x == 0.0f && y == 0.0f && hasFlatSide()) {
      return 0.0f;
    }
    return cDefAlternativeSplit
        ? (x > y
            ? (cDefCornerHeights[1][0] - cDefCornerHeights[1][1])
            : (cDefCornerHeights[0][0] - cDefCornerHeights[0][1]))
        : (x + y > 0.0f
            ? (cDefCornerHeights[1][0] - cDefCornerHeights[1][1])
            : (cDefCornerHeights[0][0] - cDefCornerHeights[0][1]));
  }

  void SplitSurface::notifyContact() {
    cType.executeContactScript();
  }

  void SplitSurface::notifyImpact() {
    cType.executeImpactScript();
  }

  float SplitSurface::getSurfaceFriction() {
    return cType.getSurfaceFriction();
  }

  float SplitSurface::getSurfaceGrip() {
    return cType.getSurfaceGrip();
  }

  float SplitSurface::getSurfaceBounce() {
    return cType.getSurfaceBounce();
  }

  SplitSurface::Respawn SplitSurface::isRespawnAllowed() {
    return cType.isRespawnAllowed()
        ? (cDefCondition.has_value() ? Respawn::CONDITIONAL : Respawn::YES)
        : Respawn::NO;
  }

  bool SplitSurface::isRespawnPossible() {
    return cType.isRespawnAllowed() && (!cDefCondition.has_value() || cDefCondition->isTrue());
  }

  std::unique_ptr<LiteralVertex> SplitSurface::getBoundaryCrossingPoint(LiteralVertex& start, LiteralVertex& end, float* mLowestGradient, float infinity) {
    *mLowestGradient = 2.0f;
    float mXMovement = end.x - start.x;
    float mYMovement = end.y - start.y;
    float mNorth = cDefY + 0.5f;
    float mEast  = cDefX + 0.5f;
    float mSouth = cDefY - 0.5f;
    float mWest  = cDefX - 0.5f;
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
      return std::make_unique<LiteralVertex>(mImpactX, mImpactY, mImpactZ);
    }

    // Line doesn't cross boundary
    return nullptr;
  }

  bool SplitSurface::inNorthSplit(float x, float y) const {
    x -= cDefX;
    y -= cDefY;
    return cDefAlternativeSplit ? y > x : x + y > 0.0f;
  }

  std::unique_ptr<CollisionData> SplitSurface::getImpactCollision(LiteralVertex& start, LiteralVertex& end, bool northSplit) {
    float mStartHeight = getHeightAt(start.x, start.y, northSplit);
    float mEndHeight = getHeightAt(end.x, end.y, northSplit);
    if ((start.z > mStartHeight) != (end.z > mEndHeight) && start.z > mStartHeight) {
      float mGradient = CollisionUtils::getCrossingPoint(start.z, end.z, mStartHeight, mEndHeight);
      double mXImpact = start.x + (end.x - start.x) * mGradient;
      double mYImpact = start.y + (end.y - start.y) * mGradient;
      if (inNorthSplit(mXImpact, mYImpact) == northSplit) {
        if (alligned(round(mXImpact), round(mYImpact))) {
          double mZImpact = getHeightAt(mXImpact, mYImpact);
          float mXSlope = getXAcceleration(start.x, start.y);
          float mYSlope = getYAcceleration(start.x, start.y);
          LiteralVertex mImpactLocation(mXImpact, mYImpact, mZImpact);
          return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_MOUNT, mImpactLocation, -mXSlope, -mYSlope, mGradient);
        }
      }
    }
    return nullptr;
  }

  void SplitSurface::confine(double* x, double* y) {
    *x = std::clamp(*x, static_cast<double>(nextafter(cDefX - 0.5f,  INFINITY)), static_cast<double>(nextafter(cDefX + 0.5f, -INFINITY)));
    *y = std::clamp(*y, static_cast<double>(nextafter(cDefY - 0.5f,  INFINITY)), static_cast<double>(nextafter(cDefY + 0.5f, -INFINITY)));
  }

  std::unique_ptr<CollisionData> SplitSurface::getCollision(LiteralVertex& start, LiteralVertex& end, float stepHeight, double startTime, double endTime) {
    if (!cDefCondition.has_value() || cDefCondition->isTrue()) {
      if (contains(start, stepHeight)) {
        LiteralVertex mEnterPoint(start);
        confine(&(mEnterPoint.x), &(mEnterPoint.y));
        return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_MOUNT, mEnterPoint, getXAcceleration(start.x, start.y), getYAcceleration(start.x, start.y), 0.0f);
      }
    
      float mGradient;
      std::unique_ptr<LiteralVertex> mEnterPoint = getBoundaryCrossingPoint(start, end, &mGradient, -INFINITY);
      if (mEnterPoint != nullptr) {
        float mEnterHeight = getHeightAt(mEnterPoint->getX(), mEnterPoint->getY());
        if (mEnterPoint->getZ() <= mEnterHeight && mEnterPoint->getZ() >= mEnterHeight - stepHeight) {
          confine(&(mEnterPoint->x), &(mEnterPoint->y));
          return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_MOUNT, *mEnterPoint.get(), getXAcceleration(start.x, start.y), getYAcceleration(start.x, start.y), mGradient);
        }
      }

      std::unique_ptr<CollisionData> mImpact = getImpactCollision(start, end, true);
      if (mImpact != nullptr) {
        return mImpact;
      }
      return getImpactCollision(start, end, false);
    }
    
    // No event
    return nullptr;
  }

  std::unique_ptr<CollisionData> SplitSurface::getRollingEvent(LiteralVertex& start, LiteralVertex& end, double startTime, double endTime) {
    
    // Check if surface has disappeared or object is no longer on it.
    if ((cDefCondition.has_value() && !cDefCondition->isTrue()) || !contains(start, 0.5)) {
      return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_LEAVE, LiteralVertex(start), getXAcceleration(start.x, start.y), getYAcceleration(start.x, start.y), 0.0f);
    }
    
    // Check if the object has crossed the split line.
    float mGradient;
    std::unique_ptr<LiteralVertex> mLeavePoint = getSplitCrossingPoint(start, end, &mGradient);
    if (mLeavePoint != nullptr) {
      confine(&(mLeavePoint->x), &(mLeavePoint->y));
      mLeavePoint->z = nextafterf(getHeightAt(mLeavePoint->x, mLeavePoint->y), INFINITY);
      return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_LEAVE, *mLeavePoint.get(), getXAcceleration(start.x, start.y), getYAcceleration(start.x, start.y), mGradient);
    }

    // Check if the object has moved off the surface.
    mLeavePoint = getBoundaryCrossingPoint(start, end, &mGradient, INFINITY);
    if (mLeavePoint != nullptr) {
      return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_LEAVE, *mLeavePoint.get(), getXAcceleration(start.x, start.y), getYAcceleration(start.x, start.y), mGradient);
    }
    
    // Object is still on the surface.
    return nullptr;
  }

  void SplitSurface::getRestingLocation(LiteralVertex& location) {
    float mXLocation = location.x - cDefX;
    float mYLocation = location.y - cDefY;
    float mSplitDistance = mXLocation + (cDefAlternativeSplit ? -(location.y - cDefY): location.y - cDefY);
    float mXDistance = 0.5f - fabs(mXLocation);
    float mYDistance = 0.5f - fabs(mYLocation);
    float mAbsSplitDistance = fabs(mSplitDistance);
    if (mAbsSplitDistance < mXDistance && mAbsSplitDistance < mYDistance) {
      mXLocation -= mSplitDistance / 2.0f;
      mYLocation = cDefAlternativeSplit ? mXLocation : -mXLocation;
      bool mInNorth = inNorthSplit(location.x, location.y);
      float mXDirection = mInNorth == cDefAlternativeSplit ?  INFINITY : -INFINITY;
      float mYDirection = mInNorth                         ? -INFINITY :  INFINITY;
      location.x = nextafterf(cDefX + mXLocation, mXDirection);
      location.y = nextafterf(cDefY + mYLocation, mYDirection);
    } else if (mXDistance < mYDistance) {
      mXLocation = mXLocation > 0.0f
                ? nextafterf(cDefX + 0.5f, -INFINITY)
                : nextafterf(cDefX - 0.5f,  INFINITY);
      location.x = mXLocation;
    } else {
      mYLocation = mYLocation > 0.0f
                ? nextafterf(cDefY + 0.5f, -INFINITY)
                : nextafterf(cDefY - 0.5f,  INFINITY);
      location.y = mYLocation;
    }
    location.z = getHeightAt(location.x, location.y);
  }

  Zone& SplitSurface::getZone() {
    return cOwner.getObjectZone();
  }

  bool SplitSurface::isSolid() {
    return true;
  }

  void SplitSurface::adjustPosition(LiteralVertex& location, double milliseconds) {
    // Nothing to do.
  } 

  int SplitSurface::getYStart() const {
    return cDefY;
  }

  int SplitSurface::getYEnd() const {
    return cDefY;
  }

  IWorldObject* SplitSurface::getOwner() {
    return &cOwner;
  }

  int SplitSurface::getXStart() const {
    return cDefX;
  }

  int SplitSurface::getXEnd() const {
    return cDefX;
  }
}

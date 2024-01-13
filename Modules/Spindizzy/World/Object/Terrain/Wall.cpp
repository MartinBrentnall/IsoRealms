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
#include "Wall.h"

#include "Modules/Spindizzy/Assets/Type/IWallPattern.h"
#include "Modules/Spindizzy/TerrainType/TerrainType.h"

#include "Terrain.h"

namespace IsoRealms::Spindizzy {
  Wall::Wall(int x, int y, int z, int length, int height, int topSlope, int bottomSlope, Direction facing, std::optional<Condition>& condition, TerrainType* type, IWorldObject& owner) :
            cDefX(x),
            cDefY(y),
            cDefZ(z),
            cDefLength(length),
            cDefHeight(topSlope < 0 ? height - cDefLength * topSlope : height),
            cDefTopSlope(topSlope),
            cDefBottomSlope(bottomSlope),
            cDefFacing(facing),
            cDefCondition(condition),
            cDefType(type),
            cDefOwner(owner) {
  }

  const std::optional<Condition>& Wall::getCondition() {
    return cDefCondition;
  }

  Wall::WallEdge Wall::getTopEdge(int location) {
    location -= cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? cDefX : cDefY;
    int mStartLocation = location * cDefTopSlope;
    int mEndLocation = (location + 1) * cDefTopSlope;
    return WallEdge(cDefZ + cDefHeight + mStartLocation, cDefZ + cDefHeight + mEndLocation);
  }

  Wall::WallEdge Wall::getBottomEdge(int location) {
    location -= cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? cDefX : cDefY;
    int mStartLocation = location * cDefBottomSlope;
    int mEndLocation = (location + 1) * cDefBottomSlope;
    return WallEdge(cDefZ + mStartLocation, cDefZ + mEndLocation);
  }

  int Wall::getXStart() const {
    return cDefX;
  }

  int Wall::getXEnd() const {
    bool mFacingPole = cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH;
    return mFacingPole ? cDefX + (cDefLength - 1) : cDefX;
  }

  int Wall::getYStart() const {
    return cDefY;
  }

  int Wall::getYEnd() const {
    bool mFacingPole = cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH;
    return mFacingPole ? cDefY : cDefY + (cDefLength - 1);
  }

  int Wall::getZStart() const {
    return cDefZ;
  }

  int Wall::getZEnd() const {
    return cDefHeight + cDefLength * cDefTopSlope;
  }

  void Wall::saveCache(std::ostream& cache, bool physical) {
    cache.write(reinterpret_cast<const char*>(&Terrain::CACHE_WALL), sizeof(Terrain::CACHE_WALL));
    cache.write(reinterpret_cast<const char*>(&physical),            sizeof(physical));
    cache.write(reinterpret_cast<const char*>(&cDefX),               sizeof(cDefX));
    cache.write(reinterpret_cast<const char*>(&cDefY),               sizeof(cDefY));
    cache.write(reinterpret_cast<const char*>(&cDefZ),               sizeof(cDefZ));
    cache.write(reinterpret_cast<const char*>(&cDefLength),          sizeof(cDefLength));
    cache.write(reinterpret_cast<const char*>(&cDefHeight),          sizeof(cDefHeight));
    cache.write(reinterpret_cast<const char*>(&cDefTopSlope),        sizeof(cDefTopSlope));
    cache.write(reinterpret_cast<const char*>(&cDefBottomSlope),     sizeof(cDefBottomSlope));
    cache.write(reinterpret_cast<const char*>(&cDefFacing),          sizeof(cDefFacing));
    if (cDefCondition.has_value()) {
      cDefCondition->saveCache(cache, Terrain::CACHE_CONDITION, Terrain::CACHE_CONDITION_ELEMENT, Terrain::CACHE_CONDITION_END);
    } else {
      cache.write(reinterpret_cast<const char*>(&Terrain::CACHE_SURFACE_END), sizeof(Terrain::CACHE_SURFACE_END));
    }
  }

  std::vector<std::unique_ptr<IVisualElement>> Wall::getStaticVisuals() {
    switch (cDefFacing) {
      case Direction::NORTH: return cDefType->getNorthWallPattern()->getStaticVisuals(this);
      case Direction::SOUTH: return cDefType->getSouthWallPattern()->getStaticVisuals(this);
      case Direction::EAST:  return cDefType->getEastWallPattern()->getStaticVisuals(this);
      case Direction::WEST:  return cDefType->getWestWallPattern()->getStaticVisuals(this);
    }
    throw IllegalStateException("WARNING: Wall::getStaticVisuals: cDefFacing has illegal value");
  }

  void Wall::render() {
    if (!cDefCondition.has_value() || cDefCondition->isTrue()) {
      switch (cDefFacing) {
        case Direction::NORTH: cDefType->getNorthWallPattern()->render(cDefX, cDefY, cDefZ, cDefLength, cDefHeight, cDefTopSlope, cDefBottomSlope, cDefFacing);  break;
        case Direction::SOUTH: cDefType->getSouthWallPattern()->render(cDefX, cDefY, cDefZ, cDefLength, cDefHeight, cDefTopSlope, cDefBottomSlope, cDefFacing); break;
        case Direction::EAST:  cDefType->getEastWallPattern()->render(cDefX, cDefY, cDefZ, cDefLength, cDefHeight, cDefTopSlope, cDefBottomSlope, cDefFacing);  break;
        case Direction::WEST:  cDefType->getWestWallPattern()->render(cDefX, cDefY, cDefZ, cDefLength, cDefHeight, cDefTopSlope, cDefBottomSlope, cDefFacing);  break;
      }
    }
    glEnd();
  }

  void Wall::renderOutline() {
    float mBlockRadius = 0.5f;
    double mFromX = cDefX + (cDefFacing == Direction::EAST  ? mBlockRadius : -mBlockRadius);
    double mFromY = cDefY + (cDefFacing == Direction::NORTH ? mBlockRadius : -mBlockRadius);
    double mToX = (cDefFacing == Direction::WEST  || cDefFacing == Direction::EAST)  ? (mFromX) : cDefX - mBlockRadius + cDefLength;
    double mToY = (cDefFacing == Direction::SOUTH || cDefFacing == Direction::NORTH) ? (mFromY) : cDefY - mBlockRadius + cDefLength;
    double mHighStartSlopeZ = (cDefZ + cDefHeight) * 0.5f;
    double mHighEndSlopeZ =  ((cDefZ + cDefHeight) + cDefTopSlope * cDefLength) * 0.5f;
    double mLowStartSlopeZ = cDefZ * 0.5f;
    double mLowEndSlopeZ =  (cDefZ + cDefBottomSlope * cDefLength) * 0.5f;

    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.0f, 1.0f, 1.0f);
    glLineWidth(6.0f);
    glBegin(GL_LINE_LOOP);
    if (cDefFacing == Direction::EAST || cDefFacing == Direction::SOUTH) {
      glVertex3f(mFromX, mFromY, mLowStartSlopeZ);
      glVertex3f(mToX,   mToY,   mLowEndSlopeZ);
      glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
    } else {
      glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
      glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      glVertex3f(mToX,   mToY,   mLowEndSlopeZ);
      glVertex3f(mFromX, mFromY, mLowStartSlopeZ);
    }
    glEnd();
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
  }

  void Wall::renderSelectionHighlight() {
    float mBlockRadius = 0.5f;
    double mFromX = cDefX + (cDefFacing == Direction::EAST  ? mBlockRadius : -mBlockRadius);
    double mFromY = cDefY + (cDefFacing == Direction::NORTH ? mBlockRadius : -mBlockRadius);
    double mToX = (cDefFacing == Direction::WEST  || cDefFacing == Direction::EAST)  ? (mFromX) : cDefX - mBlockRadius + cDefLength;
    double mToY = (cDefFacing == Direction::SOUTH || cDefFacing == Direction::NORTH) ? (mFromY) : cDefY - mBlockRadius + cDefLength;
    double mHighStartSlopeZ = (cDefZ + cDefHeight) * 0.5f;
    double mHighEndSlopeZ =  ((cDefZ + cDefHeight) + cDefTopSlope * cDefLength) * 0.5f;
    double mLowStartSlopeZ = cDefZ * 0.5f;
    double mLowEndSlopeZ =  (cDefZ + cDefBottomSlope * cDefLength) * 0.5f;

    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    if (cDefFacing == Direction::EAST || cDefFacing == Direction::SOUTH) {
      double mXOffset = cDefFacing == Direction::EAST  ?  0.001 : 0.0;
      double mYOffset = cDefFacing == Direction::SOUTH ? -0.001 : 0.0;
      glVertex3f(mFromX + mXOffset, mFromY + mYOffset, mLowStartSlopeZ);
      glVertex3f(mToX   + mXOffset, mToY   + mYOffset, mLowEndSlopeZ);
      glVertex3f(mToX   + mXOffset, mToY   + mYOffset, mHighEndSlopeZ);
      glVertex3f(mFromX + mXOffset, mFromY + mYOffset, mHighStartSlopeZ);
    } else {
      double mXOffset = cDefFacing == Direction::WEST  ? -0.001 : 0.0;
      double mYOffset = cDefFacing == Direction::NORTH ?  0.001 : 0.0;
      glVertex3f(mFromX + mXOffset, mFromY + mYOffset, mHighStartSlopeZ);
      glVertex3f(mToX   + mXOffset, mToY   + mYOffset, mHighEndSlopeZ);
      glVertex3f(mToX   + mXOffset, mToY   + mYOffset, mLowEndSlopeZ);
      glVertex3f(mFromX + mXOffset, mFromY + mYOffset, mLowStartSlopeZ);
    }
    glEnd();
    glDisable(GL_BLEND);
  }

  Wall::WallEdge::WallEdge(int startHeight, int endHeight) {
    cStartHeight = startHeight;
    cEndHeight = endHeight;
  }

  int Wall::WallEdge::getStartHeight() {
    return cStartHeight;
  }

  int Wall::WallEdge::getEndHeight() {
    return cEndHeight;
  }

  bool Wall::isAtZoneEdge() {
    switch (cDefFacing) {
      case Direction::NORTH: return cDefY == cDefOwner.getObjectZone().getEndY();
      case Direction::SOUTH: return cDefY == cDefOwner.getObjectZone().getStartY();
      case Direction::EAST:  return cDefX == cDefOwner.getObjectZone().getEndX();
      case Direction::WEST:  return cDefX == cDefOwner.getObjectZone().getStartX();
    }
    throw IllegalStateException("WARNING: Wall::isAtZoneEdge(): cDefFacing has illegal value");
  }

  void Wall::updateState(PhysicalState& state) {
    switch (cDefFacing) {
      case Direction::NORTH: state.cMomentum.y = 0.0f; state.cLocation.y = std::max(state.cLocation.y, cDefY + 0.55); break;
      case Direction::SOUTH: state.cMomentum.y = 0.0f; state.cLocation.y = std::min(state.cLocation.y, cDefY - 0.55); break;
      case Direction::EAST:  state.cMomentum.x = 0.0f; state.cLocation.x = std::max(state.cLocation.x, cDefX + 0.55); break;
      case Direction::WEST:  state.cMomentum.x = 0.0f; state.cLocation.x = std::min(state.cLocation.x, cDefX - 0.55); break;
    }
  }

  float Wall::getSouthEdge(float craftRadius) {
    switch (cDefFacing) {
      case Direction::NORTH: return (cDefY + 0.5f);
      case Direction::SOUTH: return (cDefY - 0.5f) - craftRadius;
      case Direction::EAST:  return  cDefY - 0.5f;
      case Direction::WEST:  return  cDefY - 0.5f;
    }
    throw IllegalStateException("WARNING: Wall::getSouthEdge: cDefFacing has illegal value");
  }

  float Wall::getEastEdge(float craftRadius) {
    switch (cDefFacing) {
      case Direction::NORTH: return   cDefX + (cDefLength - 1) + 0.5f;
      case Direction::SOUTH: return   cDefX + (cDefLength - 1) + 0.5f;
      case Direction::EAST:  return   cDefX +                    0.5f  + craftRadius;
      case Direction::WEST:  return  (cDefX -                    0.5f);
    }
    throw IllegalStateException("WARNING: Wall::getEastEdge: cDefFacing has illegal value");
  }

  float Wall::getNorthEdge(float craftRadius) {
    switch (cDefFacing) {
      case Direction::NORTH: return   cDefY +                    0.5f  + craftRadius;
      case Direction::SOUTH: return  (cDefY -                    0.5f);
      case Direction::EAST:  return   cDefY + (cDefLength - 1) + 0.5f;
      case Direction::WEST:  return   cDefY + (cDefLength - 1) + 0.5f;
    }
    throw IllegalStateException("WARNING: Wall::getNorthEdge: cDefFacing has illegal value");
  }

  float Wall::getWestEdge(float craftRadius) {
    switch (cDefFacing) {
      case Direction::NORTH: return  cDefX - 0.5f;
      case Direction::SOUTH: return  cDefX - 0.5f;
      case Direction::EAST:  return (cDefX + 0.5f);
      case Direction::WEST:  return (cDefX - 0.5f) - craftRadius;
    }
    throw IllegalStateException("WARNING: Wall::getWestEdge: cDefFacing has illegal value");
  }

  bool Wall::contains(LiteralVertex& location, float craftRadius, float craftHeight, float stepHeight) {
    float mTouchDistance = isAtZoneEdge() ? 0.001f : craftRadius;
    float mSouthEdge = getSouthEdge(mTouchDistance);
    float mWestEdge  = getWestEdge(mTouchDistance);
    float mNorthEdge = getNorthEdge(mTouchDistance);
    float mEastEdge  = getEastEdge(mTouchDistance);
    if (location.y > mSouthEdge && location.y < mNorthEdge && location.x > mWestEdge && location.x < mEastEdge) {
      float mWallHeight = getHeightAt(cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? location.x : location.y);
      float mWallLow    = getLowAt(cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? location.x : location.y);
      if (location.z >= mWallLow - craftHeight && location.z < mWallHeight - stepHeight) {
        return true;
      }
    }
    return false;
  }

  std::unique_ptr<CollisionData> Wall::checkVerticalEdgeCollision(LiteralVertex& start, LiteralVertex& end, float startZ, float endZ, float craftRadius, bool sliding) {
    if ((start.z > startZ) != (end.z > endZ)) {
      float mGradient  = CollisionUtils::getCrossingPoint(start.z, end.z, startZ, endZ);
      float mXImpact   = start.x + (end.x - start.x) * mGradient;
      float mYImpact   = start.y + (end.y - start.y) * mGradient;
      float mZImpact   = start.z + (end.z - start.z) * mGradient;
      float mNorthEdge = getNorthEdge(craftRadius);
      float mEastEdge  = getEastEdge(craftRadius);
      float mSouthEdge = getSouthEdge(craftRadius);
      float mWestEdge  = getWestEdge(craftRadius);
      bool mIgnoreX = sliding && (cDefFacing == Direction::EAST  || cDefFacing == Direction::WEST);
      bool mIgnoreY = sliding && (cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH);
      if ((mIgnoreY || (mYImpact >= mSouthEdge && mYImpact <= mNorthEdge)) && (mIgnoreX || (mXImpact >= mWestEdge && mXImpact <= mEastEdge))) {
        float mXLocation = cDefFacing == Direction::EAST  ? nextafterf(mEastEdge,   INFINITY)
                         : cDefFacing == Direction::WEST  ? nextafterf(mWestEdge,  -INFINITY)
                         :                                  mXImpact;
        float mYLocation = cDefFacing == Direction::NORTH ? nextafterf(mNorthEdge,  INFINITY)
                         : cDefFacing == Direction::SOUTH ? nextafterf(mSouthEdge, -INFINITY)
                         :                                  mYImpact;
        LiteralVertex mImpactLocation(mXLocation, mYLocation, mZImpact);
        return std::make_unique<CollisionData>(this, sliding ? CollisionData::Type::WALL_LEAVE : CollisionData::Type::WALL_CLIP, mImpactLocation, mGradient);
      }
    }
    return nullptr;
  }

  std::unique_ptr<CollisionData> Wall::getCollision(LiteralVertex& start, LiteralVertex& end, IPhysicalObject* object) {
    if (!cDefCondition.has_value() || cDefCondition->isTrue()) {
      // TODO: Only do one way collision detection
      
      float mCraftHeight = object->getHeight();
      float mCraftRadius = (object->getHome() == nullptr && isAtZoneEdge()) ? 0.001f : object->getRadius();
      float mStepReach = object->getStepReach();
      float mXMovement = end.x - start.x;
      float mYMovement = end.y - start.y;
      
      if (contains(start, mCraftRadius, mCraftHeight, mStepReach)) {
        switch (cDefFacing) {
          case Direction::NORTH: return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_IMPACT, LiteralVertex(start.x,                                          nextafterf(getNorthEdge(mCraftRadius),  INFINITY), start.z), 0.0f);
          case Direction::SOUTH: return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_IMPACT, LiteralVertex(start.x,                                          nextafterf(getSouthEdge(mCraftRadius), -INFINITY), start.z), 0.0f);
          case Direction::EAST:  return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_IMPACT, LiteralVertex(nextafterf(getEastEdge(mCraftRadius),  INFINITY), start.y,                                           start.z), 0.0f);
          case Direction::WEST:  return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_IMPACT, LiteralVertex(nextafterf(getWestEdge(mCraftRadius), -INFINITY), start.y,                                           start.z), 0.0f);
        }
      }
      
      float mOffset = cDefFacing == Direction::NORTH || cDefFacing == Direction::EAST
                    ?   0.5f + mCraftRadius
                    : -(0.5f + mCraftRadius);
      
      // Direct impact detection
      if ((cDefFacing == Direction::NORTH && start.y > end.y) || (cDefFacing == Direction::SOUTH && start.y < end.y)) {
        std::optional<IsoRealms::CollisionData> mVertex = CollisionUtils::getYCrossingPoint(start, end, cDefY + mOffset);
        if (mVertex && mVertex->getX() >= cDefX - 0.5f && mVertex->getX() < (cDefX + cDefLength) - 0.5f) {
          float mWallHeight = getHeightAt(mVertex->getX());
          float mWallLow    = getLowAt(mVertex->getX());
          if (mVertex->getZ() >= mWallLow - mCraftHeight && mVertex->getZ() < mWallHeight - mStepReach) {
            float mDirection = cDefFacing == Direction::NORTH ? INFINITY : -INFINITY;
            return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_IMPACT, LiteralVertex(mVertex->getX(), nextafterf(mVertex->getY(), mDirection), mVertex->getZ()), mVertex->getGradient());
          }
        }
      }
      
      if ((cDefFacing == Direction::EAST && start.x > end.x) || (cDefFacing == Direction::WEST && start.x < end.x)) {
        std::optional<IsoRealms::CollisionData> mVertex = CollisionUtils::getXCrossingPoint(start, end, cDefX + mOffset);
        if (mVertex && mVertex->getY() >= cDefY - 0.5f && mVertex->getY() < (cDefY + cDefLength) - 0.5f) {
          float mWallHeight = getHeightAt(mVertex->getY());
          float mWallLow    = getLowAt(mVertex->getY());
          if (mVertex->getZ() >= mWallLow - mCraftHeight && mVertex->getZ() < mWallHeight - mStepReach) {
            float mDirection = cDefFacing == Direction::EAST ? INFINITY : -INFINITY;
            return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_IMPACT, LiteralVertex(nextafterf(mVertex->getX(), mDirection), mVertex->getY(), mVertex->getZ()), mVertex->getGradient());
          }
        }
      }    
      
      // Edge clip detection
      switch (cDefFacing) {
        case Direction::NORTH: {
          if (start.x != end.x) {
            float mEdgePosition = start.x < end.x ? cDefX - 0.5f : cDefX + (cDefLength - 1) + 0.5f;
            std::optional<IsoRealms::CollisionData> mVertex = CollisionUtils::getXCrossingPoint(start, end, mEdgePosition);
            if (mVertex && mVertex->getY() >= cDefY + 0.5f && mVertex->getY() <= cDefY + 0.5f + mCraftRadius) {
              float mWallHeight = getHeightAt(mEdgePosition);
              float mWallLow    = getLowAt(mEdgePosition);
              if (mVertex->getZ() >= mWallLow - mCraftHeight && mVertex->getZ() < mWallHeight - mStepReach) {
                float mGradient = (mEdgePosition - start.x) / mXMovement; // TODO: Why isn't this gradient exactly same as the one in the CollisionData?  We shouldn't need this; we should just be able to use the one in the collision vertex.
                float mDirection = start.x < end.x ? INFINITY : -INFINITY;
                return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_CLIP, LiteralVertex(nextafterf(mVertex->getX(), mDirection), nextafterf(cDefY + 0.5f + mCraftRadius, INFINITY), mVertex->getZ()), mGradient);
              }
            }
          }
          break;
        }
        
        case Direction::SOUTH: {
          if (start.x != end.x) {
            float mEdgePosition = start.x < end.x ? cDefX - 0.5f : cDefX + (cDefLength - 1) + 0.5f;
            std::optional<IsoRealms::CollisionData> mVertex = CollisionUtils::getXCrossingPoint(start, end, mEdgePosition);
            if (mVertex && mVertex->getY() <= cDefY - 0.5f && mVertex->getY() >= (cDefY - 0.5f) - mCraftRadius) {
              float mWallHeight = getHeightAt(mEdgePosition);
              float mWallLow    = getLowAt(mEdgePosition);
              if (mVertex->getZ() >= mWallLow - mCraftHeight && mVertex->getZ() < mWallHeight - mStepReach) {
                float mGradient = (mEdgePosition - start.x) / mXMovement; // TODO: Why isn't this gradient exactly same as the one in the CollisionData?  We shouldn't need this; we should just be able to use the one in the collision vertex.
                float mDirection = start.x < end.x ? INFINITY : -INFINITY;
                return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_CLIP, LiteralVertex(nextafterf(mVertex->getX(), mDirection), nextafterf((cDefY - 0.5f) - mCraftRadius, -INFINITY), mVertex->getZ()), mGradient);
              }
            }
          }
          break;
        }
        
        case Direction::EAST: {
          if (start.y != end.y) {
            float mEdgePosition = start.y < end.y ? cDefY - 0.5f : cDefY + (cDefLength - 1) + 0.5f;
            std::optional<IsoRealms::CollisionData> mVertex = CollisionUtils::getYCrossingPoint(start, end, mEdgePosition);
            if (mVertex && mVertex->getX() >= cDefX + 0.5f && mVertex->getX() <= cDefX + 0.5f + mCraftRadius) {
              float mWallHeight = getHeightAt(mEdgePosition);
              float mWallLow    = getLowAt(mEdgePosition);
              if (mVertex->getZ() >= mWallLow - mCraftHeight && mVertex->getZ() < mWallHeight - mStepReach) {
                float mGradient = (mEdgePosition - start.y) / mYMovement; // TODO: Why isn't this gradient exactly same as the one in the CollisionData?  We shouldn't need this; we should just be able to use the one in the collision vertex.
                float mDirection = start.y < end.y ? INFINITY : -INFINITY;
                return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_CLIP, LiteralVertex(nextafterf(cDefX + 0.5f + mCraftRadius, INFINITY), nextafterf(mVertex->getY(), mDirection), mVertex->getZ()), mGradient);
              }
            }
          }
          break;
        }
        
        case Direction::WEST: {
          if (start.y != end.y) {
            float mEdgePosition = start.y < end.y ? cDefY - 0.5f : cDefY + (cDefLength - 1) + 0.5f;
            std::optional<IsoRealms::CollisionData> mVertex = CollisionUtils::getYCrossingPoint(start, end, mEdgePosition);
            if (mVertex && mVertex->getX() <= cDefX - 0.5f && mVertex->getX() >= (cDefX - 0.5f) - mCraftRadius) {
              float mWallHeight = getHeightAt(mEdgePosition);
              float mWallLow    = getLowAt(mEdgePosition);
              if (mVertex->getZ() >= mWallLow - mCraftHeight && mVertex->getZ() < mWallHeight - mStepReach) {
                float mGradient = (mEdgePosition - start.y) / mYMovement; // TODO: Why isn't this gradient exactly same as the one in the CollisionData?  We shouldn't need this; we should just be able to use the one in the collision vertex.
                float mDirection = start.y < end.y ? INFINITY : -INFINITY;
                return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_CLIP, LiteralVertex(nextafterf((cDefX - 0.5f) - mCraftRadius, -INFINITY), nextafterf(mVertex->getY(), mDirection), mVertex->getZ()), mGradient);
              }
            }
          }
          break;
        }
      }
      
      // Top Clip Detection
      float mStartHeight = getHeightAt(cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? start.x : start.y) - mStepReach;
      float mEndHeight   = getHeightAt(cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? end.x   : end.y) - mStepReach;
      std::unique_ptr<CollisionData> mClipEvent = checkVerticalEdgeCollision(start, end, mStartHeight, mEndHeight, mCraftRadius, false);
      if (mClipEvent != nullptr) {
        return mClipEvent;
      }
      float mStartLow = getLowAt(cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? start.x : start.y) - mStepReach;
      float mEndLow   = getLowAt(cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? end.x   : end.y) - mStepReach;
      mClipEvent = checkVerticalEdgeCollision(start, end, mStartLow - mCraftHeight, mEndLow - mCraftHeight, mCraftRadius, false);
      if (mClipEvent != nullptr) {
        return mClipEvent;
      }
    }
    return nullptr;
  }

  float Wall::getHeightAt(float location) {
    int mStart = cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? cDefX : cDefY;
    return ((location + 0.5f) - mStart) * cDefTopSlope + cDefHeight + cDefZ;
  }

  float Wall::getLowAt(float location) {
    int mStart = cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? cDefX : cDefY;
    return ((location + 0.5f) - mStart) * cDefBottomSlope + cDefZ;
  }

  int Wall::getX() {
    return cDefX;
  }

  int Wall::getY() {
    return cDefY;
  }

  int Wall::getZ() {
    return cDefZ;
  }

  int Wall::getLength() {
    return cDefLength;
  }

  int Wall::getHeight() {
    return cDefHeight;
  }

  int Wall::getTopSlope() {
    return cDefTopSlope;
  }
  
  int Wall::getBottomSlope() {
    return cDefBottomSlope;
  }

  Wall::Direction Wall::getFaceDirection() {
    return cDefFacing;
  }

  float Wall::getBounce() {
    return cDefType->getWallBounce();
  }

  std::unique_ptr<CollisionData> Wall::getSlidingEvent(LiteralVertex& start, LiteralVertex& end, IPhysicalObject* object) {
    
    // Leave by moving away
    switch (cDefFacing) {
      case Direction::NORTH: if (start.y < end.y) {return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_LEAVE, LiteralVertex(start), 0.0f);} break;
      case Direction::SOUTH: if (start.y > end.y) {return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_LEAVE, LiteralVertex(start), 0.0f);} break;
      case Direction::EAST:  if (start.x < end.x) {return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_LEAVE, LiteralVertex(start), 0.0f);} break;
      case Direction::WEST:  if (start.x > end.x) {return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_LEAVE, LiteralVertex(start), 0.0f);} break;
    }
    
    // Leave by horizontal edge
    if (cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH) {
      float mXMovement = end.x - start.x;
      float mEdgePosition = start.x > end.x ? cDefX - 0.5f : cDefX + (cDefLength - 1) + 0.5f;
      std::optional<IsoRealms::CollisionData> mVertex = CollisionUtils::getXCrossingPoint(start, end, mEdgePosition);
      if (mVertex) {
        float mGradient = (mEdgePosition - start.x) / mXMovement; // TODO: Why isn't this gradient exactly same as the one in the CollisionData?  We shouldn't need this; we should just be able to use the one in the collision vertex.
        return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_LEAVE, LiteralVertex(mVertex->getX(), mVertex->getY(), mVertex->getZ()), mGradient);
      }
      if (start.x < cDefX - 0.5f || start.x > cDefX + 0.5f + (cDefLength - 1)) {
        return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_LEAVE, LiteralVertex(start.x, start.y, start.z), 0.0f);
      }
    } else {
      float mYMovement = end.y - start.y;
      float mEdgePosition = start.y > end.y ? cDefY - 0.5f : cDefY + (cDefLength - 1) + 0.5f;
      std::optional<IsoRealms::CollisionData> mVertex = CollisionUtils::getYCrossingPoint(start, end, mEdgePosition);
      if (mVertex) {
        float mGradient = (mEdgePosition - start.y) / mYMovement; // TODO: Why isn't this gradient exactly same as the one in the CollisionData?  We shouldn't need this; we should just be able to use the one in the collision vertex.
        return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_LEAVE, LiteralVertex(mVertex->getX(), mVertex->getY(), mVertex->getZ()), mGradient);
      }
      if (start.y < cDefY - 0.5f || start.y > cDefY + 0.5f + (cDefLength - 1)) {
        return std::make_unique<CollisionData>(this, CollisionData::Type::WALL_LEAVE, LiteralVertex(start.x, start.y, start.z), 0.0f);
      }
    }
    
    // Leave by vertical edge
    float mStepHeight = object->getStepReach();
    float mCraftHeight = object->getHeight();
    float mCraftRadius = object->getRadius();
    float mStartHeight = getHeightAt(cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? start.x : start.y) - mStepHeight;
    float mEndHeight   = getHeightAt(cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? end.x   : end.y)   - mStepHeight;
    std::unique_ptr<CollisionData> mClipEvent = checkVerticalEdgeCollision(start, end, mStartHeight, mEndHeight, mCraftRadius, true);
    if (mClipEvent != nullptr) {
      return mClipEvent;
    }
    float mStartLow = getLowAt(cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? start.x : start.y) - mStepHeight;
    float mEndLow   = getLowAt(cDefFacing == Direction::NORTH || cDefFacing == Direction::SOUTH ? end.x   : end.y)   - mStepHeight;
    mClipEvent = checkVerticalEdgeCollision(start, end, mStartLow - mCraftHeight, mEndLow - mCraftHeight, mCraftRadius, true);
    if (mClipEvent != nullptr) {
      return mClipEvent;
    }
    return nullptr;
  }

  IWorldObject* Wall::getOwner() {
    return &cDefOwner;
  }

  Zone* Wall::getZone() {
    return &cDefOwner.getObjectZone();
  }

  void Wall::bindValues() {
    cDefOwner.getObjectZone().bindValues2(this);
  }

  void Wall::unbindValues() {
    cDefOwner.getObjectZone().unbindValues2();
  }
}

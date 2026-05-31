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
#include "Terrain.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/TerrainType/TerrainType.h"
#include "Modules/Equilibria/World/World.h"

namespace IsoRealms::Equilibria {
  


  Terrain::Terrain(Zone& zone, JSONObject object) :
            cZone(zone),
            cDefType(nullptr),
            cDefStartX(object.getInteger(JSON_X) + cZone.getStartX()),
            cDefStartY(object.getInteger(JSON_Y) + cZone.getStartY()),
            cDefStartZ(object.getInteger(JSON_Z) + cZone.getStartZ() - 1), //(object.getInteger(JSON_HEIGHT) < 0 ? 0 : 1)),
            cDefEndX(cDefStartX + object.getInteger(JSON_WIDTH) - 1),
            cDefEndY(cDefStartY + object.getInteger(JSON_LENGTH) - 1),
            cDefEndZ(cDefStartZ + object.getInteger(JSON_HEIGHT)),
            cDefCornerHeight{{object.getInteger(JSON_SOUTH_WEST_CORNER), object.getInteger(JSON_NORTH_WEST_CORNER)},
                             {object.getInteger(JSON_SOUTH_EAST_CORNER), object.getInteger(JSON_NORTH_EAST_CORNER)}},
            cDefFlags(getBehaviourFlags(object.getString(JSON_BEHAVIOUR))
                   | (object.getBoolean(JSON_STEPPED_BOTTOM)    ? FLAG_STEPPED_BOTTOM    : FLAGS_NORMAL)
                   | (object.getBoolean(JSON_ALTERNATIVE_SPLIT) ? FLAG_ALTERNATIVE_SPLIT : FLAGS_NORMAL)) {
    cZone.getWorld().getEquilibria().getProject().init([this, object]() {
      cDefType = cZone.getWorld().getEquilibria().get<TerrainType>(nullptr, object.getString(JSON_TYPE));
      if (object.hasMember(JSON_CONDITION)) {
        cDefCondition = std::make_optional<Condition>(object.getObject(JSON_CONDITION), cZone.getWorld().getEquilibria().getTerrainStateConditionElements());
      }
    });
    cZone.getWorld().registerTerrain(this, !(cDefFlags & FLAG_INVISIBLE), !(cDefFlags & FLAG_GHOST));
  }

  Terrain::Terrain(Zone& zone, TerrainType& type, int startX, int startY, int startZ, int endX, int endY, int endZ, int southWestHeight, int southEastHeight, int northWestHeight, int northEastHeight, bool alternativeSplit, bool steppedBottom, bool addition) :
            cZone(zone),
            cDefType(&type),
            cDefStartX( endX > startX              ? startX : endX),
            cDefStartY( endY > startY              ? startY : endY),
            cDefStartZ((endZ > startZ) == addition ? startZ : endZ),
            cDefEndX(   endX > startX              ? endX   : startX),
            cDefEndY(   endY > startY              ? endY   : startY),
            cDefEndZ(  (endZ > startZ) == addition ? endZ   : startZ),
            cDefCornerHeight{{southWestHeight, northWestHeight},
                             {southEastHeight, northEastHeight}},
            cDefFlags((alternativeSplit ? FLAG_ALTERNATIVE_SPLIT : FLAGS_NORMAL)
                    | (steppedBottom    ? FLAG_STEPPED_BOTTOM    : FLAGS_NORMAL)) {
    (addition ? cDefStartZ : cDefEndZ)--;
    cZone.getWorld().registerTerrain(this, true, true);
    cZone.getWorld().flagTerrainForInitialisation(cDefStartX - 1, cDefEndX + 1, cDefStartY - 1, cDefEndY + 1);
  }

  Terrain::Terrain(Zone& zone, Terrain& terrain, int x, int y, int z) :
            cZone(zone),
            cDefType(terrain.cDefType),
            cDefStartX(terrain.cDefStartX + x),
            cDefStartY(terrain.cDefStartY + y),
            cDefStartZ(terrain.cDefStartZ + z),
            cDefEndX(terrain.cDefEndX + x),
            cDefEndY(terrain.cDefEndY + y),
            cDefEndZ(terrain.cDefEndZ + z),
            cDefCornerHeight{{terrain.cDefCornerHeight[0][0], terrain.cDefCornerHeight[0][1]},
                             {terrain.cDefCornerHeight[1][0], terrain.cDefCornerHeight[1][1]}},
            cDefFlags(terrain.cDefFlags) {
    cZone.getWorld().registerTerrain(this, true, true);
    cZone.getWorld().flagTerrainForInitialisation(cDefStartX - 1, cDefEndX + 1, cDefStartY - 1, cDefEndY + 1);
  }

  void Terrain::save(JSONObject object, int originX, int originY, int originZ) {
    object.addString(JSON_TYPE, cZone.getWorld().getEquilibria().getResourceID(cDefType));
    object.addString(JSON_BEHAVIOUR,          getBehaviourString());
    object.addInteger(JSON_X,                 cDefStartX      - originX);
    object.addInteger(JSON_Y,                 cDefStartY      - originY);
    object.addInteger(JSON_Z,                (cDefStartZ + 1) - originZ);
    object.addInteger(JSON_WIDTH,            (cDefEndX + 1)   - cDefStartX);
    object.addInteger(JSON_LENGTH,           (cDefEndY + 1)   - cDefStartY);
    object.addInteger(JSON_HEIGHT,            cDefEndZ        - cDefStartZ);
    object.addInteger(JSON_NORTH_WEST_CORNER, cDefCornerHeight[0][1]);
    object.addInteger(JSON_NORTH_EAST_CORNER, cDefCornerHeight[1][1]);
    object.addInteger(JSON_SOUTH_EAST_CORNER, cDefCornerHeight[1][0]);
    object.addInteger(JSON_SOUTH_WEST_CORNER, cDefCornerHeight[0][0]);
    if (isSplit()) {
      object.addBoolean(JSON_ALTERNATIVE_SPLIT, (cDefFlags & FLAG_ALTERNATIVE_SPLIT) != 0);
    }
    if (cDefFlags & FLAG_STEPPED_BOTTOM && ((cDefEndX != cDefStartX && getXSlope() != 0) || (cDefEndY != cDefStartY && getYSlope() != 0))) {
      object.addBoolean(JSON_STEPPED_BOTTOM, true);
    }
    if (cDefCondition.has_value()) {
      JSONObject mConditionObject = object.addObject(JSON_CONDITION);
      cDefCondition->save(mConditionObject);
    }
  }

  void Terrain::loadCachedSurfaces(std::ifstream& cache) {
    if (cache) {
      // TODO: Make sure cache hasn't reached end
      unsigned char mEntityType;
      cache.read(reinterpret_cast<char*>(&mEntityType), sizeof(mEntityType));
      World& mWorld = cZone.getWorld();
      bool mPhysical;
      while (mEntityType != CACHE_BLOCK_END) {
        switch (mEntityType) {
          case CACHE_SURFACE: {
            cache.read(reinterpret_cast<char*>(&mPhysical), sizeof(mPhysical));
            std::unique_ptr<Surface> mSurface = std::make_unique<Surface>(cache, cZone.getWorld().getEquilibria().getTerrainStateConditionElements(), *cDefType, *this);
            std::vector<std::unique_ptr<ISurface>>* mSurfaces = mPhysical ?                                                              &cRuntimeSurfacesPhysical
                                                              : mSurface->getCondition().has_value() || cDefFlags & FLAG_FORCE_DYNAMIC ? &cRuntimeSurfacesDynamicVisual
                                                              :                                                                          &cRuntimeSurfacesStaticVisual;
            mSurfaces->emplace_back(std::move(mSurface));
            break;
          }

          case CACHE_SPLIT_SURFACE: {
            cache.read(reinterpret_cast<char*>(&mPhysical), sizeof(mPhysical));
            std::unique_ptr<SplitSurface> mSurface = std::make_unique<SplitSurface>(cache, cZone.getWorld().getEquilibria().getTerrainStateConditionElements(), *cDefType, *this);
            std::vector<std::unique_ptr<ISurface>>* mSurfaces = mPhysical ?                                                              &cRuntimeSurfacesPhysical
                                                              : mSurface->getCondition().has_value() || cDefFlags & FLAG_FORCE_DYNAMIC ? &cRuntimeSurfacesDynamicVisual
                                                              :                                                                          &cRuntimeSurfacesStaticVisual;
            mSurfaces->emplace_back(std::move(mSurface));
            break;
          }

          case CACHE_WALL: {
            loadWall(cache);
            break;
          }

          case CACHE_BLOCK_END: {
            // Nothing to do.
            return;
          }

          default:
            throw ParseException("Unknown entity type: " + Utils::toString(static_cast<int>(mEntityType)));

        }
        cache.read(reinterpret_cast<char*>(&mEntityType), sizeof(mEntityType));
      }

      for (std::unique_ptr<ISurface>& mSurface : cRuntimeSurfacesPhysical) {
        mWorld.attachPhysicalSurface(mSurface.get(), false);
      }
      for (std::unique_ptr<Wall>& mWall : cRuntimeWallsPhysical) {
        mWorld.attachPhysicalWall(mWall.get());
      }
    }
  }

  void Terrain::saveCachedSurfaces(std::ostream& cache) {
    for (std::unique_ptr<ISurface>& mSurface : cRuntimeSurfacesStaticVisual) {
      mSurface->saveCache(cache, false);
    }
    for (std::unique_ptr<ISurface>& mSurface : cRuntimeSurfacesDynamicVisual) {
      mSurface->saveCache(cache, false);
    }
    for (std::unique_ptr<ISurface>& mSurface : cRuntimeSurfacesPhysical) {
      mSurface->saveCache(cache, true);
    }
    for (std::unique_ptr<Wall>& mWall : cRuntimeWallsStaticVisual) {
      mWall->saveCache(cache, false);
    }
    for (std::unique_ptr<Wall>& mWall : cRuntimeWallsDynamicVisual) {
      mWall->saveCache(cache, false);
    }
    for (std::unique_ptr<Wall>& mWall : cRuntimeWallsPhysical) {
      mWall->saveCache(cache, true);
    }
    cache.write(reinterpret_cast<const char*>(&CACHE_BLOCK_END), sizeof(CACHE_BLOCK_END));
  }

  void Terrain::loadWall(std::ifstream& cache) {
    // TODO: Make consistent with how surfaces are loaded
    bool mPhysical;
    int mX;
    int mY;
    int mZ;
    int mLength;
    int mHeight;
    int mSlopeTop;
    int mSlopeBottom;
    std::string mDirection;
    Wall::Direction mFacing;

    cache.read(reinterpret_cast<char*>(&mPhysical),    sizeof(mPhysical));
    cache.read(reinterpret_cast<char*>(&mX),           sizeof(mX));
    cache.read(reinterpret_cast<char*>(&mY),           sizeof(mY));
    cache.read(reinterpret_cast<char*>(&mZ),           sizeof(mZ));
    cache.read(reinterpret_cast<char*>(&mLength),      sizeof(mLength));
    cache.read(reinterpret_cast<char*>(&mHeight),      sizeof(mHeight));
    cache.read(reinterpret_cast<char*>(&mSlopeTop),    sizeof(mSlopeTop));
    cache.read(reinterpret_cast<char*>(&mSlopeBottom), sizeof(mSlopeBottom));
    cache.read(reinterpret_cast<char*>(&mFacing),      sizeof(mFacing));

    if (mSlopeTop < 0) {
      mHeight += mSlopeTop * mLength;
    }
    
    std::optional<Condition> mWallCondition;
    char mNextEntity;
    cache.read(reinterpret_cast<char*>(&mNextEntity), sizeof(mNextEntity));
    if (mNextEntity == Terrain::CACHE_CONDITION) {
      mWallCondition = std::make_optional<Condition>(cache, cZone.getWorld().getEquilibria().getTerrainStateConditionElements(), CACHE_CONDITION, CACHE_CONDITION_ELEMENT, CACHE_CONDITION_END);
    }
    
    std::vector<std::unique_ptr<Wall>>* mWalls = mPhysical ?                                                    &cRuntimeWallsPhysical
                                               : mWallCondition.has_value() || cDefFlags & FLAG_FORCE_DYNAMIC ? &cRuntimeWallsDynamicVisual
                                               :                                                                &cRuntimeWallsStaticVisual;
    mWalls->emplace_back(std::make_unique<Wall>(mX, mY, mZ, mLength, mHeight, mSlopeTop, mSlopeBottom, mFacing, mWallCondition, *cDefType, *this));
  }

  int Terrain::getXSlope() const {
    return Equilibria::getTerrainSlope(cDefCornerHeight[0][1], cDefCornerHeight[0][0], cDefCornerHeight[1][1], cDefCornerHeight[1][0]);
  }

  int Terrain::getYSlope() const {
    return Equilibria::getTerrainSlope(cDefCornerHeight[0][0], cDefCornerHeight[1][0], cDefCornerHeight[0][1], cDefCornerHeight[1][1]);
  }

  std::vector<std::unique_ptr<ISurface>> Terrain::generateSurfaces(ISurface::Direction faceDirection) {
    std::vector<std::unique_ptr<ISurface>> mRawSurfaces;
    int mXSlope = getXSlope();
    int mYSlope = getYSlope();
    if (cDefFlags & FLAG_STEPPED_BOTTOM && faceDirection == ISurface::Direction::DOWN) {
      if (mXSlope != 0 && mYSlope != 0) {
        for (int y = cDefStartY; y <= cDefEndY; y++) {
          for (int x = cDefStartX; x <= cDefEndX; x++) {
            std::unique_ptr<ISurface> mRawSurface = createSurface(faceDirection, y, x, y, x, cDefCondition);
            mRawSurfaces.emplace_back(std::move(mRawSurface));
          }
        }
      } else if (mYSlope != 0) {
        for (int y = cDefStartY; y <= cDefEndY; y++) {
          std::unique_ptr<ISurface> mRawSurface = createSurface(faceDirection, y, cDefEndX, y, cDefStartX, cDefCondition);
          mRawSurfaces.emplace_back(std::move(mRawSurface));
        }
      } else if (mXSlope != 0) {
        for (int x = cDefStartX; x <= cDefEndX; x++) {
          std::unique_ptr<ISurface> mRawSurface = createSurface(faceDirection, cDefEndY, x, cDefStartY, x, cDefCondition);
          mRawSurfaces.emplace_back(std::move(mRawSurface));
        }
      } else {
        std::unique_ptr<ISurface> mRawSurface = createSurface(faceDirection, cDefEndY, cDefEndX, cDefStartY, cDefStartX, cDefCondition);
        mRawSurfaces.emplace_back(std::move(mRawSurface));
      }
    } else {
      std::unique_ptr<ISurface> mRawSurface = createSurface(faceDirection, cDefEndY, cDefEndX, cDefStartY, cDefStartX, cDefCondition);
      mRawSurfaces.emplace_back(std::move(mRawSurface));
    }
    return mRawSurfaces;
  }

  int Terrain::getBottomHeight(int x, int y) const {
    return cDefFlags & FLAG_STEPPED_BOTTOM ? getSurfaceHeight(x, y) - (cDefEndZ - cDefStartZ)
                                           : cDefStartZ;
  }

  std::unique_ptr<ISurface> Terrain::createSurface(ISurface::Direction direction, int north, int east, int south, int west, std::optional<Condition>& condition) {
    switch (direction) {
      case ISurface::Direction::UP: {
        int mXSlope = getXSlope();
        int mYSlope = getYSlope();
        int mHeight = getSurfaceHeight(mXSlope > 0 ? west : east, mYSlope > 0 ? south : north);
        if (isSplit()) {
          return std::make_unique<SplitSurface>(cDefFlags & FLAG_ALTERNATIVE_SPLIT, cDefStartX, cDefStartY, cDefEndZ, cDefCornerHeight[0][0], cDefCornerHeight[1][0], cDefCornerHeight[0][1], cDefCornerHeight[1][1], condition, *cDefType, *this);
        } else {
          return std::make_unique<Surface>(north, east, south, west, mHeight, mXSlope, mYSlope, direction, condition, cDefType, *this);
        }
      }
      
      case ISurface::Direction::DOWN: {
        // TODO: Make sure the surface does not violate the stepping
        int mHeight = getBottomHeight(east, north);
        return std::make_unique<Surface>(north, east, south, west, mHeight, 0, 0, direction, condition, nullptr, *this);
      }
    }
    throw ArgumentException("WARNING: Terrain::createSurface: Invalid value for argument \"direction\"");
  }

  int Terrain::getSurfaceHeight(int x, int y) const {
    return abs(((getXSlope() >= 0 ? cDefStartX : cDefEndX) - x) * getXSlope())
         + abs(((getYSlope() >= 0 ? cDefStartY : cDefEndY) - y) * getYSlope())
         + cDefEndZ;
  }

  int Terrain::getWallSlope(Wall::Direction direction) {
    switch (direction) {
      case Wall::Direction::NORTH: return cDefCornerHeight[1][1] - cDefCornerHeight[0][1];
      case Wall::Direction::EAST:  return cDefCornerHeight[1][1] - cDefCornerHeight[1][0];
      case Wall::Direction::SOUTH: return cDefCornerHeight[1][0] - cDefCornerHeight[0][0];
      case Wall::Direction::WEST:  return cDefCornerHeight[0][1] - cDefCornerHeight[0][0];
    }
    throw ArgumentException("WARNING: Terrain::getWallSlope: Invalid value for argument \"direction\"");
  }

  int Terrain::getMinimumWallElevation(Wall::Direction direction) {
    switch (direction) {
      case Wall::Direction::NORTH: return std::min(cDefCornerHeight[1][1], cDefCornerHeight[0][1]);
      case Wall::Direction::EAST:  return std::min(cDefCornerHeight[1][1], cDefCornerHeight[1][0]);
      case Wall::Direction::SOUTH: return std::min(cDefCornerHeight[1][0], cDefCornerHeight[0][0]);
      case Wall::Direction::WEST:  return std::min(cDefCornerHeight[0][1], cDefCornerHeight[0][0]);
    }
    throw ArgumentException("WARNING: Terrain::getMinimumWallElevation: Invalid value for argument \"direction\"");
  }

  std::unique_ptr<Wall> Terrain::createWall(WallTemplate* wallTemplate) {
    int mX                              = wallTemplate->getX();
    int mY                              = wallTemplate->getY();
    int mLength                         = wallTemplate->getLength();
    Wall::Direction mFaceDirection      = wallTemplate->getFaceDirection();
    int mStartHeight                    = wallTemplate->getStartHeight();
    int mEndHeight                      = wallTemplate->getEndHeight();
    int mBottomSlope                    = wallTemplate->getBottomSlope();
    int mTopSlope                       = wallTemplate->getTopSlope();
    std::optional<Condition> mCondition = wallTemplate->getCondition();
    return std::make_unique<Wall>(mX, mY, mStartHeight, mLength, mEndHeight, mTopSlope, mBottomSlope, mFaceDirection, mCondition, *cDefType, *this);
  }

  std::vector<std::unique_ptr<Wall>> Terrain::generateWalls(Wall::Direction facing, int location) {
    bool mFacesPole = facing == Wall::Direction::NORTH || facing == Wall::Direction::SOUTH;
    int mSlope = getWallSlope(facing);
    std::vector<std::unique_ptr<Wall>> mWalls;
    if (cDefFlags & FLAG_STEPPED_BOTTOM && mSlope != 0) {
      int mStart = mFacesPole ? cDefStartX : cDefStartY;
      int mEnd   = mFacesPole ? cDefEndX   : cDefEndY;
      for (int i = mStart; i <= mEnd; i++) {
        int mX = mFacesPole ? i : location;
        int mY = mFacesPole ? location : i;
        int mBaseHeight = getBottomHeight(mX, mY);
        int mOtherAxisSlope = mFacesPole ? getYSlope() : getXSlope();
        int mHeight = cDefEndZ - cDefStartZ + std::max(0, (facing == Wall::Direction::SOUTH || facing == Wall::Direction::WEST) ? -mOtherAxisSlope : mOtherAxisSlope);

        mWalls.emplace_back(std::make_unique<Wall>(mX, mY, mBaseHeight, 1, mHeight, mSlope, 0, facing, cDefCondition, *cDefType, *this));
      }
    } else {
      int mX = mFacesPole ? cDefStartX : location;
      int mY = mFacesPole ? location : cDefStartY;
      int mLength = (mFacesPole ? cDefEndX - cDefStartX : cDefEndY - cDefStartY) + 1;
      int mLowestX = mFacesPole ? (mSlope > 0 ? cDefStartX : cDefEndX) : mX;
      int mLowestY = mFacesPole ? mY : (mSlope > 0 ? cDefStartY : cDefEndY);
      int mBaseHeight = getBottomHeight(mX, mY);
      int mHeight = cDefFlags & FLAG_STEPPED_BOTTOM ? (cDefEndZ - cDefStartZ) + getMinimumWallElevation(facing)
                                                    : (getSurfaceHeight(mLowestX, mLowestY) + getMinimumWallElevation(facing)) - cDefStartZ;
      if (!cDefType->isSolid() && mHeight > 0) {
        mHeight = -mHeight;
      }
      mWalls.emplace_back(std::make_unique<Wall>(mX, mY, mBaseHeight, mLength, mHeight, mSlope, 0, facing, cDefCondition, *cDefType, *this));
    }
    return mWalls;
  }

  int Terrain::getOuterWallFaceLocation(Wall::Direction direction) {
    switch (direction) {
      case Wall::Direction::NORTH: return cDefEndY;
      case Wall::Direction::EAST:  return cDefEndX;
      case Wall::Direction::SOUTH: return cDefStartY;
      case Wall::Direction::WEST:  return cDefStartX;
    }
    throw ArgumentException("Terrain::getOuterWallFaceLocation: Invalid value for argument \"direction\"");
  }

  std::vector<std::unique_ptr<IVisualElement>> Terrain::getStaticVisuals() {
    std::vector<std::unique_ptr<IVisualElement>> mAllVisuals;
    for (std::unique_ptr<ISurface>& mStaticSurface : cRuntimeSurfacesStaticVisual) {
      std::vector<std::unique_ptr<IVisualElement>> mTileVisuals = mStaticSurface->getStaticVisuals();
      mAllVisuals.insert(mAllVisuals.end(), std::make_move_iterator(mTileVisuals.begin()), std::make_move_iterator(mTileVisuals.end()));
    }
    for (std::unique_ptr<Wall>& mStaticWall : cRuntimeWallsStaticVisual) {
      std::vector<std::unique_ptr<IVisualElement>> mWallVisuals = mStaticWall->getStaticVisuals();
      mAllVisuals.insert(mAllVisuals.end(), std::make_move_iterator(mWallVisuals.begin()), std::make_move_iterator(mWallVisuals.end()));
    }
    return mAllVisuals;
  }

  void Terrain::renderRuntime() const {
    for (const std::unique_ptr<ISurface>& mSurface : cRuntimeSurfacesDynamicVisual) {
      mSurface->render();
    }
    for (const std::unique_ptr<Wall>& mWall : cRuntimeWallsDynamicVisual) {
      mWall->render();
    }
  }

  void Terrain::renderEditing() const {
    if (cDefStartZ > cDefEndZ) {
      float z       = getBottom();
      float zs      = getTop();
      glLineWidth(3.0f);
      glBegin(GL_LINES);
      glColor3f(1.0f, 0.0f, 0.0f);
      Utils::renderVolumeLines(cDefStartX - 0.5f, cDefEndX + 0.5f, cDefStartY - 0.5f, cDefEndY + 0.5f, z, zs);
      glColor3f(1.0f, 1.0f, 1.0f);
      glEnd();
      glLineWidth(1.0f);
    } else if (cDefCondition.has_value() && !cDefCondition.value().isTrue()) {
      for (const std::unique_ptr<ISurface>& mSurface : cRuntimeSurfacesDynamicVisual) {
        mSurface->renderOutline();
      }
      for (const std::unique_ptr<Wall>& mSurface : cRuntimeWallsDynamicVisual) {
        mSurface->renderOutline();
      }
    }
    renderRuntime();
  }

  Zone& Terrain::getZone() {
    return cZone;
  }

  int Terrain::getXStart() const {
    return cDefStartX;
  }

  int Terrain::getXEnd() const {
    return cDefEndX;
  }

  int Terrain::getYStart() const {
    return cDefStartY;
  }

  int Terrain::getYEnd() const {
    return cDefEndY;
  }

  float Terrain::getBottom() const {
    return std::min(cDefStartZ, cDefEndZ) * 0.5f;
  }

  float Terrain::getTop() const {
    int mWidth  = (cDefEndX - cDefStartX) + 1;
    int mLength = (cDefEndY - cDefStartY) + 1;
    int mXSlopeHeight = mWidth  * std::abs(getXSlope());
    int mYSlopeHeight = mLength * std::abs(getYSlope());
    int mHighestCorner = mXSlopeHeight + mYSlopeHeight == 0 ? std::max({cDefCornerHeight[1][0], cDefCornerHeight[0][0], cDefCornerHeight[1][1], cDefCornerHeight[0][1]}) : 0;
    return (std::max(cDefStartZ, cDefEndZ) + mXSlopeHeight + mYSlopeHeight + mHighestCorner) * 0.5f;
  }

  void Terrain::generateWalls(Wall::Direction faceDirection) {
    World& mWorld = cZone.getWorld();
    
    // Physical surfaces
    if (!(cDefFlags & FLAG_GHOST)) {
      std::vector<std::unique_ptr<WallTemplate>> mWalls = cZone.getWorld().createWallTemplates(this, faceDirection, false);
      for (unsigned int i = 0; i < mWalls.size(); i++) {
        std::unique_ptr<Wall> mWall = createWall(mWalls[i].get());
        Wall* mRawWall = cRuntimeWallsPhysical.emplace_back(std::move(mWall)).get();
        mWorld.attachPhysicalWall(mRawWall);
      }
    }

    // Visual surfaces
    if (!(cDefFlags & FLAG_INVISIBLE)) {
      std::vector<std::unique_ptr<WallTemplate>> mWalls = cZone.getWorld().createWallTemplates(this, faceDirection, true);
      for (unsigned int i = 0; i < mWalls.size(); i++) {
        std::optional<Condition>& mCondition = mWalls[i]->getCondition();
        std::unique_ptr<Wall> mWall = createWall(mWalls[i].get());
        if (mCondition.has_value() || cDefFlags & FLAG_FORCE_DYNAMIC) {
          cRuntimeWallsDynamicVisual.emplace_back(std::move(mWall));
        } else {
          cRuntimeWallsStaticVisual.emplace_back(std::move(mWall));
        }
      }
    }
  }

  void Terrain::generateSurfaces() {
    World& mWorld = cZone.getWorld();

    // Physical surfaces
    if (!(cDefFlags & FLAG_GHOST)) {
      std::vector<std::unique_ptr<SurfaceTemplate>> mTopSurfaces = cZone.getWorld().createSurfaceTemplates(this, ISurface::Direction::UP, false);
      for (unsigned int i = 0; i < mTopSurfaces.size(); i++) {
        int mNorth = mTopSurfaces[i]->getNorth();
        int mEast = mTopSurfaces[i]->getEast();
        int mSouth = mTopSurfaces[i]->getSouth();
        int mWest = mTopSurfaces[i]->getWest();
        std::optional<Condition>& mCondition = mTopSurfaces[i]->getCondition();
        std::unique_ptr<ISurface> mSurface = createSurface(ISurface::Direction::UP, mNorth, mEast, mSouth, mWest, mCondition);
        ISurface* mRawSurface = cRuntimeSurfacesPhysical.emplace_back(std::move(mSurface)).get();
        mWorld.attachPhysicalSurface(mRawSurface, false);
      }
    }

    // Visual surfaces
    if (!(cDefFlags & FLAG_INVISIBLE)) {
      std::vector<std::unique_ptr<SurfaceTemplate>> mTopSurfaces = cZone.getWorld().createSurfaceTemplates(this, ISurface::Direction::UP, true);
      for (unsigned int i = 0; i < mTopSurfaces.size(); i++) {
        int mNorth = mTopSurfaces[i]->getNorth();
        int mEast = mTopSurfaces[i]->getEast();
        int mSouth = mTopSurfaces[i]->getSouth();
        int mWest = mTopSurfaces[i]->getWest();
        std::optional<Condition>& mCondition = mTopSurfaces[i]->getCondition();
        std::unique_ptr<ISurface> mSurface = createSurface(ISurface::Direction::UP, mNorth, mEast, mSouth, mWest, mCondition);
        if (mCondition.has_value() || cDefFlags & FLAG_FORCE_DYNAMIC) {
          cRuntimeSurfacesDynamicVisual.emplace_back(std::move(mSurface));
        } else {
          cRuntimeSurfacesStaticVisual.emplace_back(std::move(mSurface));
        }
      }
    }

    generateWalls(Wall::Direction::SOUTH);
    generateWalls(Wall::Direction::NORTH);
    generateWalls(Wall::Direction::EAST);
    generateWalls(Wall::Direction::WEST);
  }

  bool Terrain::isFlat() {
    return cDefCornerHeight[0][1] == 0
        && cDefCornerHeight[1][1] == 0
        && cDefCornerHeight[0][0] == 0
        && cDefCornerHeight[1][0] == 0;
  }

  void Terrain::flagForInitialisation() {
    
    // Detach physical stuff from the world.
    World& mWorld = cZone.getWorld();
    for (const std::unique_ptr<ISurface>& mSurface : cRuntimeSurfacesPhysical) {
      mWorld.detachPhysicalSurface(mSurface.get());
    }
    for (const std::unique_ptr<Wall>& mWall : cRuntimeWallsPhysical) {
      mWorld.detachPhysicalWall(mWall.get());
    }

    // Clear all of our generated surfaces and walls.
    cRuntimeSurfacesStaticVisual.clear();
    cRuntimeSurfacesDynamicVisual.clear();
    cRuntimeSurfacesPhysical.clear();
    cRuntimeWallsStaticVisual.clear();
    cRuntimeWallsDynamicVisual.clear();
    cRuntimeWallsPhysical.clear();
    
    // Tell the parent zone that we want to regenerate our surfaces and walls.
    cZone.flagForInitialisation(this);
  }

  bool Terrain::isGhost() {
    return !cDefType->isSolid();
  }

  bool Terrain::isSplit() {
    return !isFlat() && getXSlope() == 0 && getYSlope() == 0;
  }

  unsigned int Terrain::getOrderIndex() {
    return cZone.getOrderIndex(this);
  }

  std::optional<Condition>& Terrain::getCondition() {
    return cDefCondition;
  }

  void Terrain::setFlags(char flags) {
    char mOldFlags = cDefFlags;
    cDefFlags = flags;
    if (mOldFlags != cDefFlags) {
      World& mWorld = cZone.getWorld();
      mWorld.updateTerrain(this, !(cDefFlags & FLAG_INVISIBLE), !(cDefFlags & FLAG_GHOST));
    }
  }

  void Terrain::removed() {
    World& mWorld = cZone.getWorld();
    for (std::unique_ptr<ISurface>& mSurface : cRuntimeSurfacesPhysical) {
      mWorld.detachPhysicalSurface(mSurface.get());
    }
    for (std::unique_ptr<Wall>& mWall : cRuntimeWallsPhysical) {
      mWorld.detachPhysicalWall(mWall.get());
    }
  }
  
  bool Terrain::isType(const TerrainType* const type) const {
    return cDefType == type;
  }

  bool Terrain::contains(const LiteralVertex& location) const {
    if (location.x >= cDefStartX && location.x <= cDefEndX && location.y >= cDefStartY && location.y <= cDefEndY) { // TODO: This is probably inaccurate for whatever it's purpose is.
      if (cDefStartZ > cDefEndZ) {
        return location.z > getSurfaceHeight(location.x, location.y) && location.z <= getBottomHeight(location.x, location.y);
      } else {
        return location.z > getBottomHeight(location.x, location.y) && location.z <= getSurfaceHeight(location.x, location.y);
      }
    }
    return false;
  }

  void Terrain::renderSelectionHighlight() const {
    glBindTexture(GL_TEXTURE_2D, 0);
    if (cDefStartZ > cDefEndZ) {
      float mSouth  = cDefStartY  - 0.501f;
      float mWest   = cDefStartX  - 0.501f;
      float mBottom = getBottom() - 0.001f;
      float mNorth  = cDefEndY    + 0.501f;
      float mEast   = cDefEndX    + 0.501f;
      float mTop    = getTop()    + 0.001f;
      glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
      Utils::renderVolumeCuboid(mWest, mEast, mSouth, mNorth, mBottom, mTop);
    } else if (cRuntimeSurfacesStaticVisual.empty() && cRuntimeWallsStaticVisual.empty() && cRuntimeSurfacesDynamicVisual.empty() && cRuntimeWallsDynamicVisual.empty()) {
      float mSouth  = cDefStartY  - 0.501f;
      float mWest   = cDefStartX  - 0.501f;
      float mBottom = getBottom() - 0.001f;
      float mNorth  = cDefEndY    + 0.501f;
      float mEast   = cDefEndX    + 0.501f;
      float mTop    = getTop()    + 0.001f;
      glColor4f(0.5f, 0.5f, 1.0f, 0.5f);
      Utils::renderVolumeCuboid(mWest, mEast, mSouth, mNorth, mBottom, mTop);
    } else {
      glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
      for (const std::unique_ptr<ISurface>& mSurface : cRuntimeSurfacesStaticVisual) {
        mSurface->renderHighlight();
      }
      for (const std::unique_ptr<Wall>& mWall : cRuntimeWallsStaticVisual) {
        mWall->renderSelectionHighlight();
      }
      for (const std::unique_ptr<ISurface>& mSurface : cRuntimeSurfacesDynamicVisual) {
        mSurface->renderHighlight();
      }
      for (const std::unique_ptr<Wall>& mWall : cRuntimeWallsDynamicVisual) {
        mWall->renderSelectionHighlight();
      }
    }
  }

  void Terrain::remove() {
    World& mWorld = cZone.getWorld();
    mWorld.unregisterTerrain(this);
    mWorld.flagTerrainForInitialisation(cDefStartX - 1, cDefEndX + 1, cDefStartY - 1, cDefEndY + 1);
    cZone.remove(this);
  }

  void Terrain::getProperties(IPropertyMaker& owner) {
    const Metadata& mMetadata = cZone.getWorld().getEquilibria().getMetadata("Terrain");
    std::vector<ConditionElement*> mElements = cDefType->getTerrainStateConditionElements();
    owner.createPropertyCondition(mMetadata.getPropertyData("Condition"), mElements, [this]()->std::optional<Condition>& {return cDefCondition;}, [this](std::optional<Condition>& condition) {
      cDefCondition = condition;
      cZone.getWorld().flagTerrainForInitialisation(cDefStartX - 1, cDefEndX + 1, cDefStartY - 1, cDefEndY + 1);
      cZone.updateDisplayList();
    });
    if (!cZone.getWorld().isBasicProperties()) {
      owner.createPropertyList(mMetadata.getPropertyData("Behaviour"),
                               std::vector<std::string>{BEHAVIOUR_NORMAL,
                                                        BEHAVIOUR_INVISIBLE,
                                                        BEHAVIOUR_GHOST,
                                                        BEHAVIOUR_DYNAMIC,
                                                        BEHAVIOUR_DYNAMIC_GHOST},
                               [this]() {
                                 return getBehaviourString();
                               }, [this](const std::string& value) {
                                 cDefFlags = (~cDefFlags & FLAG_BEHAVIOUR_MASK) | getBehaviourFlags(value);
                               });
    }
  }

  std::string Terrain::getTypeName() const {
    return "Terrain";
  }

  Zone& Terrain::getObjectZone() {
    return cZone;
  }
  
  std::string Terrain::getBehaviourString() const {
    return (cDefFlags & FLAG_BEHAVIOUR_MASK) ==  FLAG_INVISIBLE                   ? BEHAVIOUR_INVISIBLE
        :  (cDefFlags & FLAG_BEHAVIOUR_MASK) ==  FLAG_GHOST                       ? BEHAVIOUR_GHOST
        :  (cDefFlags & FLAG_BEHAVIOUR_MASK) ==  FLAG_FORCE_DYNAMIC               ? BEHAVIOUR_DYNAMIC
        :  (cDefFlags & FLAG_BEHAVIOUR_MASK) == (FLAG_GHOST | FLAG_FORCE_DYNAMIC) ? BEHAVIOUR_DYNAMIC_GHOST
        :                                                                           BEHAVIOUR_NORMAL;
  }
  
  char Terrain::getBehaviourFlags(const std::string& value) {
    return value == BEHAVIOUR_INVISIBLE     ? FLAG_INVISIBLE
         : value == BEHAVIOUR_GHOST         ? FLAG_GHOST
         : value == BEHAVIOUR_DYNAMIC       ? FLAG_FORCE_DYNAMIC
         : value == BEHAVIOUR_DYNAMIC_GHOST ? FLAG_FORCE_DYNAMIC | FLAG_FORCE_DYNAMIC
         :                                    FLAGS_NORMAL;
  }
}

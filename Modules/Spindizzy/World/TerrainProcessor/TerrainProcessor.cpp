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
#include "TerrainProcessor.h"

#include "IsoRealms/Project.h"

#include "Modules/Spindizzy/World/Object/Terrain/Terrain.h"
#include "Modules/Spindizzy/World/Object/Terrain/Wall.h"
#include "Modules/Spindizzy/World/Object/Zone/Zone.h"

namespace IsoRealms::Spindizzy {
  TerrainProcessor::TerrainProcessor(bool physical) :
            cPhysical(physical) {
  }

  void TerrainProcessor::registerTerrain(Terrain* terrain) {
    int mXStart = terrain->getXStart() ;
    int mXEnd   = terrain->getXEnd();
    int mYStart = terrain->getYStart();
    int mYEnd   = terrain->getYEnd();
    cCacheAccessMutex.lock();
    cTerrain.add(mXStart, mXEnd, mYStart, mYEnd, terrain);
    cCacheAccessMutex.unlock();
  }

  void TerrainProcessor::unregisterTerrain(Terrain* terrain) {
    cCacheAccessMutex.lock();
    cTerrain.remove(terrain);
    cCacheAccessMutex.unlock();
  }

  void TerrainProcessor::updateTerrain(Terrain* terrain, bool toRegister) {
    if (cTerrain.contains(terrain) && !toRegister) {
      unregisterTerrain(terrain);
    } else if (!cTerrain.contains(terrain) && toRegister) {
      registerTerrain(terrain);
    }
  }

  void TerrainProcessor::flagForInitialisation(int xStart, int xEnd, int yStart, int yEnd) {
    cCacheAccessMutex.lock();
    for (Terrain* mNearbyTerrain : cTerrain.search(xStart, xEnd, yStart, yEnd)) {
      mNearbyTerrain->flagForInitialisation();
    }
    
    // Also clear nearby cached surface columns.
    cSurfaceColumns.search(xStart, xEnd, yStart, yEnd).clear();
    cCacheAccessMutex.unlock();
  }



































  std::vector<Terrain*> TerrainProcessor::getTerrain(int south, int north, int west, int east) {
    std::vector<Terrain*> mNearbyTerrain;
    for (Terrain* mElement : cTerrain.search(west, east, south, north)) {
      mNearbyTerrain.emplace_back(mElement);
    }
    std::sort(mNearbyTerrain.begin(), mNearbyTerrain.end(), sort);
    return mNearbyTerrain;
  }

  bool sort(Terrain* a, Terrain* b) {
    unsigned int mIndexA = a->getOrderIndex();
    unsigned int mIndexB = b->getOrderIndex();
    return mIndexB > mIndexA; 
  }

  ISurface* TerrainProcessor::getSurfaceAt(std::vector<std::unique_ptr<ISurface>>& surfaces, int x, int y) {
    for (std::unique_ptr<ISurface>& mSurface : surfaces) {
      if (mSurface->alligned(x, y)) {
        return mSurface.get();
      }
    }
    return nullptr;
  }

  Wall* TerrainProcessor::findSurfaceAt(std::vector<std::unique_ptr<Wall>>& walls, int x, int y) {
    for (std::unique_ptr<Wall>& mWall : walls) {
      if (x >= mWall->getXStart() && x <= mWall->getXEnd() && y >= mWall->getYStart() && y <= mWall->getYEnd()) {
        return mWall.get();
      }
    }
    return nullptr;
  }

  TerrainProcessor::FullTileColumn* TerrainProcessor::getTileColumn(int x, int y) {
    cCacheAccessMutex.lock();
    for (std::unique_ptr<FullTileColumn>& mTileColumn : cSurfaceColumns.search(x, y)) {
      if (mTileColumn->isAt(x, y)) {
        cCacheAccessMutex.unlock();
        return mTileColumn.get();
      }
    }
    cCacheAccessMutex.unlock();
    return nullptr;
  }

  std::optional<Condition> TerrainProcessor::getSurfaceTileCondition(Terrain* terrain, int x, int y, ISurface::Direction facing, const std::vector<Terrain*>& nearbyTerrain) {
    FullTileColumn* mFullTileColumn = getTileColumn(x, y);

    // In case the full tile column isn't cached yet, we'll create it and add it to the cache for next time.
    if (mFullTileColumn == nullptr) {
      std::vector<std::unique_ptr<TileColumn>> mPossibleTileColumns;
      std::optional<Condition> mNoCondition;
      mPossibleTileColumns.emplace_back(std::make_unique<TileColumn>(mNoCondition));
  //    std::cout << (count++) << ": " << x << " , " << y << ": " << nearbyTerrain.size() << std::endl;
      for (unsigned int i = 0; i < nearbyTerrain.size(); i++) {
        std::vector<std::unique_ptr<ISurface>> mTopSurfaces    = nearbyTerrain[i]->generateSurfaces(ISurface::Direction::UP);
        std::vector<std::unique_ptr<ISurface>> mBottomSurfaces = nearbyTerrain[i]->generateSurfaces(ISurface::Direction::DOWN);
        std::optional<Condition>& mCondition       = nearbyTerrain[i]->getCondition();
        bool mGhost                                = nearbyTerrain[i]->isGhost();
        ISurface* mTopSurface = getSurfaceAt(mTopSurfaces, x, y);
        if (mTopSurface != nullptr) {
          
          // Split possible columns into mutually exclusive conditions
          std::vector<std::unique_ptr<TileColumn>> mSplitColumns;
          if (mCondition.has_value()) {
            for (unsigned int j = 0; j < mPossibleTileColumns.size(); j++) {
              std::unique_ptr<TileColumn> mTileColumn = mPossibleTileColumns[j]->split(mCondition);
              if (mTileColumn != nullptr) {
                mSplitColumns.push_back(std::move(mTileColumn));
              }
            }
          }
          for (unsigned int j = 0; j < mSplitColumns.size(); j++) {
            mPossibleTileColumns.push_back(std::move(mSplitColumns[j]));
          }
          for (unsigned int j = 0; j < mPossibleTileColumns.size(); j++) {
            std::optional<Condition>& mColumnCondition = mPossibleTileColumns[j]->getCondition();
            bool mIsCompatible = mColumnCondition.has_value() ? mColumnCondition->isCompatibleWith(mCondition)
                               : mCondition.has_value()       ? !mCondition->isAbsolute() || mCondition->isTrue()
                               :                                true;
            if (mIsCompatible) {
              ISurface* mBottomSurface = getSurfaceAt(mBottomSurfaces, x, y);
              int mTop = mTopSurface->getSurfaceCellHeight(x, y);
              int mBottom = mBottomSurface->getSurfaceCellHeight(x, y);
              bool mTopExtended = mTopSurface->getSurfaceCellElevation(x, y) != 0;
              bool mBottomExtended = mBottomSurface->getSurfaceCellElevation(x, y) != 0;
              std::unique_ptr<TileBlock> mTileBlock = std::make_unique<TileBlock>(nearbyTerrain[i], mTop, mBottom, mTopExtended, mBottomExtended);
              mPossibleTileColumns[j]->addTileBlock(std::move(mTileBlock), mGhost);
            }
          }
        }
      }

      // Add full tile column to the cache.
      cCacheAccessMutex.lock();
      std::unique_ptr<FullTileColumn> mNewTileColumn = std::make_unique<FullTileColumn>(std::move(mPossibleTileColumns), x, y);
      mFullTileColumn = mNewTileColumn.get();
      cSurfaceColumns.add(x, x, y, y, std::move(mNewTileColumn));
      cCacheAccessMutex.unlock();
    }

    // Now create the actual condition.
    std::optional<Condition> mComposedCondition;
    std::vector<std::unique_ptr<TileColumn>>* mCachedTileColumn = mFullTileColumn->getTileColumns();
    for (unsigned int i = 0; i < mCachedTileColumn->size(); i++) {
      if ((*mCachedTileColumn)[i]->isTileVisible(terrain, facing)) {
        std::optional<Condition>& mEnabledCondition = (*mCachedTileColumn)[i]->getCondition();
        if (mEnabledCondition.has_value()) {
          if (!mComposedCondition.has_value()) {
            mComposedCondition = Condition(false);
          }
          mComposedCondition = mComposedCondition->compose(mEnabledCondition);
        } else {
          mComposedCondition = Condition(true);
        }
      }
    }
    if (mComposedCondition.has_value() && mComposedCondition->isAbsolute() && mComposedCondition->isTrue()) {
      mComposedCondition.reset();
    } else if (!mComposedCondition.has_value()) {
      mComposedCondition = Condition(false);
    }
    return mComposedCondition;
  }

  bool TerrainProcessor::inSurface(std::vector<std::unique_ptr<SurfaceTemplate>>& surfaces, int x, int y) {
    for (unsigned int i = 0; i < surfaces.size(); i++) {
      if (surfaces[i]->alligned(x, y)) {
        return true;
      }
    }
    return false;
  }

  int TerrainProcessor::getNorth(Terrain* terrain, std::vector<std::unique_ptr<SurfaceTemplate>>& calculatedSurfaces, int west, int east, int south, ISurface::Direction facing, const std::vector<Terrain*>& nearbyTerrain) {
    std::optional<Condition> mSurfaceCondition = getSurfaceTileCondition(terrain, west, south, facing, nearbyTerrain);

    std::vector<std::unique_ptr<ISurface>> mRawSurfaces = terrain->generateSurfaces(facing);
    ISurface* mSurface = getSurfaceAt(mRawSurfaces, west, south);
    
    for (int y = south + 1; y <= mSurface->getYEnd(); y++) {
      for (int x = west; x <= east; x++) {
        std::optional<Condition> mTileCondition = getSurfaceTileCondition(terrain, x, y, facing, nearbyTerrain);
        ISurface* mOtherSurface = getSurfaceAt(mRawSurfaces, x, y);
        bool mSurfaceSame   = mSurface == mOtherSurface;
        bool mUsedTile      = inSurface(calculatedSurfaces, x, y); // TODO: Might be able to remove this test
        if (mSurfaceCondition != mTileCondition || !mSurfaceSame || mUsedTile) {
          return y - 1;
        }
      }
    }
    return mSurface->getYEnd();
  }

  int TerrainProcessor::getEast(Terrain* terrain, std::vector<std::unique_ptr<SurfaceTemplate>>& calculatedSurfaces, int x, int y, ISurface::Direction facing, const std::vector<Terrain*>& nearbyTerrain) {
    std::optional<Condition> mSurfaceCondition = getSurfaceTileCondition(terrain, x, y, facing, nearbyTerrain);
    std::vector<std::unique_ptr<ISurface>> mRawSurfaces = terrain->generateSurfaces(facing);
    ISurface* mSurface = getSurfaceAt(mRawSurfaces, x, y);

    for (int i = x + 1; i <= terrain->getXEnd(); i++) {
      std::optional<Condition> mTileCondition = getSurfaceTileCondition(terrain, i, y, facing, nearbyTerrain);
      ISurface* mOtherSurface = getSurfaceAt(mRawSurfaces, i, y);
      bool mSurfaceSame   = mSurface == mOtherSurface;
      bool mUsedTile      = inSurface(calculatedSurfaces, i, y);
      if (mSurfaceCondition != mTileCondition || !mSurfaceSame || mUsedTile) {
        return i - 1;
      }
    }
    return terrain->getXEnd();
  }

  std::vector<std::unique_ptr<SurfaceTemplate>> TerrainProcessor::getSurfaces(Terrain* terrain, ISurface::Direction facing) {
    std::vector<std::unique_ptr<SurfaceTemplate>> mCalculatedSurfaces;
    
    std::vector<Terrain*> mNearbyTerrain = getTerrain(terrain->getYStart(), terrain->getYEnd(), terrain->getXStart(), terrain->getXEnd());
    for (int y = terrain->getYStart(); y <= terrain->getYEnd(); y++) {
      for (int x = terrain->getXStart(); x <= terrain->getXEnd(); x++) {
        std::optional<Condition> mSurfaceCondition = getSurfaceTileCondition(terrain, x, y, facing, mNearbyTerrain);
        bool mTileOccupied = inSurface(mCalculatedSurfaces, x, y);
        bool mTileNeeded = !mSurfaceCondition.has_value() || !mSurfaceCondition->isAbsolute() || mSurfaceCondition->isTrue();

        if (!mTileOccupied && mTileNeeded) {
          int mXEnd = getEast(terrain, mCalculatedSurfaces, x, y, facing, mNearbyTerrain);
          int mYEnd = getNorth(terrain, mCalculatedSurfaces, x, mXEnd, y, facing, mNearbyTerrain);
          mCalculatedSurfaces.emplace_back(std::make_unique<SurfaceTemplate>(mYEnd, mXEnd, y, x, mSurfaceCondition));
        }
      }
    }
    return mCalculatedSurfaces;
  }

  /*
  * Wall processing
  */

  std::unique_ptr<WallColumn> TerrainProcessor::getRawWallColumn(Terrain* terrain, int x, int y, Wall::Direction facing) {
    bool mFacingPole = facing == Wall::Direction::NORTH || facing == Wall::Direction::SOUTH;
    int mRowLocation = mFacingPole ? y : x;
    int mCellLocation = mFacingPole ? x : y;
    std::vector<std::unique_ptr<Wall>> mWalls = terrain->generateWalls(facing, mRowLocation);
    Wall* mWall = findSurfaceAt(mWalls, x, y);
    if (mWall != nullptr) {
      Wall::WallEdge mTopEdge = mWall->getTopEdge(mCellLocation);
      Wall::WallEdge mBottomEdge = mWall->getBottomEdge(mCellLocation);
      std::optional<Condition> mCondition = terrain->getCondition();
      WallSegment mSegment(mBottomEdge.getStartHeight(), mBottomEdge.getEndHeight(), mTopEdge.getStartHeight(), mTopEdge.getEndHeight());
      return std::make_unique<WallColumn>(&mSegment, mCondition);
    }
    return nullptr;
  }

  std::vector<std::unique_ptr<WallColumn>> TerrainProcessor::getPhysicalWallColumn(Terrain* terrain, int x, int y, Wall::Direction facing, const std::vector<Terrain*>& nearbyTerrain) {
    std::unique_ptr<WallColumn> mRawWallColumn = getRawWallColumn(terrain, x, y, facing);
    std::vector<std::unique_ptr<WallColumn>> mPhysicalColumns;
    if (mRawWallColumn == nullptr || mRawWallColumn->isSubtraction()) {
      return mPhysicalColumns;
    }
    mPhysicalColumns.emplace_back(std::move(mRawWallColumn));
    unsigned int mCurrentIndex = terrain->getOrderIndex();
    for (unsigned int i = 0; i < nearbyTerrain.size(); i++) {
      unsigned int mComparisonIndex = nearbyTerrain[i]->getOrderIndex();
      if (mComparisonIndex <= mCurrentIndex) {
        continue;
      }
      if (x >= nearbyTerrain[i]->getXStart() && x <= nearbyTerrain[i]->getXEnd() && y >= nearbyTerrain[i]->getYStart() && y <= nearbyTerrain[i]->getYEnd()) {
        std::unique_ptr<WallColumn> mWallColumn = getRawWallColumn(nearbyTerrain[i], x, y, facing);
        if (!mWallColumn->empty()) {
          mWallColumn->convertToAddition();
          std::optional<Condition> mCondition = nearbyTerrain[i]->getCondition();
          std::vector<std::unique_ptr<WallColumn>> mSplitColumns;
          for (unsigned int j = 0; j < mPhysicalColumns.size(); j++) {
            std::unique_ptr<WallColumn> mNewPossibility = mPhysicalColumns[j]->split(mCondition);
            if (mNewPossibility != nullptr) {
              mSplitColumns.emplace_back(std::move(mNewPossibility));
            }
          }
          for (unsigned int j = 0; j < mSplitColumns.size(); j++) {
            mPhysicalColumns.emplace_back(std::move(mSplitColumns[j]));
          }

          for (unsigned int j = 0; j < mPhysicalColumns.size(); j++) {
            if (mPhysicalColumns[j]->isCompatibleWith(mCondition)) {
              mPhysicalColumns[j]->applyOverlapping(mWallColumn.get());
            }
          }
        }
      }
    }

    // Shave the top off invisible surfaces
    std::vector<std::unique_ptr<ISurface>> mSurfaces = terrain->generateSurfaces(ISurface::Direction::UP);
    ISurface* mSurface = getSurfaceAt(mSurfaces, x, y);
    int mHeight = mSurface->getSurfaceCellHeight(x, y);
    std::optional<Condition> mCondition = getSurfaceTileCondition(terrain, x, y, ISurface::Direction::UP, nearbyTerrain);
    for (unsigned int i = 0; i < mPhysicalColumns.size(); i++) {
      mPhysicalColumns[i]->shaveTop(mHeight, mCondition);
    }
    return mPhysicalColumns;
  }

  Wall::Direction TerrainProcessor::getOppositeOf(Wall::Direction facing) {
    switch (facing) {
      case Wall::Direction::NORTH: return Wall::Direction::SOUTH;
      case Wall::Direction::SOUTH: return Wall::Direction::NORTH;
      case Wall::Direction::WEST:  return Wall::Direction::EAST;
      case Wall::Direction::EAST:  return Wall::Direction::WEST;
    }
    throw ArgumentException("ERROR: TerrainProcessor::getOppositeOf: Argument value is not recognised.");
  }

  std::vector<std::unique_ptr<WallColumn>> TerrainProcessor::getPhysicalWallMasks(int x, int y, Wall::Direction facing, const std::vector<Terrain*>& nearbyTerrain) {
    std::vector<std::unique_ptr<WallColumn>> mOpposingMask;
    mOpposingMask.emplace_back(std::make_unique<WallColumn>());
    for (unsigned int i = 0; i < nearbyTerrain.size(); i++) {
      if (x >= nearbyTerrain[i]->getXStart() && x <= nearbyTerrain[i]->getXEnd() && y >= nearbyTerrain[i]->getYStart() && y <= nearbyTerrain[i]->getYEnd()) {
        std::vector<std::unique_ptr<WallColumn>> mOpposingPossibilities = getPhysicalWallColumn(nearbyTerrain[i], x, y, facing, nearbyTerrain);
        for (unsigned int k = 0; k < mOpposingPossibilities.size(); k++) {
          std::optional<Condition>& mCondition = mOpposingPossibilities[k]->getCondition();
          std::vector<std::unique_ptr<WallColumn>> mSplitColumns;
          for (unsigned int j = 0; j < mOpposingMask.size(); j++) {
            std::unique_ptr<WallColumn> mNewPossibility = mOpposingMask[j]->split(mCondition);
            if (mNewPossibility != nullptr) {
              mSplitColumns.emplace_back(std::move(mNewPossibility));
            }
          }
          for (unsigned int j = 0; j < mSplitColumns.size(); j++) {
            mOpposingMask.emplace_back(std::move(mSplitColumns[j]));
          }
          for (unsigned int j = 0; j < mOpposingMask.size(); j++) {
            if (mOpposingMask[j]->isCompatibleWith(mCondition)) {
              mOpposingMask[j]->unite(mOpposingPossibilities[k]);
            }
          }
        }
      }
    }
    return mOpposingMask;
  }

  std::vector<std::unique_ptr<WallColumn>> TerrainProcessor::getOptimisedWallColumn(Terrain* terrain, int x, int y, Wall::Direction facing, const std::vector<Terrain*>& nearbyTerrain) {
    std::vector<std::unique_ptr<WallColumn>> mWallColumns = getPhysicalWallColumn(terrain, x, y, facing, nearbyTerrain);
    switch (facing) {
      case Wall::Direction::NORTH: y++; break;
      case Wall::Direction::EAST:  x++; break;
      case Wall::Direction::SOUTH: y--; break;
      case Wall::Direction::WEST:  x--; break;
    }
    facing = getOppositeOf(facing);
    if (!cPhysical) {
      Zone& mZone = terrain->getZone();
      if (!mZone.aligned(x, y)) {
        return mWallColumns;
      }
    }
    std::vector<std::unique_ptr<WallColumn>> mWallMasks = getPhysicalWallMasks(x, y, facing, nearbyTerrain);

    for (unsigned int i = 0; i < mWallMasks.size(); i++) {
      std::optional<Condition>& mMaskCondition = mWallMasks[i]->getCondition();
      std::vector<std::unique_ptr<WallColumn>> mSplitColumns;
      for (unsigned int j = 0; j < mWallColumns.size(); j++) {
        std::unique_ptr<WallColumn> mNewPossibility = mWallColumns[j]->split(mMaskCondition);
        if (mNewPossibility != nullptr) {
          mSplitColumns.emplace_back(std::move(mNewPossibility));
        }
      }
      for (unsigned int j = 0; j < mSplitColumns.size(); j++) {
        mWallColumns.emplace_back(std::move(mSplitColumns[j]));
      }

      for (int j = mWallColumns.size() - 1; j >= 0; j--) {
        if (mWallColumns[j]->isCompatibleWith(mMaskCondition)) {
          mWallColumns[j]->removeHiddenSections(mWallMasks[i].get());
        }
      }
    }
    return mWallColumns;
  }

  std::vector<std::unique_ptr<WallColumn>> TerrainProcessor::getVisibleWallColumn(Terrain* terrain, int x, int y, Wall::Direction facing, const std::vector<Terrain*>& nearbyTerrain) {
    std::vector<std::unique_ptr<WallColumn>> mWallColumns = getOptimisedWallColumn(terrain, x, y, facing, nearbyTerrain);
    switch (facing) {
      case Wall::Direction::NORTH: y++; break;
      case Wall::Direction::EAST:  x++; break;
      case Wall::Direction::SOUTH: y--; break;
      case Wall::Direction::WEST:  x--; break;
    }
    facing = getOppositeOf(facing);
    std::vector<std::unique_ptr<WallColumn>> mWallMasks = getPhysicalWallMasks(x, y, facing, nearbyTerrain);

    for (unsigned int i = 0; i < mWallMasks.size(); i++) {
      WallColumn* mWallMask = mWallMasks[i].get();
      if (!mWallMask->empty()) {
        std::vector<std::unique_ptr<WallColumn>> mSplitColumns;
        std::optional<Condition>& mCondition = mWallMask->getCondition();
        for (unsigned int j = 0; j < mWallColumns.size(); j++) {
          std::unique_ptr<WallColumn> mNewPossibility = mWallColumns[j]->split(mCondition);
          if (mNewPossibility != nullptr) {
            mSplitColumns.emplace_back(std::move(mNewPossibility));
          }
        }
        for (unsigned int j = 0; j < mSplitColumns.size(); j++) {
          mWallColumns.emplace_back(std::move(mSplitColumns[j]));
        }

        for (unsigned int j = 0; j < mWallColumns.size(); j++) {
          if (mWallColumns[j]->isCompatibleWith(mCondition)) {
            mWallColumns[j]->applyOverlapping(mWallMask);
          }
        }
      }
    }

    // Shave the top off invisible surfaces
  /*  std::vector<ISurface*> mSurfaces = terrain->getSurfaces(ISurface::Direction::UP);
    ISurface* mSurface = getSurfaceAt(mSurfaces, x, y);
    int mHeight = mSurface->getSurfaceCellHeight(x, y);
    Condition* mCondition = getSurfaceTileCondition(terrain, x, y, ISurface::Direction::UP);
    for (unsigned int i = 0; i < mWallColumns.size(); i++) {
      mWallColumns[i]->shaveTop(mHeight, mCondition);
    }*/
    return mWallColumns;
  }

  int mInstanceCount = 0;

  std::vector<std::unique_ptr<WallTemplate>> TerrainProcessor::getWalls(Terrain* terrain, Wall::Direction facing) {
    bool mXWalls         = facing == Wall::Direction::WEST || facing == Wall::Direction::EAST;
    int mStartCell       = mXWalls ? terrain->getYStart() : terrain->getXStart();
    int mEndCell         = mXWalls ? terrain->getYEnd()   : terrain->getXEnd();
    int mStartRow        = mXWalls ? terrain->getXStart() : terrain->getYStart();
    int mEndRow          = mXWalls ? terrain->getXEnd()   : terrain->getYEnd();
    
    std::vector<Terrain*> mNearbyTerrain = getTerrain(terrain->getYStart() - (facing == Wall::Direction::SOUTH ? 1 : 0),
                                                      terrain->getYEnd()   + (facing == Wall::Direction::NORTH ? 1 : 0),
                                                      terrain->getXStart() - (facing == Wall::Direction::WEST  ? 1 : 0),
                                                      terrain->getXEnd()   + (facing == Wall::Direction::EAST  ? 1 : 0));
    
    std::vector<std::unique_ptr<WallTemplate>> mWalls;
    std::unique_ptr<std::vector<std::unique_ptr<WallTemplate>>> mExtendedConstructionData;
    std::unique_ptr<std::vector<std::unique_ptr<WallTemplate>>> mCompletedConstructionData = std::make_unique<std::vector<std::unique_ptr<WallTemplate>>>();
    for (int row = mStartRow; row <= mEndRow; row++) {
      for (int cell = mStartCell; cell <= mEndCell; cell++) {
        mExtendedConstructionData = std::make_unique<std::vector<std::unique_ptr<WallTemplate>>>();
        int mX = mXWalls ? row : cell;
        int mY = mXWalls ? cell : row;
        std::vector<std::unique_ptr<WallColumn>> mWallColumns = cPhysical ? getVisibleWallColumn(terrain, mX, mY, facing, mNearbyTerrain)
                                                                        : getOptimisedWallColumn(terrain, mX, mY, facing, mNearbyTerrain);
        for (unsigned int j = 0; j < mWallColumns.size(); j++) {
          std::vector<std::unique_ptr<WallSegment>>& mSections = mWallColumns[j]->getSections();
          std::optional<Condition>& mColumnCondition = mWallColumns[j]->getCondition();
          for (unsigned int i = 0; i < mSections.size(); i++) {

            // Attempt to extend each surface with this column
            bool mTileUsed = false;
            for (int j = mCompletedConstructionData->size() - 1; j >= 0; j--) {
              if ((*mCompletedConstructionData)[j]->unite(mSections[i].get(), mColumnCondition)) {
                mExtendedConstructionData->emplace_back(std::move((*mCompletedConstructionData)[j]));
                mCompletedConstructionData->erase(mCompletedConstructionData->begin() + j);
                mTileUsed = true;
                break;
              }
            }

            if (!mTileUsed) {
              mExtendedConstructionData->emplace_back(std::make_unique<WallTemplate>(mX, mY, mSections[i].get(), mColumnCondition, facing));
            }
          }
        }

        for (unsigned int i = 0; i < mCompletedConstructionData->size(); i++) {
          mWalls.emplace_back(std::move((*mCompletedConstructionData)[i]));
        }

        // The extended columns are ready in the next iteration
        mCompletedConstructionData = std::move(mExtendedConstructionData);
      }

      // End of row reached; we must construct any pending datas now
      for (unsigned int i = 0; i < mCompletedConstructionData->size(); i++) {
        mWalls.emplace_back(std::move((*mCompletedConstructionData)[i]));
      }
      mCompletedConstructionData->clear();
    }
    return mWalls;
  }

  TerrainProcessor::FullTileColumn::FullTileColumn(std::vector<std::unique_ptr<TileColumn>> tileColumns, int x, int y) :
              cX(x),
              cY(y),
              cSurfaceColumns(std::move(tileColumns)) {
  }

  bool TerrainProcessor::FullTileColumn::isAt(int x, int y) {
    return cX == x && cY == y;
  }

  std::vector<std::unique_ptr<TileColumn>>* TerrainProcessor::FullTileColumn::getTileColumns() {
    return &cSurfaceColumns;
  }

  int TerrainProcessor::FullTileColumn::getY() const {
    return cY;
  }

  int TerrainProcessor::FullTileColumn::getX() const {
    return cX;
  }
}

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
#include "BlockSubtractor.h"

void BlockSubtractor::notifyZoneAction(IZone* zone) {
  cCache.setZone(zone);
}

void BlockSubtractor::initPlugin(IZone* zone) {
  cCache.setZone(zone);
}

void BlockSubtractor::registerSurfaceProvider(ISurfaceProvider* provider) {
  cCache.add(provider);
}

void BlockSubtractor::unregisterSurfaceProvider(ISurfaceProvider* provider) {
  cCache.remove(provider);
}

void BlockSubtractor::setDirty() {
  cCache.setDirty();
}

void BlockSubtractor::reinitialise() {
  cCache.reinitialise();
}

bool BlockSubtractor::isSurfaceTileRemoved(int testTileHeight, int testTileElevation, int topTileHeight, int topTileElevation, int bottomTileHeight, int bottomTileElevation, bool columnHasPriority) {
  if (topTileHeight >= bottomTileHeight) {
  
    // Handle an addition
    if (columnHasPriority) {
    
      // Column surface must only be equal to the test to replace it
      return topTileHeight + (topTileElevation > 0 ? 1 : 0) >= testTileHeight + (testTileElevation > 0 ? 1 : 0) && topTileHeight >= testTileHeight && bottomTileHeight < testTileHeight + (testTileElevation > 0 ? 1 : 0);
    } else { // Tile has priority

      // Column surface must exceed the test to replace it
      return (topTileHeight == testTileHeight ? topTileHeight + (topTileElevation > 0 ? 1 : 0) > testTileHeight + (testTileElevation > 0 ? 1 : 0)
                                              : topTileHeight + (topTileElevation > 0 ? 1 : 0) >= testTileHeight + (testTileElevation > 0 ? 1 : 0))
                            && bottomTileHeight < testTileHeight + (testTileElevation > 0 ? 1 : 0);
    }
  }

  // Handle a subtraction  
  return /*columnHasPriority                                    // Subtraction must have priority to remove the surface
      && */bottomTileHeight >= testTileHeight                   // Tile must be inside the subtraction base heights
      && topTileHeight <= testTileHeight                      // Tile must be inside the subtraction base heights
      && topTileHeight < testTileHeight + testTileElevation;  // Tile must meet the subtraction including elevation
}

bool BlockSubtractor::isSurfaceTileHidden(int testTileHeight, int testTileElevation, int topTileHeight, int topTileElevation, int bottomTileHeight, int bottomTileElevation) {
  return testTileElevation == 0                              // Hiding onlysupp orted for flat surfaces.
      && topTileHeight + topTileElevation > testTileHeight   // Column must extend above the surface
      && bottomTileHeight <= testTileHeight;                 // Column must extend to at least cover the surface
}

ITileSurface* BlockSubtractor::getSurfaceAt(std::vector<ITileSurface*> surfaces, int x, int y) {
  for (unsigned int i = 0; i < surfaces.size(); i++) {
    BlockArea* mSurfaceCoverage = surfaces[i]->getCoverage();
    // TODO: Deallocate mSurfaceCoverage
    if (mSurfaceCoverage->alligned(x, y)) {
      return surfaces[i];
    }
  }
  return NULL;
}

IWallSurface* BlockSubtractor::findSurfaceAt(std::vector<IWallSurface*> surfaces, int x, int y) {
  for (unsigned int i = 0; i < surfaces.size(); i++) {
    BlockArea* mSurfaceCoverage = surfaces[i]->getCoverage();
    // TODO: Deallocate mSurfaceCoverage
    if (mSurfaceCoverage->alligned(x, y)) {
      return surfaces[i];
    }
  }
  return NULL;
}

bool BlockSubtractor::isSurfaceTileVisible(ITileSurface* surface, int x, int y, std::vector<ISurfaceProvider*> columns, bool columnHasPriority, ITileSurface::FaceDirection facing, bool isTileHidden) {
  bool mIsTileHidden = isTileHidden;
  bool mIsTopSurface = facing == ITileSurface::UP;
  int mSurfaceHeight    = mIsTopSurface ? surface->getSurfaceCellHeight(x, y)    : -surface->getSurfaceCellHeight(x, y);
  int mSurfaceElevation = mIsTopSurface ? surface->getSurfaceCellElevation(x, y) : -surface->getSurfaceCellElevation(x, y);
  for (unsigned int i = 0; i < columns.size(); i++) {
    BlockArea* mBlockArea = columns[i]->getCoverage();
    if (mBlockArea->alligned(x, y)) {
      std::vector<ITileSurface*> mTopSurfaces = columns[i]->getTileSurfaces(ITileSurface::UP);
      std::vector<ITileSurface*> mBottomSurfaces = columns[i]->getTileSurfaces(ITileSurface::DOWN);
      ITileSurface* mTopSurface = getSurfaceAt(mTopSurfaces, x, y);
      ITileSurface* mBottomSurface = getSurfaceAt(mBottomSurfaces, x, y);
      int mBottomHeight    = mIsTopSurface ? mBottomSurface->getSurfaceCellHeight(x, y)    : -mBottomSurface->getSurfaceCellHeight(x, y);
      int mBottomElevation = mIsTopSurface ? mBottomSurface->getSurfaceCellElevation(x, y) : -mBottomSurface->getSurfaceCellElevation(x, y);
      int mTopHeight       = mIsTopSurface ? mTopSurface->getSurfaceCellHeight(x, y)       : -mTopSurface->getSurfaceCellHeight(x, y);
      int mTopElevation    = mIsTopSurface ? mTopSurface->getSurfaceCellElevation(x, y)    : -mTopSurface->getSurfaceCellElevation(x, y);
      bool mIsSubtraction = mBottomHeight > mTopHeight;
      if (isSurfaceTileRemoved(mSurfaceHeight, mSurfaceElevation, mTopHeight, mTopElevation, mBottomHeight, mBottomElevation, columnHasPriority)) {
        if (columnHasPriority) {
          return false;
        } else if (!columnHasPriority) {
          mIsTileHidden = !mIsSubtraction;
        }
      } 
      if (mIsSubtraction ? isSurfaceTileHidden(mSurfaceHeight, mSurfaceElevation, mBottomHeight, mBottomElevation, mTopHeight,    mTopElevation)
                         : isSurfaceTileHidden(mSurfaceHeight, mSurfaceElevation, mTopHeight,    mTopElevation,    mBottomHeight, mBottomElevation)) {
        mIsTileHidden = !mIsSubtraction;
      }
    }
  }
  return !mIsTileHidden;
}

bool BlockSubtractor::isSurfaceTileVisible(ISurfaceProvider* provider, ITileSurface* surface, int x, int y, ITileSurface::FaceDirection facing) {
  // TODO: Cache needs to know what it's relating to (e.g. the ISurfaceProvider) to determine priority
  std::vector<ITileSurface*> mTopSurfaces = provider->getTileSurfaces(ITileSurface::UP);
  std::vector<ITileSurface*> mBottomSurfaces = provider->getTileSurfaces(ITileSurface::DOWN);
  ITileSurface* mTopSurface = getSurfaceAt(mTopSurfaces, x, y);
  ITileSurface* mBottomSurface = getSurfaceAt(mBottomSurfaces, x, y);
  int mTopHeight = mTopSurface->getSurfaceCellHeight(x, y);
  int mBottomHeight = mBottomSurface->getSurfaceCellHeight(x, y);
  bool mIsSubtraction = mBottomHeight > mTopHeight;
  std::vector<ISurfaceProvider*> mSurfaceColumnsWithoutPriority = cCache.getSurfaceProviders(false, provider);
  if (!isSurfaceTileVisible(surface, x, y, mSurfaceColumnsWithoutPriority, false, facing, mIsSubtraction) != mIsSubtraction) {
    return false;
  }
  std::vector<ISurfaceProvider*> mSurfaceColumnsWithPriority = cCache.getSurfaceProviders(true, provider);
  return isSurfaceTileVisible(surface, x, y, mSurfaceColumnsWithPriority, true, facing, false);
}

bool BlockSubtractor::inSurface(std::vector<ITileSurface*> surfaces, int x, int y) {
  for (unsigned int i = 0; i < surfaces.size(); i++) {
    if (surfaces[i]->alligned(x, y)) {
      return true;
    }
  }
  return false;
}

int BlockSubtractor::getCompleteRows(ISurfaceProvider* provider, std::vector<ITileSurface*> calculatedSurfaces, ITileSurface* surface, int west, int east, int south, ITileSurface::FaceDirection facing/*, Condition* condition TODO:CONDITIONAL*/) {
  BlockArea* mSurfaceCoverage = surface->getCoverage();
  if (south == mSurfaceCoverage->getNorth()) {
    return south;
  }

  for (int y = south + 1; y <= mSurfaceCoverage->getNorth(); y++) { // TODO: Isn't the first row ALWAYS going to be true?
    bool mCompleteRow = true;
    for (int x = west; x <= east; x++) {
      // TODO: ==================== CONDITIONAL CONSIDERATIONS NEED TO GO HERE ACCORDING TO PROTOTYPE!
      if (!isSurfaceTileVisible(provider, surface, x, y, facing)/*TODO:CONDITIONAL || !mConditionSame*/) {
        mCompleteRow = false;
        break;
      }
    }
    if (!mCompleteRow) {
      return y - 1;
    } 
  }
  return mSurfaceCoverage->getNorth();
  // TODO: No return yet; deliberate warning to finish implementing this function!
}

std::vector<ITileSurface*> BlockSubtractor::getTileSurfaces(ISurfaceProvider* provider, ITileSurface::FaceDirection faceDirection) {
  std::vector<ITileSurface*> mTileSurfaces = provider->getTileSurfaces(faceDirection);
  BlockArea* mBlockCoverage = provider->getCoverage();

  int mWest;
  int mEast;
  int mSouth;
  bool mDefiningSurface = false;
  std::vector<ITileSurface*> mCalculatedSurfaces;
  ITileSurface* mPreviousSurface = NULL;
  for (int y = mBlockCoverage->getSouth(); y <= mBlockCoverage->getNorth(); y++) {
    for (int x = mBlockCoverage->getWest(); x <= mBlockCoverage->getEast(); x++) {
      ITileSurface* mSurface = getSurfaceAt(mTileSurfaces, x, y);
      bool mOccupiedByCalculatedSurface = inSurface(mCalculatedSurfaces, x, y);
      bool mSurfaceCellVisible = isSurfaceTileVisible(provider, mSurface, x, y, faceDirection);
      if (mPreviousSurface == NULL) {
        mPreviousSurface = mSurface;
      }
      bool mSurfaceSame = mSurface == mPreviousSurface;
      // TODO: ==================== CONDITIONAL CONSIDERATIONS NEED TO GO HERE ACCORDING TO PROTOTYPE!
      if (mSurfaceCellVisible && !mOccupiedByCalculatedSurface) {
        if (!mDefiningSurface && mSurface != NULL) {
          mWest = x;
          mEast = x;
          mSouth = y;
// TODO:CONDITIONAL          mConditionSame = true; // Condition by definition cannot have changed when we start
          mDefiningSurface = true;
        } else if (/*&& mConditionSame TODO:CONDITIONAL && */mDefiningSurface && x >= mWest && mSurface == mPreviousSurface) {
          mEast = x;
        } 
      }

      /*
       * We must finish constructing the surface when we encounter one of the following:
       * 
       *   - A hidden / removed surface cell.
       *   - The location of an already constructed surface rectangle.
       *   - A physical surface that is different from the current one.
       *   - A surface cell that has a different condition than the current one.
       *   - The end of the current row.
       */
      if (mDefiningSurface && (x == mBlockCoverage->getEast() || !mSurfaceCellVisible || mOccupiedByCalculatedSurface || !mSurfaceSame /*|| !mConditionSame TODO:CONDITIONAL*/)) {
        // TODO: DUPLICATE SUBSURFACE!
        // TODO: ==================== CONDITIONAL CONSIDERATIONS NEED TO GO HERE ACCORDING TO PROTOTYPE!
        int mNorth = getCompleteRows(provider, mCalculatedSurfaces, mPreviousSurface, mWest, mEast, mSouth, faceDirection/*, mThisCondition TODO:CONDITIONAL*/); 
        mCalculatedSurfaces.push_back(provider->createSubSurface(faceDirection, mNorth, mEast, mSouth, mWest));
        mDefiningSurface = false;
        // TODO: DUPLICATE SUBSURFACE END!
        if (mSurfaceCellVisible && !mOccupiedByCalculatedSurface && (!mSurfaceSame /*|| !mConditionSame TODO:CONDITIONAL*/)) {
          mWest = x;
          mEast = x;
          mSouth = y;
// TODO:CONDITIONAL          mConditionSame = true; // Condition by definition cannot have changed when we start
          mDefiningSurface = true;
          if (x == mBlockCoverage->getEast()) {
            // TODO: DUPLICATE SUBSURFACE!
            // TODO: ==================== CONDITIONAL CONSIDERATIONS NEED TO GO HERE ACCORDING TO PROTOTYPE!
            int mNorth = getCompleteRows(provider, mCalculatedSurfaces, mPreviousSurface, mWest, mEast, mSouth, faceDirection/*, mThisCondition TODO:CONDITIONAL*/); 
            mCalculatedSurfaces.push_back(provider->createSubSurface(faceDirection, mNorth, mEast, mSouth, mWest));
            mDefiningSurface = false;
            // TODO: DUPLICATE SUBSURFACE END!
          }
        }
      }
      mPreviousSurface = mSurface;
    }
    mPreviousSurface = NULL;
  }
  return mCalculatedSurfaces;
}

/*
 * Wall processing
 */

WallColumn* BlockSubtractor::getRawWallColumn(ISurfaceProvider* provider, int x, int y, IWallSurface::FaceDirection facing) {
  bool mFacingPole = facing == IWallSurface::NORTH || facing == IWallSurface::SOUTH;
  int mRowLocation = mFacingPole ? y : x;
  int mCellLocation = mFacingPole ? x : y;
  std::vector<IWallSurface*> mWallSurfaces = provider->getWallSurfaces(mRowLocation, facing);
  IWallSurface* mWallSurface = findSurfaceAt(mWallSurfaces, x, y);
  if (mWallSurface != NULL) {
    IWallEdge* mTopEdge = mWallSurface->getTopEdge(mCellLocation);
    IWallEdge* mBottomEdge = mWallSurface->getBottomEdge(mCellLocation);
    return new WallColumn(mBottomEdge->getStartHeight(), mBottomEdge->getEndHeight(), mTopEdge->getStartHeight(), mTopEdge->getEndHeight());
  }
  return NULL;
}

std::vector<WallColumn*> BlockSubtractor::getPhysicalWallColumn(ISurfaceProvider* provider, int x, int y, IWallSurface::FaceDirection facing) {
  WallColumn* mRawWallColumn = getRawWallColumn(provider, x, y, facing);
  std::vector<WallColumn*> mProcessedWallColumns;
  if (mRawWallColumn == NULL) {
    return mProcessedWallColumns;
  }
  mProcessedWallColumns.push_back(mRawWallColumn);
  std::vector<ISurfaceProvider*> mSurfaceProviders = cCache.getSurfaceProviders(true, provider);
  for (unsigned int i = 0; i < mSurfaceProviders.size(); i++) {
    BlockArea* mBlockArea = mSurfaceProviders[i]->getCoverage();
    if (mBlockArea->alligned(x, y)) {
      WallColumn* mWallColumn = getRawWallColumn(mSurfaceProviders[i], x, y, facing);
      if (mWallColumn != NULL) {
        mWallColumn->convertToAddition();
        std::vector<int> mIndicesToRemove;
        std::vector<WallColumn*> mColumnsAdded;
        for (unsigned int j = 0; j < mProcessedWallColumns.size(); j++) {
          switch (mProcessedWallColumns[j]->getRelationWith(mWallColumn)) {
            case WallColumn::INSIDE: {
              WallColumn* mSplitColumn = new WallColumn(*mProcessedWallColumns[j]);
              mSplitColumn->lowerTop(mWallColumn);
              mProcessedWallColumns[j]->raiseBottom(mWallColumn);
              if (mSplitColumn->isAddition()) {
                mColumnsAdded.push_back(mSplitColumn);
              }
              if (!mProcessedWallColumns[j]->isAddition()) {
                mIndicesToRemove.push_back(j);
              }
              break;
            }
            
            case WallColumn::COVERING: {
              mIndicesToRemove.push_back(j);
              break;
            }
  
            case WallColumn::HIGHER: {
              if (!mProcessedWallColumns[j]->lowerTop(mWallColumn)) {
                mIndicesToRemove.push_back(j);
              }
              break;
            }
  
            case WallColumn::LOWER: {
              if (!mProcessedWallColumns[j]->raiseBottom(mWallColumn)) {
                mIndicesToRemove.push_back(j);
              }
              break;
            }
          }
        }
        for (int j = mIndicesToRemove.size() - 1; j >= 0; j--) {
          mProcessedWallColumns.erase(mProcessedWallColumns.begin() + mIndicesToRemove[j]);
        }
        mIndicesToRemove.clear();
        for (unsigned int j = 0; j < mColumnsAdded.size(); j++) {
          mProcessedWallColumns.push_back(mColumnsAdded[j]);
        }
        mColumnsAdded.clear();
      }
    }
  }
  return mProcessedWallColumns;
}

IWallSurface::FaceDirection BlockSubtractor::getOppositeOf(IWallSurface::FaceDirection facing) {
  switch (facing) {
    case IWallSurface::NORTH: return IWallSurface::SOUTH;
    case IWallSurface::SOUTH: return IWallSurface::NORTH;
    case IWallSurface::WEST:  return IWallSurface::EAST;
    case IWallSurface::EAST:  return IWallSurface::WEST;
  }
  std::cout << "ERROR: Unknown face direction" << std::endl;
  exit(1);
}

void BlockSubtractor::uniteColumns(std::vector<WallColumn*>* result, std::vector<WallColumn*> toAdd) {
  for (unsigned int i = 0; i < toAdd.size(); i++) {
    result->push_back(toAdd[i]);
  }

  for (int i = 0; i < (signed int) (result->size() - 1); i++) {
    for (unsigned int j = i + 1; j < result->size(); j++) {
      if ((*result)[i]->unite((*result)[j])) {
        result->erase(result->begin() + j--);
      }
    }
  }
}

std::vector<WallColumn*> BlockSubtractor::getOptimisedWallColumn(ISurfaceProvider* provider, int x, int y, IWallSurface::FaceDirection facing) {
  std::vector<WallColumn*> mPhysicalWallColumn = getPhysicalWallColumn(provider, x, y, facing);
  switch (facing) {
    case IWallSurface::NORTH: y++; break;
    case IWallSurface::EAST:  x++; break;
    case IWallSurface::SOUTH: y--; break;
    case IWallSurface::WEST:  x--; break;
  }
  facing = getOppositeOf(facing);

  std::vector<ISurfaceProvider*> mSurfaceProviders = cCache.getSurfaceProviders();
  std::vector<WallColumn*> mOpposingMask;
  for (unsigned int i = 0; i < mSurfaceProviders.size(); i++) {
    BlockArea* mCoverage = mSurfaceProviders[i]->getCoverage();
    if (mCoverage->alligned(x, y)/*TODO:CONDITIONAL && !(mExclusionBlock->isDynamic()) */) {
      std::vector<WallColumn*> mOpposingFaces = getPhysicalWallColumn(mSurfaceProviders[i], x, y, facing);
      uniteColumns(&mOpposingMask, mOpposingFaces);
    }
  }

  for (unsigned int i = 0; i < mOpposingMask.size(); i++) {
    for (int j = mPhysicalWallColumn.size() - 1; j >= 0; j--) {
      if (mOpposingMask[i]->isCovering(mPhysicalWallColumn[j])) {
        mPhysicalWallColumn.erase(mPhysicalWallColumn.begin() + j);
      }
    }
  }
  return mPhysicalWallColumn;
}

std::vector<IWallSurface*> BlockSubtractor::getWallSurfaces(ISurfaceProvider* provider, IWallSurface::FaceDirection facing) {
  BlockArea* mCoverage = provider->getCoverage();
  bool mXWalls = facing == IWallSurface::WEST || facing == IWallSurface::EAST;
  int mStartCell = mXWalls ? mCoverage->getSouth() : mCoverage->getWest();
  int mEndCell = mXWalls ? mCoverage->getNorth() : mCoverage->getEast();
  int mStartRow = mXWalls ? mCoverage->getWest() : mCoverage->getSouth();
  int mEndRow = mXWalls ? mCoverage->getEast() : mCoverage->getNorth();
  std::vector<IWallSurface*> mWallSurfaces;
  std::vector<WallConstructionData*>* mExtendedConstructionData;
  std::vector<WallConstructionData*>* mCompletedConstructionData = new std::vector<WallConstructionData*>();
  for (int row = mStartRow; row <= mEndRow; row++) {
    for (int cell = mStartCell; cell <= mEndCell; cell++) {
      mExtendedConstructionData = new std::vector<WallConstructionData*>();
      int mX = mXWalls ? row : cell;
      int mY = mXWalls ? cell : row;
      std::vector<WallColumn*> mWallColumns = getOptimisedWallColumn(provider, mX, mY, facing);

      // TODO:CONDITIONAL; Here's where we would loop through MANY SETS of WallColumns, each of different condition instead of just one.
      for (unsigned int i = 0; i < mWallColumns.size(); i++) {

        // Attempt to extend each surface with this column
        bool mTileUsed = false;
        for (int j = mCompletedConstructionData->size() - 1; j >= 0; j--) {
          if ((*mCompletedConstructionData)[j]->unite(mWallColumns[i])) {
            mExtendedConstructionData->push_back((*mCompletedConstructionData)[j]);
            mCompletedConstructionData->erase(mCompletedConstructionData->begin() + j);
            mTileUsed = true;
            break;
          }
        }

        if (!mTileUsed) {
          WallConstructionData* mNewConstructionData = new WallConstructionData(mWallColumns[i]);
          mExtendedConstructionData->push_back(mNewConstructionData);
        }
      }

      for (unsigned int i = 0; i < mCompletedConstructionData->size(); i++) {
        IWallSurface* mWallSurface = (*mCompletedConstructionData)[i]->constructSurface(provider, mX - (mXWalls ? 0 : 1), mY - (mXWalls ? 1 : 0), facing);
        mWallSurfaces.push_back(mWallSurface);
      }

      // The extended columns are ready in the next iteration
      delete mCompletedConstructionData;
      mCompletedConstructionData = mExtendedConstructionData;
    }

    // End of row reached; we must construct any pending datas now
    int mX = mXWalls ? row : mEndCell;
    int mY = mXWalls ? mEndCell : row;
    for (unsigned int i = 0; i < mExtendedConstructionData->size(); i++) {
      IWallSurface* mWallSurface = (*mExtendedConstructionData)[i]->constructSurface(provider, mX, mY, facing);
      mWallSurfaces.push_back(mWallSurface);
    }
    mExtendedConstructionData->clear();
  }
  return mWallSurfaces;
}

std::string BlockSubtractor::getName() {
  return "Block Subtractor";
}

extern "C" IPlugin* create() {
  return new BlockSubtractor();
}

extern "C" void destroy(IPlugin* blockSubtractor) {
  delete blockSubtractor;
}

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

void BlockSubtractor::initPlugin(IZone* zone, unsigned int pass) {
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

Condition* BlockSubtractor::getSurfaceTileCondition(ISurfaceProvider* provider, int x, int y, ITileSurface::FaceDirection facing) {
  std::vector<TileColumn*> mPossibleTileColumns;
  mPossibleTileColumns.push_back(new TileColumn(new Condition(true)));
  std::vector<ISurfaceProvider*> mSurfaceProviders = cCache.getSurfaceProviders();
  for (unsigned int i = 0; i < mSurfaceProviders.size(); i++) {
    std::vector<ITileSurface*> mTopSurfaces    = mSurfaceProviders[i]->getTileSurfaces(ITileSurface::UP);
    std::vector<ITileSurface*> mBottomSurfaces = mSurfaceProviders[i]->getTileSurfaces(ITileSurface::DOWN);
    Condition* mCondition                      = mSurfaceProviders[i]->getCondition();
    bool mGhost                                = mSurfaceProviders[i]->isGhost();
    ITileSurface* mTopSurface = getSurfaceAt(mTopSurfaces, x, y);
    if (mTopSurface != NULL) {
      ITileSurface* mBottomSurface = getSurfaceAt(mBottomSurfaces, x, y);
      int mTop = mTopSurface->getSurfaceCellHeight(x, y);
      int mBottom = mBottomSurface->getSurfaceCellHeight(x, y);
      bool mTopExtended = mTopSurface->getSurfaceCellElevation(x, y) != 0;
      bool mBottomExtended = mBottomSurface->getSurfaceCellElevation(x, y) != 0;
      TileBlock* mTileBlock = new TileBlock(mSurfaceProviders[i], mTop, mBottom, mTopExtended, mBottomExtended);
      
      // Split possible columns into mutually exclusive conditions
      std::vector<TileColumn*> mSplitColumns;
      if (mCondition != NULL) {
        for (unsigned int i = 0; i < mPossibleTileColumns.size(); i++) {
          TileColumn* mTileColumn = mPossibleTileColumns[i]->split(mCondition);
          if (mTileColumn != NULL) {
            mSplitColumns.push_back(mTileColumn);
          }
        }
      }
      for (unsigned int i = 0; i < mSplitColumns.size(); i++) {
        mPossibleTileColumns.push_back(mSplitColumns[i]);
      }
      for (unsigned int i = 0; i < mPossibleTileColumns.size(); i++) {
        Condition* mColumnCondition = mPossibleTileColumns[i]->getCondition();
        if (mCondition == NULL || mColumnCondition->isCompatibleWith(mCondition)) {
          mPossibleTileColumns[i]->addTileBlock(mTileBlock, mGhost, mCondition);
        }
//        mPossibleTileColumns[i]->debug();
      }
    }
  }
  Condition* mComposedCondition = new Condition(false);
  for (unsigned int i = 0; i < mPossibleTileColumns.size(); i++) {
    if (mPossibleTileColumns[i]->isTileVisible(provider, facing)) {
      Condition* mEnabledCondition = mPossibleTileColumns[i]->getCondition();
      mComposedCondition->compose(mEnabledCondition);
    }
  }
  return mComposedCondition;
}

bool BlockSubtractor::inSurface(std::vector<ITileSurfaceTemplate*> surfaces, int x, int y) {
  for (unsigned int i = 0; i < surfaces.size(); i++) {
    if (surfaces[i]->alligned(x, y)) {
      return true;
    }
  }
  return false;
}

int BlockSubtractor::getNorth(ISurfaceProvider* provider, std::vector<ITileSurfaceTemplate*>& calculatedSurfaces, int west, int east, int south, ITileSurface::FaceDirection faceDirection) {
  Condition* mSurfaceCondition = getSurfaceTileCondition(provider, west, south, faceDirection);
  std::vector<ITileSurface*> mRawSurfaces = provider->getTileSurfaces(faceDirection);
  ITileSurface* mSurface = getSurfaceAt(mRawSurfaces, west, south);
  
  BlockArea* mSurfaceCoverage = mSurface->getCoverage();
  for (int y = south + 1; y <= mSurfaceCoverage->getNorth(); y++) {
    for (int x = west; x <= east; x++) {
      Condition* mTileCondition = getSurfaceTileCondition(provider, x, y, faceDirection);
      ITileSurface* mTileSurface = getSurfaceAt(mRawSurfaces, x, y);
      bool mConditionSame = *mSurfaceCondition == *mTileCondition;
      bool mSurfaceSame   = mTileSurface == mSurface;
      bool mUsedTile      = inSurface(calculatedSurfaces, x, y); // TODO: Might be able to remove this test
      if (!mConditionSame || !mSurfaceSame || mUsedTile) {
        return y - 1;
      }
    }
  }
  return mSurfaceCoverage->getNorth();
}

int BlockSubtractor::getEast(ISurfaceProvider* provider, std::vector<ITileSurfaceTemplate*>& calculatedSurfaces, int x, int y, ITileSurface::FaceDirection faceDirection) {
  Condition* mSurfaceCondition = getSurfaceTileCondition(provider, x, y, faceDirection);
  std::vector<ITileSurface*> mRawSurfaces = provider->getTileSurfaces(faceDirection);
  ITileSurface* mSurface = getSurfaceAt(mRawSurfaces, x, y);

  BlockArea* mBlockCoverage = provider->getCoverage();
  for (int i = x + 1; i <= mBlockCoverage->getEast(); i++) {
    Condition* mTileCondition = getSurfaceTileCondition(provider, i, y, faceDirection);
    ITileSurface* mTileSurface = getSurfaceAt(mRawSurfaces, i, y);
    bool mConditionSame = *mSurfaceCondition == *mTileCondition;
    bool mSurfaceSame   = mTileSurface == mSurface;
    bool mUsedTile      = inSurface(calculatedSurfaces, i, y);
    if (!mConditionSame || !mSurfaceSame || mUsedTile) {
      return i - 1;
    }
  }
  return mBlockCoverage->getEast();
}

std::vector<ITileSurfaceTemplate*> BlockSubtractor::getTileSurfaces(ISurfaceProvider* provider, ITileSurface::FaceDirection faceDirection) {
  std::vector<ITileSurfaceTemplate*> mCalculatedSurfaces;
  
  BlockArea* mBlockCoverage = provider->getCoverage();
  for (int y = mBlockCoverage->getSouth(); y <= mBlockCoverage->getNorth(); y++) {
    for (int x = mBlockCoverage->getWest(); x <= mBlockCoverage->getEast(); x++) {
      Condition* mSurfaceCondition = getSurfaceTileCondition(provider, x, y, faceDirection);
      bool mTileOccupied = inSurface(mCalculatedSurfaces, x, y);
      bool mTileNeeded = !mSurfaceCondition->isAbsolute() || mSurfaceCondition->isTrue();

      if (!mTileOccupied && mTileNeeded) {
        int mEast = getEast(provider, mCalculatedSurfaces, x, y, faceDirection);
        int mNorth = getNorth(provider, mCalculatedSurfaces, x, mEast, y, faceDirection); 
/*        std::cout << "CREATING SURFACE WITH CONDITION:" << std::endl;
        mSurfaceCondition->debug();*/
        if (mSurfaceCondition->isAbsolute() && mSurfaceCondition->isTrue()) {
          mSurfaceCondition = NULL;
        }
        ITileSurfaceTemplate* mTileSurfaceTemplate = new TileSurfaceTemplate(mNorth, mEast, y, x, mSurfaceCondition);
        mCalculatedSurfaces.push_back(mTileSurfaceTemplate);
      }
    }
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
  for (int i = mProcessedWallColumns.size() - 1; i >= 0; i--) {
    // TODO: This should not only be based on the start bottom height, but also the end bottom height aswell.
    int mWallBottom = mProcessedWallColumns[i]->getBottomHeightStart();
    std::vector<ITileSurface*> mTileSurfaces = provider->getTileSurfaces(ITileSurface::UP);
    ITileSurface* mTileSurface = getSurfaceAt(mTileSurfaces, x, y);
    int mHeight = mTileSurface->getSurfaceCellHeight(x, y);
    
    // TODO: Take condition into real consideration
    Condition* mCondition = getSurfaceTileCondition(provider, x, y, ITileSurface::UP);
    if (mWallBottom >= mHeight && mCondition->isAbsolute() && !mCondition->isTrue()) {
      mProcessedWallColumns.erase(mProcessedWallColumns.begin() + i);
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

extern "C" IPlugin* create() {
  return new BlockSubtractor();
}

extern "C" void destroy(IPlugin* blockSubtractor) {
  delete blockSubtractor;
}

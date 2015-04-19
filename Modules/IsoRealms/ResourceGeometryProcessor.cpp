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
#include "ResourceGeometryProcessor.h"

ResourceGeometryProcessor::ResourceGeometryProcessor(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cRemoveHiddenSurfaces = false;
  cCompareOtherContainers = false;
  cCacheAccessMutex = SDL_CreateMutex();
  cIndex = 0;
}

void ResourceGeometryProcessor::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cRemoveHiddenSurfaces = node->getBooleanAttribute("removeHiddenSurfaces");
  cCompareOtherContainers = node->getBooleanAttribute("compareOtherContainers");
}

void ResourceGeometryProcessor::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("removeHiddenSurfaces", cRemoveHiddenSurfaces ? "true" : "false");
  node->addAttribute("compareOtherContainers", cCompareOtherContainers ? "true" : "false");
}

std::vector<ResourceGeometryProcessor::IndexedGeometricElement*> ResourceGeometryProcessor::getGeometricElements(int x, int y) {
  while (SDL_mutexP(cCacheAccessMutex) == -1);
  std::vector<IndexedGeometricElement*> mIndexedGeometricElements = cGeometricElements.getElements(x, y);
  SDL_mutexV(cCacheAccessMutex);
  std::sort(mIndexedGeometricElements.begin(), mIndexedGeometricElements.end(), sort);
  return mIndexedGeometricElements;
}

bool sort(ResourceGeometryProcessor::IndexedGeometricElement* a, ResourceGeometryProcessor::IndexedGeometricElement* b) {
  unsigned int mIndexA = a->getIndex();
  unsigned int mIndexB = b->getIndex();
  return mIndexB > mIndexA; 
}

void ResourceGeometryProcessor::registerGeometricElement(IGeometricElement* element) {
//  cCache->add(element);
  IndexedGeometricElement* mIndexedGeometricElement = new IndexedGeometricElement(element, cIndex++);
  cGeometricElements.add(mIndexedGeometricElement);
}

void ResourceGeometryProcessor::unregisterGeometricElement(IGeometricElement* element) {
  BlockArea* mBounds = element->getCoverage();
  int mSouth = mBounds->getSouth();
  int mNorth = mBounds->getNorth();
  int mWest = mBounds->getWest();
  int mEast = mBounds->getEast();
  while (SDL_mutexP(cCacheAccessMutex) == -1);
  std::vector<IndexedGeometricElement*> mIndexedGeometricElements = cGeometricElements.getElements(mSouth, mNorth, mWest, mEast);
  for (unsigned int i = 0; i < mIndexedGeometricElements.size(); i++) {
    IGeometricElement* mElement = mIndexedGeometricElements[i]->getGeometricElement();
    if (mElement == element) {
      cGeometricElements.remove(mIndexedGeometricElements[i]);
    }
    mElement->setDirty();
  }
  std::vector<FullTileColumn*> mTileColumns = cTileColumns.getElements(mSouth, mNorth, mWest, mEast);
  for (unsigned int i = 0; i < mTileColumns.size(); i++) {
    cTileColumns.remove(mTileColumns[i]);
  }
  SDL_mutexV(cCacheAccessMutex);
}

void ResourceGeometryProcessor::setDirty() {
//  cCache->setDirty();
//   for (std::map<IZone*, ResourceGeometryProcessorCache*>::iterator i = cCache.begin(); i != cCache.end(); i++) {
//     i->second->setDirty();
//   }
}

void ResourceGeometryProcessor::reinitialise() {
//  cCache->reinitialise();
//   for (std::map<IZone*, ResourceGeometryProcessorCache*>::iterator i = cCache.begin(); i != cCache.end(); i++) {
//     i->second->reinitialise();
//   }
}

ITileSurface* ResourceGeometryProcessor::getSurfaceAt(std::vector<ITileSurface*> surfaces, int x, int y) {
  for (unsigned int i = 0; i < surfaces.size(); i++) {
    BlockArea* mSurfaceCoverage = surfaces[i]->getCoverage();
    bool mIsAligned = mSurfaceCoverage->alligned(x, y);
    surfaces[i]->destroyCoverage(mSurfaceCoverage);
    if (mIsAligned) {
      return surfaces[i];
    }
  }
  return nullptr;
}

IWallSurface* ResourceGeometryProcessor::findSurfaceAt(std::vector<IWallSurface*> surfaces, int x, int y) {
  for (unsigned int i = 0; i < surfaces.size(); i++) {
    BlockArea* mSurfaceCoverage = surfaces[i]->getCoverage();
    bool mIsAligned = mSurfaceCoverage->alligned(x, y);
    surfaces[i]->destroyCoverage(mSurfaceCoverage);
    if (mIsAligned) {
      return surfaces[i];
    }
  }
  return nullptr;
}

int count = 0;

ResourceGeometryProcessor::FullTileColumn* ResourceGeometryProcessor::getTileColumn(int x, int y) {
  while (SDL_mutexP(cCacheAccessMutex) == -1);
  std::vector<FullTileColumn*> mTileColumns = cTileColumns.getElements(x, y);
  for (unsigned int i = 0; i < mTileColumns.size(); i++) {
    if (mTileColumns[i]->isAt(x, y)) {
      SDL_mutexV(cCacheAccessMutex);
      return mTileColumns[i];
    }
  }
  SDL_mutexV(cCacheAccessMutex);
  return nullptr;
}

Condition* ResourceGeometryProcessor::getSurfaceTileCondition(IGeometricElement* element, int x, int y, ITileSurface::FaceDirection facing) {
  FullTileColumn* mFullTileColumn = getTileColumn(x, y);
  if (mFullTileColumn == nullptr) {
    std::vector<TileColumn*> mPossibleTileColumns;
    mPossibleTileColumns.push_back(new TileColumn(nullptr));
    std::vector<IndexedGeometricElement*> mGeometricElements = getGeometricElements(x, y);
//    std::cout << (count++) << ": " << x << " , " << y << ": " << mGeometricElements.size() << std::endl;
    for (unsigned int i = 0; i < mGeometricElements.size(); i++) {
      IGeometricElement* mGeometricElement = mGeometricElements[i]->getGeometricElement();
      std::vector<ITileSurface*> mTopSurfaces    = mGeometricElement->getTileSurfaces(ITileSurface::UP);
      std::vector<ITileSurface*> mBottomSurfaces = mGeometricElement->getTileSurfaces(ITileSurface::DOWN);
      Condition* mCondition                      = mGeometricElement->getCondition();
      bool mGhost                                = mGeometricElement->isGhost();
      ITileSurface* mTopSurface = getSurfaceAt(mTopSurfaces, x, y);
      if (mTopSurface != nullptr) {
        ITileSurface* mBottomSurface = getSurfaceAt(mBottomSurfaces, x, y);
        int mTop = mTopSurface->getSurfaceCellHeight(x, y);
        int mBottom = mBottomSurface->getSurfaceCellHeight(x, y);
        bool mTopExtended = mTopSurface->getSurfaceCellElevation(x, y) != 0;
        bool mBottomExtended = mBottomSurface->getSurfaceCellElevation(x, y) != 0;
        TileBlock* mTileBlock = new TileBlock(mGeometricElement, mTop, mBottom, mTopExtended, mBottomExtended);
        
        // Split possible columns into mutually exclusive conditions
        std::vector<TileColumn*> mSplitColumns;
        if (mCondition != nullptr) {
          for (unsigned int j = 0; j < mPossibleTileColumns.size(); j++) {
            TileColumn* mTileColumn = mPossibleTileColumns[j]->split(mCondition);
            if (mTileColumn != nullptr) {
              mSplitColumns.push_back(mTileColumn);
            }
          }
        }
        for (unsigned int j = 0; j < mSplitColumns.size(); j++) {
          mPossibleTileColumns.push_back(mSplitColumns[j]);
        }
        bool mTileBlockUsed = false;
        for (unsigned int j = 0; j < mPossibleTileColumns.size(); j++) {
          Condition* mColumnCondition = mPossibleTileColumns[j]->getCondition();
          bool mIsCompatible = mColumnCondition != nullptr ? mColumnCondition->isCompatibleWith(mCondition)
                             : mCondition != nullptr       ? !mCondition->isAbsolute() || mCondition->isTrue()
                             :                               true;
          if (mIsCompatible) {
            if (mPossibleTileColumns[j]->addTileBlock(mTileBlock, mGhost, mCondition)) {
              mTileBlockUsed = true;
            }
          }
          if (mColumnCondition != nullptr) {
            delete mColumnCondition;
          }
        }
        if (!mTileBlockUsed) {
          delete mTileBlock;
        }
      }
    }
    mFullTileColumn = new FullTileColumn(mPossibleTileColumns, x, y);
    while (SDL_mutexP(cCacheAccessMutex) == -1);
    cTileColumns.add(mFullTileColumn);
    SDL_mutexV(cCacheAccessMutex);
  }
  Condition* mComposedCondition = nullptr;
  std::vector<TileColumn*>* mCachedTileColumn = mFullTileColumn->getTileColumns();
  for (unsigned int i = 0; i < mCachedTileColumn->size(); i++) {
    if ((*mCachedTileColumn)[i]->isTileVisible(element, facing)) {
      Condition* mEnabledCondition = (*mCachedTileColumn)[i]->getCondition();
      if (mEnabledCondition != nullptr) {
        if (mComposedCondition == nullptr) {
          mComposedCondition = new Condition(false);
        }
        Condition* mModifiedCondition = mComposedCondition->compose(mEnabledCondition);
        delete mEnabledCondition;
        delete mComposedCondition;
        mComposedCondition = mModifiedCondition;
      } else {
        if (mComposedCondition != nullptr) {
          delete mComposedCondition;
        }
        mComposedCondition = new Condition(true);
      }
    }
  }
  if (mComposedCondition != nullptr && mComposedCondition->isAbsolute() && mComposedCondition->isTrue()) {
    delete mComposedCondition;
    mComposedCondition = nullptr;
  } else if (mComposedCondition == nullptr) {
    mComposedCondition = new Condition(false);
  }
  return mComposedCondition;
}

bool ResourceGeometryProcessor::inSurface(std::vector<ITileSurfaceTemplate*> surfaces, int x, int y) {
  for (unsigned int i = 0; i < surfaces.size(); i++) {
    if (surfaces[i]->alligned(x, y)) {
      return true;
    }
  }
  return false;
}

bool ResourceGeometryProcessor::safeEquals(Condition* a, Condition* b) {
  return a == nullptr ? b == nullptr
       : b == nullptr ? false
       : *a == *b;
}

int ResourceGeometryProcessor::getNorth(IGeometricElement* element, std::vector<ITileSurfaceTemplate*>& calculatedSurfaces, int west, int east, int south, ITileSurface::FaceDirection faceDirection) {
  Condition* mSurfaceCondition = getSurfaceTileCondition(element, west, south, faceDirection);

  std::vector<ITileSurface*> mRawSurfaces = element->getTileSurfaces(faceDirection);
  ITileSurface* mSurface = getSurfaceAt(mRawSurfaces, west, south);
  
  BlockArea* mSurfaceCoverage = mSurface->getCoverage();
  for (int y = south + 1; y <= mSurfaceCoverage->getNorth(); y++) {
    for (int x = west; x <= east; x++) {
      Condition* mTileCondition = getSurfaceTileCondition(element, x, y, faceDirection);
      ITileSurface* mTileSurface = getSurfaceAt(mRawSurfaces, x, y);
      bool mConditionSame = safeEquals(mSurfaceCondition, mTileCondition);
      if (mTileCondition != nullptr) {
        delete mTileCondition;
      }
      bool mSurfaceSame   = mTileSurface == mSurface;
      bool mUsedTile      = inSurface(calculatedSurfaces, x, y); // TODO: Might be able to remove this test
      if (!mConditionSame || !mSurfaceSame || mUsedTile) {
        mSurface->destroyCoverage(mSurfaceCoverage);
        if (mSurfaceCondition != nullptr) {
          delete mSurfaceCondition;
        }
        return y - 1;
      }
    }
  }
  int mNorth = mSurfaceCoverage->getNorth();
  mSurface->destroyCoverage(mSurfaceCoverage);
  if (mSurfaceCondition != nullptr) {
    delete mSurfaceCondition;
  }
  return mNorth;
}

int ResourceGeometryProcessor::getEast(IGeometricElement* element, std::vector<ITileSurfaceTemplate*>& calculatedSurfaces, int x, int y, ITileSurface::FaceDirection faceDirection) {
  Condition* mSurfaceCondition = getSurfaceTileCondition(element, x, y, faceDirection);
  std::vector<ITileSurface*> mRawSurfaces = element->getTileSurfaces(faceDirection);
  ITileSurface* mSurface = getSurfaceAt(mRawSurfaces, x, y);

  BlockArea* mBlockCoverage = element->getCoverage();
  for (int i = x + 1; i <= mBlockCoverage->getEast(); i++) {
    Condition* mTileCondition = getSurfaceTileCondition(element, i, y, faceDirection);
    ITileSurface* mTileSurface = getSurfaceAt(mRawSurfaces, i, y);
    bool mConditionSame = safeEquals(mSurfaceCondition, mTileCondition);
    if (mTileCondition != nullptr) {
      delete mTileCondition;
    }
    bool mSurfaceSame   = mTileSurface == mSurface;
    bool mUsedTile      = inSurface(calculatedSurfaces, i, y);
    if (!mConditionSame || !mSurfaceSame || mUsedTile) {
      element->destroyCoverage(mBlockCoverage);
      if (mSurfaceCondition != nullptr) {
        delete mSurfaceCondition;
      }
      return i - 1;
    }
  }
  int mEast = mBlockCoverage->getEast();
  element->destroyCoverage(mBlockCoverage);
  if (mSurfaceCondition != nullptr) {
    delete mSurfaceCondition;
  }
  return mEast;
}

std::vector<ITileSurfaceTemplate*> ResourceGeometryProcessor::getTileSurfaces(IGeometricElement* element, ITileSurface::FaceDirection faceDirection) {
  std::vector<ITileSurfaceTemplate*> mCalculatedSurfaces;
  
  BlockArea* mBlockCoverage = element->getCoverage();
  for (int y = mBlockCoverage->getSouth(); y <= mBlockCoverage->getNorth(); y++) {
    for (int x = mBlockCoverage->getWest(); x <= mBlockCoverage->getEast(); x++) {
      Condition* mSurfaceCondition = getSurfaceTileCondition(element, x, y, faceDirection);
      bool mTileOccupied = inSurface(mCalculatedSurfaces, x, y);
      bool mTileNeeded = mSurfaceCondition == nullptr || !mSurfaceCondition->isAbsolute() || mSurfaceCondition->isTrue();

      if (!mTileOccupied && mTileNeeded) {
        int mEast = getEast(element, mCalculatedSurfaces, x, y, faceDirection);
        int mNorth = getNorth(element, mCalculatedSurfaces, x, mEast, y, faceDirection); 
        ITileSurfaceTemplate* mTileSurfaceTemplate = new TileSurfaceTemplate(mNorth, mEast, y, x, mSurfaceCondition);
        mCalculatedSurfaces.push_back(mTileSurfaceTemplate);
      } else if (mSurfaceCondition != nullptr) {
        delete mSurfaceCondition;
      }
    }
  }
  element->destroyCoverage(mBlockCoverage);
  return mCalculatedSurfaces;
}

/*
 * Wall processing
 */

WallColumnPossibility* ResourceGeometryProcessor::getRawWallColumn(IGeometricElement* element, int x, int y, IWallSurface::FaceDirection facing) {
  bool mFacingPole = facing == IWallSurface::NORTH || facing == IWallSurface::SOUTH;
  int mRowLocation = mFacingPole ? y : x;
  int mCellLocation = mFacingPole ? x : y;
  std::vector<IWallSurface*> mWallSurfaces = element->getWallSurfaces(mRowLocation, facing);
  IWallSurface* mWallSurface = findSurfaceAt(mWallSurfaces, x, y);
  if (mWallSurface != nullptr) {
    IWallEdge* mTopEdge = mWallSurface->getTopEdge(mCellLocation);
    IWallEdge* mBottomEdge = mWallSurface->getBottomEdge(mCellLocation);
    Condition* mCondition = element->getCondition();
    WallColumn* mWallColumn = new WallColumn(mBottomEdge->getStartHeight(), mBottomEdge->getEndHeight(), mTopEdge->getStartHeight(), mTopEdge->getEndHeight());
    WallColumnPossibility* mPossibility = new WallColumnPossibility(mWallColumn, mCondition);
    mWallSurface->destroyEdge(mTopEdge);
    mWallSurface->destroyEdge(mBottomEdge);
    delete mWallColumn;
    return mPossibility;
  }
  return nullptr;
}

std::vector<WallColumnPossibility*> ResourceGeometryProcessor::getPhysicalWallColumn(IGeometricElement* element, int x, int y, IWallSurface::FaceDirection facing) {
  WallColumnPossibility* mRawWallColumn = getRawWallColumn(element, x, y, facing);
  std::vector<WallColumnPossibility*> mPhysicalColumns;
  if (mRawWallColumn == nullptr || mRawWallColumn->isSubtraction()) {
    delete mRawWallColumn;
    return mPhysicalColumns;
  }
  mPhysicalColumns.push_back(mRawWallColumn);
  std::vector<IndexedGeometricElement*> mGeometricElements = getGeometricElements(x, y);

  unsigned int mCurrentIndex = 0;
  bool mIndexSet = false;
  for (unsigned int i = 0; i < mGeometricElements.size(); i++) {
    IGeometricElement* mGeometricElement = mGeometricElements[i]->getGeometricElement();
    if (mGeometricElement == element) {
      mCurrentIndex = mGeometricElements[i]->getIndex();
      mIndexSet = true;
      break;
    }
  }
  if (!mIndexSet) {
    std::cout << "Index not found for current geometric element.  Was the element registered?" << std::endl;
    exit(1);
  }
  
  for (unsigned int i = 0; i < mGeometricElements.size(); i++) {
    IGeometricElement* mGeometricElement = mGeometricElements[i]->getGeometricElement();
    unsigned int mComparisonIndex = mGeometricElements[i]->getIndex();
    if (mComparisonIndex <= mCurrentIndex) {
      continue;
    }
/*    std::cout << "Applying surface provide " << i << std::endl;*/
    BlockArea* mBlockArea = mGeometricElement->getCoverage();
    if (mBlockArea->alligned(x, y)) {
      WallColumnPossibility* mWallColumn = getRawWallColumn(mGeometricElement, x, y, facing);
      if (!mWallColumn->empty()) {
        mWallColumn->convertToAddition();
        Condition* mCondition = mGeometricElement->getCondition();
        std::vector<WallColumnPossibility*> mSplitColumns;
        for (unsigned int j = 0; j < mPhysicalColumns.size(); j++) {
          WallColumnPossibility* mNewPossibility = mPhysicalColumns[j]->split(mCondition);
          if (mNewPossibility != nullptr) {
            mSplitColumns.push_back(mNewPossibility);
          }
        }
        for (unsigned int j = 0; j < mSplitColumns.size(); j++) {
          mPhysicalColumns.push_back(mSplitColumns[j]);
        }

        for (unsigned int j = 0; j < mPhysicalColumns.size(); j++) {
          if (mPhysicalColumns[j]->isCompatibleWith(mCondition)) {
            mPhysicalColumns[j]->applyOverlapping(mWallColumn);
          }
        }
      }
      delete mWallColumn;
    }
    mGeometricElement->destroyCoverage(mBlockArea);
  }

  // Shave the top off invisible surfaces
  std::vector<ITileSurface*> mTileSurfaces = element->getTileSurfaces(ITileSurface::UP);
  ITileSurface* mTileSurface = getSurfaceAt(mTileSurfaces, x, y);
  int mHeight = mTileSurface->getSurfaceCellHeight(x, y);
  Condition* mCondition = getSurfaceTileCondition(element, x, y, ITileSurface::UP);
  for (unsigned int i = 0; i < mPhysicalColumns.size(); i++) {
    mPhysicalColumns[i]->shaveTop(mHeight, mCondition);
  }
  if (mCondition != nullptr) {
    delete mCondition;
  }
  return mPhysicalColumns;
}

IWallSurface::FaceDirection ResourceGeometryProcessor::getOppositeOf(IWallSurface::FaceDirection facing) {
  switch (facing) {
    case IWallSurface::NORTH: return IWallSurface::SOUTH;
    case IWallSurface::SOUTH: return IWallSurface::NORTH;
    case IWallSurface::WEST:  return IWallSurface::EAST;
    case IWallSurface::EAST:  return IWallSurface::WEST;
  }
  std::cout << "ERROR: Unknown face direction" << std::endl;
  exit(1);
}

std::vector<WallColumnPossibility*> ResourceGeometryProcessor::getPhysicalWallMasks(int x, int y, IWallSurface::FaceDirection facing) {
  std::vector<IndexedGeometricElement*> mGeometricElements = getGeometricElements(x, y);
  std::vector<WallColumnPossibility*> mOpposingMask;
  mOpposingMask.push_back(new WallColumnPossibility());
  for (unsigned int i = 0; i < mGeometricElements.size(); i++) {
    IGeometricElement* mGeometricElement = mGeometricElements[i]->getGeometricElement();
    BlockArea* mCoverage = mGeometricElement->getCoverage();
    if (mCoverage->alligned(x, y)) {
      std::vector<WallColumnPossibility*> mOpposingPossibilities = getPhysicalWallColumn(mGeometricElement, x, y, facing);
      for (unsigned int k = 0; k < mOpposingPossibilities.size(); k++) {
        Condition* mCondition = mOpposingPossibilities[k]->getCondition();
        std::vector<WallColumnPossibility*> mSplitColumns;
        for (unsigned int j = 0; j < mOpposingMask.size(); j++) {
          WallColumnPossibility* mNewPossibility = mOpposingMask[j]->split(mCondition);
          if (mNewPossibility != nullptr) {
            mSplitColumns.push_back(mNewPossibility);
          }
        }
        for (unsigned int j = 0; j < mSplitColumns.size(); j++) {
          mOpposingMask.push_back(mSplitColumns[j]);
        }
        for (unsigned int j = 0; j < mOpposingMask.size(); j++) {
          if (mOpposingMask[j]->isCompatibleWith(mCondition)) {
            mOpposingMask[j]->unite(mOpposingPossibilities[k]);
          }
        }
        if (mCondition != nullptr) {
          delete mCondition;
        }
        delete mOpposingPossibilities[k];
      }
    }
    mGeometricElement->destroyCoverage(mCoverage);
  }
  for (int i = mOpposingMask.size() - 1; i >= 0; i--) {
    if (mOpposingMask.empty()) {
      delete mOpposingMask[i];
      mOpposingMask.erase(mOpposingMask.begin() + i);
    }
  }
  return mOpposingMask;
}

std::vector<WallColumnPossibility*> ResourceGeometryProcessor::getOptimisedWallColumn(IGeometricElement* element, int x, int y, IWallSurface::FaceDirection facing) {
  std::vector<WallColumnPossibility*> mWallColumns= getPhysicalWallColumn(element, x, y, facing);
  switch (facing) {
    case IWallSurface::NORTH: y++; break;
    case IWallSurface::EAST:  x++; break;
    case IWallSurface::SOUTH: y--; break;
    case IWallSurface::WEST:  x--; break;
  }
  facing = getOppositeOf(facing);
  if (!cCompareOtherContainers) {
    IElementContainer* mElementContainer = element->getElementContainer();
    BlockArea* mCoverage = mElementContainer->getCoverage();
    if (!mCoverage->alligned(x, y)) {
      return mWallColumns;
    }
  }
  std::vector<WallColumnPossibility*> mWallMasks = getPhysicalWallMasks(x, y, facing);
  
  for (unsigned int i = 0; i < mWallMasks.size(); i++) {
    Condition* mMaskCondition = mWallMasks[i]->getCondition();
    std::vector<WallColumnPossibility*> mSplitColumns;
    for (unsigned int j = 0; j < mWallColumns.size(); j++) {
      WallColumnPossibility* mNewPossibility = mWallColumns[j]->split(mMaskCondition);
      if (mNewPossibility != nullptr) {
        mSplitColumns.push_back(mNewPossibility);
      }
    }
    for (unsigned int j = 0; j < mSplitColumns.size(); j++) {
      mWallColumns.push_back(mSplitColumns[j]);
    }
    
    for (int j = mWallColumns.size() - 1; j >= 0; j--) {
      if (mWallColumns[j]->isCompatibleWith(mMaskCondition)) {
        mWallColumns[j]->removeHiddenSections(mWallMasks[i]);
      }
    }
    if (mMaskCondition != nullptr) {
      delete mMaskCondition;
    }
    delete mWallMasks[i];
  }
  return mWallColumns;
}

std::vector<WallColumnPossibility*> ResourceGeometryProcessor::getVisibleWallColumn(IGeometricElement* element, int x, int y, IWallSurface::FaceDirection facing) {
  std::vector<WallColumnPossibility*> mWallColumns = getOptimisedWallColumn(element, x, y, facing);
  switch (facing) {
    case IWallSurface::NORTH: y++; break;
    case IWallSurface::EAST:  x++; break;
    case IWallSurface::SOUTH: y--; break;
    case IWallSurface::WEST:  x--; break;
  }
  facing = getOppositeOf(facing);
  std::vector<WallColumnPossibility*> mWallMasks = getPhysicalWallMasks(x, y, facing);

  for (unsigned int i = 0; i < mWallMasks.size(); i++) {
    WallColumnPossibility* mWallMask = mWallMasks[i];
    if (!mWallMask->empty()) {
      std::vector<WallColumnPossibility*> mSplitColumns;
      Condition* mCondition = mWallMask->getCondition();
      for (unsigned int j = 0; j < mWallColumns.size(); j++) {
        WallColumnPossibility* mNewPossibility = mWallColumns[j]->split(mCondition);
        if (mNewPossibility != nullptr) {
          mSplitColumns.push_back(mNewPossibility);
        }
      }
      for (unsigned int j = 0; j < mSplitColumns.size(); j++) {
        mWallColumns.push_back(mSplitColumns[j]);
      }

      for (unsigned int j = 0; j < mWallColumns.size(); j++) {
        if (mWallColumns[j]->isCompatibleWith(mCondition)) {
          mWallColumns[j]->applyOverlapping(mWallMask);
        }
      }
      if (mCondition != nullptr) {
        delete mCondition;
      }
    }
    delete mWallMasks[i];
  }
  mWallMasks.clear();

  // Shave the top off invisible surfaces
/*  std::vector<ITileSurface*> mTileSurfaces = element->getTileSurfaces(ITileSurface::UP);
  ITileSurface* mTileSurface = getSurfaceAt(mTileSurfaces, x, y);
  int mHeight = mTileSurface->getSurfaceCellHeight(x, y);
  Condition* mCondition = getSurfaceTileCondition(element, x, y, ITileSurface::UP);
  for (unsigned int i = 0; i < mWallColumns.size(); i++) {
    mWallColumns[i]->shaveTop(mHeight, mCondition);
  }*/
  return mWallColumns;
}

int mInstanceCount = 0;

std::vector<IWallSurfaceTemplate*> ResourceGeometryProcessor::getWallSurfaces(IGeometricElement* element, IWallSurface::FaceDirection facing) {
  BlockArea* mCoverage = element->getCoverage();
  bool mXWalls = facing == IWallSurface::WEST || facing == IWallSurface::EAST;
  int mStartCell = mXWalls ? mCoverage->getSouth() : mCoverage->getWest();
  int mEndCell = mXWalls ? mCoverage->getNorth() : mCoverage->getEast();
  int mStartRow = mXWalls ? mCoverage->getWest() : mCoverage->getSouth();
  int mEndRow = mXWalls ? mCoverage->getEast() : mCoverage->getNorth();
  element->destroyCoverage(mCoverage);
  std::vector<IWallSurfaceTemplate*> mWallSurfaces;
  std::vector<WallConstructionData*>* mExtendedConstructionData;
  std::vector<WallConstructionData*>* mCompletedConstructionData = new std::vector<WallConstructionData*>();
  for (int row = mStartRow; row <= mEndRow; row++) {
    for (int cell = mStartCell; cell <= mEndCell; cell++) {
      mExtendedConstructionData = new std::vector<WallConstructionData*>();
      int mX = mXWalls ? row : cell;
      int mY = mXWalls ? cell : row;
      std::vector<WallColumnPossibility*> mWallColumns = cRemoveHiddenSurfaces ? getVisibleWallColumn(element, mX, mY, facing)
                                                                               : getOptimisedWallColumn(element, mX, mY, facing);
      for (unsigned int j = 0; j < mWallColumns.size(); j++) {
        std::vector<WallColumn*> mSections = mWallColumns[j]->getSections();
        Condition* mColumnCondition = mWallColumns[j]->getCondition();
        for (unsigned int i = 0; i < mSections.size(); i++) {

          // Attempt to extend each surface with this column
          bool mTileUsed = false;
          for (int j = mCompletedConstructionData->size() - 1; j >= 0; j--) {
            if ((*mCompletedConstructionData)[j]->unite(mSections[i], mColumnCondition)) {
              mExtendedConstructionData->push_back((*mCompletedConstructionData)[j]);
              mCompletedConstructionData->erase(mCompletedConstructionData->begin() + j);
              mTileUsed = true;
              break;
            }
          }

          if (!mTileUsed) {
            WallConstructionData* mNewConstructionData = new WallConstructionData(mX, mY, mSections[i], mColumnCondition, facing);
            mExtendedConstructionData->push_back(mNewConstructionData);
          }
        }
        if (mColumnCondition != nullptr) {
          delete mColumnCondition;
        }
        delete mWallColumns[j];
      }

      for (unsigned int i = 0; i < mCompletedConstructionData->size(); i++) {
        mWallSurfaces.push_back((*mCompletedConstructionData)[i]);
      }

      // The extended columns are ready in the next iteration
      delete mCompletedConstructionData;
      mCompletedConstructionData = mExtendedConstructionData;
    }

    // End of row reached; we must construct any pending datas now
    for (unsigned int i = 0; i < mExtendedConstructionData->size(); i++) {
      mWallSurfaces.push_back((*mExtendedConstructionData)[i]);
    }
    mExtendedConstructionData->clear();
  }
  delete mExtendedConstructionData;
  return mWallSurfaces;
}

void ResourceGeometryProcessor::destroyTileTemplate(ITileSurfaceTemplate* tileTemplate) {
  delete tileTemplate;
}

void ResourceGeometryProcessor::destroyWallTemplate(IWallSurfaceTemplate* wallTemplate) {
  delete wallTemplate;
}

void ResourceGeometryProcessor::initElementsComplete() {
//  cCache.clear();
}

ResourceGeometryProcessor::IndexedGeometricElement::IndexedGeometricElement(IGeometricElement* element, unsigned int order) {
  cGeometricElement = element;
  cOrder = order;
}

bool ResourceGeometryProcessor::IndexedGeometricElement::operator<(const ResourceGeometryProcessor::IndexedGeometricElement& value) const {
  return cOrder < value.cOrder;
}

IGeometricElement* ResourceGeometryProcessor::IndexedGeometricElement::getGeometricElement() {
  return cGeometricElement;
}

BlockArea* ResourceGeometryProcessor::IndexedGeometricElement::getCoverage() {
  return cGeometricElement->getCoverage();
}

unsigned int ResourceGeometryProcessor::IndexedGeometricElement::getIndex() {
  return cOrder;
}

ResourceGeometryProcessor::FullTileColumn::FullTileColumn(std::vector<TileColumn*> tileColumns, int x, int y) {
  cTileColumns = tileColumns;
  cX = x;
  cY = y;
}

bool ResourceGeometryProcessor::FullTileColumn::isAt(int x, int y) {
  return cX == x && cY == y;
}

BlockArea* ResourceGeometryProcessor::FullTileColumn::getCoverage() {
  BlockLocation* mStart = new BlockLocation(cX, cY, 0);
  BlockLocation* mEnd   = new BlockLocation(cX, cY, 0);
  return new BlockArea(*mStart, *mEnd);
}

std::vector<TileColumn*>* ResourceGeometryProcessor::FullTileColumn::getTileColumns() {
  return &cTileColumns;
}

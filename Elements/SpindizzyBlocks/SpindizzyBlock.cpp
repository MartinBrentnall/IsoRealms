/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "SpindizzyBlock.h"

const unsigned int SpindizzyBlock::INIT_REGISTER_BLOCKS = 0;
const unsigned int SpindizzyBlock::INIT_PROCESS_BLOCKS = 1;
const unsigned int SpindizzyBlock::INIT_REGISTER_SURFACES = 2;
const unsigned int SpindizzyBlock::INIT_USE_SURFACES = 3;

SpindizzyBlock::SpindizzyBlock(ISpindizzyBlockFactory* elementFactory, BlockLocation* startLocation, BlockLocation* endLocation, SpindizzyBlockProperties* blockProperties, bool addition) : ISpindizzyBlock(elementFactory) {
  cStartLocation = BlockLocation(endLocation->x > startLocation->x              ? startLocation->x : endLocation->x,
                                 endLocation->y > startLocation->y              ? startLocation->y : endLocation->y,
                                (endLocation->z > startLocation->z) == addition ? startLocation->z : endLocation->z);
  cEndLocation = BlockLocation(endLocation->x > startLocation->x              ? endLocation->x : startLocation->x,
                               endLocation->y > startLocation->y              ? endLocation->y : startLocation->y,
                              (endLocation->z > startLocation->z) == addition ? endLocation->z : startLocation->z);
  (addition ? cStartLocation.z : cEndLocation.z)--;
  cNorthWestHeight = blockProperties->getNorthWestHeight();
  cNorthEastHeight = blockProperties->getNorthEastHeight();
  cSouthWestHeight = blockProperties->getSouthWestHeight();
  cSouthEastHeight = blockProperties->getSouthEastHeight();
  cSplitType = blockProperties->isSplitNorthWestSouthEast() ? NORTH_SOUTH : EAST_WEST;
  cSteppedBottom = blockProperties->isSteppedBottom();
  cCondition = blockProperties->getCondition();
}

SpindizzyBlock::SpindizzyBlock(ISpindizzyBlockFactory* elementFactory, DOMNodeWrapper* node) : ISpindizzyBlock(elementFactory) {
}

ISpindizzyTexture* SpindizzyBlock::getTileSurfaceTexture() {
  ISpindizzyBlockFactory* mFactory = getElementFactory();
  BlockTypeProperties* mBlockTypeProperties = mFactory->getBlockTypeProperties();
  return isSplit() ? mBlockTypeProperties->getSplitNETexture() : mBlockTypeProperties->getSurfaceTexture();
}

int SpindizzyBlock::getXSlope() {
  if (cNorthEastHeight - cNorthWestHeight == cSouthEastHeight - cSouthWestHeight) {
    return cNorthEastHeight - cNorthWestHeight ;
  }
  return 0;
}

int SpindizzyBlock::getYSlope() {
  if (cSouthWestHeight - cNorthWestHeight == cSouthEastHeight - cNorthEastHeight) {
    return cNorthWestHeight - cSouthWestHeight;
  }
  return 0;
}

std::vector<ITileSurface*> SpindizzyBlock::getTileSurfaces(ITileSurface::FaceDirection faceDirection) {
  std::vector<ITileSurface*> mRawSurfaces;
  int mXSlope = getXSlope();
  int mYSlope = getYSlope();
  if (cSteppedBottom && faceDirection == ITileSurface::DOWN) {
    if (mXSlope != 0 && mYSlope != 0) {
      for (int y = cStartLocation.y; y <= cEndLocation.y; y++) {
        for (int x = cStartLocation.x; x <= cEndLocation.x; x++) {
          ITileSurface* mRawSurface = createSubSurface(faceDirection, y, x, y, x, cCondition);
          mRawSurfaces.push_back(mRawSurface);
        }
      }
    } else if (mYSlope != 0) {
      for (int y = cStartLocation.y; y <= cEndLocation.y; y++) {
        ITileSurface* mRawSurface = createSubSurface(faceDirection, y, cEndLocation.x, y, cStartLocation.x, cCondition);
        mRawSurfaces.push_back(mRawSurface);
      }
    } else if (mXSlope != 0) {
      for (int x = cStartLocation.x; x <= cEndLocation.x; x++) {
        ITileSurface* mRawSurface = createSubSurface(faceDirection, cEndLocation.y, x, cStartLocation.y, x, cCondition);
        mRawSurfaces.push_back(mRawSurface);
      }
    } else {
      ITileSurface* mRawSurface = createSubSurface(faceDirection, cEndLocation.y, cEndLocation.x, cStartLocation.y, cStartLocation.x, cCondition);
      mRawSurfaces.push_back(mRawSurface);
    }
  } else {
    ITileSurface* mRawSurface = createSubSurface(faceDirection, cEndLocation.y, cEndLocation.x, cStartLocation.y, cStartLocation.x, cCondition);
    mRawSurfaces.push_back(mRawSurface);
  }
  return mRawSurfaces;
}

int SpindizzyBlock::getBottomHeight(int x, int y) {
  return cSteppedBottom ? getTileSurfaceHeight(x, y) - (cEndLocation.z - cStartLocation.z)
                        : cStartLocation.z;
}

ISpindizzyTileSurface* SpindizzyBlock::createSubSurface(ITileSurface::FaceDirection faceDirection, int north, int east, int south, int west, Condition* condition) {
  ISpindizzyTexture* mTexture = getTileSurfaceTexture();
  switch (faceDirection) {
    case ITileSurface::UP: {
      int mXSlope = getXSlope();
      int mYSlope = getYSlope();
      int mHeight = getTileSurfaceHeight(mXSlope > 0 ? west : east, mYSlope > 0 ? south : north);
      BlockLocation mSurfaceLocation(cStartLocation.x, cStartLocation.y, cEndLocation.z);
      ISpindizzyBlockFactory* mFactory = getElementFactory();
      BlockTypeProperties* mBlockTypeProperties = mFactory->getBlockTypeProperties();
      if (isSplit()) {
        return new TileSplitSurface(cSplitType == NORTH_SOUTH, mSurfaceLocation, mTexture, cNorthWestHeight, cNorthEastHeight, cSouthEastHeight, cSouthWestHeight, condition, mBlockTypeProperties);
      } else {
        return new TileSurface(mTexture, north, east, south, west, mHeight, mXSlope, mYSlope, faceDirection, condition, mBlockTypeProperties);
      }
    }
    
    case ITileSurface::DOWN: {
      // TODO: Make sure the subsurface does not violate the stepping
      int mHeight = getBottomHeight(east, north);
      return new TileSurface(mTexture, north, east, south, west, mHeight, 0, 0, faceDirection, condition, NULL);
    }
  }
  std::cout << "ERROR: Face direction does not exist" << std::endl;
  exit(1);
}

std::vector<ITileSurfaceTemplate*> SpindizzyBlock::calculateTileSurfaces(const ITileSurface::FaceDirection faceDirection) {
  ISpindizzyBlockSet* mSpindizzyBlockSet = getElementSet();
  return mSpindizzyBlockSet->getTileSurfaces(this, faceDirection);
}

ISpindizzyTexture* SpindizzyBlock::getWallTexture(WallSurface::FaceDirection direction) {
  ISpindizzyBlockFactory* mFactory = getElementFactory();
  BlockTypeProperties* mBlockTypeProperties = mFactory->getBlockTypeProperties();
  switch (direction) {
    case IWallSurface::NORTH: return mBlockTypeProperties->getNorthWallTexture();
    case IWallSurface::EAST:  return mBlockTypeProperties->getEastWallTexture();
    case IWallSurface::SOUTH: return mBlockTypeProperties->getSouthWallTexture();
    case IWallSurface::WEST:  return mBlockTypeProperties->getWestWallTexture();
  }
  // TODO: Throw a wobbly
  std::cout << "Unknown face direction in SpindizzyBlock!" << std::endl;
  exit(1);
}

int SpindizzyBlock::getTileSurfaceHeight(int x, int y) {
  return abs(((getXSlope() >= 0 ? cStartLocation.x : cEndLocation.x) - x) * getXSlope())
       + abs(((getYSlope() >= 0 ? cStartLocation.y : cEndLocation.y) - y) * getYSlope())
       + cEndLocation.z;
}

int SpindizzyBlock::getWallSlope(IWallSurface::FaceDirection facing) {
  switch (facing) {
    case IWallSurface::NORTH: return cNorthEastHeight - cNorthWestHeight;
    case IWallSurface::EAST:  return cNorthEastHeight - cSouthEastHeight;
    case IWallSurface::SOUTH: return cSouthEastHeight - cSouthWestHeight;
    case IWallSurface::WEST:  return cNorthWestHeight - cSouthWestHeight;
  }
  // TODO: Throw wobbly
  std::cout << "Warning: unknown case for face direction" << std::endl;
  exit(1);
}

int SpindizzyBlock::getMinimumWallElevation(IWallSurface::FaceDirection facing) {
  switch (facing) {
    case IWallSurface::NORTH: return min(cNorthEastHeight, cNorthWestHeight);
    case IWallSurface::EAST:  return min(cNorthEastHeight, cSouthEastHeight);
    case IWallSurface::SOUTH: return min(cSouthEastHeight, cSouthWestHeight);
    case IWallSurface::WEST:  return min(cNorthWestHeight, cSouthWestHeight);
  }
  // TODO: Throw wobbly
  std::cout << "Warning: unknown case for face direction" << std::endl;
  exit(1);
}

ISpindizzyWallSurface* SpindizzyBlock::createSubSurface(int x, int y, IWallSurface::FaceDirection facing, int length, int startHeight, int endHeight, int bottomSlope, int topSlope, Condition* condition) {
  ISpindizzyTexture* mTexture = getWallTexture(facing);
  // TODO: Bottom slope.
  return new WallSurface(x, y, startHeight, length, endHeight, topSlope, facing, mTexture, condition);
}

std::vector<IWallSurface*> SpindizzyBlock::getWallSurfaces(int location, IWallSurface::FaceDirection facing) {
  bool mFacesPole = facing == IWallSurface::NORTH || facing == IWallSurface::SOUTH;
  int mSlope = getWallSlope(facing);
  std::vector<IWallSurface*> mWallSurfaces;
  ISpindizzyTexture* mTexture = getWallTexture(facing);
  if (cSteppedBottom && mSlope != 0) {
    int mStart = mFacesPole ? cStartLocation.x : cStartLocation.y;
    int mEnd   = mFacesPole ? cEndLocation.x   : cEndLocation.y;
    for (int i = mStart; i <= mEnd; i++) {
      int mX = mFacesPole ? i : location;
      int mY = mFacesPole ? location : i;
      int mBaseHeight = getBottomHeight(mX, mY);
      int mHeight = cEndLocation.z - cStartLocation.z;
      IWallSurface* mWallSurface = new WallSurface(mX, mY, mBaseHeight, 1, mHeight, mSlope, facing, mTexture, cCondition);
      mWallSurfaces.push_back(mWallSurface);
    }
  } else {
    int mX = mFacesPole ? cStartLocation.x : location;
    int mY = mFacesPole ? location : cStartLocation.y;
    int mLength = (mFacesPole ? cEndLocation.x - cStartLocation.x : cEndLocation.y - cStartLocation.y) + 1;
    int mLowestX = mFacesPole ? (mSlope > 0 ? cStartLocation.x : cEndLocation.x) : mX;
    int mLowestY = mFacesPole ? mY : (mSlope > 0 ? cStartLocation.y : cEndLocation.y);
    int mBaseHeight = getBottomHeight(mX, mY);
    int mHeight = cSteppedBottom ? (cEndLocation.z - cStartLocation.z) + getMinimumWallElevation(facing)
                                 : (getTileSurfaceHeight(mLowestX, mLowestY) + getMinimumWallElevation(facing)) - cStartLocation.z;
    IWallSurface* mWallSurface = new WallSurface(mX, mY, mBaseHeight, mLength, mHeight, mSlope, facing, mTexture, cCondition);
    mWallSurfaces.push_back(mWallSurface);
  }
  return mWallSurfaces;
}

int SpindizzyBlock::getOuterWallFaceLocation(IWallSurface::FaceDirection facing) {
  switch (facing) {
    case IWallSurface::NORTH: return cEndLocation.y;
    case IWallSurface::EAST:  return cEndLocation.x;
    case IWallSurface::SOUTH: return cStartLocation.y;
    case IWallSurface::WEST:  return cStartLocation.x;
  }
  // TODO: Throw wobbly
  std::cout << "Warning: unknown case for face direction" << std::endl;
  exit(1);
}

std::vector<IWallSurfaceTemplate*> SpindizzyBlock::calculateWallSurfaces(const IWallSurface::FaceDirection facing) {
  ISpindizzyBlockSet* mSpindizzyBlockSet = getElementSet();
  return mSpindizzyBlockSet->getWallSurfaces(this, facing);
}

void SpindizzyBlock::renderStatic() {
  for (unsigned int i = 0; i < cStaticTileSurfaces.size(); i++) {
    cStaticTileSurfaces[i]->render();
  }
  
/*  std::vector<ITileSurface*> mBottomTileSurfaces = calculateTileSurfaces(ITileSurface::DOWN);
  for (unsigned int i = 0; i < mBottomTileSurfaces.size(); i++) {
    mBottomTileSurfaces[i]->render();
    delete mBottomTileSurfaces[i];
  }*/
  for (unsigned int i = 0; i < cStaticWallSurfaces.size(); i++) {
    cStaticWallSurfaces[i]->render();
  }
}

void SpindizzyBlock::render() {
  for (unsigned int i = 0; i < cDynamicTileSurfaces.size(); i++) {
    cDynamicTileSurfaces[i]->render();
  }
  for (unsigned int i = 0; i < cDynamicWallSurfaces.size(); i++) {
    cDynamicWallSurfaces[i]->render();
  }
}

void SpindizzyBlock::cacheSurfaces() {
  signalElementDirty();
}

std::set<bool*> SpindizzyBlock::getInputs() {
  if (cCondition != NULL) {
    return cCondition->getInputs();
  }
  std::set<bool*> mNoInputs;
  return mNoInputs;
}

void SpindizzyBlock::removed() {
  ISpindizzyBlockSet* mSpindizzyBlockSet = getElementSet();
  mSpindizzyBlockSet->unregisterSurfaceProvider(this);
}

void SpindizzyBlock::added() {
  ISpindizzyBlockSet* mSpindizzyBlockSet = getElementSet();
  mSpindizzyBlockSet->registerSurfaceProvider(this);
  mSpindizzyBlockSet->setDirty();
}

void SpindizzyBlock::generateWallSurfaces(IWallSurface::FaceDirection faceDirection) {
  std::vector<IWallSurfaceTemplate*> mWallSurfaces = calculateWallSurfaces(faceDirection);
  for (unsigned int i = 0; i < mWallSurfaces.size(); i++) {
    int mX = mWallSurfaces[i]->getX();
    int mY = mWallSurfaces[i]->getY();
    int mLength = mWallSurfaces[i]->getLength();
    IWallSurface::FaceDirection mFaceDirection = mWallSurfaces[i]->getFaceDirection();
    int mStartHeight = mWallSurfaces[i]->getStartHeight();
    int mEndHeight = mWallSurfaces[i]->getEndHeight();
    int mBottomSlope = mWallSurfaces[i]->getBottomSlope();
    int mTopSlope = mWallSurfaces[i]->getTopSlope();
    
    Condition* mCondition = mWallSurfaces[i]->getCondition();
    ISpindizzyWallSurface* mWallSurface = createSubSurface(mX, mY, mFaceDirection, mLength, mStartHeight, mEndHeight, mBottomSlope, mTopSlope, mCondition);
    if (mCondition == NULL || (mCondition->isAbsolute() && mCondition->isTrue())) {
      cStaticWallSurfaces.push_back(mWallSurface);
    } else {
      cDynamicWallSurfaces.push_back(mWallSurface);
    }
    ISpindizzyBlockSet* mBlockElementSet = getElementSet();
    // TODO: This should only happen in runtime
    mBlockElementSet->registerWallSurface(mWallSurface);
  }
}

bool SpindizzyBlock::initElement(unsigned int pass) {
  switch (pass) {
    case INIT_REGISTER_BLOCKS: {
      ISpindizzyBlockSet* mSpindizzyBlockSet = getElementSet();
      mSpindizzyBlockSet->registerSurfaceProvider(this);
      return false;
    }

    case INIT_PROCESS_BLOCKS: {
      std::vector<ITileSurfaceTemplate*> mTopTileSurfaces = calculateTileSurfaces(ITileSurface::UP);
      for (unsigned int i = 0; i < mTopTileSurfaces.size(); i++) {
        int mNorth = mTopTileSurfaces[i]->getNorth();
        int mEast = mTopTileSurfaces[i]->getEast();
        int mSouth = mTopTileSurfaces[i]->getSouth();
        int mWest = mTopTileSurfaces[i]->getWest();
        Condition* mCondition = mTopTileSurfaces[i]->getCondition();
        ISpindizzyTileSurface* mTileSurface = createSubSurface(ITileSurface::UP, mNorth, mEast, mSouth, mWest, mCondition);
        if (mCondition == NULL) {
          cStaticTileSurfaces.push_back(mTileSurface);
        } else {
          cDynamicTileSurfaces.push_back(mTileSurface);
        }
        ISpindizzyBlockSet* mBlockElementSet = getElementSet();
        // TODO: This should only happen in runtime
        mBlockElementSet->registerRollableSurface(mTileSurface);
        
        // TODO: SHOULD NOT DELETE THIS BECAUSE WE DIDN'T CREATE IT; THE SURFACE PROCESSOR DID!
        delete mTopTileSurfaces[i];
      }
      // TODO: Use the calculator to calculate surfaces

      generateWallSurfaces(IWallSurface::SOUTH);
      generateWallSurfaces(IWallSurface::NORTH);
      generateWallSurfaces(IWallSurface::EAST);
      generateWallSurfaces(IWallSurface::WEST);
      return true;
    }
  }
  // TODO: Throw exception of some kind
  return true;
}

std::vector<IVisualElement*> SpindizzyBlock::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
//  if (!cDynamicTileSurfaces.empty()) {
    mVisualElements.push_back(this);
//  }
  return mVisualElements;
}

std::vector<IDynamicElement*> SpindizzyBlock::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
  return mDynamicElements;
}

std::vector<IInteractiveElement*> SpindizzyBlock::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  return mInteractiveElements;
}

bool SpindizzyBlock::isFlat() {
  return cNorthWestHeight == 0 
      && cNorthEastHeight == 0
      && cSouthWestHeight == 0
      && cSouthEastHeight == 0;
}

void SpindizzyBlock::setDirty() {
  signalElementDirty();
}

bool SpindizzyBlock::isGhost() {
  return false;
}

bool SpindizzyBlock::isSplit() {
  return !isFlat() && getXSlope() == 0 && getYSlope() == 0;
}

BlockArea* SpindizzyBlock::getCoverage() {
  return new BlockArea(cStartLocation, cEndLocation);
}

Condition* SpindizzyBlock::getCondition() {
  return cCondition;
}

void SpindizzyBlock::save(DOMNodeWriter* node, BlockLocation& zoneLocation) {
  DOMNodeWriter* mLocationNode = node->addBranch("Location");
  cStartLocation.saveRelative(mLocationNode, zoneLocation);
  // TODO: Only save size if it's bigger than 1.
  DOMNodeWriter* mSizeNode = node->addBranch("Size");
  BlockLocation mEndLocation = cEndLocation;
  mEndLocation.x++;
  mEndLocation.y++;
  mEndLocation.saveRelative(mSizeNode, cStartLocation);
  if (!isFlat()) {
    if (isSplit()) {
      DOMNodeWriter* mSplitNode = node->addBranch("Split");
      mSplitNode->addAttribute("NorthWest", cNorthWestHeight);
      mSplitNode->addAttribute("NorthEast", cNorthEastHeight);
      mSplitNode->addAttribute("SouthEast", cSouthEastHeight);
      mSplitNode->addAttribute("SouthWest", cSouthWestHeight);
      mSplitNode->addAttribute("Direction", cSplitType == NORTH_SOUTH ? "East"  : "West");
    } else {
      DOMNodeWriter* mSlopeNode = node->addBranch("Slope");
      int mXSlope = getXSlope();
      int mYSlope = getYSlope();
      mSlopeNode->addAttribute("NorthSouth", mYSlope);
      mSlopeNode->addAttribute("WestEast", mXSlope);
      if (cSteppedBottom && ((cEndLocation.x != cStartLocation.x && mXSlope != 0) || (cEndLocation.y != cStartLocation.y && mYSlope != 0))) {
        mSlopeNode->addAttribute("Stepped", cSteppedBottom);
      }
    }
  }
  if (cCondition != NULL) {
    DOMNodeWriter* mConditionNode = node->addBranch("Condition");
    cCondition->save(mConditionNode);
  }
}


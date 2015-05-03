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
#include "ElementSpindizzyBlock.h"

const unsigned int ElementSpindizzyBlock::INIT_PROCESS_BLOCKS = 0;

ElementSpindizzyBlock::ElementSpindizzyBlock(ISpindizzyBlockType* elementType, BlockLocation* startLocation, BlockLocation* endLocation, SpindizzyBlockProperties* blockProperties, bool addition, IElementContainer* container) {
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
  cBlockType = elementType;
  cContainer = container;
}

ElementSpindizzyBlock::ElementSpindizzyBlock(ISpindizzyBlockType* elementType, DOMNodeWrapper* node) {
  cBlockType = elementType;
}

ITexture** ElementSpindizzyBlock::getTileSurfaceTexture() {
  BlockTypeProperties* mBlockTypeProperties = cBlockType->getBlockTypeProperties();
  return isSplit() ? mBlockTypeProperties->getSplitNETexture() : mBlockTypeProperties->getSurfaceTexture();
}

TextureRotation ElementSpindizzyBlock::getTileSurfaceRotation() {
  BlockTypeProperties* mBlockTypeProperties = cBlockType->getBlockTypeProperties();
  return mBlockTypeProperties->getSurfaceRotation();
}

int ElementSpindizzyBlock::getXSlope() {
  if (cNorthEastHeight - cNorthWestHeight == cSouthEastHeight - cSouthWestHeight) {
    return cNorthEastHeight - cNorthWestHeight ;
  }
  return 0;
}

int ElementSpindizzyBlock::getYSlope() {
  if (cSouthWestHeight - cNorthWestHeight == cSouthEastHeight - cNorthEastHeight) {
    return cNorthWestHeight - cSouthWestHeight;
  }
  return 0;
}

std::vector<ITileSurface*> ElementSpindizzyBlock::getTileSurfaces(ITileSurface::FaceDirection faceDirection) {
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

int ElementSpindizzyBlock::getBottomHeight(int x, int y) {
  return cSteppedBottom ? getTileSurfaceHeight(x, y) - (cEndLocation.z - cStartLocation.z)
                        : cStartLocation.z;
}

ISpindizzyTileSurface* ElementSpindizzyBlock::createSubSurface(ITileSurface::FaceDirection faceDirection, int north, int east, int south, int west, Condition* condition) {
  ITexture** mTexture = getTileSurfaceTexture();
  TextureRotation mRotation = getTileSurfaceRotation();
  switch (faceDirection) {
    case ITileSurface::UP: {
      int mXSlope = getXSlope();
      int mYSlope = getYSlope();
      int mHeight = getTileSurfaceHeight(mXSlope > 0 ? west : east, mYSlope > 0 ? south : north);
      BlockLocation mSurfaceLocation(cStartLocation.x, cStartLocation.y, cEndLocation.z);
      BlockTypeProperties* mBlockTypeProperties = cBlockType->getBlockTypeProperties();
      if (isSplit()) {
        return new TileSplitSurface(cSplitType == NORTH_SOUTH, mSurfaceLocation, mTexture, cNorthWestHeight, cNorthEastHeight, cSouthEastHeight, cSouthWestHeight, condition, mBlockTypeProperties);
      } else {
        return new TileSurface(mTexture, mRotation, north, east, south, west, mHeight, mXSlope, mYSlope, faceDirection, condition, mBlockTypeProperties, this);
      }
    }
    
    case ITileSurface::DOWN: {
      // TODO: Make sure the subsurface does not violate the stepping
      int mHeight = getBottomHeight(east, north);
      return new TileSurface(mTexture, mRotation, north, east, south, west, mHeight, 0, 0, faceDirection, condition, NULL, this);
    }
  }
  std::cout << "ERROR: Face direction does not exist" << std::endl;
  exit(1);
}

std::vector<ITileSurfaceTemplate*> ElementSpindizzyBlock::calculateTileSurfaces(const ITileSurface::FaceDirection faceDirection, bool visual) {
  ISpindizzyBlockSet* mSpindizzyBlockSet = cBlockType->getSpindizzyBlockInterface();
  return mSpindizzyBlockSet->getTileSurfaces(this, faceDirection, visual);
}

WallType* ElementSpindizzyBlock::getWallType() {
  BlockTypeProperties* mBlockTypeProperties = cBlockType->getBlockTypeProperties();
  return mBlockTypeProperties->getWallType();
}

ITexture** ElementSpindizzyBlock::getWallTexture(WallSurface::FaceDirection direction) {
  BlockTypeProperties* mBlockTypeProperties = cBlockType->getBlockTypeProperties();
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

ITexture** ElementSpindizzyBlock::getWallTextureTop(WallSurface::FaceDirection direction) {
  BlockTypeProperties* mBlockTypeProperties = cBlockType->getBlockTypeProperties();
  switch (direction) {
    case IWallSurface::NORTH: return mBlockTypeProperties->getNorthWallTextureTop();
    case IWallSurface::EAST:  return mBlockTypeProperties->getEastWallTextureTop();
    case IWallSurface::SOUTH: return mBlockTypeProperties->getSouthWallTextureTop();
    case IWallSurface::WEST:  return mBlockTypeProperties->getWestWallTextureTop();
  }
  // TODO: Throw a wobbly
  std::cout << "Unknown face direction in SpindizzyBlock!" << std::endl;
  exit(1);
}

ITexture** ElementSpindizzyBlock::getWallTextureBottom(WallSurface::FaceDirection direction) {
  BlockTypeProperties* mBlockTypeProperties = cBlockType->getBlockTypeProperties();
  switch (direction) {
    case IWallSurface::NORTH: return mBlockTypeProperties->getNorthWallTextureBottom();
    case IWallSurface::EAST:  return mBlockTypeProperties->getEastWallTextureBottom();
    case IWallSurface::SOUTH: return mBlockTypeProperties->getSouthWallTextureBottom();
    case IWallSurface::WEST:  return mBlockTypeProperties->getWestWallTextureBottom();
  }
  // TODO: Throw a wobbly
  std::cout << "Unknown face direction in SpindizzyBlock!" << std::endl;
  exit(1);
}

bool ElementSpindizzyBlock::isWallBottomFlipped(WallSurface::FaceDirection direction) {
  BlockTypeProperties* mBlockTypeProperties = cBlockType->getBlockTypeProperties();
  switch (direction) {
    case IWallSurface::NORTH: return mBlockTypeProperties->isNorthWallBottomFlipped();
    case IWallSurface::EAST:  return mBlockTypeProperties->isEastWallBottomFlipped();
    case IWallSurface::SOUTH: return mBlockTypeProperties->isSouthWallBottomFlipped();
    case IWallSurface::WEST:  return mBlockTypeProperties->isWestWallBottomFlipped();
  }
  // TODO: Throw a wobbly
  std::cout << "Unknown face direction in SpindizzyBlock!" << std::endl;
  exit(1);
}

int ElementSpindizzyBlock::getTileSurfaceHeight(int x, int y) {
  return abs(((getXSlope() >= 0 ? cStartLocation.x : cEndLocation.x) - x) * getXSlope())
       + abs(((getYSlope() >= 0 ? cStartLocation.y : cEndLocation.y) - y) * getYSlope())
       + cEndLocation.z;
}

int ElementSpindizzyBlock::getWallSlope(IWallSurface::FaceDirection facing) {
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

int ElementSpindizzyBlock::getMinimumWallElevation(IWallSurface::FaceDirection facing) {
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

ISpindizzyWallSurface* ElementSpindizzyBlock::createSubSurface(IWallSurfaceTemplate* wallTemplate) {
  int mX                                     = wallTemplate->getX();
  int mY                                     = wallTemplate->getY();
  int mLength                                = wallTemplate->getLength();
  IWallSurface::FaceDirection mFaceDirection = wallTemplate->getFaceDirection();
  int mStartHeight                           = wallTemplate->getStartHeight();
  int mEndHeight                             = wallTemplate->getEndHeight();
//  int mBottomSlope                           = wallTemplate->getBottomSlope();
  int mTopSlope                              = wallTemplate->getTopSlope();
  Condition* mCondition                      = wallTemplate->getCondition();
  WallType* mWallType                        = getWallType();
  ITexture** mTexture                        = getWallTexture(mFaceDirection);
  ITexture** mTextureTop                     = getWallTextureTop(mFaceDirection);
  ITexture** mTextureBottom                  = getWallTextureBottom(mFaceDirection);
  bool mFlipBottom                           = isWallBottomFlipped(mFaceDirection);
  // TODO: Bottom slope.
  // TODO: Delete template condition
  Condition* mCopyCondition = mCondition != nullptr ? new Condition(*mCondition) : nullptr;
  return new WallSurface(mX, mY, mStartHeight, mLength, mEndHeight, mTopSlope, mFaceDirection, mWallType, mTexture, mTextureTop, mTextureBottom, mFlipBottom, mCopyCondition);
}

void ElementSpindizzyBlock::setValue() {
  cContainer->setArguments();
}

void ElementSpindizzyBlock::unsetValue() {
  cContainer->unsetArguments();
}

ISpindizzyWallSurface* ElementSpindizzyBlock::createSampleWallSurface(int location, IWallSurface::FaceDirection facing) {
  bool mFacesPole = facing == IWallSurface::NORTH || facing == IWallSurface::SOUTH;
  int mSlope = getWallSlope(facing);
  std::vector<IWallSurface*> mWallSurfaces;
  WallType* mWallType = getWallType();
  ITexture** mTexture = getWallTexture(facing);
  ITexture** mTextureTop = getWallTextureTop(facing);
  ITexture** mTextureBottom = getWallTextureBottom(facing);
  bool mFlipBottom = isWallBottomFlipped(facing);
  int mX = mFacesPole ? cStartLocation.x : location;
  int mY = mFacesPole ? location : cStartLocation.y;
  int mLength = (mFacesPole ? cEndLocation.x - cStartLocation.x : cEndLocation.y - cStartLocation.y) + 1;
  int mLowestX = mFacesPole ? (mSlope > 0 ? cStartLocation.x : cEndLocation.x) : mX;
  int mLowestY = mFacesPole ? mY : (mSlope > 0 ? cStartLocation.y : cEndLocation.y);
  int mBaseHeight = getBottomHeight(mX, mY);
  int mHeight = cSteppedBottom ? (cEndLocation.z - cStartLocation.z) + getMinimumWallElevation(facing)
                               : (getTileSurfaceHeight(mLowestX, mLowestY) + getMinimumWallElevation(facing)) - cStartLocation.z;
  return new WallSurface(mX, mY, mBaseHeight, mLength, mHeight, mSlope, facing, mWallType, mTexture, mTextureTop, mTextureBottom, mFlipBottom, nullptr);
}

std::vector<IWallSurface*> ElementSpindizzyBlock::getWallSurfaces(int location, IWallSurface::FaceDirection facing) {
  bool mFacesPole = facing == IWallSurface::NORTH || facing == IWallSurface::SOUTH;
  int mSlope = getWallSlope(facing);
  std::vector<IWallSurface*> mWallSurfaces;
  WallType* mWallType = getWallType();
  ITexture** mTexture = getWallTexture(facing);
  ITexture** mTextureTop = getWallTextureTop(facing);
  ITexture** mTextureBottom = getWallTextureBottom(facing);
  bool mFlipBottom = isWallBottomFlipped(facing);
  if (cSteppedBottom && mSlope != 0) {
    int mStart = mFacesPole ? cStartLocation.x : cStartLocation.y;
    int mEnd   = mFacesPole ? cEndLocation.x   : cEndLocation.y;
    for (int i = mStart; i <= mEnd; i++) {
      int mX = mFacesPole ? i : location;
      int mY = mFacesPole ? location : i;
      int mBaseHeight = getBottomHeight(mX, mY);
      int mHeight = cEndLocation.z - cStartLocation.z;
      IWallSurface* mWallSurface = new WallSurface(mX, mY, mBaseHeight, 1, mHeight, mSlope, facing, mWallType, mTexture, mTextureTop, mTextureBottom, mFlipBottom, cCondition);
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
    IWallSurface* mWallSurface = new WallSurface(mX, mY, mBaseHeight, mLength, mHeight, mSlope, facing, mWallType, mTexture, mTextureTop, mTextureBottom, mFlipBottom, cCondition);
    mWallSurfaces.push_back(mWallSurface);
  }
  return mWallSurfaces;
}

int ElementSpindizzyBlock::getOuterWallFaceLocation(IWallSurface::FaceDirection facing) {
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

std::vector<IWallSurfaceTemplate*> ElementSpindizzyBlock::calculateWallSurfaces(const IWallSurface::FaceDirection facing, bool visual) {
  ISpindizzyBlockSet* mSpindizzyBlockSet = cBlockType->getSpindizzyBlockInterface();
  return mSpindizzyBlockSet->getWallSurfaces(this, facing, visual);
}

void ElementSpindizzyBlock::createSampleSurfaces() {
  ISpindizzyTileSurface* mTopSurface = createSubSurface(ITileSurface::UP, cEndLocation.y, cEndLocation.x, cEndLocation.y, cEndLocation.x, NULL);
  cDynamicTileSurfaces.push_back(mTopSurface);
  cDynamicWallSurfaces.push_back(createSampleWallSurface(cStartLocation.x, IWallSurface::WEST));
  cDynamicWallSurfaces.push_back(createSampleWallSurface(cStartLocation.y, IWallSurface::SOUTH));
  cDynamicWallSurfaces.push_back(createSampleWallSurface(cEndLocation.x,   IWallSurface::EAST));
  cDynamicWallSurfaces.push_back(createSampleWallSurface(cEndLocation.y,   IWallSurface::NORTH));
}

void ElementSpindizzyBlock::renderStatic() {
  ISpindizzyBlockSet* mSpindizzyBlockSet = cBlockType->getSpindizzyBlockInterface();
  bool mEditing = mSpindizzyBlockSet->isEditing();
  
  for (unsigned int i = 0; i < cStaticTileSurfaces.size(); i++) {
    cStaticTileSurfaces[i]->render();
    if (!mEditing) {
      delete cStaticTileSurfaces[i];
    }
  }

/* std::vector<ITileSurface*> mBottomTileSurfaces = calculateTileSurfaces(ITileSurface::DOWN);
    for (unsigned int i = 0; i < mBottomTileSurfaces.size(); i++) {
    mBottomTileSurfaces[i]->render();
    delete mBottomTileSurfaces[i];
  }*/
  for (unsigned int i = 0; i < cStaticWallSurfaces.size(); i++) {
    cStaticWallSurfaces[i]->render();
    if (!mEditing) {
      delete cStaticWallSurfaces[i];
    }
  }
  
  if (mEditing) {
    cStaticTileSurfaces.clear();
    cStaticWallSurfaces.clear();
  }
}

void ElementSpindizzyBlock::renderRuntime() {
  for (unsigned int i = 0; i < cDynamicTileSurfaces.size(); i++) {
    cDynamicTileSurfaces[i]->render();
  }
  for (unsigned int i = 0; i < cDynamicWallSurfaces.size(); i++) {
    cDynamicWallSurfaces[i]->render();
  }
}

IElementBounds* ElementSpindizzyBlock::getBounds() {
  return this;
}

float ElementSpindizzyBlock::getWest() {
  return cStartLocation.x;
}

float ElementSpindizzyBlock::getEast() {
  return cEndLocation.x;
}

float ElementSpindizzyBlock::getSouth() {
  return cStartLocation.y;
}

float ElementSpindizzyBlock::getNorth() {
  return cEndLocation.y;
}

float ElementSpindizzyBlock::getBottom() {
  return cStartLocation.z;
}

float ElementSpindizzyBlock::getTop() {
  return cEndLocation.z;
}

std::set<IBoolean*> ElementSpindizzyBlock::getInputs() {
  if (cCondition != NULL) {
    return cCondition->getInputs();
  }
  std::set<IBoolean*> mNoInputs;
  return mNoInputs;
}

// TODO  ISpindizzyBlockSet* mSpindizzyBlockSet = getSpindizzyBlockInterface();
//   mSpindizzyBlockSet->unregisterSurfaceProvider(this);
//   mSpindizzyBlockSet->setDirty();

void ElementSpindizzyBlock::generateWallSurfaces(IWallSurface::FaceDirection faceDirection) {
  ISpindizzyBlockSet* mSpindizzyBlockSet = cBlockType->getSpindizzyBlockInterface();
  bool mEditing = mSpindizzyBlockSet->isEditing();
  
  // Physical surfaces
  if (!mEditing) {
    std::vector<IWallSurfaceTemplate*> mWallSurfaces = calculateWallSurfaces(faceDirection, false);
    for (unsigned int i = 0; i < mWallSurfaces.size(); i++) {
      ISpindizzyWallSurface* mWallSurface = createSubSurface(mWallSurfaces[i]);
      mSpindizzyBlockSet->registerWallSurface(mWallSurface);
      mSpindizzyBlockSet->destroyWallTemplate(mWallSurfaces[i], false);
    }
  }

  // Visual surfaces
  std::vector<IWallSurfaceTemplate*> mWallSurfaces = calculateWallSurfaces(faceDirection, true);
  for (unsigned int i = 0; i < mWallSurfaces.size(); i++) {
    Condition* mCondition = mWallSurfaces[i]->getCondition();
    ISpindizzyWallSurface* mWallSurface = createSubSurface(mWallSurfaces[i]);
    if (mCondition != NULL) {
      cDynamicWallSurfaces.push_back(mWallSurface);
    } else {
      cStaticWallSurfaces.push_back(mWallSurface);
    }
    mSpindizzyBlockSet->destroyWallTemplate(mWallSurfaces[i], true);
  }
}

IElementType* ElementSpindizzyBlock::getElementType() {
  return cBlockType;
}

bool ElementSpindizzyBlock::initElement(unsigned int pass) {
  ISpindizzyBlockSet* mSpindizzyBlockSet = cBlockType->getSpindizzyBlockInterface();
  bool mEditing = mSpindizzyBlockSet->isEditing();

  switch (pass) {
    case INIT_PROCESS_BLOCKS: {
      
      // Physical surfaces
      if (!mEditing) {
        std::vector<ITileSurfaceTemplate*> mTopTileSurfaces = calculateTileSurfaces(ITileSurface::UP, false);
        for (unsigned int i = 0; i < mTopTileSurfaces.size(); i++) {
          int mNorth = mTopTileSurfaces[i]->getNorth();
          int mEast = mTopTileSurfaces[i]->getEast();
          int mSouth = mTopTileSurfaces[i]->getSouth();
          int mWest = mTopTileSurfaces[i]->getWest();
          Condition* mCondition = mTopTileSurfaces[i]->getCondition();
          ISpindizzyTileSurface* mTileSurface = createSubSurface(ITileSurface::UP, mNorth, mEast, mSouth, mWest, mCondition);
          mSpindizzyBlockSet->registerRollableSurface(mTileSurface);
          mSpindizzyBlockSet->destroyTileTemplate(mTopTileSurfaces[i], false);
        }
      }

      // Visual surfaces
      std::vector<ITileSurfaceTemplate*> mTopTileSurfaces = calculateTileSurfaces(ITileSurface::UP, true);
      for (unsigned int i = 0; i < mTopTileSurfaces.size(); i++) {
        Condition* mCondition = mTopTileSurfaces[i]->getCondition();
        int mNorth = mTopTileSurfaces[i]->getNorth();
        int mEast = mTopTileSurfaces[i]->getEast();
        int mSouth = mTopTileSurfaces[i]->getSouth();
        int mWest = mTopTileSurfaces[i]->getWest();
        ISpindizzyTileSurface* mTileSurface = createSubSurface(ITileSurface::UP, mNorth, mEast, mSouth, mWest, mCondition);
        if (mCondition != NULL) {
          cDynamicTileSurfaces.push_back(mTileSurface);
        } else {
          cStaticTileSurfaces.push_back(mTileSurface);
        }
        mSpindizzyBlockSet->destroyTileTemplate(mTopTileSurfaces[i], true);
      }

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

bool ElementSpindizzyBlock::isFlat() {
  return cNorthWestHeight == 0 
      && cNorthEastHeight == 0
      && cSouthWestHeight == 0
      && cSouthEastHeight == 0;
}

void ElementSpindizzyBlock::setDirty() {
//   for (unsigned int i = 0; i < cStaticTileSurfaces.size(); i++) {
//     delete cStaticTileSurfaces[i];
//   }
//   cStaticTileSurfaces.clear();
//   for (unsigned int i = 0; i < cStaticTileSurfaces.size(); i++) {
//     delete cStaticWallSurfaces[i];
//   }
//   cStaticWallSurfaces.clear();
}

IElementContainer* ElementSpindizzyBlock::getElementContainer() {
  return cContainer;
}

bool ElementSpindizzyBlock::isGhost() {
  return false;
}

bool ElementSpindizzyBlock::isSplit() {
  return !isFlat() && getXSlope() == 0 && getYSlope() == 0;
}

BlockArea* ElementSpindizzyBlock::getCoverage() {
  return new BlockArea(cStartLocation, cEndLocation);
}

void ElementSpindizzyBlock::destroyCoverage(BlockArea* coverage) {
  delete coverage;
}

Condition* ElementSpindizzyBlock::getCondition() {
  return cCondition;
}

void ElementSpindizzyBlock::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& zoneLocation) {
  cStartLocation.saveRelative(node, zoneLocation);
  // TODO: Only save size if it's bigger than 1.
  BlockLocation mEndLocation = cEndLocation;
  mEndLocation.x++;
  mEndLocation.y++;
  mEndLocation.saveRelative(node, cStartLocation, "width", "length", "height");
  if (!isFlat()) {
    if (isSplit()) {
      node->addAttribute("northWestCorner", cNorthWestHeight);
      node->addAttribute("northEastCorner", cNorthEastHeight);
      node->addAttribute("southEastCorner", cSouthEastHeight);
      node->addAttribute("southWestCorner", cSouthWestHeight);
      node->addAttribute("split", cSplitType == NORTH_SOUTH ? "East"  : "West");
    } else {
      int mXSlope = getXSlope();
      int mYSlope = getYSlope();
      node->addAttribute("ySlope", mYSlope);
      node->addAttribute("xSlope", mXSlope);
      if (cSteppedBottom && ((cEndLocation.x != cStartLocation.x && mXSlope != 0) || (cEndLocation.y != cStartLocation.y && mYSlope != 0))) {
        node->addAttribute("steppedBottom", cSteppedBottom);
      }
    }
  }
  if (cCondition != NULL) {
    DOMNodeWriter* mConditionNode = node->addBranch("Condition");
    cCondition->save(mConditionNode);
  }
}

ElementSpindizzyBlock::~ElementSpindizzyBlock() {
  for (unsigned int i = 0; i < cStaticTileSurfaces.size(); i++) {
    delete cStaticTileSurfaces[i];
  }
  for (unsigned int i = 0; i < cStaticWallSurfaces.size(); i++) {
    delete cStaticWallSurfaces[i];
  }
  for (unsigned int i = 0; i < cDynamicTileSurfaces.size(); i++) {
    delete cDynamicTileSurfaces[i];
  }
  for (unsigned int i = 0; i < cDynamicWallSurfaces.size(); i++) {
    delete cDynamicWallSurfaces[i];
  }
//  delete cCondition;
}

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
#include "AbstractSpindizzyBlock.h"

AbstractSpindizzyBlock::AbstractSpindizzyBlock(IElementFactory* elementFactory, BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** spindizzyTextureSet, SpindizzyBlockProperties* blockProperties, bool addition) : IElement(elementFactory) {
  cSpindizzyTextureSet = spindizzyTextureSet;
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
  cInitStage = CACHE_SURFACES;
}

AbstractSpindizzyBlock::AbstractSpindizzyBlock(IElementFactory* elementFactory, ISpindizzyTextureSet** spindizzyTextureSet, DOMNodeWrapper* node) : IElement(elementFactory) {
  cSpindizzyTextureSet = spindizzyTextureSet;
}

ISpindizzyTextureSet* AbstractSpindizzyBlock::getTextureSet() {
  return *cSpindizzyTextureSet;
}

int AbstractSpindizzyBlock::getXSlope() {
  if (cNorthEastHeight - cNorthWestHeight == cSouthEastHeight - cSouthWestHeight) {
    return cNorthEastHeight - cNorthWestHeight ;
  }
  return 0;
}

int AbstractSpindizzyBlock::getYSlope() {
  if (cSouthWestHeight - cNorthWestHeight == cSouthEastHeight - cNorthEastHeight) {
    return cNorthWestHeight - cSouthWestHeight;
  }
  return 0;
}

std::vector<IRollableSurface*> AbstractSpindizzyBlock::getRollableSurfaces(IRollableSurface::FaceDirection faceDirection) {
  std::vector<IRollableSurface*> mRawSurfaces;
  int mXSlope = getXSlope();
  int mYSlope = getYSlope();
  if (cSteppedBottom && faceDirection == IRollableSurface::DOWN) {
    if (mXSlope != 0 && mYSlope != 0) {
      for (int y = cStartLocation.y; y <= cEndLocation.y; y++) {
        for (int x = cStartLocation.x; x <= cEndLocation.x; x++) {
          IRollableSurface* mRawSurface = createSubSurface(faceDirection, y, x, y, x);
          mRawSurfaces.push_back(mRawSurface);
        }
      }
    } else if (mYSlope != 0) {
      for (int y = cStartLocation.y; y <= cEndLocation.y; y++) {
        IRollableSurface* mRawSurface = createSubSurface(faceDirection, y, cEndLocation.x, y, cStartLocation.x);
        mRawSurfaces.push_back(mRawSurface);
      }
    } else if (mXSlope != 0) {
      for (int x = cStartLocation.x; x <= cEndLocation.x; x++) {
        IRollableSurface* mRawSurface = createSubSurface(faceDirection, cEndLocation.y, x, cStartLocation.y, x);
        mRawSurfaces.push_back(mRawSurface);
      }
    } else {
      IRollableSurface* mRawSurface = createSubSurface(faceDirection, cEndLocation.y, cEndLocation.x, cStartLocation.y, cStartLocation.x);
      mRawSurfaces.push_back(mRawSurface);
    }
  } else {
    IRollableSurface* mRawSurface = createSubSurface(faceDirection, cEndLocation.y, cEndLocation.x, cStartLocation.y, cStartLocation.x);
    mRawSurfaces.push_back(mRawSurface);
  }
  return mRawSurfaces;
}

/* TODO: Enable this; it's never been used or tested!  WILL NEED MODIFICATION
Condition AbstractSpindizzyBlock::createCondition(Condition* mSurfaceCondition) {
  Condition* mSurfaceCondition = (mThisCondition != NULL ? mThisCondition : (cCondition != NULL ? new GroupAndCondition() : NULL));
  if (cCondition != NULL) {
    mSurfaceCondition->addCondition(cCondition);
  }
}
*/

int AbstractSpindizzyBlock::getBottomHeight(int x, int y) {
  return cSteppedBottom ? getRollableSurfaceHeight(x, y) - (cEndLocation.z - cStartLocation.z)
                        : cStartLocation.z;
}

IRollableSurface* AbstractSpindizzyBlock::createSubSurface(IRollableSurface::FaceDirection faceDirection, int north, int east, int south, int west) {
  ISpindizzyTextureSet::TextureType mTextureType = getRollableSurfaceTexture();
  switch (faceDirection) {
    case IRollableSurface::UP: {
      int mXSlope = getXSlope();
      int mYSlope = getYSlope();
      int mHeight = getRollableSurfaceHeight(mXSlope > 0 ? west : east, mYSlope > 0 ? south : north);
      // TODO: Get condition for surface.
      BlockLocation mSurfaceLocation(cStartLocation.x, cStartLocation.y, cEndLocation.z);
      return isSplit() ? (IRollableSurface*) new RollableSplitSurface(cSplitType == NORTH_SOUTH, mSurfaceLocation, cSpindizzyTextureSet, mTextureType, cNorthWestHeight, cNorthEastHeight, cSouthEastHeight, cSouthWestHeight/* TODO:CONDITIONAL, mSurfaceCondition*/)
                       : (IRollableSurface*) new RollableSurface(cSpindizzyTextureSet, mTextureType, north, east, south, west, mHeight, mXSlope, mYSlope, faceDirection/*, TODO:CONDITIONAL  mSurfaceCondition*/);
    }
    
    case IRollableSurface::DOWN: {
      // TODO: Make sure the subsurface does not violate the stepping
      int mHeight = getBottomHeight(east, north);
      // TODO: Get condition for surface.
      return new RollableSurface(cSpindizzyTextureSet, mTextureType, north, east, south, west, mHeight, 0, 0, faceDirection/*, TODO:CONDITIONAL  mSurfaceCondition*/);
    }
  }
  std::cout << "ERROR: Face direction does not exist" << std::endl;
  exit(1);
}

std::vector<IRollableSurface*> AbstractSpindizzyBlock::calculateRollableSurfaces(const IRollableSurface::FaceDirection faceDirection) {
  ISurfaceCalculator* mSurfaceCalculator = dynamic_cast<ISurfaceCalculator*>(getElementSet());
  if (mSurfaceCalculator == NULL) {
    std::cout << "Warning: dynamic_cast failed for surface calculation!  Surfaces may appear incorrect!" << std::endl;
    std::vector<IRollableSurface*> mRawSurface;
    return getRollableSurfaces(faceDirection);
  }
  return mSurfaceCalculator->getRollableSurfaces(this, faceDirection);
}

ISpindizzyTextureSet::TextureType AbstractSpindizzyBlock::getWallTexture(WallSurface::FaceDirection direction) {
  switch (direction) {
    case IWallSurface::NORTH: return getNorthWallTexture();
    case IWallSurface::EAST:  return getEastWallTexture();
    case IWallSurface::SOUTH: return getSouthWallTexture();
    case IWallSurface::WEST:  return getWestWallTexture();
  }
  // TODO: Throw a wobbly
  std::cout << "Unknown face direction in AbstractSpindizzyBlock!" << std::endl;
  exit(1);
}

int AbstractSpindizzyBlock::getRollableSurfaceHeight(int x, int y) {
  return abs(((getXSlope() >= 0 ? cStartLocation.x : cEndLocation.x) - x) * getXSlope())
       + abs(((getYSlope() >= 0 ? cStartLocation.y : cEndLocation.y) - y) * getYSlope())
       + cEndLocation.z;
}

int AbstractSpindizzyBlock::getWallSlope(IWallSurface::FaceDirection facing) {
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

int AbstractSpindizzyBlock::getMinimumWallElevation(IWallSurface::FaceDirection facing) {
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

IWallSurface* AbstractSpindizzyBlock::createSubSurface(int x, int y, IWallSurface::FaceDirection facing, int length, int startHeight, int endHeight, int bottomSlope, int topSlope) {
  ISpindizzyTextureSet::TextureType mTexture = getWallTexture(facing);
  // TODO: Bottom slope.
  return new WallSurface(x, y, startHeight, length, endHeight, topSlope, facing, cSpindizzyTextureSet, mTexture);
}

std::vector<IWallSurface*> AbstractSpindizzyBlock::getWallSurfaces(int location, IWallSurface::FaceDirection facing) {
  bool mFacesPole = facing == IWallSurface::NORTH || facing == IWallSurface::SOUTH;
  int mSlope = getWallSlope(facing);
  std::vector<IWallSurface*> mWallSurfaces;
  ISpindizzyTextureSet::TextureType mTexture = getWallTexture(facing);
  if (cSteppedBottom && mSlope != 0) {
    int mStart = mFacesPole ? cStartLocation.x : cStartLocation.y;
    int mEnd   = mFacesPole ? cEndLocation.x   : cEndLocation.y;
    for (int i = mStart; i <= mEnd; i++) {
      int mX = mFacesPole ? i : location;
      int mY = mFacesPole ? location : i;
      int mBaseHeight = getBottomHeight(mX, mY);
      int mHeight = cEndLocation.z - cStartLocation.z;
      IWallSurface* mWallSurface = new WallSurface(mX, mY, mBaseHeight, 1, mHeight, mSlope, facing, cSpindizzyTextureSet, mTexture);
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
                                 : (getRollableSurfaceHeight(mLowestX, mLowestY) + getMinimumWallElevation(facing)) - cStartLocation.z;
    IWallSurface* mWallSurface = new WallSurface(mX, mY, mBaseHeight, mLength, mHeight, mSlope, facing, cSpindizzyTextureSet, mTexture);
    mWallSurfaces.push_back(mWallSurface);
  }
  return mWallSurfaces;
}

int AbstractSpindizzyBlock::getOuterWallFaceLocation(IWallSurface::FaceDirection facing) {
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

std::vector<IWallSurface*> AbstractSpindizzyBlock::calculateWallSurfaces(const IWallSurface::FaceDirection facing) {
  ISurfaceCalculator* mSurfaceCalculator = dynamic_cast<ISurfaceCalculator*>(getElementSet());
  if (mSurfaceCalculator == NULL) {
    std::cout << "Warning: dynamic_cast failed for surface calculation!  Surfaces may appear incorrect!" << std::endl;
    return getWallSurfaces(getOuterWallFaceLocation(facing), facing);
  }
  return mSurfaceCalculator->getWallSurfaces(this, facing);
}

void AbstractSpindizzyBlock::renderStatic() {
  std::vector<IRollableSurface*> mTopRollableSurfaces = calculateRollableSurfaces(IRollableSurface::UP);
  for (unsigned int i = 0; i < mTopRollableSurfaces.size(); i++) {
    mTopRollableSurfaces[i]->render();
    delete mTopRollableSurfaces[i];
  }
  std::vector<IRollableSurface*> mBottomRollableSurfaces = calculateRollableSurfaces(IRollableSurface::DOWN);
  for (unsigned int i = 0; i < mBottomRollableSurfaces.size(); i++) {
    mBottomRollableSurfaces[i]->render();
    delete mBottomRollableSurfaces[i];
  }
  if (cStartLocation.z <= cEndLocation.z) {
    std::vector<IWallSurface*> mNorthWallSurfaces = calculateWallSurfaces(IWallSurface::NORTH);
    for (unsigned int i = 0; i < mNorthWallSurfaces.size(); i++) {
      mNorthWallSurfaces[i]->render();
      delete mNorthWallSurfaces[i];
    }
    std::vector<IWallSurface*> mEastWallSurfaces = calculateWallSurfaces(IWallSurface::EAST);
    for (unsigned int i = 0; i < mEastWallSurfaces.size(); i++) {
      mEastWallSurfaces[i]->render();
      delete mEastWallSurfaces[i];
    }
    std::vector<IWallSurface*> mSouthWallSurfaces = calculateWallSurfaces(IWallSurface::SOUTH);
    for (unsigned int i = 0; i < mSouthWallSurfaces.size(); i++) {
      mSouthWallSurfaces[i]->render();
      delete mSouthWallSurfaces[i];
    }
    std::vector<IWallSurface*> mWestWallSurfaces = calculateWallSurfaces(IWallSurface::WEST);
    for (unsigned int i = 0; i < mWestWallSurfaces.size(); i++) {
      mWestWallSurfaces[i]->render();
      delete mWestWallSurfaces[i];
    }
  }
}

void AbstractSpindizzyBlock::removed() {
  ISurfaceCalculator* mSurfaceCalculator = dynamic_cast<ISurfaceCalculator*>(getElementSet());
  if (mSurfaceCalculator == NULL) {
    std::cout << "Warning: dynamic_cast failed for surface calculation!  Surfaces may appear incorrect!" << std::endl;
  } else {
    mSurfaceCalculator->unregisterRollableSurfaceProvider(this);
  }
}

void AbstractSpindizzyBlock::added() {
  ISurfaceCalculator* mSurfaceCalculator = dynamic_cast<ISurfaceCalculator*>(getElementSet());
  if (mSurfaceCalculator == NULL) {
    std::cout << "Warning: dynamic_cast failed for surface calculation!  Surfaces may appear incorrect!" << std::endl;
  } else {
    mSurfaceCalculator->registerRollableSurfaceProvider(this);
    mSurfaceCalculator->setDirty();
  }
}

bool AbstractSpindizzyBlock::initElement() {
  switch (cInitStage) {
    case CACHE_SURFACES: {
      ISurfaceCalculator* mSurfaceCalculator = dynamic_cast<ISurfaceCalculator*>(getElementSet());
      if (mSurfaceCalculator == NULL) {
        std::cout << "Warning: dynamic_cast failed for surface calculation!  Surfaces may appear incorrect!" << std::endl;
      } else {
        mSurfaceCalculator->registerRollableSurfaceProvider(this);
      }
      cInitStage = CALCULATE_SURFACES;
      return false;
    }

    case CALCULATE_SURFACES: {
      // TODO: Use the calculator to calculate surfaces
      return true;
    }
  }
  // TODO: Throw exception of some kind
  return true;
}

std::vector<IVisualElement*> AbstractSpindizzyBlock::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  return mVisualElements;
}

std::vector<IDynamicElement*> AbstractSpindizzyBlock::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
  return mDynamicElements;
}

std::vector<IInteractiveElement*> AbstractSpindizzyBlock::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  return mInteractiveElements;
}

bool AbstractSpindizzyBlock::isFlat() {
  return cNorthWestHeight == 0 
      && cNorthEastHeight == 0
      && cSouthWestHeight == 0
      && cSouthEastHeight == 0;
}

void AbstractSpindizzyBlock::setDirty() {
  signalElementDirty();
}

bool AbstractSpindizzyBlock::isSplit() {
  return !isFlat() && getXSlope() == 0 && getYSlope() == 0;
}

BlockArea* AbstractSpindizzyBlock::getCoverage() {
  return new BlockArea(cStartLocation, cEndLocation);
}

void AbstractSpindizzyBlock::save(DOMNodeWriter* node, BlockLocation& zoneLocation) {
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
}


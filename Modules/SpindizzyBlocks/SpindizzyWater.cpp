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
#include "SpindizzyWater.h"

const unsigned int SpindizzyWater::INIT_PROCESS_BLOCKS = 0;

SpindizzyWater::SpindizzyWater(ISpindizzyBlockType* elementType, BlockLocation* startLocation, BlockLocation* endLocation, ITexture** texture) : ISpindizzyBlock(elementType) {
  cTexture = texture;
  cStartLocation = BlockLocation(endLocation->x > startLocation->x ? startLocation->x : endLocation->x,
                                 endLocation->y > startLocation->y ? startLocation->y : endLocation->y,
                                 endLocation->z <= startLocation->z ? startLocation->z : endLocation->z);
  cEndLocation = BlockLocation(endLocation->x > startLocation->x ? endLocation->x : startLocation->x,
                               endLocation->y > startLocation->y ? endLocation->y : startLocation->y,
                              (endLocation->z <= startLocation->z ? endLocation->z : startLocation->z) - 1);
  cCondition = NULL;
}

bool SpindizzyWater::isGhost() {
  return true;
}

void SpindizzyWater::setDirty() {
//   for (unsigned int i = 0; i < cStaticTileSurfaces.size(); i++) {
//     delete cStaticTileSurfaces[i];
//   }
//   cStaticTileSurfaces.clear();
}

std::vector<ITileSurfaceTemplate*> SpindizzyWater::getWaterSurfaces() {
  ISpindizzyBlockSet* mSurfaceProcessor = getElementSet();
  return mSurfaceProcessor->getTileSurfaces(this, ITileSurface::UP, true);
}

std::vector<ITileSurface*> SpindizzyWater::getTileSurfaces(ITileSurface::FaceDirection facing) {
  std::vector<ITileSurface*> mSurfaces;
  ITileSurface* mWaterSurface = createSubSurface(facing, cEndLocation.y, cEndLocation.x, cStartLocation.y, cStartLocation.x, cCondition);
  mSurfaces.push_back(mWaterSurface);
  return mSurfaces;
}

void SpindizzyWater::renderStatic() {
  ISpindizzyBlockSet* mSpindizzyBlockSet = getElementSet();
  bool mEditing = mSpindizzyBlockSet->isEditing();
  
  for (unsigned int i = 0; i < cStaticTileSurfaces.size(); i++) {
    cStaticTileSurfaces[i]->render();
    if (!mEditing) {
      delete cStaticTileSurfaces[i];
    }
  }
}

void SpindizzyWater::renderRuntime() {
  for (unsigned int i = 0; i < cDynamicTileSurfaces.size(); i++) {
    cDynamicTileSurfaces[i]->render();
  }
}

ISpindizzyTileSurface* SpindizzyWater::createSubSurface(ITileSurface::FaceDirection facing, int north, int east, int south, int west, Condition* condition) {
  return new TileSurface(cTexture, STRAIGHT, north, east, south, west, facing == ITileSurface::UP ? cStartLocation.z : cEndLocation.z, 0, 0, facing, condition, NULL);
}

IWallSurface* SpindizzyWater::createSubSurface(int x, int y, IWallSurface::FaceDirection facing, int length, int startHeight, int endHeight, int topSlope, int bottomSlope) {
  return new WallSurface(x, y, startHeight, length, endHeight, 0, facing);
}

std::vector<IWallSurface*> SpindizzyWater::getWallSurfaces(int location, IWallSurface::FaceDirection facing) {
  std::vector<IWallSurface*> mWallSurfaces;
  bool mFacesPole = facing == IWallSurface::NORTH || facing == IWallSurface::SOUTH;
  int mX = mFacesPole ? cStartLocation.x : location;
  int mY = mFacesPole ? location : cStartLocation.y;
  int mLength = (mFacesPole ? cEndLocation.x - cStartLocation.x : cEndLocation.y - cStartLocation.y) + 1;
  int mBaseHeight = cStartLocation.z;
  int mHeight = cEndLocation.z - cStartLocation.z;
  IWallSurface* mWallSurface = new WallSurface(mX, mY, mBaseHeight, mLength, mHeight, 0, facing);
  mWallSurfaces.push_back(mWallSurface);
  return mWallSurfaces;
}

BlockArea* SpindizzyWater::getCoverage() {
  return new BlockArea(cStartLocation, cEndLocation);
}

void SpindizzyWater::destroyCoverage(BlockArea* coverage) {
  delete coverage;
}

Condition* SpindizzyWater::getCondition() {
  return NULL; // TODO: Allow dynamic surfaces
}

void SpindizzyWater::createSampleSurfaces() {
  ISpindizzyTileSurface* mTopSurface = createSubSurface(ITileSurface::UP, cEndLocation.y, cEndLocation.x, cEndLocation.y, cEndLocation.x, NULL);
  cStaticTileSurfaces.push_back(mTopSurface);
}

std::set<bool*> SpindizzyWater::getInputs() {
  if (cCondition != NULL) {
    return cCondition->getInputs();
  }
  std::set<bool*> mNoInputs;
  return mNoInputs;
}

/* TODO  ISpindizzyBlockSet* mSurfaceProcessor = getElementSet();
  mSurfaceProcessor->unregisterSurfaceProvider(this);*/

bool SpindizzyWater::initElement(unsigned int pass) {
  ISpindizzyBlockSet* mSurfaceProcessor = getElementSet();
  switch (pass) {
    case INIT_PROCESS_BLOCKS: {
      std::vector<ITileSurfaceTemplate*> mTopTileSurfaces = getWaterSurfaces();
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
        mSurfaceProcessor->destroyTileTemplate(mTopTileSurfaces[i], true);
      }
      // TODO: Use the calculator to calculate surfaces
      return true;
    }
  }
  // TODO: Throw exception of some kind
  return true;
}

void SpindizzyWater::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  cStartLocation.saveRelative(node, location);
  // TODO: Only save size if it's bigger than 1.
  cEndLocation.saveRelative(node, cStartLocation, "width", "length", "height");
}


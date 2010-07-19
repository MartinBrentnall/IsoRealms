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

const unsigned int SpindizzyWater::INIT_REGISTER_BLOCKS = 0;
const unsigned int SpindizzyWater::INIT_PROCESS_BLOCKS = 1;
const unsigned int SpindizzyWater::INIT_REGISTER_SURFACES = 2;
const unsigned int SpindizzyWater::INIT_USE_SURFACES = 3;

SpindizzyWater::SpindizzyWater(ISpindizzyBlockFactory* elementFactory, BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** textureSet) : Element<ISpindizzyBlockSet, ISpindizzyBlockFactory>(elementFactory) {
  cSpindizzyTextureSet = textureSet;
  cStartLocation = BlockLocation(endLocation->x > startLocation->x ? startLocation->x : endLocation->x,
                                 endLocation->y > startLocation->y ? startLocation->y : endLocation->y,
                                 endLocation->z <= startLocation->z ? startLocation->z : endLocation->z);
  cEndLocation = BlockLocation(endLocation->x > startLocation->x ? endLocation->x : startLocation->x,
                               endLocation->y > startLocation->y ? endLocation->y : startLocation->y,
                              (endLocation->z <= startLocation->z ? endLocation->z : startLocation->z) - 1);
}

bool SpindizzyWater::isGhost() {
  return true;
}

void SpindizzyWater::setDirty() {
  signalElementDirty();
}

std::vector<ITileSurfaceTemplate*> SpindizzyWater::getWaterSurfaces() {
  ISpindizzyBlockSet* mSurfaceProcessor = getElementSet();
  return mSurfaceProcessor->getTileSurfaces(this, ITileSurface::UP);
}

std::vector<ITileSurface*> SpindizzyWater::getTileSurfaces(ITileSurface::FaceDirection facing) {
  std::vector<ITileSurface*> mSurfaces;
  ITileSurface* mWaterSurface = createSubSurface(facing, cEndLocation.y, cEndLocation.x, cStartLocation.y, cStartLocation.x);
  mSurfaces.push_back(mWaterSurface);
  return mSurfaces;
}

void SpindizzyWater::renderStatic() {
  std::vector<ITileSurfaceTemplate*> mTopTileSurfaces = getWaterSurfaces();
  for (unsigned int i = 0; i < mTopTileSurfaces.size(); i++) {
    int mNorth = mTopTileSurfaces[i]->getNorth();
    int mEast = mTopTileSurfaces[i]->getEast();
    int mSouth = mTopTileSurfaces[i]->getSouth();
    int mWest = mTopTileSurfaces[i]->getWest();
    ISpindizzyTileSurface* mTileSurface = createSubSurface(ITileSurface::UP, mNorth, mEast, mSouth, mWest);
    mTileSurface->render();
    delete mTopTileSurfaces[i];
    delete mTileSurface;
  }
}

std::vector<IVisualElement*> SpindizzyWater::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  return mVisualElements;
}

std::vector<IDynamicElement*> SpindizzyWater::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
  return mDynamicElements;
}

std::vector<IInteractiveElement*> SpindizzyWater::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  return mInteractiveElements;
}

ISpindizzyTileSurface* SpindizzyWater::createSubSurface(ITileSurface::FaceDirection facing, int north, int east, int south, int west) {
//  return new TileSurface(cSpindizzyTextureSet, ISpindizzyTextureSet::WATER, north, east, south, west, cStartLocation.z, 0, 0, facing/*, TODO:CONDITIONAL  mSurfaceCondition*/);
  /*, TODO:CONDITIONAL WATER */
  return new TileSurface(cSpindizzyTextureSet, ISpindizzyTextureSet::WATER, north, east, south, west, facing == ITileSurface::UP ? cStartLocation.z : cEndLocation.z, 0, 0, facing, NULL);
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

Condition* SpindizzyWater::getCondition() {
  return NULL; // TODO: Allow dynamic surfaces
}

void SpindizzyWater::removed() {
  ISpindizzyBlockSet* mSurfaceProcessor = getElementSet();
  mSurfaceProcessor->unregisterSurfaceProvider(this);
}

void SpindizzyWater::added() {
  ISpindizzyBlockSet* mSurfaceProcessor = getElementSet();
  mSurfaceProcessor->registerSurfaceProvider(this);
  mSurfaceProcessor->setDirty();
}

bool SpindizzyWater::initElement(unsigned int pass) {
  switch (pass) {
    case INIT_REGISTER_BLOCKS: {
      ISpindizzyBlockSet* mSurfaceProcessor = getElementSet();
      mSurfaceProcessor->registerSurfaceProvider(this);
      return false;
    }

    case INIT_PROCESS_BLOCKS: {
      // TODO: Use the calculator to calculate surfaces
      return true;
    }
  }
  // TODO: Throw exception of some kind
  return true;
}

void SpindizzyWater::save(DOMNodeWriter* node, BlockLocation& location) {
  DOMNodeWriter* mLocationNode = node->addBranch("Location");
  cStartLocation.saveRelative(mLocationNode, location);
  // TODO: Only save size if it's bigger than 1.
  DOMNodeWriter* mSizeNode = node->addBranch("Size");
  cEndLocation.saveRelative(mSizeNode, cStartLocation);
}


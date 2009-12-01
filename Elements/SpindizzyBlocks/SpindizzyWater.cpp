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

SpindizzyWater::SpindizzyWater(IElementFactory* elementFactory, BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** textureSet) : IElement(elementFactory) {
  cSpindizzyTextureSet = textureSet;
  cStartLocation = BlockLocation(endLocation->x > startLocation->x ? startLocation->x : endLocation->x,
                                 endLocation->y > startLocation->y ? startLocation->y : endLocation->y,
                                 endLocation->z <= startLocation->z ? startLocation->z : endLocation->z);
  cEndLocation = BlockLocation(endLocation->x > startLocation->x ? endLocation->x : startLocation->x,
                               endLocation->y > startLocation->y ? endLocation->y : startLocation->y,
                              (endLocation->z <= startLocation->z ? endLocation->z : startLocation->z) - 1);
  cInitStage = CACHE_SURFACES;
}

void SpindizzyWater::setDirty() {
  signalElementDirty();
}

std::vector<IRollableSurface*> SpindizzyWater::getWaterSurfaces() {
  ISurfaceCalculator* mSurfaceCalculator = dynamic_cast<ISurfaceCalculator*>(getElementSet());
  if (mSurfaceCalculator == NULL) {
    return getRollableSurfaces(IRollableSurface::UP);
  }
  return mSurfaceCalculator->getRollableSurfaces(this, IRollableSurface::UP);
}

std::vector<IRollableSurface*> SpindizzyWater::getRollableSurfaces(IRollableSurface::FaceDirection facing) {
  std::vector<IRollableSurface*> mSurfaces;
  IRollableSurface* mWaterSurface = createSubSurface(facing, cEndLocation.y, cEndLocation.x, cStartLocation.y, cStartLocation.x);
  mSurfaces.push_back(mWaterSurface);
  return mSurfaces;
}

void SpindizzyWater::renderStatic() {
  std::vector<IRollableSurface*> mTopRollableSurfaces = getWaterSurfaces();
  for (unsigned int i = 0; i < mTopRollableSurfaces.size(); i++) {
    mTopRollableSurfaces[i]->render();
    delete mTopRollableSurfaces[i];
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

IRollableSurface* SpindizzyWater::createSubSurface(IRollableSurface::FaceDirection facing, int north, int east, int south, int west) {
  return new RollableSurface(cSpindizzyTextureSet, ISpindizzyTextureSet::WATER, north, east, south, west, facing == IRollableSurface::UP ? cStartLocation.z : cEndLocation.z, 0, 0, facing/*, TODO:CONDITIONAL  mSurfaceCondition*/);
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

void SpindizzyWater::removed() {
  ISurfaceCalculator* mSurfaceCalculator = dynamic_cast<ISurfaceCalculator*>(getElementSet());
  if (mSurfaceCalculator == NULL) {
    std::cout << "Warning: dynamic_cast failed for surface calculation!  Surfaces may appear incorrect!" << std::endl;
  } else {
    mSurfaceCalculator->unregisterRollableSurfaceProvider(this);
  }
}

void SpindizzyWater::added() {
  ISurfaceCalculator* mSurfaceCalculator = dynamic_cast<ISurfaceCalculator*>(getElementSet());
  if (mSurfaceCalculator == NULL) {
    std::cout << "Warning: dynamic_cast failed for surface calculation!  Surfaces may appear incorrect!" << std::endl;
  } else {
    mSurfaceCalculator->registerRollableSurfaceProvider(this);
    mSurfaceCalculator->setDirty();
  }
}

bool SpindizzyWater::initElement() {
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

void SpindizzyWater::save(DOMNodeWriter* node, BlockLocation& location) {
  DOMNodeWriter* mLocationNode = node->addBranch("Location");
  cStartLocation.saveRelative(mLocationNode, location);
  // TODO: Only save size if it's bigger than 1.
  DOMNodeWriter* mSizeNode = node->addBranch("Size");
  cEndLocation.saveRelative(mSizeNode, cStartLocation);
}

//  glBindTexture(GL_TEXTURE_2D, 0);
/*  if (symbol == WATER) {
    glDisable(GL_CULL_FACE);
  }*/
/*  if (symbol == WATER) {
    glEnable(GL_CULL_FACE);
  }*/

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

SpindizzyWater::SpindizzyWater(IElementFactory* elementFactory, BlockLocation* startLocation, int east, int north, ISpindizzyTextureSet** textureSet) : IElement(elementFactory) {
  cSpindizzyTextureSet = textureSet;
  cStartLocation = BlockLocation(east  > startLocation->x ? east  : startLocation->x,
                                 north > startLocation->y ? north : startLocation->y,
                                         startLocation->z);;
  cEast  = east  > startLocation->x ? startLocation->x : east;
  cNorth = north > startLocation->y ? startLocation->y : north;
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
  IRollableSurface* mWaterSurface = createSubSurface(facing, cNorth, cEast, cStartLocation.y, cStartLocation.x);
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
  return new RollableSurface(cSpindizzyTextureSet, ISpindizzyTextureSet::WATER, north, east, south, west, cStartLocation.z, 0, 0, facing/*, TODO:CONDITIONAL  mSurfaceCondition*/);
}

IWallSurface* SpindizzyWater::createSubSurface(int, int, IWallSurface::FaceDirection, int, int, int, int, int) {
  return NULL; // Water doesn't have walls.
}

std::vector<IWallSurface*> SpindizzyWater::getWallSurfaces(int, IWallSurface::FaceDirection) {
  std::vector<IWallSurface*> mEmptyVector;  
  return mEmptyVector;
}

BlockArea* SpindizzyWater::getCoverage() {
  BlockLocation mEndLocation(cEast, cNorth, cStartLocation.z);
  return new BlockArea(cStartLocation, mEndLocation);
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
  mSizeNode->addAttribute("x", cEast - cStartLocation.x);
  mSizeNode->addAttribute("y", cNorth - cStartLocation.y);
}

//  glBindTexture(GL_TEXTURE_2D, 0);
/*  if (symbol == WATER) {
    glDisable(GL_CULL_FACE);
  }*/
/*  if (symbol == WATER) {
    glEnable(GL_CULL_FACE);
  }*/

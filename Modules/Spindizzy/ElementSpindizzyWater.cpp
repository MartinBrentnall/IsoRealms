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
#include "ElementSpindizzyWater.h"

const unsigned int ElementSpindizzyWater::INIT_PROCESS_BLOCKS = 0;

ElementSpindizzyWater::ElementSpindizzyWater(ISpindizzyBlockType* elementType, BlockLocation* startLocation, BlockLocation* endLocation, ITexture** texture, IElementContainer* container) {
  cWaterType = elementType;
  cContainer = container;
  cTexture = texture;
  cStartLocation = BlockLocation(endLocation->x > startLocation->x ? startLocation->x : endLocation->x,
                                 endLocation->y > startLocation->y ? startLocation->y : endLocation->y,
                                 endLocation->z <= startLocation->z ? startLocation->z : endLocation->z);
  cEndLocation = BlockLocation(endLocation->x > startLocation->x ? endLocation->x : startLocation->x,
                               endLocation->y > startLocation->y ? endLocation->y : startLocation->y,
                              (endLocation->z <= startLocation->z ? endLocation->z : startLocation->z) - 1);
  cCondition = nullptr;
}

bool ElementSpindizzyWater::isGhost() {
  return true;
}

void ElementSpindizzyWater::setDirty() {
//   for (unsigned int i = 0; i < cStaticTileSurfaces.size(); i++) {
//     delete cStaticTileSurfaces[i];
//   }
//   cStaticTileSurfaces.clear();
}

IElementContainer* ElementSpindizzyWater::getElementContainer() {
  return cContainer;
}

std::vector<ITileSurfaceTemplate*> ElementSpindizzyWater::getWaterSurfaces() {
  ISpindizzyBlockSet* mSurfaceProcessor = cWaterType->getSpindizzyBlockInterface();
  return mSurfaceProcessor->getTileSurfaces(this, ITileSurface::UP, true);
}

std::vector<ITileSurface*> ElementSpindizzyWater::getTileSurfaces(ITileSurface::FaceDirection facing) {
  std::vector<ITileSurface*> mSurfaces;
  ITileSurface* mWaterSurface = createSubSurface(facing, cEndLocation.y, cEndLocation.x, cStartLocation.y, cStartLocation.x, cCondition);
  mSurfaces.push_back(mWaterSurface);
  return mSurfaces;
}

void ElementSpindizzyWater::renderStatic() {
  ISpindizzyBlockSet* mSpindizzyBlockSet = cWaterType->getSpindizzyBlockInterface();
  bool mEditing = mSpindizzyBlockSet->isEditing();
  
  for (unsigned int i = 0; i < cStaticTileSurfaces.size(); i++) {
    cStaticTileSurfaces[i]->render();
    if (!mEditing) {
      delete cStaticTileSurfaces[i];
    }
  }
}

void ElementSpindizzyWater::renderRuntime() {
  for (unsigned int i = 0; i < cDynamicTileSurfaces.size(); i++) {
    cDynamicTileSurfaces[i]->render();
  }
}

ISpindizzyTileSurface* ElementSpindizzyWater::createSubSurface(ITileSurface::FaceDirection facing, int north, int east, int south, int west, Condition* condition) {
  return new TileSurface(cTexture, STRAIGHT, north, east, south, west, facing == ITileSurface::UP ? cStartLocation.z : cEndLocation.z, 0, 0, facing, condition, nullptr, nullptr);
}

IWallSurface* ElementSpindizzyWater::createSubSurface(int x, int y, IWallSurface::FaceDirection facing, int length, int startHeight, int endHeight, int topSlope, int bottomSlope) {
  return new WallSurface(x, y, startHeight, length, endHeight, 0, facing);
}

std::vector<IWallSurface*> ElementSpindizzyWater::getWallSurfaces(int location, IWallSurface::FaceDirection facing) {
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

BlockArea* ElementSpindizzyWater::getCoverage() {
  return new BlockArea(cStartLocation, cEndLocation);
}

void ElementSpindizzyWater::destroyCoverage(BlockArea* coverage) {
  delete coverage;
}

Condition* ElementSpindizzyWater::getCondition() {
  return nullptr; // TODO: Allow dynamic surfaces
}

void ElementSpindizzyWater::createSampleSurfaces() {
  ISpindizzyTileSurface* mTopSurface = createSubSurface(ITileSurface::UP, cEndLocation.y, cEndLocation.x, cEndLocation.y, cEndLocation.x, nullptr);
  cStaticTileSurfaces.push_back(mTopSurface);
}

std::set<IBoolean*> ElementSpindizzyWater::getInputs() {
  if (cCondition != nullptr) {
    return cCondition->getInputs();
  }
  std::set<IBoolean*> mNoInputs;
  return mNoInputs;
}

/* TODO  ISpindizzyBlockSet* mSurfaceProcessor = cWaterType->getElementSet();
  mSurfaceProcessor->unregisterSurfaceProvider(this);*/

std::string ElementSpindizzyWater::getTypeName() {
  return "Spindizzy Water"; // TODO
}
  
std::vector<IObjectProperty*> ElementSpindizzyWater::getProperties() {
  return std::vector<IObjectProperty*>();
}

IElementType* ElementSpindizzyWater::getElementType() {
  return cWaterType;
}

bool ElementSpindizzyWater::initElement(unsigned int pass) {
  ISpindizzyBlockSet* mSurfaceProcessor = cWaterType->getSpindizzyBlockInterface();
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
        if (mCondition == nullptr) {
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

void ElementSpindizzyWater::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  std::string mElementTypePath = resourceLocator->getPath(cWaterType);
  node->addAttribute("type", mElementTypePath);
  cStartLocation.saveRelative(node, location);
  // TODO: Only save size if it's bigger than 1.
  cEndLocation.saveRelative(node, cStartLocation, "width", "length", "height");
}

IElementBounds* ElementSpindizzyWater::getBounds() {
  return this;
}

PickedElement* ElementSpindizzyWater::pickElement(Vertex& start, Vertex& end) {
  Vertex mStart(start.x, start.y, start.z / IsoRealmsConstants::BLOCK_HEIGHT);
  Vertex mEnd(  end.x,   end.y,   end.z   / IsoRealmsConstants::BLOCK_HEIGHT);
  CollisionVertex* mClosestCollision = nullptr;
  for (ISpindizzyTileSurface* mTileSurface : cStaticTileSurfaces) {
    CollisionVertex* mCollisionVertex = mTileSurface->pickSurface(mStart, mEnd);
    if (mCollisionVertex != nullptr && (mClosestCollision == nullptr || mCollisionVertex->gradient < mClosestCollision->gradient)) {
      // TODO: Delete Picked Element
      mClosestCollision = mCollisionVertex;
    }
  }

  for (ISpindizzyTileSurface* mTileSurface : cDynamicTileSurfaces) {
    CollisionVertex* mCollisionVertex = mTileSurface->pickSurface(mStart, mEnd);
    if (mCollisionVertex != nullptr && (mClosestCollision == nullptr || mCollisionVertex->gradient < mClosestCollision->gradient)) {
      // TODO: Delete Picked Element
      mClosestCollision = mCollisionVertex;
    }
  }
  return mClosestCollision != nullptr ? new PickedElement(mClosestCollision, this) : nullptr;
}

float ElementSpindizzyWater::getWest() {
  return cStartLocation.x - IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyWater::getEast() {
  return cEndLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyWater::getSouth() {
  return cStartLocation.y - IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyWater::getNorth() {
  return cEndLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyWater::getBottom() {
  return cEndLocation.z * IsoRealmsConstants::BLOCK_HEIGHT;
}

float ElementSpindizzyWater::getTop() {
  return cStartLocation.z * IsoRealmsConstants::BLOCK_HEIGHT;
}

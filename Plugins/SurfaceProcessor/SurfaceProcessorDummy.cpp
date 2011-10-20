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
#include "SurfaceProcessorDummy.h"


void SurfaceProcessorDummy::registerSurfaceProvider(ISurfaceProvider*) {
  // Nothing to do.
}

void SurfaceProcessorDummy::unregisterSurfaceProvider(ISurfaceProvider*) {
  // Nothing to do.
}

std::vector<ITileSurfaceTemplate*> SurfaceProcessorDummy::getTileSurfaces(ISurfaceProvider* provider, ITileSurface::FaceDirection faceDirection) {
  std::vector<ITileSurface*> mTileSurfaces = provider->getTileSurfaces(faceDirection);
  std::vector<ITileSurfaceTemplate*> mTileSurfaceTemplates;
  for (unsigned int i = 0; i < mTileSurfaces.size(); i++) {
    // TODO: Make some dummy templates from the surfaces
//    mTileSurfaceTemplate.push_back();
  }
  return mTileSurfaceTemplates;
}

int SurfaceProcessorDummy::getOuterWallFaceLocation(BlockArea* coverage, IWallSurface::FaceDirection facing) {
  switch (facing) {
    case IWallSurface::NORTH: return coverage->getNorth();
    case IWallSurface::EAST:  return coverage->getEast();
    case IWallSurface::SOUTH: return coverage->getSouth();
    case IWallSurface::WEST:  return coverage->getWest();
  }
  // TODO: Throw wobbly
  std::cout << "Warning: unknown case for face direction" << std::endl;
  exit(1);
}

std::vector<IWallSurfaceTemplate*> SurfaceProcessorDummy::getWallSurfaces(ISurfaceProvider* provider, IWallSurface::FaceDirection facing) {
  BlockArea* mCoverage = provider->getCoverage();
  // TODO: Get all visible walls
  int mOuterFaceLocation = getOuterWallFaceLocation(mCoverage, facing);
  std::vector<IWallSurface*> mRawSurfaces = provider->getWallSurfaces(mOuterFaceLocation, facing);
// TODO: SHOULDN'T DO THIS HERE:  delete mCoverage;
  std::vector<IWallSurfaceTemplate*> mDummySurfaceTemplates;
  return mDummySurfaceTemplates;
}

void SurfaceProcessorDummy::destroyTileTemplate(ITileSurfaceTemplate* tileTemplate) {
  delete tileTemplate;
}

void SurfaceProcessorDummy::destroyWallTemplate(IWallSurfaceTemplate* wallTemplate) {
  delete wallTemplate;
}

void SurfaceProcessorDummy::initElementsComplete() {
  // Nothing to do.
}

void SurfaceProcessorDummy::setDirty() {
  // Nothing to do.
}

void SurfaceProcessorDummy::reinitialise() {
  // Nothing to do.
}

extern "C" IPlugin* create() {
  return new SurfaceProcessorDummy();
}

extern "C" void destroy(IPlugin* dummySurfaceProcessor) {
  delete dummySurfaceProcessor;
}

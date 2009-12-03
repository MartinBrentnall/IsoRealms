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
#include "BlockSubtractorCache.h"

BlockSubtractorCache::BlockSubtractorCache() {
  cCurrentZoneProviders = NULL;
}

void BlockSubtractorCache::clear() {
  // TODO: Delete the allocated std::vectors!
  cOrderedSurfaceProvidersByZone.clear();
}

void BlockSubtractorCache::setZone(Zone* zone) {
  cCurrentZoneProviders = cOrderedSurfaceProvidersByZone[zone];
  if (cCurrentZoneProviders == NULL) {
    cCurrentZoneProviders = new std::vector<IRollableSurfaceProvider*>();
    cOrderedSurfaceProvidersByZone[zone] = cCurrentZoneProviders;
  }
}

int BlockSubtractorCache::getIndex(IRollableSurfaceProvider* provider) {
  for (unsigned int i = 0; i < cCurrentZoneProviders->size(); i++) {
    if ((*cCurrentZoneProviders)[i] == provider) {
      return i;
    }
  }
  // TODO: Throw wobbly
  std::cout << "WARNING: Surface provider wasn't found.  Did you forget to set the Zone?" << std::endl;
  return -1;
}

void BlockSubtractorCache::add(IRollableSurfaceProvider* provider) {
  cCurrentZoneProviders->push_back(provider);
}

void BlockSubtractorCache::remove(IRollableSurfaceProvider* provider) {
  int mIndex = getIndex(provider);
  std::cout << "Removing provider at index: " << mIndex << ".." << std::endl;
  cCurrentZoneProviders->erase(cCurrentZoneProviders->begin() + mIndex);
  setDirty(); // TODO: Should this be here?  Maybe we should call it manually instead when necessary.
}

void BlockSubtractorCache::setDirty() {
  for (unsigned int i = 0; i < cCurrentZoneProviders->size(); i++) {
    (*cCurrentZoneProviders)[i]->setDirty();
  }
}

std::vector<IRollableSurfaceProvider*> BlockSubtractorCache::getSurfaceProviders(bool priority, IRollableSurfaceProvider* provider) {
  std::vector<IRollableSurfaceProvider*> mReturnList;
  if (cCurrentZoneProviders != NULL) {
    if (!priority) {
      for (unsigned int i = 0; i < cCurrentZoneProviders->size(); i++) {
        if (provider == (*cCurrentZoneProviders)[i]) {
          return mReturnList;
        }
        mReturnList.push_back((*cCurrentZoneProviders)[i]);
      }
    } else {
      for (int i = cCurrentZoneProviders->size() - 1; i >= 0; i--) {
        if (provider == (*cCurrentZoneProviders)[i]) {
          return mReturnList;
        }
        mReturnList.push_back((*cCurrentZoneProviders)[i]);
      }
    }
    mReturnList.clear();
    std::cout << "Warning: Specified provider isn't cached!  Surfaces will not be calculated correctly!" << std::endl;
  }
  return mReturnList;
}

std::vector<IRollableSurfaceProvider*> BlockSubtractorCache::getSurfaceProviders() {
  return *cCurrentZoneProviders;
}

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

void BlockSubtractorCache::setZone(Zone* zone) {
  cCurrentZoneProviders = cOrderedSurfaceProvidersByZone[zone];
  if (cCurrentZoneProviders == NULL) {
    cCurrentZoneProviders = new std::vector<ISurfaceProvider*>();
    cOrderedSurfaceProvidersByZone[zone] = cCurrentZoneProviders;
  }
}

int BlockSubtractorCache::getIndex(std::vector<ISurfaceProvider*>* list, ISurfaceProvider* provider) {
  for (unsigned int i = 0; i < list->size(); i++) {
    if ((*list)[i] == provider) {
      return i;
    }
  }
  return -1;
}

void BlockSubtractorCache::add(ISurfaceProvider* provider) {
  cCurrentZoneProviders->push_back(provider);
}

void BlockSubtractorCache::remove(ISurfaceProvider* provider) {
  for (std::map<Zone*, std::vector<ISurfaceProvider*>*>::iterator i = cOrderedSurfaceProvidersByZone.begin(); i != cOrderedSurfaceProvidersByZone.end(); ++i) {
    int mIndex = getIndex(i->second, provider);
    if (mIndex >= 0) {
      i->second->erase(i->second->begin() + mIndex);
      // TODO: This call only works for current zone, but this remove function works regardless of zone....  hmmm...
      setDirty();
      return;
    }
  }
  std::cout << "Warning: Surface provider not found in this cache!" << std::endl;
}

void BlockSubtractorCache::setDirty() {
  for (unsigned int i = 0; i < cCurrentZoneProviders->size(); i++) {
    (*cCurrentZoneProviders)[i]->setDirty();
  }
}

void BlockSubtractorCache::reinitialise() {
  for (std::map<Zone*, std::vector<ISurfaceProvider*>*>::iterator i = cOrderedSurfaceProvidersByZone.begin(); i != cOrderedSurfaceProvidersByZone.end(); ++i) {
    for (unsigned int j = 0; j < i->second->size(); j++) {
      (*i->second)[j]->setDirty();
    }
    delete i->second;
  }
  cOrderedSurfaceProvidersByZone.clear();
}

std::vector<ISurfaceProvider*> BlockSubtractorCache::getSurfaceProviders(bool priority, ISurfaceProvider* provider) {
  std::vector<ISurfaceProvider*> mReturnList;
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

std::vector<ISurfaceProvider*> BlockSubtractorCache::getSurfaceProviders() {
  return *cCurrentZoneProviders;
}

BlockSubtractorCache::~BlockSubtractorCache() {
  for (std::map<Zone*, std::vector<ISurfaceProvider*>*>::iterator i = cOrderedSurfaceProvidersByZone.begin(); i != cOrderedSurfaceProvidersByZone.end(); ++i) {
    delete i->second;
  }
}

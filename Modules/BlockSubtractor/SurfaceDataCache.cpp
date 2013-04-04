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
#include "SurfaceDataCache.h"

std::vector<TileColumn*>* SurfaceDataCache::getTileColumn(unsigned int x, unsigned int y) {
  if (x < cTileColumns.size()) {
    std::vector<std::vector<TileColumn*>*>* mRow = cTileColumns[x];
    if (y < mRow->size()) {
      return (*mRow)[y];
    }
  }
  return NULL;
}

void SurfaceDataCache::putTileColumn(std::vector<TileColumn*>* tileColumn, unsigned int x, unsigned int y) {
  while (cTileColumns.size() <= x) {
    cTileColumns.push_back(new std::vector<std::vector<TileColumn*>*>());
  }
  std::vector<std::vector<TileColumn*>*>* mRow = cTileColumns[x];
  while (mRow->size() <= y) {
    mRow->push_back(NULL);
  }
  (*mRow)[y] = tileColumn;
}

void SurfaceDataCache::clear() {
  for (unsigned int i = 0; i < cTileColumns.size(); i++) {
    for (unsigned int j = 0; j < cTileColumns[i]->size(); j++) {
      if ((*cTileColumns[i])[j] != NULL) {
        for (unsigned int k = 0; k < (*cTileColumns[i])[j]->size(); k++) {
          delete (*(*cTileColumns[i])[j])[k];
        }
        (*cTileColumns[i])[j]->clear();
        delete (*cTileColumns[i])[j];
      }
    }
    cTileColumns[i]->clear();
    delete cTileColumns[i];
  }
  cTileColumns.clear();
}

SurfaceDataCache::~SurfaceDataCache() {
  clear();
}



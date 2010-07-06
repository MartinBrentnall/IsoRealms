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
#include "TileColumn.h"

void TileColumn::addTileBlock(TileBlock* tileBlock) {
  std::vector<unsigned int> mToRemove;
  bool mMerged = false;
  for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
    TileBlock* mSplitTileBlock = cTileBlocks[i]->split(*tileBlock);
    if (mSplitTileBlock != NULL) {
      cTileBlocks.push_back(mSplitTileBlock);
      return;
    }
    if (cTileBlocks[i]->merge(*tileBlock)) {
      if (tileBlock->isAddition()) {
        if (mMerged) {
          mToRemove.push_back(i);
        }
        tileBlock = cTileBlocks[i];
        mMerged = true;
      }
    }
    if (!cTileBlocks[i]->isAddition()) {
      mToRemove.push_back(i);
    }
  }
  if (!mMerged && tileBlock->isAddition()) {
    cTileBlocks.push_back(tileBlock);
  }
  for (unsigned int i = 0; i < mToRemove.size(); i++) {
    cTileBlocks.erase(cTileBlocks.begin() + mToRemove[i]);
  }
}

void TileColumn::debug() {
  std::cout << "Column contents:" << std::endl;
  for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
    cTileBlocks[i]->debug();
  }
}

bool TileColumn::isBottomTileVisible(ISurfaceProvider* surfaceProvider) {
  debug();
  std::cout << "Is surface provider \"" << surfaceProvider << "\" represented at bottom?" << std::endl;
  for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
    std::cout << "   " << i << ": " << cTileBlocks[i]->getBottomSurfaceProvider() << std::endl;
    if (cTileBlocks[i]->getBottomSurfaceProvider() == surfaceProvider) {
      std::cout << "          Found it!" << std::endl;
      return true;
    }
  }
  std::cout << "          Not Found!" << std::endl;
  return false;
}

bool TileColumn::isTopTileVisible(ISurfaceProvider* surfaceProvider) {
  debug();
  std::cout << "Is surface provider \"" << surfaceProvider << "\" represented at top?" << std::endl;
  for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
    std::cout << "   " << i << ": " << cTileBlocks[i]->getTopSurfaceProvider() << std::endl;
    if (cTileBlocks[i]->getTopSurfaceProvider() == surfaceProvider) {
      std::cout << "          Found it!" << std::endl;
      return true;
    }
  }
  std::cout << "          Not Found!" << std::endl;
  return false;
}

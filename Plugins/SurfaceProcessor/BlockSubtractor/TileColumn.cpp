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

TileColumn::TileColumn(Condition* condition) {
  cCondition = condition;
}

TileColumn::TileColumn(TileColumn* tileColumn, Condition* condition) {
  cCondition = condition;
  for (unsigned int i = 0; i < tileColumn->cGhostTileBlocks.size(); i++) {
    cGhostTileBlocks.push_back(new TileBlock(*tileColumn->cGhostTileBlocks[i]));
  }
  for (unsigned int i = 0; i < tileColumn->cTileBlocks.size(); i++) {
    cTileBlocks.push_back(new TileBlock(*tileColumn->cTileBlocks[i]));
  }
}

void TileColumn::addTileBlock(TileBlock* tileBlock, bool ghost, Condition* condition) {
  std::vector<unsigned int> mToRemove;
  bool mMerged = false;
  unsigned int mTileBlockIndex = -1;
  if (!ghost) {
    for (int i = cGhostTileBlocks.size() - 1; i >= 0; i--) {
      if (cGhostTileBlocks[i]->subtractAsGhost(*tileBlock)) {
        cGhostTileBlocks.erase(cGhostTileBlocks.begin() + i);
      }
    }
    for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
      TileBlock* mSplitTileBlock = cTileBlocks[i]->split(*tileBlock);
      if (mSplitTileBlock != NULL) {
        cTileBlocks.push_back(mSplitTileBlock);
        return;
      }
      if (cTileBlocks[i]->merge(*tileBlock)) {
        if (tileBlock->isAddition()) {
          if (mMerged) {
            mToRemove.push_back(mTileBlockIndex);
          }
          tileBlock = cTileBlocks[i];
          mTileBlockIndex = i;
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
    for (int i = mToRemove.size() - 1; i >= 0; i--) {
      cTileBlocks.erase(cTileBlocks.begin() + mToRemove[i]);
    }
  } else {
    cGhostTileBlocks.push_back(tileBlock);
    // TODO: Needs more testing to make sure this is correct handling for ghost blocks
    // TODO: DUPLICATE CODE
    for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
      TileBlock* mSplitTileBlock = cTileBlocks[i]->split(*tileBlock);
      if (mSplitTileBlock != NULL) {
        cTileBlocks.push_back(mSplitTileBlock);
        return;
      }
      if (cTileBlocks[i]->merge(*tileBlock)) {
        if (tileBlock->isAddition()) {
          if (mMerged) {
            mToRemove.push_back(mTileBlockIndex);
          }
          tileBlock = cTileBlocks[i];
          mTileBlockIndex = i;
          mMerged = true;
        }
      }
      if (!cTileBlocks[i]->isAddition()) {
        mToRemove.push_back(i);
      }
    }
  }
}

bool TileColumn::isTileVisible(ISurfaceProvider* surfaceProvider, ITileSurface::FaceDirection facing) {
  for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
    if (cTileBlocks[i]->getSurfaceProvider(facing) == surfaceProvider) {
      return true;
    }
  }
  for (unsigned int i = 0; i < cGhostTileBlocks.size(); i++) {
    if (cGhostTileBlocks[i]->getSurfaceProvider(facing) == surfaceProvider) {
      return true;
    }
  }
  return false;
}

TileColumn* TileColumn::split(Condition* condition) {
  if (cCondition != NULL) {
    Condition* mSplitCondition = cCondition->split(condition);
    if (mSplitCondition != NULL) {
      return new TileColumn(this, mSplitCondition);
    }
  } else {
    cCondition = condition;
    Condition* mNegatedCondition = new Condition(*cCondition);
    mNegatedCondition->negate();
    return new TileColumn(this, mNegatedCondition);
  }
  return NULL;
}

Condition* TileColumn::getCondition() {
  return cCondition;
}

void TileColumn::debug() {
  std::cout << "Column has " << cTileBlocks.size() << " blocks for: ";
  cCondition->debug();
  for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
    std::cout << "   Block " << i << ": ";
    cTileBlocks[i]->debug();
  }
}


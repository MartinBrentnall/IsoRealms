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

bool TileColumn::addTileBlock(TileBlock* tileBlock, bool ghost, Condition* condition) {
  std::vector<unsigned int> mToRemove;
  int mTileBlockIndex = -1;
  if (!ghost) {
    for (int i = cGhostTileBlocks.size() - 1; i >= 0; i--) {
      if (cGhostTileBlocks[i]->subtractAsGhost(*tileBlock)) {
        delete cGhostTileBlocks[i];
        cGhostTileBlocks.erase(cGhostTileBlocks.begin() + i);
      }
    }
    for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
      TileBlock* mSplitTileBlock = cTileBlocks[i]->split(*tileBlock);
      if (mSplitTileBlock != NULL) {
        cTileBlocks.push_back(mSplitTileBlock);
        return false;
      }
      if (cTileBlocks[i]->merge(*tileBlock)) {
        if (tileBlock->isAddition()) {
          if (mTileBlockIndex != -1) {
            mToRemove.push_back(mTileBlockIndex);
          }
          tileBlock = cTileBlocks[i];
          mTileBlockIndex = i;
        }
      }
      if (!cTileBlocks[i]->isAddition()) {
        mToRemove.push_back(i);
      }
    }
    for (int i = mToRemove.size() - 1; i >= 0; i--) {
      delete cTileBlocks[mToRemove[i]];
      cTileBlocks.erase(cTileBlocks.begin() + mToRemove[i]);
    }
    bool mTileBlockUsed = mTileBlockIndex == -1 && tileBlock->isAddition();
    if (mTileBlockUsed) {
      cTileBlocks.push_back(tileBlock);
    }
    return mTileBlockUsed;
  } else {
    cGhostTileBlocks.push_back(tileBlock);
    // TODO: Needs more testing to make sure this is correct handling for ghost blocks
    // TODO: DUPLICATE CODE
    for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
      TileBlock* mSplitTileBlock = cTileBlocks[i]->split(*tileBlock);
      if (mSplitTileBlock != NULL) {
        cTileBlocks.push_back(mSplitTileBlock);
        return true;
      }
      if (cTileBlocks[i]->merge(*tileBlock)) {
        if (tileBlock->isAddition()) {
          if (mTileBlockIndex != -1) {
            mToRemove.push_back(mTileBlockIndex);
          }
          tileBlock = cTileBlocks[i];
          mTileBlockIndex = i;
        }
      }
      if (!cTileBlocks[i]->isAddition()) {
        mToRemove.push_back(i);
      }
    }
    return true;
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
  } else if (condition != NULL) {
    cCondition = condition;
    Condition* mNegatedCondition = new Condition(*condition);
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

TileColumn::~TileColumn() {
  for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
    delete cTileBlocks[i];
  }
  for (unsigned int i = 0; i < cGhostTileBlocks.size(); i++) {
    delete cGhostTileBlocks[i];
  }
}



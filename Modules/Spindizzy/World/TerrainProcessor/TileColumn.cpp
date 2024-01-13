/*
 * Copyright 2023 Martin Brentnall
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

namespace IsoRealms::Spindizzy {
  TileColumn::TileColumn(std::optional<Condition>& condition) :
            cCondition(condition) {
  }

  TileColumn::TileColumn(TileColumn* tileColumn, std::optional<Condition>& condition) :
            cCondition(condition) {
    for (unsigned int i = 0; i < tileColumn->cGhostTileBlocks.size(); i++) {
      cGhostTileBlocks.emplace_back(std::make_unique<TileBlock>(*tileColumn->cGhostTileBlocks[i]));
    }
    for (unsigned int i = 0; i < tileColumn->cTileBlocks.size(); i++) {
      cTileBlocks.emplace_back(std::make_unique<TileBlock>(*tileColumn->cTileBlocks[i]));
    }
  }

  bool TileColumn::addTileBlock(std::unique_ptr<TileBlock> tileBlock, bool ghost) {
    std::vector<unsigned int> mToRemove;
    int mTileBlockIndex = -1;
    if (!ghost) {
      for (int i = cGhostTileBlocks.size() - 1; i >= 0; i--) {
        if (cGhostTileBlocks[i]->subtractAsGhost(*tileBlock)) {
          cGhostTileBlocks.erase(cGhostTileBlocks.begin() + i);
        }
      }
      for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
        std::unique_ptr<TileBlock> mSplitTileBlock = cTileBlocks[i]->split(*tileBlock);
        if (mSplitTileBlock != nullptr) {
          cTileBlocks.emplace_back(std::move(mSplitTileBlock));
          return false;
        }
        if (cTileBlocks[i]->merge(*tileBlock)) {
          if (tileBlock->isAddition()) {
            if (mTileBlockIndex != -1) {
              mToRemove.push_back(mTileBlockIndex);
            }
            tileBlock = std::make_unique<TileBlock>(*cTileBlocks[i]);
            mTileBlockIndex = i;
          }
        }
        if (!cTileBlocks[i]->isAddition()) {
          mToRemove.push_back(i);
        }
      }
      for (int i = mToRemove.size() - 1; i >= 0; i--) {
        cTileBlocks.erase(cTileBlocks.begin() + mToRemove[i]);
      }
      bool mTileBlockUsed = mTileBlockIndex == -1 && tileBlock->isAddition();
      if (mTileBlockUsed) {
        cTileBlocks.emplace_back(std::move(tileBlock));
      }
      return mTileBlockUsed;
    } else {
      cGhostTileBlocks.emplace_back(std::make_unique<TileBlock>(*tileBlock));
      // TODO: Needs more testing to make sure this is correct handling for ghost blocks
      // TODO: DUPLICATE CODE
      for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
        std::unique_ptr<TileBlock> mSplitTileBlock = cTileBlocks[i]->split(*tileBlock);
        if (mSplitTileBlock != nullptr) {
          cTileBlocks.emplace_back(std::move(mSplitTileBlock));
          return true;
        }
        if (cTileBlocks[i]->merge(*tileBlock)) {
          if (tileBlock->isAddition()) {
            if (mTileBlockIndex != -1) {
              mToRemove.push_back(mTileBlockIndex);
            }
            tileBlock = std::make_unique<TileBlock>(*cTileBlocks[i]);
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

  bool TileColumn::isTileVisible(Terrain* terrain, ISurface::Direction facing) {
    for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
      if (cTileBlocks[i]->getTerrain(facing) == terrain) {
        return true;
      }
    }
    for (unsigned int i = 0; i < cGhostTileBlocks.size(); i++) {
      if (cGhostTileBlocks[i]->getTerrain(facing) == terrain) {
        return true;
      }
    }
    return false;
  }

  std::unique_ptr<TileColumn> TileColumn::split(std::optional<Condition>& condition) {
    if (cCondition.has_value()) {
      std::vector<Condition> mSplitCondition = cCondition->split(condition);
      if (mSplitCondition.size() == 2) {
        cCondition = Condition(mSplitCondition[0]);
        std::optional<Condition> mOtherSplitCondition(mSplitCondition[1]);
        return std::make_unique<TileColumn>(this, mOtherSplitCondition);
      }
    } else if (condition.has_value()) {
      cCondition = Condition(*condition);
      std::optional<Condition> mNegatedCondition = cCondition->negate();
      return std::make_unique<TileColumn>(this, mNegatedCondition);
    }
    return nullptr;
  }

  std::optional<Condition>& TileColumn::getCondition() {
    return cCondition;
  }

  void TileColumn::debug() {
    std::cout << "Column has " << cTileBlocks.size() << " blocks for: ";
    if (cCondition.has_value()) {
      cCondition->debug();
    } else {
      std::cout << "Unconditional" << std::endl;
    }
    for (unsigned int i = 0; i < cTileBlocks.size(); i++) {
      std::cout << "   Block " << i << ": ";
      cTileBlocks[i]->debug();
    }
  }
}

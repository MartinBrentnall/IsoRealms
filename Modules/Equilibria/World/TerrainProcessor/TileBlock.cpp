/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "TileBlock.h"

#include "Modules/Equilibria/World/Object/Terrain/Terrain.h"

namespace IsoRealms::Equilibria {
  int TileBlock::cInstanceCount = 0;

  bool TileBlock::isAddition() {
    return cTop.cHeight > cBottom.cHeight;
  }

  TileBlock::TileBlock(Terrain* terrain, int top, int bottom, bool topExtended, bool bottomExtended) :
            cTop(terrain, top, topExtended),
            cBottom(terrain, bottom, bottomExtended) {
  }

  TileBlock::TileBlock(const TileBlock& tileBlock) :
            cTop(tileBlock.cTop.cTerrain, tileBlock.cTop.cHeight, tileBlock.cTop.cExtended),
            cBottom(tileBlock.cBottom.cTerrain, tileBlock.cBottom.cHeight, tileBlock.cBottom.cExtended) {
  }

  std::unique_ptr<TileBlock> TileBlock::split(TileBlock& splitter) {
    if (!splitter.isAddition() && splitter.cTop > cBottom && splitter.cBottom < cTop) {
      std::unique_ptr<TileBlock> mLowerTileBlock = std::make_unique<TileBlock>(cBottom.cTerrain, splitter.cTop.cHeight, cBottom.cHeight, cBottom.cExtended, false);
      cBottom = splitter.cBottom;
      mLowerTileBlock->cTop.cTerrain = splitter.cTop.cTerrain;
      cBottom.cTerrain = splitter.cBottom.cTerrain;
      cBottom.cExtended = false;
      return mLowerTileBlock;
    }
    return nullptr;
  }

  bool TileBlock::isHigher(TileBlock& other) {
    return other.cTop > cTop || (other.cTop == cTop && (other.cTop.cExtended == cTop.cExtended || other.cTop.cExtended));
  }

  bool TileBlock::isLower(TileBlock& other) {
    return other.cBottom < cBottom || (other.cBottom == cBottom && (other.cBottom.cExtended == cBottom.cExtended || other.cBottom.cExtended));
  }

  bool TileBlock::merge(TileBlock& merger) {
    if (merger.isAddition()) {
      if (merger.cBottom <= cTop && merger.cTop >= cBottom) {
        if (isLower(merger))  {
          cBottom = merger.cBottom;
        }
        if (isHigher(merger)) {
          cTop = merger.cTop;
        }
        return true;
      }
    } else {
      if ((merger.cTop < cTop || (merger.cTop == cTop && cTop.cExtended)) && (merger.cBottom > cBottom || (merger.cBottom == cBottom && cBottom.cExtended))) {
        if (merger.cTop <= cBottom && merger.cBottom >= cTop) {
          cTop.cHeight = 0;
          cBottom.cHeight = 0;
          return true;
        }
        if (merger.cTop > cBottom) {
          cTop = merger.cTop;
          cTop.cExtended = false;
        }
        if (merger.cBottom < cTop) {
          cBottom = merger.cBottom;
          cBottom.cExtended = false;
        }
        return true;
      }
    }
    return false;
  }

  bool TileBlock::subtractAsGhost(TileBlock& other) {
    if (other.cTop >= cTop && other.cBottom <= cTop) {
      cTop.cTerrain = nullptr;
    }
    if (other.cBottom <= cBottom && other.cTop >= cBottom) {
      cBottom.cTerrain = nullptr;
    }
    return cBottom.cTerrain == nullptr && cTop.cTerrain == nullptr;
  }

  Terrain* TileBlock::getTerrain(ISurface::Direction facing) {
    return facing == ISurface::Direction::UP ? cTop.cTerrain : cBottom.cTerrain;
  }

  void TileBlock::debug() {
    for (int i = -2; i < 8; i++) {
      if (i > cBottom.cHeight && i <= cTop.cHeight) {
        std::cout << "[x]";
      } else {
        std::cout << "   ";
      }
    }
    std::cout << std::endl;
  }

  TileBlock::~TileBlock() {
  //  std::cout << "Destroying tile block " << cTop << " to " << cBottom << ": " << (--cInstanceCount) << std::endl;
  }
}

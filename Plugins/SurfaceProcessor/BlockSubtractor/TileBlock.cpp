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
#include "TileBlock.h"

bool TileBlock::isAddition() {
  return cTop > cBottom;
}

TileBlock::TileBlock(ISurfaceProvider* surfaceProvider, int top, int bottom, bool topExtended, bool bottomExtended) {
  cTop = top;
  cBottom = bottom;
  cTopSurfaceProvider = surfaceProvider;
  cBottomSurfaceProvider = surfaceProvider;
  cTopExtended = topExtended;
  cBottomExtended = bottomExtended;
}

TileBlock* TileBlock::split(TileBlock& tileBlock) {
  if (!tileBlock.isAddition() && tileBlock.cTop > cBottom && tileBlock.cBottom < cTop) {
    TileBlock* mLowerTileBlock = new TileBlock(cBottomSurfaceProvider, tileBlock.cTop, cBottom, cBottomExtended, false);
    cBottom = tileBlock.cBottom;
    mLowerTileBlock->cTopSurfaceProvider = tileBlock.cTopSurfaceProvider;
    cBottomSurfaceProvider = tileBlock.cBottomSurfaceProvider;
    cBottomExtended = false;
    return mLowerTileBlock;
  }
  return NULL;
}

bool TileBlock::isHigher(TileBlock& tileBlock) {
  return tileBlock.cTop > cTop || (tileBlock.cTop == cTop && (tileBlock.cTopExtended == cTopExtended || tileBlock.cTopExtended));
}

bool TileBlock::isLower(TileBlock& tileBlock) {
  return tileBlock.cBottom < cBottom || (tileBlock.cBottom == cBottom && (tileBlock.cBottomExtended == cBottomExtended || tileBlock.cBottomExtended));
}

bool TileBlock::merge(TileBlock& tileBlock) {
  if (tileBlock.isAddition()) {
    if (tileBlock.cBottom <= cTop && tileBlock.cTop >= cBottom) {
      if (isLower(tileBlock))  {
        cBottom = tileBlock.cBottom;
        cBottomSurfaceProvider = tileBlock.cBottomSurfaceProvider;
        cBottomExtended = tileBlock.cBottomExtended;
      }
      if (isHigher(tileBlock)) {
        cTop = tileBlock.cTop;
        cTopSurfaceProvider = tileBlock.cTopSurfaceProvider;
        cTopExtended = tileBlock.cTopExtended;
      }
      return true;
    }
  } else {
    if ((tileBlock.cTop < cTop || (tileBlock.cTop == cTop && cTopExtended)) && (tileBlock.cBottom > cBottom || (tileBlock.cBottom == cBottom && cBottomExtended))) {
      if (tileBlock.cTop <= cBottom && tileBlock.cBottom >= cTop) {
        cTop = 0;
        cBottom = 0;
        return true;
      }
      if (tileBlock.cTop > cBottom) {
        cTop = tileBlock.cTop;
        cTopSurfaceProvider = tileBlock.cTopSurfaceProvider;
        cTopExtended = false;
      }
      if (tileBlock.cBottom < cTop) {
        cBottom = tileBlock.cBottom;
        cBottomSurfaceProvider = tileBlock.cBottomSurfaceProvider;
        cBottomExtended = false;
      }
      return true;
    }
  }
  return false;
}

bool TileBlock::subtractAsGhost(TileBlock& tileBlock) {
  if (tileBlock.cTop >= cTop && tileBlock.cBottom <= cTop) {
    cTopSurfaceProvider = NULL;
  }
  if (tileBlock.cBottom <= cBottom && tileBlock.cTop >= cBottom) {
    cBottomSurfaceProvider = NULL;
  }
  return cBottomSurfaceProvider == NULL && cTopSurfaceProvider == NULL;
}

ISurfaceProvider* TileBlock::getTopSurfaceProvider() {
  return cTopSurfaceProvider;
}

ISurfaceProvider* TileBlock::getBottomSurfaceProvider() {
  return cBottomSurfaceProvider;
}

void TileBlock::debug() {
  std::cout << "From " << cBottom << " to " << cTop << ": ";
  for (int i = -2; i < 8; i++) {
    if (i > cBottom && i <= cTop) {
      std::cout << "[x]";
    } else {
      std::cout << "   ";
    }
  }
  std::cout << std::endl;
}

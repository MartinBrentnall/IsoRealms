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
#pragma once

#include <iostream>
#include <memory>

#include "Modules/Equilibria/ISurface.h"

namespace IsoRealms::Equilibria {
  class Terrain;

  class TileBlock {
    private:
    struct Surface {
      Terrain* cTerrain;
      int cHeight;
      bool cExtended;

      Surface(Terrain* terrain, int height, bool extended) :
              cTerrain(terrain),
              cHeight(height),
              cExtended(extended) {
      }

      bool operator>=(const Surface& other) const {
        return cHeight >= other.cHeight;
      }

      bool operator<=(const Surface& other) const {
        return cHeight <= other.cHeight;
      }

      bool operator==(const Surface& other) const {
        return cHeight == other.cHeight;
      }

      bool operator>(const Surface& other) const {
        return !operator<=(other);
      }

      bool operator<(const Surface& other) const {
        return !operator>=(other);
      }

      bool operator!=(const Surface& other) const {
        return !operator==(other);
      }
    };

    static int cInstanceCount;
    Surface cTop;
    Surface cBottom;

    bool isHigher(TileBlock&);
    bool isLower(TileBlock&);

    public:
    TileBlock(Terrain*, int, int, bool, bool);
    TileBlock(const TileBlock&);

    bool isAddition();
    std::unique_ptr<TileBlock> split(TileBlock&);
    bool merge(TileBlock&);
    bool subtractAsGhost(TileBlock&);
    
    Terrain* getTerrain(ISurface::Direction facing);
    
    void debug();
    
    ~TileBlock();
  };
}

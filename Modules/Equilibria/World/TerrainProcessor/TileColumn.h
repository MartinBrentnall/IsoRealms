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

#include <vector>

#include "IsoRealms.h"

#include "TileBlock.h"

namespace IsoRealms::Equilibria {
  class TileColumn {
    private:
    std::vector<std::unique_ptr<TileBlock>> cTileBlocks;
    std::vector<std::unique_ptr<TileBlock>> cGhostTileBlocks;
    std::optional<Condition> cCondition;

    public:
    TileColumn(std::optional<Condition>& condition);
    TileColumn(TileColumn*, std::optional<Condition>& condition);
      
    bool addTileBlock(std::unique_ptr<TileBlock> segment, bool ghost);
    bool isTileVisible(Terrain* terrain, ISurface::Direction facing);
    std::unique_ptr<TileColumn> split(std::optional<Condition>& condition);
    std::optional<Condition>& getCondition();
    void debug();
  };
}

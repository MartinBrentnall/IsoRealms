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

#include "IsoRealms.h"

#include "WallSegment.h"
#include "Modules/Equilibria/World/Object/Terrain/Wall.h"

namespace IsoRealms::Equilibria {
  class WallTemplate final {
    private:
    int cX;
    int cY;
    int cLength = 1;
    Wall::Direction cFacing;
    int cBottomHeightStart;
    int cBottomHeightSlope;
    int cTopHeightStart;
    int cTopHeightSlope;
    std::optional<Condition> cCondition;

    public:
    WallTemplate(int x, int y, WallSegment* segment, std::optional<Condition>& condition, Wall::Direction facing);
    bool unite(WallSegment* segment, std::optional<Condition>& condition);

    int getX();
    int getY();
    Wall::Direction getFaceDirection();
    int getLength();
    int getStartHeight();
    int getEndHeight();
    int getBottomSlope();
    int getTopSlope();
    std::optional<Condition>& getCondition();
  };
}

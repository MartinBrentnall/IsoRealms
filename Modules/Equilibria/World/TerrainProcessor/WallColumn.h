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

#include <optional>
#include <vector>

#include "IsoRealms.h"

#include "WallSegment.h"

namespace IsoRealms::Equilibria {
  class WallColumn {
    private:
    std::vector<std::unique_ptr<WallSegment>> cSegments;
    std::optional<Condition> cCondition;


    public:
    WallColumn();
    WallColumn(WallSegment*, std::optional<Condition>& condition);
    WallColumn(WallColumn*, std::optional<Condition>& condition);
    
    std::vector<std::unique_ptr<WallSegment>>& getSections();
    void applyOverlapping(WallColumn* wallSegment);
    void unite(std::unique_ptr<WallColumn>& wallSegment);
    std::unique_ptr<WallColumn> split(std::optional<Condition>& condition);
    bool isCompatibleWith(std::optional<Condition>& condition);
    std::optional<Condition>& getCondition();
    bool empty();
    void convertToAddition();
    void shaveTop(int, std::optional<Condition>& condition);
    bool isSubtraction();
    void removeHiddenSections(WallColumn* column);
    
    void debug();
  };
}

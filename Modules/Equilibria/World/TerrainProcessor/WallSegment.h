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

namespace IsoRealms::Equilibria {
  class WallSegment final {
    public:

    enum class Relation {
      INSIDE,   /// Tested segment is inside this one.
      COVERING, /// Tested segment covers this one completely.
      HIGHER,   /// Tested segment does not cover the bottom of this one.
      LOWER     /// Tested segment does not cover the top of this one.
    };

    WallSegment(int bottomStart, int bottomEnd, int topStart, int topEnd);
    
    int getBottomStart();
    int getBottomEnd();
    int getTopStart();
    int getTopEnd();
    bool isCovering(WallSegment* segment);
    bool isAddition();
    bool unite(WallSegment* segment);
    Relation getRelationWith(WallSegment* segment);
    bool raiseBottom(WallSegment* segment);
    bool lowerTop(WallSegment* segment);
    void convertToAddition();
    void debug();
    
    private:
    int cBottomStart;
    int cBottomEnd;
    int cTopStart;
    int cTopEnd;

    bool isInside(WallSegment* segment);
    bool isHigher(WallSegment* segment);
    bool isOverlapping(WallSegment* segment);
  };
}

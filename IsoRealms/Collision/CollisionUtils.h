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
#pragma once

#include <cmath>
#include <optional>

#include "IsoRealms/Literals.h"

#include "CollisionData.h"

namespace IsoRealms {
  class CollisionUtils {
    public:
    static void clamp(double, double, double*);
    static double getCrossingPoint(double, double, double);
    static double getCrossingPoint(double, double, double, double);
    static bool contains(float, float, float, float, float, float);
    static bool contains(LiteralVertex&, float, float, float, float, float, float);
    static std::optional<CollisionData> getXCrossingPoint(LiteralVertex&, LiteralVertex&, float);
    static std::optional<CollisionData> getYCrossingPoint(LiteralVertex&, LiteralVertex&, float);
    static std::optional<CollisionData> getZCrossingPoint(LiteralVertex&, LiteralVertex&, float);
    static std::optional<CollisionData> getXCrossingPoint(LiteralVertex&, LiteralVertex&, float, float, float, float, float);
    static std::optional<CollisionData> getYCrossingPoint(LiteralVertex&, LiteralVertex&, float, float, float, float, float);
    static std::optional<CollisionData> getZCrossingPoint(LiteralVertex&, LiteralVertex&, float, float, float, float, float);
    static std::optional<CollisionData> getCrossingPoint(LiteralVertex&, LiteralVertex&, float, float, float, float, float, float, bool);
    static bool hasCrossed(LiteralVertex&, LiteralVertex&, float, float, float, float, float, float, bool);
    static std::optional<CollisionData> getEntryPoint(LiteralVertex&, LiteralVertex&, float, float, float, float, float, float);
    static std::optional<CollisionData> getExitPoint(LiteralVertex&, LiteralVertex&, float, float, float, float, float, float);
  };
}

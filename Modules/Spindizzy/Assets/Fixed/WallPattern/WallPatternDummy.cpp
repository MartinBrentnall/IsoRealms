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

#include "Modules/Spindizzy/World/Object/Terrain/Wall.h"

#include "WallPatternDummy.h"

namespace IsoRealms::Spindizzy {
  std::vector<std::unique_ptr<IVisualElement>> WallPatternDummy::getStaticVisuals(Wall* wall) const {
    std::vector<std::unique_ptr<IVisualElement>> mVisuals;
    return mVisuals;
  }

  void WallPatternDummy::render(float x, float y, float z, float length, float height, float topSlope, float bottomSlope, Wall::Direction facing) const {
    // Nothing to do.
  }

  bool WallPatternDummy::contains(ITexture* texture) {
    return false;
  }

  void WallPatternDummy::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    // Nothing to do.
  }

  void WallPatternDummy::hintInUse(bool inUse) {
    // Nothing to do.
  }
}

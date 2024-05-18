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

#include "SurfacePatternDummy.h"

namespace IsoRealms::Spindizzy {
  bool SurfacePatternDummy::contains(ITexture* texture) {
    return false;
  }

  std::vector<std::unique_ptr<IVisualElement>> SurfacePatternDummy::getStaticVisuals(Surface* surface) {
    std::vector<std::unique_ptr<IVisualElement>> mVisuals;
    return mVisuals;
  }

  std::vector<std::unique_ptr<IVisualElement>> SurfacePatternDummy::getStaticVisuals(SplitSurface* surface) {
    std::vector<std::unique_ptr<IVisualElement>> mVisuals;
    return mVisuals;
  }

  void SurfacePatternDummy::render(float startX, float endX, float startY, float endY, float z, float xSlope, float ySlope, ISurface::Direction facing) const {
    // Nothing to do.
  }

  void SurfacePatternDummy::render(float x, float y, float z, float heightSW, float heightSE, float heightNW, float heightNE, bool alternativeSplit) const {
    // Nothing to do.
  }

  void SurfacePatternDummy::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SurfacePatternDummy::renderAssetIcon() const {
    return false;
  }

  void SurfacePatternDummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }
}

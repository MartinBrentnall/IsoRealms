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

#include "Modules/Equilibria/ISurface.h"

namespace IsoRealms::Equilibria {
  class SplitSurface;
  class Surface;

  class ISurfacePattern : public IResource {
    public:
    virtual bool contains(ITexture* texture) = 0;
    virtual std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(Surface* surface) = 0;
    virtual std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(SplitSurface* surface) = 0;
    virtual void render(float startX, float endX, float startY, float endY, float z, float xSlope, float ySlope, ISurface::Direction facing) const = 0;
    virtual void render(float x, float y, float z, float heightSW, float heightSE, float heightNW, float heighNE, bool alternativeSplit) const = 0;
    virtual void hintInUse(bool inUse) = 0;

    virtual ~ISurfacePattern() {
    }
  };
}

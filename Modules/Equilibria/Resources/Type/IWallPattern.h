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

#include "Modules/Equilibria/World/Object/Terrain/Wall.h"

namespace IsoRealms::Equilibria {
  class Wall;

  class IWallPattern : public IResource {
    public:
    virtual bool contains(ITexture* texture) = 0;
    virtual std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(Wall* surface) const = 0;
    virtual void render(float x, float y, float z, float length, float height, float topSlope, float bottomSlope, Wall::Direction facing) const = 0;
    virtual void hintInUse(bool inUse) = 0;

    virtual ~IWallPattern() {
    }
  };
}

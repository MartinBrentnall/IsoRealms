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

#include "IsoRealms/Common/IVisualElement.h"
#include "IsoRealms/IAssetBrowser.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/World/Object/Terrain/Wall.h"

namespace IsoRealms::Spindizzy {
  class Wall;

  class IWallPattern {
    public:
    virtual bool contains(ITexture* texture) = 0;
    virtual void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const = 0;
    virtual std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(Wall* surface) const = 0;
    virtual void render(float x, float y, float z, float length, float height, float topSlope, float bottomSlope, Wall::Direction facing) const = 0;
    virtual void hintInUse(bool inUse) = 0;

    virtual ~IWallPattern() {
    }
  };
}

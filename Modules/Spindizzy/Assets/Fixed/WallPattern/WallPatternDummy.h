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

#include <GL/glew.h>

#include "IsoRealms/IAssets.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Assets/Type/IWallPattern.h"

namespace IsoRealms::Spindizzy {
  class WallPatternDummy : public IWallPattern {
    public:
    
    /***************************\
     * Implements IWallPattern *
    \***************************/
    bool contains(ITexture* texture) override;
    std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(Wall* wall) const override;
    void render(float x, float y, float z, float length, float height, float topSlope, float bottomSlope, Wall::Direction facing) const override;
    void hintInUse(bool inUse) override;
    
    /**************************************\
     * Implements IAsset via IWallPattern *
    \**************************************/
    bool renderAssetIcon() const override;
    void saveAsset(DOMNodeWriter* node) const override;
  };
}

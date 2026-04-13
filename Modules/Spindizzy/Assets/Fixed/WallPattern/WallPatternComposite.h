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

#include <GL/glew.h>
 
#include "IsoRealms.h"
 
#include "Modules/Spindizzy/Assets/Client/WallPattern.h"
#include "Modules/Spindizzy/Assets/Type/IWallPattern.h"
 
namespace IsoRealms::Spindizzy {
  class TerrainType;
 
  class WallPatternComposite : public IWallPattern {
    public:
    WallPatternComposite(const Metadata& metadata, TerrainType& owner);
    WallPatternComposite(const Metadata& metadata, TerrainType& owner, JSONObject object);
 
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
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    inline static const std::string JSON_PATTERN  = "pattern";
    inline static const std::string JSON_PATTERNS = "patterns";

    // External interfaces.
    TerrainType& cOwner;
    const Metadata& cMetadata;

    // Definition data.
    std::vector<std::unique_ptr<WallPattern>> cDefWallPatterns;
  };
}
 
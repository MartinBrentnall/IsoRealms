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

#include <cmath>

#include "IsoRealms.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  /**
   * Resource definition for a model that can be used to represent a chunk
   * of debris from a broken craft or other object.  Specifically, it is a
   * tetrahedron with configurable colours for the outline and for each of its
   * four faces.
   */
  class DebrisChunk final : public IModel,
                            public IModelInstance {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    DebrisChunk(Spindizzy& spindizzy, IResourceData& data);
    DebrisChunk(Spindizzy& spindizzy, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    /*********************\
     * Module interfaces *
    \*********************/
    void updateEditing(unsigned int milliseconds);

    /*********************\
     * Implements IModel *
    \*********************/
    IModelInstance* createModel() override;
    bool renderPreview() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    /*****************************\
     * Implements IModelInstance *
    \*****************************/
    void update(unsigned int milliseconds) override;
    void render() const override;

    private:

    // JSON members.
    inline static const std::string JSON_OUTLINE       = "outline";
    inline static const std::string JSON_OUTLINE_WIDTH = "outlineWidth";
    inline static const std::string JSON_SIDE_1        = "side1";
    inline static const std::string JSON_SIDE_2        = "side2";
    inline static const std::string JSON_SIDE_3        = "side3";
    inline static const std::string JSON_SIDE_4        = "side4";

    // Default constants.
    inline static const float DEFAULT_OUTLINE_WIDTH = 0.18f;

    Project& cProject;

    // Definition data.
    Colour cDefSide[4];               /// Colours used for each side of this debris chunk.
    Colour cDefOutline;               /// Colour used for the outline of this debris chunk.
    float cDefOutlineWidth;           /// Width of the outline drawn on each side of this debris chunk.

    // Runtime data.
    LiteralTexture cTextures[4];          /// Textures applied to each side of this debris chunk.
    bool cNeedsRedrawing;

    // Editing data.
    float cEditingIconRotation; /// Animation value for rotation of the model icon.

    // Internal functions.
    void regenerateTextures();
    void generateTexture(Colour& colour);
    void setNeedsRedrawing();
  };
}

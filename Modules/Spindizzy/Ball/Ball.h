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
   * Resource definition for a ball craft texture, with configurable colours.
   */
  class Ball final : public ITexture {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    Ball(Spindizzy& spindizzy, IResourceData& data);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    /***********************\
     * Implements ITexture *
    \***********************/
    void set() const override;
    void hintTextureInUse(bool) override;
    ITexture* getTexture() override;
    void coord(float x, float y) const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    inline static const std::string JSON_FILL    = "fill";
    inline static const std::string JSON_OUTLINE = "outline";
    inline static const std::string JSON_SHINE   = "shine";

    // Resource definition constants.
    inline static const float CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);

    Project& cProject;

    // Definition data.
    Colour cDefFill;            /// Colour used for the fill of the ball.
    Colour cDefOutline;         /// Colour used for the outline of the ball.
    Colour cDefShine;           /// Colour used for the shine of the ball.

    // Runtime data.
    LiteralTexture cTexture; /// The actual texture.
    bool cNeedsRedrawing;

    // Internal Functions.
    void updateTexture();
    void renderCircle(float, Colour& colour);
    void setNeedsRedrawing();
  };
}

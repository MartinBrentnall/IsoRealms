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

#include "IsoRealms/Literals.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

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
    Ball(IProject* project, Spindizzy* spindizzy);
    Ball(IProject* project, Spindizzy* spindizzy, JSONObject object, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(JSONObject object, IAssetIdentifier* identifier) const;
    void hintInUse(bool);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /***********************\
     * Implements ITexture *
    \***********************/
    void set() const override;
    void hintTextureInUse(bool) override;
    ITexture* getTexture() override;
    void coord(float x, float y) const override;
    void saveAsset(JSONObject object) const override;

    private:

    // JSON members.
    static const std::string JSON_FILL;
    static const std::string JSON_OUTLINE;
    static const std::string JSON_SHINE;

    // Resource definition constants.
    static const float CIRCLE_RESOLUTION;

    IProject* cProject;

    // Definition data.
    LiteralTexture cDefTexture; /// The actual texture.
    Colour cDefFill;            /// Colour used for the fill of the ball.
    Colour cDefOutline;         /// Colour used for the outline of the ball.
    Colour cDefShine;           /// Colour used for the shine of the ball.

    // Runtime data.
    bool cNeedsRedrawing;

    // Internal Functions.
    void updateTexture();
    void renderCircle(float, Colour& colour);
    void setNeedsRedrawing();
  };
}

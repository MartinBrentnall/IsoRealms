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
#include <queue>

#include "IsoRealms.h"

namespace IsoRealms::UI {
  class ScreenGradient final : public IScreen {
    public:

    // Constructors.
    ScreenGradient(const Metadata& metadata, IComponentData& owner);
    ScreenGradient(const Metadata& metadata, IComponentData& owner, JSONObject object);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;

    /*********************************\
     * Implements IAsset via IScreen *
    \*********************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    inline static const std::string JSON_COLOUR_A    = "colourA";
    inline static const std::string JSON_COLOUR_B    = "colourB";
    inline static const std::string JSON_ORIENTATION = "orientation";

    inline static const std::string VALUE_HORIZONTAL = "Horizontal";
    inline static const std::string VALUE_VERTICAL   = "Vertical";

    // Definition data.
    Colour cDefColourA;
    Colour cDefColourB;
    bool cDefVertical = false;
  };
}


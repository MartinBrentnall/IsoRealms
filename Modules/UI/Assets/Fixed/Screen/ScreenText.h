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
  class ScreenText final : public IScreen {
    public:
    
    // Constructors.
    ScreenText(IProject& project, IResourceData& owner);
    ScreenText(IProject& project, IResourceData& owner, JSONObject object);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;

    /*********************************\
     * Implements IAsset via IScreen *
    \*********************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.
    static const std::string JSON_ALIGNMENT;
    static const std::string JSON_COLOUR;
    static const std::string JSON_FONT;
    static const std::string JSON_SHADOW_OFFSET;
    static const std::string JSON_VALUE;

    static const std::string ALIGNMENT_CENTER;
    static const std::string ALIGNMENT_LEFT;
    static const std::string ALIGNMENT_RIGHT;

    // Constants.
    static const float DEFAULT_SHADOW_OFFSET;

    // External interfaces.
    IProject& cProject;

    // Definition data.
    String cDefString;              /// String to be shown.
    Font cDefFont;                  /// Font used to render the string.
    Colour cDefColour;              /// The colour to use.
    IFont::Alignment cDefAlignment; /// Alignment to the screen.
    float cDefShadowOffset;         /// Shadow offset value.
    
    std::string getAlignment() const;
    void setAlignment(const std::string& alignment);
  };
}

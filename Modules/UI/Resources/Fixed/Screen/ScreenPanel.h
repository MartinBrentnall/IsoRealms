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
  class ScreenPanel final : public IScreen {
    public:
    
    // Constructors.
    ScreenPanel(const Metadata& metadata, IComponentData& owner);
    ScreenPanel(const Metadata& metadata, IComponentData& owner, JSONObject object);

    /**********************\
     * Implements IScreen *
    \**********************/
    void getResourceProperties(IComponentDefiner& definer) override;
    bool isDefaultConfiguration() const override;
    void renderScreen(float scale, float aspectRatio) const override;

    private:    

    // Constants.
    inline static const float CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);
    
    // JSON members.
    inline static const std::string JSON_COLOUR      = "colour";
    inline static const std::string JSON_CORNER_SIZE = "cornerSize";

    // Global data.
    static std::unique_ptr<LiteralTexture> cGlobalCornerTexture; // Texture applied to the corners of panels.
    static unsigned int cGlobalInstanceCount;                    // Counts panel instances.

    // Global private functions.
    static void initTextures(Project& project);
    static void renderCorner(float xStart, float xEnd, bool xFlip, float yStart, float yEnd, bool yFlip);
    static void renderRectangle(float xStart, float xEnd, float yStart, float yEnd);

    // Definition data.
    Colour cDefColour;           /// Colour of the panel.
    float cDefCornerSize = 0.0f; /// Size of the panel corner.
  };
}

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
#include <queue>

#include "IsoRealms/Literals.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::UI {
  class ScreenPanel final : public IScreen {
    public:
    
    // Constructors.
    ScreenPanel(IProject& project, Project& owner);
    ScreenPanel(IProject& project, Project& owner, JSONObject object);

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
    

    // Constants.
    static const float CIRCLE_RESOLUTION;
    
    // JSON members.
    static const std::string JSON_COLOUR;
    static const std::string JSON_CORNER_SIZE;

    // Global data.
    static std::unique_ptr<LiteralTexture> cGlobalCornerTexture; // Texture applied to the corners of panels.
    static unsigned int cGlobalInstanceCount;                    // Counts panel instances.

    // Global private functions.
    static void initTextures(IProject& project);
    static void renderCorner(float xStart, float xEnd, bool xFlip, float yStart, float yEnd, bool yFlip);
    static void renderRectangle(float xStart, float xEnd, float yStart, float yEnd);
    
    // Definition data.
    Colour cDefColour;    /// Colour of the panel.
    float cDefCornerSize; /// Size of the panel corner.
  };
}

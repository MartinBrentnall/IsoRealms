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

#include "IsoRealms/Literals.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::UI {
  class UI;

  /**
   * Resource definition for a screen that paints a rectangular panel using a
   * configured colour.  The panel can optionally have rounded corners.
   */
  class Panel final : public IScreen {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    Panel(IProject* project, UI* ui);
    Panel(IProject* project, UI* ui, JSONObject object, IOptions* options, IResourceData* data);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void registerAssets(IAssetRegistry* assets);
    void save(JSONObject object, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;

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
    static void initTextures(IProject* project);
    static void renderCorner(float xStart, float xEnd, bool xFlip, float yStart, float yEnd, bool yFlip);
    static void renderRectangle(float xStart, float xEnd, float yStart, float yEnd);
    
    // Definition data.
    Colour cDefColour;    /// Colour of the panel.
    float cDefCornerSize; /// Size of the panel corner.
  };
}

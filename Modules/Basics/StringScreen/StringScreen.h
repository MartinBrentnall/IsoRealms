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

namespace IsoRealms::Basics {
  class Basics;
  
  /**
   * Resource definition to configure a string to be used as a screen.
   */
  class StringScreen final : public IScreen {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    StringScreen(IProject* project, Basics* basics);
    StringScreen(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
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

    // Definition data.
    String cDefString;              /// String to be shown.
    Font cDefFont;                  /// Font used to render the string.
    Colour cDefColour;              /// The colour to use.
    IFont::Alignment cDefAlignment; /// Alignment to the screen.
    float cDefShadowOffset;         /// Shadow offset value.
  };
}

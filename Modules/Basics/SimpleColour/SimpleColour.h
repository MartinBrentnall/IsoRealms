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
#include <GL/glew.h>
#include <iostream>

#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a simple Colour variable.
   */
  class SimpleColour final : public IColour {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    SimpleColour(IProject* project, Basics* basics);
    SimpleColour(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(JSONObject object, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /**********************\
     * Implements IColour *
    \**********************/
    void set() const override;
    float getRed() const override;
    float getGreen() const override;
    float getBlue() const override;
    float getAlpha() const override;
    void saveAsset(JSONObject object) const override;

    /***********************\
     * Scripting Interface *
    \***********************/
    void setRed(float value);
    void setGreen(float value);
    void setBlue(float value);
    void setAlpha(float value);

    private:

    // JSON members.
    static const std::string JSON_RED;
    static const std::string JSON_GREEN;
    static const std::string JSON_BLUE;
    static const std::string JSON_ALPHA;

    // Property names.
    static const std::string PROPERTY_RED;
    static const std::string PROPERTY_GREEN;
    static const std::string PROPERTY_BLUE;
    static const std::string PROPERTY_ALPHA;
    static const std::string PROPERTY_HUE;
    static const std::string PROPERTY_SATURATION;
    static const std::string PROPERTY_LIGHTNESS;

    // Definition data.
    float cDefRed;   /// Initial red intensity.
    float cDefGreen; /// Initial green intensity.
    float cDefBlue;  /// Initial blue intensity.
    float cDefAlpha; /// Initial alpha intensity.

    // Runtime data.
    float cRuntimeRed;   /// Current red intensity.
    float cRuntimeGreen; /// Current green intensity.
    float cRuntimeBlue;  /// Current blue intensity.
    float cRuntimeAlpha; /// Current alpha intensity.

    // Editing data.
    float cEditingLastKnownHue;        /// Last known hue value.
    float cEditingLastKnownSaturation; /// Last known saturation value.
    float cEditingLastKnownLightness;  /// Last known lightness value.

    // Scripting Interface.
    LuaBinding<SimpleColour> cLuaBinding;

    // Misc.
    IStateNotifier<IColour>* cStateNotifier;
  };
}

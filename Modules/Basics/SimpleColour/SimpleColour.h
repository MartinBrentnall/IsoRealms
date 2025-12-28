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
#include <GL/glew.h>
#include <iostream>

#include "IsoRealms.h"

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
    SimpleColour(Basics& basics, IResourceData& data);
    SimpleColour(Basics& basics, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(PropertyMaker& owner, const Metadata& metadata);
    void removed();

    /*********************\
     * Module interfaces *
    \*********************/
    void reset();

    /**********************\
     * Implements IColour *
    \**********************/
    void set() const override;
    float getRed() const override;
    float getGreen() const override;
    float getBlue() const override;
    float getAlpha() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

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
    IStateNotifier* cStateNotifier;

    // Private functions.
    void resetColour();
  };
}

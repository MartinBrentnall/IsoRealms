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
   * Component definition for a simple Colour variable.
   */
  class SimpleColour final : public IColour {
    public:

    /***********************\
     * Component Interface *
    \***********************/
    SimpleColour(Basics& basics, IComponentData& data);
    void define(IComponentDefiner& definer);
    void publish(ResourcePublisher& publisher);
    bool renderIcon() const;

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

    /***********************\
     * Scripting Interface *
    \***********************/
    void setRed(float value);
    void setGreen(float value);
    void setBlue(float value);
    void setAlpha(float value);

    private:
    
    // Definition data.
    float cDefRed   = 0.0f; /// Initial red intensity.
    float cDefGreen = 0.0f; /// Initial green intensity.
    float cDefBlue  = 0.0f; /// Initial blue intensity.
    float cDefAlpha = 0.0f; /// Initial alpha intensity.

    // Runtime data.
    float cRuntimeRed   = 0.0f; /// Current red intensity.
    float cRuntimeGreen = 0.0f; /// Current green intensity.
    float cRuntimeBlue  = 0.0f; /// Current blue intensity.
    float cRuntimeAlpha = 0.0f; /// Current alpha intensity.

    // Editing data.
    float cEditingLastKnownHue;        /// Last known hue value.
    float cEditingLastKnownSaturation; /// Last known saturation value.
    float cEditingLastKnownLightness;  /// Last known lightness value.

    // Scripting Interface.
    LuaBinding<SimpleColour> cLuaBinding;

    // Misc.
    IStateNotifier* cStateNotifier = nullptr;

    // Private functions.
    void resetColour();
  };
}

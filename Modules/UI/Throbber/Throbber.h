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

#include <GL/glew.h>
#include <GL/gl.h>
#include <cmath>
#include <sstream>
#include <vector>

#include "IsoRealms.h"

namespace IsoRealms::UI {
  class UI;

  /**
   * Component definition for a visual indication that the application is busy,
   * known as a throbber.  This throbber shows an animated ring of spots, where
   * several aspects of its appearance are configurable.
   */
  class Throbber final : public IScreen {
    public:

    /***********************\
     * Component Interface *
    \***********************/
    Throbber(UI& ui, IComponentData& data);
    void registerAssets(ComponentAssetRegistry& assets);
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);

    /*********************\
     * Module interfaces *
    \*********************/
    void updateRuntime(unsigned int milliseconds);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    
    // Default values.
    inline static const unsigned int DEFAULT_DURATION      = 2000U;
    inline static const unsigned int DEFAULT_REPETITIONS   = 1U;
    inline static const float        DEFAULT_RING_RADIUS   = 0.2f;
    inline static const float        DEFAULT_SHADOW_OFFSET = 0.008f;
    inline static const float        DEFAULT_SPOT_RADIUS   = 0.06f;
    inline static const unsigned int DEFAULT_SPOT_SIDES    = 32U;
    inline static const unsigned int DEFAULT_SPOTS         = 8U;

    // Definition data.
    unsigned int cDefDuration    = DEFAULT_DURATION;      /// Duration of the animation loop in milliseconds.
    unsigned int cDefRepetitions = DEFAULT_REPETITIONS;   /// Number of animation repetitions along the spots.
    unsigned int cDefSpots       = DEFAULT_SPOTS;         /// Number of spots to render.
    unsigned int cDefSpotSides   = DEFAULT_SPOT_SIDES;    /// Number of sides of each spot.
    float cDefSpotRadius         = DEFAULT_SPOT_RADIUS;   /// Radius of spots at their largest.
    float cDefRingRadius         = DEFAULT_RING_RADIUS;   /// Radius of ring of spots from the center of the screen.
    float cDefShadowOffset       = DEFAULT_SHADOW_OFFSET; /// Offset for the shadow of the ring.
    Colour cDefColour;                                    /// Colour of the throbber.

    // Runtime data
    unsigned int cRuntimeAnimation; /// Animation value for the throbber.

    // Private functions.
    void render(float xOffset, float yOffset) const;
    float getRadius(unsigned int spot) const;
  };
}

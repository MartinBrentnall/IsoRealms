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

#include <GL/glew.h>
#include <GL/gl.h>
#include <cmath>
#include <sstream>
#include <vector>

#include "IsoRealms/Editing.h"
#include "IsoRealms/IApplication.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::UI {
  class UI;

  /**
   * Resource definition for a visual indication that the application is busy,
   * known as a throbber.  This throbber shows an animated ring of spots, where
   * several aspects of its appearance are configurable.
   */
  class Throbber final : public IScreen {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    Throbber(IProject& project, UI& ui, IResourceData& data);
    Throbber(IProject& project, UI& ui, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IResourceData& owner);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    static const std::string JSON_COLOUR;
    static const std::string JSON_DURATION;
    static const std::string JSON_REPETITIONS;
    static const std::string JSON_RING_RADIUS;
    static const std::string JSON_SHADOW_OFFSET;
    static const std::string JSON_SPOT_RADIUS;
    static const std::string JSON_SPOT_SIDES;
    static const std::string JSON_SPOTS;

    // Default values.
    static const unsigned int DEFAULT_DURATION;
    static const unsigned int DEFAULT_REPETITIONS;
    static const float        DEFAULT_RING_RADIUS;
    static const float        DEFAULT_SHADOW_OFFSET;
    static const float        DEFAULT_SPOT_RADIUS;
    static const unsigned int DEFAULT_SPOT_SIDES;
    static const unsigned int DEFAULT_SPOTS;

    // External interfaces.
    ProjectCallbackManager cProjectCallbackManager;

    // Definition data.
    unsigned int cDefDuration;    /// Duration of the animation loop in milliseconds.
    unsigned int cDefRepetitions; /// Number of animation repetitions along the spots.
    unsigned int cDefSpots;       /// Number of spots to render.
    unsigned int cDefSpotSides;   /// Number of sides of each spot.
    float cDefSpotRadius;         /// Radius of spots at their largest.
    float cDefRingRadius;         /// Radius of ring of spots from the center of the screen.
    float cDefShadowOffset;       /// Offset for the shadow of the ring.
    Colour cDefColour;            /// Colour of the throbber.

    // Runtime data
    unsigned int cRuntimeAnimation; /// Animation value for the throbber.

    // Private functions.
    void render(float xOffset, float yOffset) const;
    float getRadius(unsigned int spot) const;
  };
}

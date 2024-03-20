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

#include "IsoRealms/Literals.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  /**
   * Resource definition for a gyroscope craft model, where the colour of each
   * quadrant and the outline is configurable.
   */
  class Gyroscope final : public I3DModelType,
                          public I3DModel {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    Gyroscope(IProject* project, Spindizzy* spindizzy);
    Gyroscope(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /***************************\
     * Implements I3DModelType *
    \***************************/
    I3DModel* createModel() override;
    bool renderPreview() const override;
    bool renderAssetIcon() const override;

    /***********************\
     * Implements I3DModel *
    \***********************/
    void update(unsigned int milliseconds) override;
    void render() const override;

    private:

    // DOM strings.
    static const std::string TAG_COLOUR_1;
    static const std::string TAG_COLOUR_2;
    static const std::string TAG_COLOUR_3;
    static const std::string TAG_COLOUR_4;
    static const std::string TAG_OUTLINE;

    // Resource definition constants.
    static const float CIRCLE_RESOLUTION; /// Degrees for each segment of a rendered circle.
    static const float SPINDLE_WIDTH;
    static const float WIDTH;
    static const float HEIGHT;

    IProject* cProject;

    // Definition data.
    Colour cDefQuadrant[4];     /// Colours of the four quadrants.
    Colour cDefOutline;         /// Colour of the outline and the spindle.
    LiteralTexture cDefTexture; /// Actual texture for the circle section.

    // Editing data.
    float cEditingIconRotation;

    /**
     * Update the circle section texture.  This is used when a colour changes.
     */
    void updateTexture();

    /**
     * Renders a filled circle segment (like a pizza slice) to the circle section
     * texture.  This function is used to render the quarters of the circle
     * section.
     *
     * @param radius Outer edge radius, where 1.0f is the edge of the texture.
     * @param startAngle Starting angle in degrees.
     * @param endAngle Finishing angle in degrees.
     * @param colour Colour to render.
     */
    void renderCircle(float radius, float startAngle, float endAngle, Colour& colour);

    /**
     * Renders a circular line to the circle section texture.  This function is
     * used to render the outer edge of the circle section.
     *
     * @param outerRadius Outer edge radius, where 1.0f is the edge of the texture.
     * @param innerRadius Inner edge radius, where 1.0f is the edge of the texture.
     * @param colour Colour to render.
     */
    void renderCircle(float outerRadius, float innerRadius, Colour& colour);
  };
}

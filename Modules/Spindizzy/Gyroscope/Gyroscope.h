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

#include "IsoRealms.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  /**
   * Resource definition for a gyroscope craft model, where the colour of each
   * quadrant and the outline is configurable.
   */
  class Gyroscope final : public IModel,
                          public IModelInstance {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    Gyroscope(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    Gyroscope(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(PropertyMaker& owner, const Metadata& metadata);

    /*********************\
     * Module interfaces *
    \*********************/
    void updateEditing(unsigned int milliseconds);

    /*********************\
     * Implements IModel *
    \*********************/
    IModelInstance* createModel() override;
    bool renderPreview() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    /*****************************\
     * Implements IModelInstance *
    \*****************************/
    void update(unsigned int milliseconds) override;
    void render() const override;

    private:

    // JSON members.
    static const std::string JSON_COLOUR_1;
    static const std::string JSON_COLOUR_2;
    static const std::string JSON_COLOUR_3;
    static const std::string JSON_COLOUR_4;
    static const std::string JSON_OUTLINE;

    // Resource definition constants.
    static const float CIRCLE_RESOLUTION; /// Degrees for each segment of a rendered circle.
    static const float SPINDLE_WIDTH;
    static const float WIDTH;
    static const float HEIGHT;

    // External interfaces.
    IProject& cProject;

    // Definition data.
    Colour cDefQuadrant[4];     /// Colours of the four quadrants.
    Colour cDefOutline;         /// Colour of the outline and the spindle.

    // Runtime data.
    LiteralTexture cTexture;        /// Actual texture for the circle section.
    bool cNeedsRedrawing;

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

    void setNeedsRedrawing();
  };
}

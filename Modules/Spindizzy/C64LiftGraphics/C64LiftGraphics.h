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
#include <map>

#include "IsoRealms.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  /**
   * Resource definition for textures to be used as lifts.  Generates nice
   * textures in total for twelve lift types (three textures are intended to
   * be rotated 180 degrees for an additional three).  The colours used for the
   * nine textures are configurable.
   */
  class C64LiftGraphics final {
    public:
    
    /**********************\
     * Resource interface *
    \**********************/
    C64LiftGraphics(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    C64LiftGraphics(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(ResourceAssetRegistry& assets);  
    void save(JSONObject object) const;
    bool renderIcon();
    void hintInUse(bool inUse);
    void getProperties(PropertyMaker& owner);

    private:
    
    // Constants for rendering.
    static const float CIRCLE_RESOLUTION;
    static const float CIRCLE_OUTLINE_OUTER;
    static const float CIRCLE_OUTLINE_INNER;
    static const float CIRCLE_COLOUR_OUTER;
    static const float CIRCLE_COLOUR_INNER;
    static const float SQUARE_OUTLINE_OUTER;
    static const float SQUARE_OUTLINE_INNER;
    static const float SQUARE_COLOUR_OUTER;
    static const float SQUARE_COLOUR_INNER;
    static const float SQUARE_TRIANGLE_OUTER;
    static const float SQUARE_TRIANGLE_INNER;
    static const float DIAMOND_OUTLINE_OUTER;
    static const float DIAMOND_OUTLINE_INNER;
    static const float DIAMOND_COLOUR_OUTER;
    static const float DIAMOND_COLOUR_INNER;
    static const float DIAMOND_SQUARE_OUTLINE_OUTER;
    static const float DIAMOND_SQUARE_OUTLINE_INNER;
    static const float DIAMOND_SQUARE_COLOUR_OUTER;
    static const float DIAMOND_SQUARE_COLOUR_INNER;

    // Texture asset ID's.
    static const std::string CIRCLE_NONE;
    static const std::string CIRCLE_HALF;
    static const std::string CIRCLE_BOTH;
    static const std::string SQUARE_NONE;
    static const std::string SQUARE_HALF;
    static const std::string SQUARE_BOTH;
    static const std::string DIAMOND_NONE;
    static const std::string DIAMOND_HALF;
    static const std::string DIAMOND_BOTH;
    
    // JSON members.
    static const std::string JSON_OUTLINE;
    static const std::string JSON_PRIMARY;
    static const std::string JSON_SECONDARY;

    IProject& cProject;

    // Definition data.
    Colour cDefPrimary;                                                  /// Primary colour.
    Colour cDefSecondary;                                                /// Secondary colour.
    Colour cDefOutline;                                                  /// Outline colour.

    // Runtime data.
    std::map<std::string, std::unique_ptr<LiteralTexture>> cTextures; /// The actual texture assets.
    bool cNeedsRedrawing;
    
    // Private functions.
    void generateTextures();
    void clear();
    void renderCircle(float radius, Colour& colour);
    void renderCircularRing(float outer, float inner, Colour& colour);
    void renderSquareRing(float outer, float inner, Colour& colour);
    void renderSquareRingHalf(float outer, float inner, Colour& colour);
    void renderDiamondRing(float outer, float inner, Colour& colour);
    void renderDiamondRingHalf(float outer, float inner, Colour& colour);
    void renderDiamondRingEdges(float outer, float inner, Colour& colour);
    void renderLiftCircle();
    void renderLiftCircleHalf();
    void renderLiftCircleBoth();
    void renderLiftSquare();
    void renderLiftSquareHalf();
    void renderLiftSquareBoth();
    void renderLiftDiamond();
    void renderLiftDiamondHalf();
    void renderLiftDiamondBoth();
    void setNeedsRedrawing();
  };
}

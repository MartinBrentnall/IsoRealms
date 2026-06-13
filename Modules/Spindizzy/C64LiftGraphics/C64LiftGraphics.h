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
    C64LiftGraphics(Spindizzy& spindizzy, IResourceData& data);
    void registerAssets(ResourceAssetRegistry& assets);  
    bool renderIcon();
    void hintInUse(bool inUse);
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    private:
    
    // Constants for rendering.
    inline static const float CIRCLE_RESOLUTION            = 5.0f * (M_PI / 180.0);
    inline static const float CIRCLE_OUTLINE_OUTER         = 1.0f;
    inline static const float CIRCLE_OUTLINE_INNER         = 0.6f;
    inline static const float CIRCLE_COLOUR_OUTER          = 0.9f;
    inline static const float CIRCLE_COLOUR_INNER          = 0.7f;
    inline static const float SQUARE_OUTLINE_OUTER         = 1.0f;
    inline static const float SQUARE_OUTLINE_INNER         = 0.51f;
    inline static const float SQUARE_COLOUR_OUTER          = 0.87f;
    inline static const float SQUARE_COLOUR_INNER          = 0.64f;
    inline static const float SQUARE_TRIANGLE_OUTER        = 0.2f;
    inline static const float SQUARE_TRIANGLE_INNER        = 0.0f;
    inline static const float DIAMOND_OUTLINE_OUTER        = 1.0f;
    inline static const float DIAMOND_OUTLINE_INNER        = 0.6f;
    inline static const float DIAMOND_COLOUR_OUTER         = 0.9f;
    inline static const float DIAMOND_COLOUR_INNER         = 0.7f;
    inline static const float DIAMOND_SQUARE_OUTLINE_OUTER = 0.70f;
    inline static const float DIAMOND_SQUARE_OUTLINE_INNER = 0.40f;
    inline static const float DIAMOND_SQUARE_COLOUR_OUTER  = 0.62f;
    inline static const float DIAMOND_SQUARE_COLOUR_INNER  = 0.48f;

    // Texture asset ID's.
    inline static const std::string ASSET_ID_ALWAYS_ON      = "Always On";
    inline static const std::string ASSET_ID_STATE_1_AND_2  = "State 1 and 2";
    inline static const std::string ASSET_ID_STATE_3        = "State 3";
    inline static const std::string ASSET_ID_STATE_4        = "State 4";
    inline static const std::string ASSET_ID_STATE_5_AND_6  = "State 5 and 6";
    inline static const std::string ASSET_ID_STATE_7        = "State 7";
    inline static const std::string ASSET_ID_STATE_8        = "State 8";
    inline static const std::string ASSET_ID_STATE_9_AND_10 = "State 9 and 10";
    inline static const std::string ASSET_ID_STATE_11       = "State 11";
    
    //  External interfaces.
    Project& cProject;

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

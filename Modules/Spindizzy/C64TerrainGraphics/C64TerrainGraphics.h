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

  class C64TerrainGraphics : public ITextureUseListener,
                             public IScreenListener {
    public:
    C64TerrainGraphics(Spindizzy& spindizzy, IComponentData& data);
    void define(IComponentDefiner& definer);
    void publish(ResourcePublisher& publisher);
    void hintInUse(bool inUse);
    bool renderIcon();

    /**********************************\
     * Implements ITextureUseListener *
    \**********************************/
    void hintTextureUsed(ITexture* texture, bool inUse) override;

    /**************************************\
     * Implements IStateListener<IFloat*> *
    \**************************************/
    void stateChanged(const IFloat* resource);

    /******************************\
     * Implements IScreenListener *
    \******************************/
    void screenAdded(const IScreen* screen) override;
    void screenRemoved(const IScreen* screen) override;
    void screenPreRender(const IScreen* screen) override;
    void screenPostRender(const IScreen* screen) override;

    private:
    inline static const float TILE_SIZE                    = 0.88f;
    inline static const float SWITCH_SQUARE_OUTER          = 0.63f;
    inline static const float SWITCH_SQUARE_INNER          = 0.44f;
    inline static const float SWITCH_SQUARE_TRIANGLE_INNER = 0.0f;
    inline static const float SWITCH_DIAMOND_OUTER         = 0.66f;
    inline static const float SWITCH_DIAMOND_INNER         = 0.41f;
    inline static const float SWITCH_CIRCLE_OUTER          = 0.7f;
    inline static const float SWITCH_CIRCLE_INNER          = 0.52f;
    inline static const float ICE_EDGE_WIDTH               = 0.1f;
    inline static const float ARROW_SIZE                   = 0.66f;
    inline static const float ARROW_LINE_WIDTH             = 0.25f;
    inline static const float CIRCLE_RESOLUTION            = 5.0f * (M_PI / 180.0);
    inline static const float MAX_LUMINANCE_ADJUSTMENT     = 0.3f;

    inline static const std::string RESOURCE_ID_SURFACE_PLAIN                  = "Surface Plain";
    inline static const std::string RESOURCE_ID_SURFACE_PLAIN_SPLIT            = "Surface Plain Split";
    inline static const std::string RESOURCE_ID_SURFACE_ARROW                  = "Surface Arrow";
    inline static const std::string RESOURCE_ID_SURFACE_ICE_OR_WATER           = "Surface Ice or Water";
    inline static const std::string RESOURCE_ID_SURFACE_TRAMPOLINE             = "Surface Trampoline";
    inline static const std::string RESOURCE_ID_SURFACE_DEACTIVATE_ALL_STATES  = "Surface Deactivate All States";
    inline static const std::string RESOURCE_ID_SURFACE_ACTIVATE_STATE_1_OR_2  = "Surface Activate State 1 or 2";
    inline static const std::string RESOURCE_ID_SURFACE_ACTIVATE_STATE_3       = "Surface Activate State 3";
    inline static const std::string RESOURCE_ID_SURFACE_ACTIVATE_STATE_4       = "Surface Activate State 4";
    inline static const std::string RESOURCE_ID_SURFACE_ACTIVATE_STATE_5_OR_6  = "Surface Activate State 5 or 6";
    inline static const std::string RESOURCE_ID_SURFACE_ACTIVATE_STATE_7       = "Surface Activate State 7";
    inline static const std::string RESOURCE_ID_SURFACE_ACTIVATE_STATE_8       = "Surface Activate State 8";
    inline static const std::string RESOURCE_ID_SURFACE_ACTIVATE_STATE_9_OR_10 = "Surface Activate State 9 or 10";
    inline static const std::string RESOURCE_ID_SURFACE_ACTIVATE_STATE_11      = "Surface Activate State 11";
    inline static const std::string RESOURCE_ID_WALL_PLAIN_CAP                 = "Wall Plain Cap";
    inline static const std::string RESOURCE_ID_WALL_PLAIN_MIDDLE              = "Wall Plain Middle";
    inline static const std::string RESOURCE_ID_WALL_VARIANT_CAP               = "Wall Plain Variant Cap";
    inline static const std::string RESOURCE_ID_WALL_VARIANT_MIDDLE            = "Wall Plain Variant Middle";
    inline static const std::string RESOURCE_ID_WALL_ICE                       = "Wall Ice";

    class OrientedTexture : public ITexture {
      public:
      OrientedTexture();
        
      void addOrientation(const IFloat* angle, Project& project, bool clamp);
      
      void setRenderTarget(const IFloat* screen);
      void setScreen(const IFloat* screen);
            
      void addUseListener(ITextureUseListener* listener);

      /***********************\
       * Implements ITexture *
      \***********************/
      ITexture* getTexture() override;
      void set() const override;
      void hintTextureInUse(bool inUse) override;
      void coord(float x, float y) const override;
      
      private:
      std::map<const IFloat*, std::unique_ptr<LiteralTexture>> cTextures;
      std::set<ITextureUseListener*> cListeners;
      LiteralTexture* cCurrentTexture;
    };

    class TextureStateListener : public IStateListener {
      public:
      TextureStateListener(C64TerrainGraphics& parent, const IFloat* angle);
      void stateChanged() override;

      private:
      C64TerrainGraphics& cParent;
      const IFloat* cAngle;
    };

    Project& cProject;

    std::map<std::string, std::unique_ptr<LiteralTexture>> cTextures;
    std::map<std::string, std::unique_ptr<OrientedTexture>> cOrientedTextures;
    std::vector<std::unique_ptr<TextureStateListener>> cTextureStateListeners;
    std::set<const IFloat*> cChangedAngles;
    std::set<const IFloat*> cUniqueViews;
    Float cDefaultYaw;
    
    // Definition data.
    Colour cDefFloor;
    Colour cDefWall;
    Colour cDefGrid;
    Colour cDefHighlight;
    
    // Runtime data.
    std::map<IFloat*, std::vector<const IScreen*>> cDefOrientatiions;
    
    int cTexturesInUseCount;
    bool cNeedsFullRedraw;
    
    void setNeedsFullRedraw();

    float getGridWallLuminanceAdjustment();
    
    void clear(const IColour& colour);
    void renderSquare(float, Colour& colour);
    void renderDiamond(float, Colour& colour);
    void renderCircle(float, Colour& colour);
    void renderRectangle(float, float, float, float, const IColour& colour);
    void renderTile(Colour& colour);
    void renderIce();
    void renderPlain();
    void renderSplitPlain();
    void renderTrampoline();
    void renderSwitchSquare(float);
    void renderSwitchSquareHalf(float);
    void renderSwitchSquareBoth(float);
    void renderSwitchDiamondBoth(float);
    void renderSwitchDiamondHalf(float);
    void renderSwitchDiamondNone();
    void renderSwitchCircle(float);
    void renderSwitchCircleBoth();
    void renderSwitchCircleHalf(float);
    void renderArrow();
    void renderWallMiddle(float);
    void renderWallCap(float);
    void renderIceWall();

    void generateOrientedTextures(const IFloat* angle);
    void generateTextures();

    std::unique_ptr<LiteralTexture> createTexture(bool clamp = false);
    std::unique_ptr<OrientedTexture> createOrientedTexture();
    void performAngleRedraw(const IFloat* angle);
    void updateLater();
  };
}

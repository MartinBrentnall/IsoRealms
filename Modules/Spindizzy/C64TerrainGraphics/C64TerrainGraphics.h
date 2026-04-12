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
    C64TerrainGraphics(Spindizzy& spindizzy, IResourceData& data);
    C64TerrainGraphics(Spindizzy& spindizzy, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    void getProperties(PropertyMaker& owner, const Metadata& metadata);
    void removed();

    /**********************************\
     * Implements ITextureUseListener *
    \**********************************/
    void hintTextureUsed(ITexture* texture, bool inUse) override;

    /**************************************\
     * Implements IStateListener<IFloat*> *
    \**************************************/
    void stateChanged(const IFloat* asset);

    /******************************\
     * Implements IScreenListener *
    \******************************/
    void screenAdded(const IScreen* screen) override;
    void screenRemoved(const IScreen* screen) override;
    void screenPreRender(const IScreen* screen) override;
    void screenPostRender(const IScreen* screen) override;

    private:
    static const float TILE_SIZE;
    static const float SWITCH_SQUARE_OUTER;
    static const float SWITCH_SQUARE_INNER;
    static const float SWITCH_SQUARE_TRIANGLE_INNER;
    static const float SWITCH_DIAMOND_OUTER;
    static const float SWITCH_DIAMOND_INNER;
    static const float SWITCH_CIRCLE_OUTER;
    static const float SWITCH_CIRCLE_INNER;
    static const float ICE_EDGE_WIDTH;
    static const float ARROW_SIZE;
    static const float ARROW_LINE_WIDTH;
    static const float CIRCLE_RESOLUTION;
    static const float MAX_LUMINANCE_ADJUSTMENT;

    static const std::string ASSET_ID_SURFACE_PLAIN;
    static const std::string ASSET_ID_SURFACE_PLAIN_SPLIT;
    static const std::string ASSET_ID_SURFACE_ARROW;
    static const std::string ASSET_ID_SURFACE_ICE_OR_WATER;
    static const std::string ASSET_ID_SURFACE_TRAMPOLINE;
    static const std::string ASSET_ID_SURFACE_DEACTIVATE_ALL_STATES;
    static const std::string ASSET_ID_SURFACE_ACTIVATE_STATE_1_OR_2;
    static const std::string ASSET_ID_SURFACE_ACTIVATE_STATE_3;
    static const std::string ASSET_ID_SURFACE_ACTIVATE_STATE_4;
    static const std::string ASSET_ID_SURFACE_ACTIVATE_STATE_5_OR_6;
    static const std::string ASSET_ID_SURFACE_ACTIVATE_STATE_7;
    static const std::string ASSET_ID_SURFACE_ACTIVATE_STATE_8;
    static const std::string ASSET_ID_SURFACE_ACTIVATE_STATE_9_OR_10;
    static const std::string ASSET_ID_SURFACE_ACTIVATE_STATE_11;
    static const std::string ASSET_ID_WALL_PLAIN_CAP;
    static const std::string ASSET_ID_WALL_PLAIN_MIDDLE;
    static const std::string ASSET_ID_WALL_VARIANT_CAP;
    static const std::string ASSET_ID_WALL_VARIANT_MIDDLE;
    static const std::string ASSET_ID_WALL_ICE;

    static const std::string JSON_FLOOR;
    static const std::string JSON_GRID;
    static const std::string JSON_HIGHLIGHT;
    static const std::string JSON_WALL;

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
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;

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
    void renderSwitchDiamond(float);
    void renderSwitchDiamondHalf(float);
    void renderSwitchDiamondBoth();
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

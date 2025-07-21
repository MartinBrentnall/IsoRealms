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
                             public IScreenListener,
                             public IStateListener<IFloat*> {
    public:
    C64TerrainGraphics(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    C64TerrainGraphics(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    std::vector<std::unique_ptr<IProperty>> getProperties(IPropertyOwner& owner);
    void save(JSONObject object) const;
    bool renderIcon();
    void hintInUse(bool inUse);
    void registerAssets(ResourceAssetRegistry& assets);

    /**********************************\
     * Implements ITextureUseListener *
    \**********************************/
    void hintTextureUsed(ITexture*, bool) override;

    /**************************************\
     * Implements IStateListener<IFloat*> *
    \**************************************/
    void stateChanged(IFloat* asset) override;

    /******************************\
     * Implements IScreenListener *
    \******************************/
    void screenAdded(IProject& project, const IScreen* screen) override;
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

    static const std::string PLAIN;
    static const std::string PLAIN_SPLIT;
    static const std::string ICE_WATER;
    static const std::string ARROW;
    static const std::string SWITCH_CIRCLE_BOTH;
    static const std::string SWITCH_CIRCLE_ONE;
    static const std::string SWITCH_CIRCLE_NONE;
    static const std::string SWITCH_SQUARE_BOTH;
    static const std::string SWITCH_SQUARE_ONE;
    static const std::string SWITCH_SQUARE_NONE;
    static const std::string SWITCH_DIAMOND_BOTH;
    static const std::string SWITCH_DIAMOND_ONE;
    static const std::string SWITCH_DIAMOND_NONE;
    static const std::string TRAMPOLINE;
    static const std::string WALL_ICE;
    static const std::string WALL_MIXED_CAP;
    static const std::string WALL_MIXED_MIDDLE;
    static const std::string WALL_PLAIN_CAP;
    static const std::string WALL_PLAIN_MIDDLE;

    static const std::string JSON_FLOOR;
    static const std::string JSON_GRID;
    static const std::string JSON_HIGHLIGHT;
    static const std::string JSON_WALL;

    class OrientedTexture : public ITexture {
      public:
      OrientedTexture();
        
      void addOrientation(const IFloat* angle, IProject& project, bool clamp);
      
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
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      std::map<const IFloat*, std::unique_ptr<LiteralTexture>> cTextures;
      std::set<ITextureUseListener*> cListeners;
      LiteralTexture* cCurrentTexture;
    };

    IProject& cProject;

    std::map<std::string, std::unique_ptr<LiteralTexture>> cTextures;
    std::map<std::string, std::unique_ptr<OrientedTexture>> cOrientedTextures;
    std::set<IFloat*> cChangedAngles;
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

    void generateAngledTextures(const IFloat* angle);
    void generateTextures();

    std::unique_ptr<LiteralTexture> createTexture(bool clamp = false);
    std::unique_ptr<OrientedTexture> createOrientedTexture();
    void performAngleRedraw(IFloat* angle);
    void updateLater();
  };
}

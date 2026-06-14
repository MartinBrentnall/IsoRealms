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

#include "IsoRealms.h"

#include "Modules/Equilibria/Resources/Type/IWallPattern.h"

namespace IsoRealms::Equilibria {
  class TerrainType;

  class WallPatternCap : public IWallPattern {
    public:
    WallPatternCap(const Metadata& metadata, TerrainType& owner);
    WallPatternCap(const Metadata& metadata, TerrainType& owner, JSONObject object);

    /***************************\
     * Implements IWallPattern *
    \***************************/
    bool renderResourceIcon() const override;
    void getResourceProperties(IComponentDefiner& definer) override;
    bool isDefaultConfiguration() const override;
    bool contains(ITexture*) override;
    std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(Wall* wall) const override;
    void render(float x, float y, float z, float length, float height, float topSlope, float bottomSlope, Wall::Direction facing) const override;
    void hintInUse(bool inUse) override;

    /*****************************************\
     * Implements IResource via IWallPattern *
    \*****************************************/
    private:

    // Internal classes.
    class SectionTop : public IVisualElement {
      public:
      SectionTop(const WallPatternCap& parent, Wall* wall);

      /*****************************\
       * Implemetns IVisualElement * 
      \*****************************/
      void render() override;
      ITexture* getTexture() override;
      void prepareVisual() override;
      
      private:
      const WallPatternCap& cDefParent;
      Wall* cDefWall;
    };

    class SectionMiddle : public IVisualElement {
      public:
      SectionMiddle(const WallPatternCap& parent, Wall* wall);

      /*****************************\
       * Implemetns IVisualElement * 
      \*****************************/
      void render() override;
      ITexture* getTexture() override;
      void prepareVisual() override;
      
      private:
      const WallPatternCap& cDefParent;
      Wall* cDefWall;
    };

    class SectionBottom : public IVisualElement {
      public:
      SectionBottom(const WallPatternCap& parent, Wall* wall);

      /*****************************\
       * Implemetns IVisualElement * 
      \*****************************/
      void render() override;
      ITexture* getTexture() override;
      void prepareVisual() override;
      
      private:
      const WallPatternCap& cDefParent;
      Wall* cDefWall;
    };

    // JSON members.
    inline static const std::string JSON_BOTTOM = "bottom";
    inline static const std::string JSON_MIDDLE = "middle";
    inline static const std::string JSON_TOP    = "top";
 
    // Definition data.
    Texture cDefTextureBottom;
    Texture cDefTextureMiddle;
    Texture cDefTextureTop;
    
    // Private functions.
    void renderBottom(float x, float y, float z, float length, float bottomSlope, Wall::Direction facing) const;
    void renderMiddle(float x, float y, float z, float length, float height, float topSlope, float bottomSlope, Wall::Direction facing) const;
    void renderTop(float x, float y, float z, float length, float height, float topSlope, Wall::Direction facing) const;
  };
}

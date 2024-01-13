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

#include "IsoRealms/IAssets.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Assets/Type/IWallPattern.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  class WallPatternTile : public IWallPattern {
    public:
    WallPatternTile(IProject* project, Spindizzy* spindizzy, DOMNode& node);
    
    /***************************\
     * Implements IWallPattern *
    \***************************/
    bool contains(ITexture* texture) override;
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const override;
    std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(Wall* wall) const override;
    void render(float x, float y, float z, float length, float height, float topSlope, float bottomSlope, Wall::Direction facing) const override;
    void hintInUse(bool inUse) override;
    
    private:
    
    // Internal classes.
    class WallPatternTileSurface : public IVisualElement {
      public:
      WallPatternTileSurface(const WallPatternTile* parent, Wall* wall);

      /*****************************\
       * Implemetns IVisualElement * 
      \*****************************/
      void render() override;
      ITexture* getTexture() override;
      void prepareVisual() override;
        
      private:
      const WallPatternTile* cDefParent;
      Wall* cDefWall;
    };
    
    // DOM strings.
    static const std::string TAG_TEXTURE;

    static const std::string ATTRIBUTE_TYPE;

    static const std::string TYPE_TILE;

    // Definition data.  
    Texture cDefTexture;
  };
}

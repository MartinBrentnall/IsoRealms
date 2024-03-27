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

#include "Modules/Spindizzy/Assets/Type/ISurfacePattern.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;
  class SplitSurface;
  class Surface;

  class SurfacePatternTile : public ISurfacePattern {
    public:
    SurfacePatternTile(IProject* project, Spindizzy* spindizzy, DOMNode& node);

    /******************************\
     * Implements ISurfacePattern *
    \******************************/
    bool contains(ITexture*) override;
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const override;
    std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(Surface* surface) override;
    std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(SplitSurface* surface) override;
    void render(float startX, float endX, float startY, float endY, float z, float xSlope, float ySlope, ISurface::Direction facing) const override;
    void render(float x, float y, float z, float heightSW, float heightSE, float heightNW, float heighNE, bool alternativeSplit) const override;
    void hintInUse(bool inUse) override;
    bool renderAssetIcon() const override;

    private:

    // Internal classes.
    class SurfacePatternSurface : public IVisualElement {
      public:
      SurfacePatternSurface(SurfacePatternTile& parent, Surface* surface);

      /*****************************\
       * Implemetns IVisualElement *
      \*****************************/
      void render() override;
      ITexture* getTexture() override;
      void prepareVisual() override;
      
      private:
      const SurfacePatternTile& cDefParent;
      const Surface* cDefSurface;
    };
    
    class SurfacePatternSplitSurface : public IVisualElement {
      public:
      SurfacePatternSplitSurface(SurfacePatternTile& parent, SplitSurface* surface);

      /*****************************\
       * Implemetns IVisualElement *
      \*****************************/
      void render() override;
      ITexture* getTexture() override;
      void prepareVisual() override;
      
      private:
      const SurfacePatternTile& cDefParent;
      const SplitSurface* cDefSurface;
    };
    
    // DOM strings.
    static const std::string TAG_TEXTURE;

    static const std::string ATTRIBUTE_TYPE;

    static const std::string TYPE_TILE;
    
    // Definition data.
    Texture cDefTexture;
  };
}


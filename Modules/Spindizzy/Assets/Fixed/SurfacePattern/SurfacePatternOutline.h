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

#include "Modules/Spindizzy/Assets/Type/ISurfacePattern.h"

namespace IsoRealms::Spindizzy {
  class SplitSurface;
  class Surface;
  class TerrainType;

  class SurfacePatternOutline : public ISurfacePattern {
    public:
    SurfacePatternOutline(IProject& project, TerrainType& owner);
    SurfacePatternOutline(IProject& project, TerrainType& owner, JSONObject object);

    /******************************\
     * Implements ISurfacePattern *
    \******************************/
    bool contains(ITexture*) override;
    std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(Surface* surface) override;
    std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(SplitSurface* surface) override;
    void render(float startX, float endX, float startY, float endY, float z, float xSlope, float ySlope, ISurface::Direction facing) const override;
    void render(float x, float y, float z, float heightSW, float heightSE, float heightNW, float heighNE, bool alternativeSplit) const override;
    void hintInUse(bool inUse) override;

    /*****************************************\
     * Implements IAsset via ISurfacePattern *
    \*****************************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // Internal classes.
    class SurfacePatternSurface : public IVisualElement {
      public:
      SurfacePatternSurface(SurfacePatternOutline& parent, Surface* surface);

      /*****************************\
       * Implemetns IVisualElement *
      \*****************************/
      void render() override;
      ITexture* getTexture() override;
      void prepareVisual() override;

      private:
      const SurfacePatternOutline& cDefParent;
      const Surface* cDefSurface;
    };

    class SurfacePatternSplitSurface : public IVisualElement {
      public:
      SurfacePatternSplitSurface(SurfacePatternOutline& parent, SplitSurface* surface);

      /*****************************\
       * Implemetns IVisualElement *
      \*****************************/
      void render() override;
      ITexture* getTexture() override;
      void prepareVisual() override;

      private:
      const SurfacePatternOutline& cDefParent;
      const SplitSurface* cDefSurface;
    };
  };
}



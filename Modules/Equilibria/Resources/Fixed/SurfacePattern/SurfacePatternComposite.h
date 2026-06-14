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
 
 #include "Modules/Equilibria/Resources/Client/SurfacePattern.h"
 #include "Modules/Equilibria/Resources/Type/ISurfacePattern.h"
 
 namespace IsoRealms::Equilibria {
   class SplitSurface;
   class Surface;
   class TerrainType;
 
   class SurfacePatternComposite : public ISurfacePattern {
     public:
     SurfacePatternComposite(const Metadata& metadata, TerrainType& owner);
     SurfacePatternComposite(const Metadata& metadata, TerrainType& owner, JSONObject object);
 
     /******************************\
      * Implements ISurfacePattern *
     \******************************/
     void getResourceProperties(IComponentDefiner& definer) override;
     bool contains(ITexture*) override;
     std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(Surface* surface) override;
     std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals(SplitSurface* surface) override;
     void render(float startX, float endX, float startY, float endY, float z, float xSlope, float ySlope, ISurface::Direction facing) const override;
     void render(float x, float y, float z, float heightSW, float heightSE, float heightNW, float heighNE, bool alternativeSplit) const override;
     void hintInUse(bool inUse) override;
 
     private:
     inline static const std::string JSON_PATTERN  = "pattern";
     inline static const std::string JSON_PATTERNS = "patterns";

     // External interfaces.
     TerrainType& cOwner;

     // Definition data.
     std::vector<std::unique_ptr<SurfacePattern>> cDefSurfacePatterns;
   };
 }
  
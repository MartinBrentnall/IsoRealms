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
#include "SurfacePatternSplitVariant.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/TerrainType/TerrainType.h"
#include "Modules/Equilibria/World/Object/Terrain/SplitSurface.h"
#include "Modules/Equilibria/World/Object/Terrain/Surface.h"

namespace IsoRealms::Equilibria {
  SurfacePatternSplitVariant::SurfacePatternSplitVariant(const Metadata& metadata, TerrainType& owner) :
            cDefRegularPattern(owner.getEquilibria(), owner, [&owner]() {owner.getEquilibria().stateChanged(nullptr);}),
            cDefSplitAPattern( owner.getEquilibria(), owner, [&owner]() {owner.getEquilibria().stateChanged(nullptr);}),
            cDefSplitBPattern( owner.getEquilibria(), owner, [&owner]() {owner.getEquilibria().stateChanged(nullptr);}) {
  }

  SurfacePatternSplitVariant::SurfacePatternSplitVariant(const Metadata& metadata, TerrainType& owner, JSONObject object) :
            SurfacePatternSplitVariant(metadata, owner) {
    cDefRegularPattern.set(object, JSON_REGULAR);
    cDefSplitAPattern.set(object, JSON_SPLIT_A);
    cDefSplitBPattern.set(object, JSON_SPLIT_B);
  }
  
  bool SurfacePatternSplitVariant::contains(ITexture* texture) {
    return cDefRegularPattern->contains(texture)
        || cDefSplitAPattern->contains(texture)
        || cDefSplitBPattern->contains(texture);
  }

  std::vector<std::unique_ptr<IVisualElement>> SurfacePatternSplitVariant::getStaticVisuals(Surface* surface) {
    return cDefRegularPattern->getStaticVisuals(surface);
  }

  void SurfacePatternSplitVariant::render(float startX, float endX, float startY, float endY, float z, float xSlope, float ySlope, ISurface::Direction facing) const {
    cDefRegularPattern->render(startX, endX, startY, endY, z, xSlope, ySlope, facing);
  }

  void SurfacePatternSplitVariant::hintInUse(bool inUse) {
    cDefRegularPattern->hintInUse(inUse);
    cDefSplitAPattern->hintInUse(inUse);
    cDefSplitBPattern->hintInUse(inUse);
  }

  bool SurfacePatternSplitVariant::renderResourceIcon() const {
    return cDefRegularPattern->renderResourceIcon();
  }

  void SurfacePatternSplitVariant::getResourceProperties(IComponentDefiner& definer) {
    definer.propertyResource(JSON_REGULAR, cDefRegularPattern);
    definer.propertyResource(JSON_SPLIT_A, cDefSplitAPattern);
    definer.propertyResource(JSON_SPLIT_B, cDefSplitBPattern);
  }

  bool SurfacePatternSplitVariant::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }

  SurfacePatternSplitVariant::SurfacePatternSplitVariant(const Metadata& metadata, TerrainType& owner, JSONObject object) :
            SurfacePatternSplitVariant(metadata, owner) {
    cDefRegularPattern.set(object, JSON_REGULAR);
    cDefSplitAPattern.set(object, JSON_SPLIT_A);
    cDefSplitBPattern.set(object, JSON_SPLIT_B);
  }

std::vector<std::unique_ptr<IVisualElement>> SurfacePatternSplitVariant::getStaticVisuals(SplitSurface* surface) {
    return (surface->isAlternativeSplit() ? cDefSplitBPattern : cDefSplitAPattern)->getStaticVisuals(surface);
  }

void SurfacePatternSplitVariant::render(float x, float y, float z, float heightSW, float heightSE, float heightNW, float heightNE, bool alternativeSplit) const {
    (alternativeSplit ? cDefSplitBPattern : cDefSplitAPattern)->render(x, y, z, heightSW, heightSE, heightNW, heightNE, alternativeSplit);
  }
}

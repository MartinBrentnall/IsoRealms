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

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/TerrainType/TerrainType.h"
#include "Modules/Spindizzy/World/Object/Terrain/SplitSurface.h"
#include "Modules/Spindizzy/World/Object/Terrain/Surface.h"

namespace IsoRealms::Spindizzy {
  SurfacePatternSplitVariant::SurfacePatternSplitVariant(IProject& project, TerrainType& owner) :
            cDefRegularPattern(owner.getSpindizzy(), owner, [&owner]() {owner.getSpindizzy().stateChanged(nullptr);}),
            cDefSplitAPattern( owner.getSpindizzy(), owner, [&owner]() {owner.getSpindizzy().stateChanged(nullptr);}),
            cDefSplitBPattern( owner.getSpindizzy(), owner, [&owner]() {owner.getSpindizzy().stateChanged(nullptr);}) {
  }

  SurfacePatternSplitVariant::SurfacePatternSplitVariant(IProject& project, TerrainType& owner, JSONObject object) :
            SurfacePatternSplitVariant(project, owner) {
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

  std::vector<std::unique_ptr<IVisualElement>> SurfacePatternSplitVariant::getStaticVisuals(SplitSurface* surface) {
    return (surface->isAlternativeSplit() ? cDefSplitBPattern : cDefSplitAPattern)->getStaticVisuals(surface);
  }

  void SurfacePatternSplitVariant::render(float startX, float endX, float startY, float endY, float z, float xSlope, float ySlope, ISurface::Direction facing) const {
    cDefRegularPattern->render(startX, endX, startY, endY, z, xSlope, ySlope, facing);
  }

  void SurfacePatternSplitVariant::render(float x, float y, float z, float heightSW, float heightSE, float heightNW, float heightNE, bool alternativeSplit) const {
    (alternativeSplit ? cDefSplitBPattern : cDefSplitAPattern)->render(x, y, z, heightSW, heightSE, heightNW, heightNE, alternativeSplit);
  }

  void SurfacePatternSplitVariant::hintInUse(bool inUse) {
    cDefRegularPattern->hintInUse(inUse);
    cDefSplitAPattern->hintInUse(inUse);
    cDefSplitBPattern->hintInUse(inUse);
  }

  bool SurfacePatternSplitVariant::renderAssetIcon() const {
    return cDefRegularPattern->renderAssetIcon();
  }

  void SurfacePatternSplitVariant::saveAsset(JSONObject object) const {
    cDefRegularPattern.save(object, JSON_REGULAR);
    cDefSplitAPattern.save(object, JSON_SPLIT_A);
    cDefSplitBPattern.save(object, JSON_SPLIT_B);
  }

  std::vector<std::unique_ptr<IProperty>> SurfacePatternSplitVariant::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<SurfacePattern>>("Regular", "TODO", cDefRegularPattern));
    mProperties.emplace_back(std::make_unique<PropertyAsset<SurfacePattern>>("Split A", "TODO", cDefSplitAPattern));
    mProperties.emplace_back(std::make_unique<PropertyAsset<SurfacePattern>>("Split B", "TODO", cDefSplitBPattern));
    return mProperties;
  }

  bool SurfacePatternSplitVariant::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }

  const std::string SurfacePatternSplitVariant::JSON_REGULAR = "regular";
  const std::string SurfacePatternSplitVariant::JSON_SPLIT_A = "splitA";
  const std::string SurfacePatternSplitVariant::JSON_SPLIT_B = "splitB";
}


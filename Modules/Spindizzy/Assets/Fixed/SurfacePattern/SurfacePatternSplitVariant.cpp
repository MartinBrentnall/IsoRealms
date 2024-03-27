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

#include "SurfacePatternSplitVariant.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/TerrainType/TerrainType.h"
#include "Modules/Spindizzy/World/Object/Terrain/SplitSurface.h"
#include "Modules/Spindizzy/World/Object/Terrain/Surface.h"

namespace IsoRealms::Spindizzy {
  const std::string SurfacePatternSplitVariant::TAG_REGULAR = "Regular";
  const std::string SurfacePatternSplitVariant::TAG_SPLIT_A = "SplitA";
  const std::string SurfacePatternSplitVariant::TAG_SPLIT_B = "SplitB";

  const std::string SurfacePatternSplitVariant::ATTRIBUTE_TYPE = "type";
  
  const std::string SurfacePatternSplitVariant::TYPE_TILE = "splitVariant";
  
  SurfacePatternSplitVariant::SurfacePatternSplitVariant(IProject* project, Spindizzy* spindizzy, DOMNode& node) :
            cDefRegularPattern(spindizzy),
            cDefSplitAPattern(spindizzy),
            cDefSplitBPattern(spindizzy) {
    cDefRegularPattern.set(node.getNode(TAG_REGULAR));
    cDefSplitAPattern.set(node.getNode(TAG_SPLIT_A));
    cDefSplitBPattern.set(node.getNode(TAG_SPLIT_B));
  }

  bool SurfacePatternSplitVariant::contains(ITexture* texture) {
    return cDefRegularPattern->contains(texture)
        || cDefSplitAPattern->contains(texture)
        || cDefSplitBPattern->contains(texture);
  }

  void SurfacePatternSplitVariant::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_TYPE, TYPE_TILE);
    DOMNodeWriter mNode = node->addBranch(TAG_REGULAR);
    cDefRegularPattern->save(&mNode, identifier);
    mNode = node->addBranch(TAG_SPLIT_A);
    cDefSplitAPattern->save(&mNode, identifier);
    mNode = node->addBranch(TAG_SPLIT_B);
    cDefSplitBPattern->save(&mNode, identifier);
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
    return false;
  }
}


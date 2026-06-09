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
#include "SurfacePatternComposite.h"

#include "IsoRealms/Persistence/JSONValue.h"
#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/TerrainType/TerrainType.h"
#include "Modules/Equilibria/World/Object/Terrain/SplitSurface.h"
#include "Modules/Equilibria/World/Object/Terrain/Surface.h"
 
namespace IsoRealms::Equilibria {
  SurfacePatternComposite::SurfacePatternComposite(const Metadata& /*metadata*/, TerrainType& owner) :
            cOwner(owner) {
    // Nothing to do.
  }
 
  SurfacePatternComposite::SurfacePatternComposite(const Metadata& metadata, TerrainType& owner, JSONObject object) :
            SurfacePatternComposite(metadata, owner) {
    for (JSONValue mPatternValue : object.getArray(JSON_PATTERNS)) {
      JSONObject mPatternObject = mPatternValue.getObject();
      cDefSurfacePatterns.emplace_back(std::make_unique<SurfacePattern>(owner.getEquilibria(), owner, nullptr)).get()->set(mPatternObject, JSON_PATTERN);
    }
  }
 
  bool SurfacePatternComposite::contains(ITexture* texture) {
    return false;
  }
 
  std::vector<std::unique_ptr<IVisualElement>> SurfacePatternComposite::getStaticVisuals(Surface* surface) {
    std::vector<std::unique_ptr<IVisualElement>> mVisuals;
    for (std::unique_ptr<SurfacePattern>& mSurfacePattern : cDefSurfacePatterns) {
      std::vector<std::unique_ptr<IVisualElement>> patternVisuals = (*mSurfacePattern)->getStaticVisuals(surface);
      mVisuals.insert(mVisuals.end(), std::make_move_iterator(patternVisuals.begin()), std::make_move_iterator(patternVisuals.end()));
    }
    return mVisuals;
  }

  std::vector<std::unique_ptr<IVisualElement>> SurfacePatternComposite::getStaticVisuals(SplitSurface* surface) {
    std::vector<std::unique_ptr<IVisualElement>> mVisuals;
    for (std::unique_ptr<SurfacePattern>& mSurfacePattern : cDefSurfacePatterns) {
      std::vector<std::unique_ptr<IVisualElement>> patternVisuals = (*mSurfacePattern)->getStaticVisuals(surface);
      mVisuals.insert(mVisuals.end(), std::make_move_iterator(patternVisuals.begin()), std::make_move_iterator(patternVisuals.end()));
    }
    return mVisuals;
  }
 
  void SurfacePatternComposite::render(float startX, float endX, float startY, float endY, float z, float xSlope, float ySlope, ISurface::Direction facing) const {
    for (const std::unique_ptr<SurfacePattern>& surfacePattern : cDefSurfacePatterns) {
      (*surfacePattern)->render(startX, endX, startY, endY, z, xSlope, ySlope, facing);
    }
  }
 
  void SurfacePatternComposite::render(float x, float y, float z, float heightSW, float heightSE, float heightNW, float heightNE, bool alternativeSplit) const {
    for (const std::unique_ptr<SurfacePattern>& surfacePattern : cDefSurfacePatterns) {
      (*surfacePattern)->render(x, y, z, heightSW, heightSE, heightNW, heightNE, alternativeSplit);
    }
  }
 
  void SurfacePatternComposite::hintInUse(bool inUse) {
    // Nothing to do.
  }
 
  bool SurfacePatternComposite::renderAssetIcon() const {
    return false;
  }

  void SurfacePatternComposite::saveAsset(JSONObject object) const {
    JSONArray mPatternsArray = object.addArray(JSON_PATTERNS);
    for (const std::unique_ptr<SurfacePattern>& mSurfacePattern : cDefSurfacePatterns) {
      JSONObject mPatternObject = mPatternsArray.addObject();
      mSurfacePattern->save(mPatternObject, JSON_PATTERN);
    }
  }
 
  void SurfacePatternComposite::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyArray(JSON_PATTERNS, cDefSurfacePatterns, [](const std::unique_ptr<SurfacePattern>& mSurfacePattern)->SurfacePattern& {return *mSurfacePattern;}, [this, &owner](SurfacePattern& surfacePattern) {
      owner.createPropertyTreeSelector(JSON_PATTERN, surfacePattern);
    }, [this]()->SurfacePattern& {
      return *cDefSurfacePatterns.emplace_back(std::make_unique<SurfacePattern>(cOwner.getEquilibria(), cOwner, nullptr));
    });
  }
 
  bool SurfacePatternComposite::isDefaultConfiguration() const {
    return false; // TODO: Implement
  }
}
 
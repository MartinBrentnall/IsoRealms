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
 #include "WallPatternComposite.h"

 #include "Modules/Spindizzy/Spindizzy.h"
 #include "Modules/Spindizzy/TerrainType/TerrainType.h"
 #include "Modules/Spindizzy/World/Object/Terrain/Wall.h"
 
 namespace IsoRealms::Spindizzy {
   WallPatternComposite::WallPatternComposite(const Metadata& metadata, TerrainType& owner) :
             cOwner(owner),
             cMetadata(metadata) {
     // Nothing to do.
   }
 
   WallPatternComposite::WallPatternComposite(const Metadata& metadata, TerrainType& owner, JSONObject object) :
             WallPatternComposite(metadata, owner) {
     for (JSONValue mPatternValue : object.getArray(JSON_PATTERNS)) {
       JSONObject mPatternObject = mPatternValue.getObject();
       cDefWallPatterns.emplace_back(std::make_unique<WallPattern>(owner.getSpindizzy(), owner, nullptr)).get()->set(mPatternObject, JSON_PATTERN);
     }
   }
 
   std::vector<std::unique_ptr<IVisualElement>> WallPatternComposite::getStaticVisuals(Wall* wall) const {
     std::vector<std::unique_ptr<IVisualElement>> mVisuals;
     for (const std::unique_ptr<WallPattern>& mWallPattern : cDefWallPatterns) {
       std::vector<std::unique_ptr<IVisualElement>> patternVisuals = (*mWallPattern)->getStaticVisuals(wall);
       mVisuals.insert(mVisuals.end(), std::make_move_iterator(patternVisuals.begin()), std::make_move_iterator(patternVisuals.end()));
     }
     return mVisuals;
   }
 
   void WallPatternComposite::render(float x, float y, float z, float length, float height, float topSlope, float bottomSlope, Wall::Direction facing) const {
     for (const std::unique_ptr<WallPattern>& wallPattern : cDefWallPatterns) {
       (*wallPattern)->render(x, y, z, length, height, topSlope, bottomSlope, facing);
     }
   }
 
   bool WallPatternComposite::contains(ITexture* texture) {
     return false;
   }
 
   void WallPatternComposite::hintInUse(bool inUse) {
     // Nothing to do.
   }
 
   bool WallPatternComposite::renderAssetIcon() const {
     return false;
   }

   void WallPatternComposite::saveAsset(JSONObject object) const {
     JSONArray mPatternsArray = object.addArray(JSON_PATTERNS);
     for (const std::unique_ptr<WallPattern>& mWallPattern : cDefWallPatterns) {
       JSONObject mPatternObject = mPatternsArray.addObject();
       mWallPattern->save(mPatternObject, JSON_PATTERN);
     }
   }
 
   void WallPatternComposite::getAssetProperties(PropertyMaker& owner) {
     owner.createPropertyArray(cMetadata.getPropertyData("Patterns"), cDefWallPatterns, [](const std::unique_ptr<WallPattern>& mWallPattern)->WallPattern& {return *mWallPattern;}, [this, &owner](WallPattern& wallPattern) {
       owner.createPropertyTreeSelector<WallPattern>(cMetadata.getPropertyData("Pattern"), wallPattern);
     }, [this]()->WallPattern& {
       return *cDefWallPatterns.emplace_back(std::make_unique<WallPattern>(cOwner.getSpindizzy(), cOwner, nullptr));
     });
   }
 
   bool WallPatternComposite::isDefaultConfiguration() const {
     return false; // TODO: Implement
   }
 }
 
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
#include "WallPattern.h"

#include "IsoRealms/Editing/Property/IProperty.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  WallPattern::WallPattern(Spindizzy& spindizzy, TerrainType& owner, std::function<void()> listener) :
            Asset<IWallPattern, TerrainType>(owner, spindizzy.getWallPattern(this, "Tile", owner, this)),
            cListener(listener) {
  }

  IWallPattern* WallPattern::createLiteralAsset(TerrainType& owner) {
    return owner.getSpindizzy().createLiteralWallPattern(this, owner);
  }
  
  IWallPattern* WallPattern::getAsset(TerrainType& owner, JSONObject object) {
    return owner.getSpindizzy().getWallPattern(this, object, owner, this);
  }
  
  IWallPattern* WallPattern::getAsset(TerrainType& owner, const std::string& id) {
    return owner.getSpindizzy().getWallPattern(this, id, owner, this);
  }
  
  std::vector<std::string> WallPattern::getAvailableProviders() const {
    return cManager.getSpindizzy().getAllWallPatterns();
  }  

  bool WallPattern::renderOtherProviderIcon(const std::string& id) const {
    return id == getID() ? renderAssetIcon() : cManager.getSpindizzy().renderWallPatternIcon(id);
  }

  bool WallPattern::hasConfiguration() const {
    return cManager.getSpindizzy().isWallPatternConfigurable(getID());
  }

  bool WallPattern::isDefaultConfiguration() const {
    return true;
  }

  void WallPattern::stateChanged(IWallPattern* asset) {
    if (asset == cAsset) {
      cListener();
    }
  }
}

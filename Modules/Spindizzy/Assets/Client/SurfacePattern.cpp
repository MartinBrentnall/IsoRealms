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
#include "SurfacePattern.h"

#include "IsoRealms/Editing/Property/IProperty.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  SurfacePattern::SurfacePattern(Spindizzy& spindizzy, TerrainType& owner, std::function<void()> listener) :
            Asset<SurfacePattern, ISurfacePattern, TerrainType>(owner, spindizzy.getSurfacePattern(this, "Tile", owner, this)),
            cListener(listener) {
  }

  ISurfacePattern* SurfacePattern::createLiteralAsset(TerrainType& owner) {
    return owner.getSpindizzy().createLiteralSurfacePattern(this, owner);
  }
  
  ISurfacePattern* SurfacePattern::getAsset(TerrainType& owner, JSONObject object) {
    return owner.getSpindizzy().getSurfacePattern(this, object, owner, this);
  }
  
  ISurfacePattern* SurfacePattern::getAsset(TerrainType& owner, const std::string& id) {
    return owner.getSpindizzy().getSurfacePattern(this, id, owner, this);
  }
  
  std::vector<std::string> SurfacePattern::getAvailableProviders() const {
    return cManager.getSpindizzy().getAllSurfacePatterns();
  }  

  bool SurfacePattern::renderOtherProviderIcon(const std::string& id) const {
    return cManager.getSpindizzy().renderSurfacePatternIcon(id);
  }

  bool SurfacePattern::hasConfiguration() const {
    return cManager.getSpindizzy().isSurfacePatternConfigurable(getID());
  }

  bool SurfacePattern::isDefaultConfiguration() const {
    return true;
  }

  void SurfacePattern::stateChanged(ISurfacePattern* asset) {
    if (asset == cAsset) {
      cListener();
    }
  }
}

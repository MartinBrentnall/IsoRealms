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
  SurfacePattern::SurfacePattern(Spindizzy& spindizzy, std::function<void()> listener) :
            Asset<ISurfacePattern, Spindizzy>(spindizzy, spindizzy.createLiteralSurfacePattern(this)),
            cListener(listener) {
  }

  ISurfacePattern* SurfacePattern::createLiteralAsset(Spindizzy& spindizzy) {
    return spindizzy.createLiteralSurfacePattern(this);
  }
  
  ISurfacePattern* SurfacePattern::getAsset(Spindizzy& spindizzy, JSONObject object) {
    return spindizzy.getSurfacePattern(this, object, this);
  }
  
  ISurfacePattern* SurfacePattern::getAsset(Spindizzy& spindizzy, const std::string& id) {
    return spindizzy.getSurfacePattern(this, id, this);
  }
  
  std::vector<std::string> SurfacePattern::getAvailableProviders() const {
    return cManager.getAllSurfacePatterns();
  }  

  bool SurfacePattern::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderSurfacePatternIcon(id);
  }

  bool SurfacePattern::hasConfiguration() const {
    return cManager.isSurfacePatternConfigurable(getID());
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

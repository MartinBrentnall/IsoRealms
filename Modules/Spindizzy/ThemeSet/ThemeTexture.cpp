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
#include "ThemeTexture.h" 

#include "ThemeSet.h"

namespace IsoRealms::Spindizzy {
  ThemeTexture::ThemeTexture(ThemeSet& parent) :
            cParent(parent),
            cTexture(nullptr) {
  }

  void ThemeTexture::registerAssets(ResourceAssetRegistry& assets, const std::string& id) {
    cStateNotifier = assets.add<ITexture>(this, id, "Spindizzy Themes");
  }
  
  void ThemeTexture::notifyChange() {
    cStateNotifier->stateChanged(this);
  }
  
  void ThemeTexture::set(ITexture* texture) {
    cTexture = texture;
  }

  ITexture* ThemeTexture::getTexture() {
    return cTexture->getTexture();
  }

  void ThemeTexture::set() const {
    if (cTexture != nullptr) {
      cTexture->set();
    }
  }

  void ThemeTexture::hintTextureInUse(bool inUse) {
    if (cTexture != nullptr) {
      cTexture->hintTextureInUse(inUse);
    }
  }
  
  void ThemeTexture::coord(float x, float y) const {
    cTexture->coord(x, y);
  }

  void ThemeTexture::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void ThemeTexture::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool ThemeTexture::isDefaultConfiguration() const {
    return true;
  }
}

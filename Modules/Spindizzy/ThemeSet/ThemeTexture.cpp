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
#include "ThemeTexture.h" 

namespace IsoRealms::Spindizzy {
  ThemeTexture::ThemeTexture(IIconAnimator* animator) {
    cAnimator = animator;
    cTexture  = nullptr;
  }

  void ThemeTexture::registerAssets(IAssetRegistry& assets, const std::string& id) {
    cStateNotifier = assets.add(this, id, "Spindizzy Themes");
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

  std::vector<std::unique_ptr<IProperty>> ThemeTexture::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ThemeTexture::isDefaultConfiguration() const {
    return true;
  }
}

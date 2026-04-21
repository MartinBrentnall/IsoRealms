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
#include "FontRegistry.h"

namespace IsoRealms {
  FontRegistry::FontRegistry() :
            AssetClientManager(&cNone) {
  }

  void FontRegistry::Dummy::print(float x, float y, float size, IFont::Alignment Alignment, const std::string& text) {
    // Dummy implementation, do nothing
  }

  float FontRegistry::Dummy::getWidth(float size, const std::string& text) {
    return 0.0f;
  }

  float FontRegistry::Dummy::getHeight(float size, const std::string& text) {
    return 0.0;
  }

  unsigned int FontRegistry::Dummy::getChar(float position, float size, const std::string& text) {
    return 0;
  }

  bool FontRegistry::Dummy::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void FontRegistry::Dummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void FontRegistry::Dummy::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool FontRegistry::Dummy::isDefaultConfiguration() const {
    return true;
  }
}

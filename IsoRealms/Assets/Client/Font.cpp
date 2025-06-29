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
#include "Font.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IResourceData.h"

namespace IsoRealms {
  Font::Font(IResourceData& owner) :
            Asset<Font, IFont, IResourceData>(owner, owner.getAssetManager().createLiteralFont(this, owner)) {
  }

  IFont* Font::createLiteralAsset(IResourceData& owner) {
    return owner.getAssetManager().createLiteralFont(this, owner);
  }
  
  IFont* Font::getAsset(IResourceData& owner, JSONObject object) {
    return owner.getAssetManager().getFont(this, object, owner);
  }
  
  IFont* Font::getAsset(IResourceData& owner, const std::string& id) {
    return owner.getAssetManager().getFont(this, id, owner);
  }
  
  std::vector<std::string> Font::getAvailableProviders() const {
    return cManager.getAssetManager().getAllFonts();
  }  

  bool Font::renderOtherProviderIcon(const std::string& id) const {
    return cManager.getAssetManager().renderFontIcon(id);
  }

  bool Font::hasConfiguration() const {
    return cManager.getAssetManager().isFontConfigurable(getID());
  }  

  bool Font::isDefaultConfiguration() const {
    return true;
  }
}

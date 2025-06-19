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
#include "Boolean.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IResourceData.h"

namespace IsoRealms {
  Boolean::Boolean(IResourceData& owner, bool defaultValue, std::function<void(bool)> listener) :
            Asset<IBoolean, IResourceData>(owner, owner.getAssetManager().createLiteralBoolean(this, owner, defaultValue)),
            cDefaultValue(defaultValue),
            cListener(listener) {
  }

  IBoolean* Boolean::createLiteralAsset(IResourceData& owner) {
    return owner.getAssetManager().createLiteralBoolean(this, owner, false);
  }
  
  IBoolean* Boolean::getAsset(IResourceData& owner, JSONObject object) {
    return owner.getAssetManager().getBoolean(this, object, owner, cListener != nullptr ? this : nullptr);
  }
  
  IBoolean* Boolean::getAsset(IResourceData& owner, const std::string& id) {
    return owner.getAssetManager().getBoolean(this, id, owner, cListener != nullptr ? this : nullptr);
  }
  
  std::vector<std::string> Boolean::getAvailableProviders() const {
    return cManager.getAssetManager().getAllBooleans();
  }

  bool Boolean::renderOtherProviderIcon(const std::string& id) const {
    return cManager.getAssetManager().renderBooleanIcon(id);
  }

  bool Boolean::hasConfiguration() const {
    return cManager.getAssetManager().isBooleanConfigurable(getID());
  }

  bool Boolean::isDefaultConfiguration() const {
    return true;
  }

  void Boolean::stateChanged(IBoolean* value) {
    if (value == cAsset) {
      cListener(cAsset->getValue());
    }
  }
}

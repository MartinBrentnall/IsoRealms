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
#include "BindingType.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IResourceData.h"

namespace IsoRealms {
  BindingType::BindingType(IResourceData& owner, std::function<void()> listener) :
            Asset<BindingType, IBindingType, IResourceData>(owner, owner.getAssetManager().createLiteralBindingType(this, owner)),
            cListener(listener) {
  }

  IBindingType* BindingType::createLiteralAsset(IResourceData& owner) {
    return owner.getAssetManager().createLiteralBindingType(this, owner);
  }

  IBindingType* BindingType::getAsset(IResourceData& owner, JSONObject object) {
    return owner.getAssetManager().getBindingType(this, object, owner);
  }

  IBindingType* BindingType::getAsset(IResourceData& owner, const std::string& id) {
    return owner.getAssetManager().getBindingType(this, id, owner);
  }

  std::vector<std::string> BindingType::getAvailableProviders() const {
    return cManager.getAssetManager().getAllBindingTypes();
  }

  bool BindingType::renderOtherProviderIcon(const std::string& id) const {
    return cManager.getAssetManager().renderBindingTypeIcon(id);
  }

  bool BindingType::hasConfiguration() const {
    return cManager.getAssetManager().isBindingTypeConfigurable(getID());
  }

  bool BindingType::isDefaultConfiguration() const {
    return true;
  }

  void BindingType::stateChanged(IBindingType* value) {
    if (value == cAsset && cListener != nullptr) {
      cListener();
    }
  }
}

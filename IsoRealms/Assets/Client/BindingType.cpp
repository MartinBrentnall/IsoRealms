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

namespace IsoRealms {
  BindingType::BindingType(IProject& project) :
            Asset<IBindingType, IProject>(project, project.createLiteralBindingType(this)) {
  }

  IBindingType* BindingType::createLiteralAsset(IProject& project) {
    return project.createLiteralBindingType(this);
  }

  IBindingType* BindingType::getAsset(IProject& project, JSONObject object) {
    return project.getBindingType(this, object);
  }

  IBindingType* BindingType::getAsset(IProject& project, const std::string& id) {
    return project.getBindingType(this, id);
  }

  std::vector<std::string> BindingType::getAvailableProviders() const {
    return cManager.getAllBindingTypes();
  }

  bool BindingType::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderBindingTypeIcon(id);
  }

  bool BindingType::hasConfiguration() const {
    return cManager.isBindingTypeConfigurable(getID());
  }

  bool BindingType::isDefaultConfiguration() const {
    return true;
  }
}

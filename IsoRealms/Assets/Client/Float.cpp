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
#include "Float.h"

#include "IsoRealms/Editing/Property/IProperty.h"

namespace IsoRealms {
  Float::Float(IProject& project, float defaultValue, std::function<void(float)> listener) :
            Asset<IFloat, IProject>(project, project.createLiteralFloat(this, defaultValue)),
            cDefaultValue(defaultValue),
            cListener(listener) {
  }

  IFloat* Float::createLiteralAsset(IProject& project) {
    return project.createLiteralFloat(this, cDefaultValue);
  }
  
  IFloat* Float::getAsset(IProject& project, JSONObject object) {
    return project.getFloat(this, object, cListener != nullptr ? this : nullptr);
  }
  
  IFloat* Float::getAsset(IProject& project, const std::string& id) {
    return project.getFloat(this, id, cListener != nullptr ? this : nullptr);
  }
  
  std::vector<std::string> Float::getAvailableProviders() const {
    return cManager.getAllFloats();
  }  

  bool Float::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderFloatIcon(id);
  }

  bool Float::hasConfiguration() const {
    return cManager.isFloatConfigurable(getID());
  }

  void Float::stateChanged(IFloat* value) {
    if (value == cAsset && cListener != nullptr) {
      cListener(cAsset->getValue());
    }
  }

  bool Float::isDefaultConfiguration() const {
    return true;
  }
}

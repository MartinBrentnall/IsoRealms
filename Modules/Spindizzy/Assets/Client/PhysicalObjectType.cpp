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
#include "PhysicalObjectType.h"

#include "IsoRealms/Editing/Property/IProperty.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  PhysicalObjectType::PhysicalObjectType(Spindizzy& spindizzy) : 
            Asset<IPhysicalObjectType, Spindizzy>(spindizzy, spindizzy.createLiteralPhysicalObjectType(this)) {
  }

  IBinding* PhysicalObjectType::getBinding(const std::string& id) const {
    return cAsset->getBinding(id);
  }

  std::string PhysicalObjectType::getBindingID(const IBinding* binding) const {
    return cAsset->getBindingID(binding);
  }

  IPhysicalObjectType* PhysicalObjectType::createLiteralAsset(Spindizzy& spindizzy) {
    return spindizzy.createLiteralPhysicalObjectType(this);
  }
  
  IPhysicalObjectType* PhysicalObjectType::getAsset(Spindizzy& spindizzy, JSONObject object) {
    return spindizzy.getPhysicalObjectType(this, object);
  }
  
  IPhysicalObjectType* PhysicalObjectType::getAsset(Spindizzy& spindizzy, const std::string& id) {
    return spindizzy.getPhysicalObjectType(this, id);
  }
  
  std::vector<std::string> PhysicalObjectType::getAvailableProviders() const {
    return cManager.getAllPhysicalObjectTypes();
  }

  bool PhysicalObjectType::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderPhysicalObjectTypeIcon(id);
  }

  bool PhysicalObjectType::hasConfiguration() const {
    return cManager.isPhysicalObjectTypeConfigurable(getID());
  }  

  bool PhysicalObjectType::isDefaultConfiguration() const {
    return true;
  }
}

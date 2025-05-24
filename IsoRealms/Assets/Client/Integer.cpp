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
#include "Integer.h"

#include "IsoRealms/Editing/Property/IProperty.h"

namespace IsoRealms {
  Integer::Integer(IProject& project) : 
            Asset<IInteger, IProject>(project, project.createLiteralInteger(this)) {
  }

  IInteger* Integer::createLiteralAsset(IProject& project) {
    return project.createLiteralInteger(this);
  }
  
  IInteger* Integer::getAsset(IProject& project, JSONObject object) {
    return project.getInteger(this, object);
  }
  
  IInteger* Integer::getAsset(IProject& project, const std::string& id) {
    return project.getInteger(this, id);
  }
  
  std::vector<std::string> Integer::getAvailableProviders() const {
    return cManager.getAllIntegers();
  }  

  bool Integer::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderIntegerIcon(id);
  }

  bool Integer::hasConfiguration() const {
    return cManager.isIntegerConfigurable(getID());
  }

  bool Integer::isDefaultConfiguration() const {
    return true;
  }
}

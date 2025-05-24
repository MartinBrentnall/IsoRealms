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
#include "String.h"

#include "IsoRealms/Editing/Property/IProperty.h"

namespace IsoRealms {
  String::String(IProject& project) : 
            Asset<IString, IProject>(project, project.createLiteralString(this)) {
  }

  IString* String::createLiteralAsset(IProject& project) {
    return project.createLiteralString(this);
  }
  
  IString* String::getAsset(IProject& project, JSONObject object) {
    return project.getString(this, object);
  }
  
  IString* String::getAsset(IProject& project, const std::string& id) {
    return project.getString(this, id);
  }
  
  std::vector<std::string> String::getAvailableProviders() const {
    return cManager.getAllStrings();
  }  

  bool String::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderStringIcon(id);
  }

  bool String::hasConfiguration() const {
    return cManager.isStringConfigurable(getID());
  }

  bool String::isDefaultConfiguration() const {
    return true;
  }
}

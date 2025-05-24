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
#include "Editable.h"

#include "IsoRealms/Editing/Property/IProperty.h"

namespace IsoRealms {
  Editable::Editable(IProject& project) : 
            Asset<IEditable, IProject>(project, project.createLiteralEditable(this)) {
  }

  IEditable* Editable::createLiteralAsset(IProject& project) {
    return project.createLiteralEditable(this);
  }
  
  IEditable* Editable::getAsset(IProject& project, JSONObject object) {
    return project.getEditable(this, object);
  }
  
  IEditable* Editable::getAsset(IProject& project, const std::string& id) {
    return project.getEditable(this, id);
  }
  
  std::vector<std::string> Editable::getAvailableProviders() const {
    return cManager.getAllEditables();
  }  

  bool Editable::renderOtherProviderIcon(const std::string& id) const {
    return false; // TODO
  }

  bool Editable::hasConfiguration() const {
    return false; // TODO cManager.isEditableConfigurable(getID());
  }  

  bool Editable::isDefaultConfiguration() const {
    return true;
  }
}

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
#include "ProjectOptions.h"

#include "IsoRealms/Editing/Property/IProperty.h"

namespace IsoRealms {
  ProjectOptions::ProjectOptions(IProject& project) : 
            Asset<IProjectOptions, IProject>(project, project.createLiteralProjectOptions(this)) {
  }

  IProjectOptions* ProjectOptions::createLiteralAsset(IProject& project) {
    return project.createLiteralProjectOptions(this);
  }
  
  IProjectOptions* ProjectOptions::getAsset(IProject& project, JSONObject object) {
    return project.getProjectOptions(this, object);
  }
  
  IProjectOptions* ProjectOptions::getAsset(IProject& project, const std::string& id) {
    return project.getProjectOptions(this, id);
  }
  
  std::vector<std::string> ProjectOptions::getAvailableProviders() const {
    return std::vector<std::string>();
    // TODO return cManager.getAllProjectOptionss();
  }  

  bool ProjectOptions::renderOtherProviderIcon(const std::string& id) const {
    return false; // TODO
  }

  bool ProjectOptions::hasConfiguration() const {
    return false; // TODO cManager.isProjectOptionsConfigurable(getID());
  }  

  bool ProjectOptions::isDefaultConfiguration() const {
    return true;
  }
}

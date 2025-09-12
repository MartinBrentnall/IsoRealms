/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ActionClient.h"

#include "IResourceData.h"
#include "Project/Project.h"

namespace IsoRealms {
  ActionClient::ActionClient(IResourceData& resource, IBindingRegistry& bindingRegistry) :
            cResource(resource),
            cBindingRegistry(bindingRegistry) {
  }
  
  bool ActionClient::isReadOnly() const {
    return cResource.isReadOnly();
  }
  
  void ActionClient::setOwner(ProjectFile* owner) {
    cResource.setOwner(owner);
  }
  
  Project& ActionClient::getProject() {
    return cResource.getProject();
  }
  
  Project& ActionClient::getAssetManager() {
    return cResource.getAssetManager();
  }

  IResourceData& ActionClient::getResourceData() {
    return cResource;
  }

  IBindingRegistry* ActionClient::getBindingRegistry() {
    return &cBindingRegistry;
  }
}

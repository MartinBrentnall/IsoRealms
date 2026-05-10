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
#include "ActionContext.h"

#include "IResourceData.h"
#include "Project/Project.h"

namespace IsoRealms {
  ActionContext::ActionContext(IResourceData& resource, IEventBindings& bindingRegistry) :
            cResource(resource),
            cBindingRegistry(bindingRegistry) {
  }
  
  bool ActionContext::isReadOnly() const {
    return cResource.isReadOnly();
  }
  
  void ActionContext::setOwner(ProjectFile* owner) {
    cResource.setOwner(owner);
  }
  
  Project& ActionContext::getProject() {
    return cResource.getProject();
  }
  
  Project& ActionContext::getAssetManager() {
    return cResource.getAssetManager();
  }

  IResourceData& ActionContext::getResourceData() {
    return cResource;
  }

  IEventBindings* ActionContext::getBindingRegistry() {
    return &cBindingRegistry;
  }
}

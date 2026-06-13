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

#include "IComponentData.h"
#include "Project/Project.h"

namespace IsoRealms {
  ActionContext::ActionContext(IComponentData& componentData, IEventBindings& bindingRegistry) :
            cComponentData(componentData),
            cBindingRegistry(bindingRegistry) {
  }
  
  bool ActionContext::isReadOnly() const {
    return cComponentData.isReadOnly();
  }
  
  void ActionContext::setOwner(ProjectFile* owner) {
    cComponentData.setOwner(owner);
  }
  
  Project& ActionContext::getProject() {
    return cComponentData.getProject();
  }
  
  Project& ActionContext::getAssetManager() {
    return cComponentData.getAssetManager();
  }

  IComponentData& ActionContext::getComponentData() {
    return cComponentData;
  }

  IEventBindings* ActionContext::getBindingRegistry() {
    return &cBindingRegistry;
  }
}

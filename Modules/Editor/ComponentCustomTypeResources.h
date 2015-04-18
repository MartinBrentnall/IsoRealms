/*
 * Copyright 2015 Martin Brentnall
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
#ifndef COMPONENT_CUSTOM_TYPE_RESOURCES_H
#define COMPONENT_CUSTOM_TYPE_RESOURCES_H

#include <IsoRealms/IComponentCustomType.h>
#include <IsoRealms/Resources/IResourceManager.h>

#include "IProjectManager.h"

template <class T> class ComponentCustomTypeResources:public IComponentCustomType,
                                                      public IProjectManagerListener {
  private:
  IResourceManager* cProjectResources;
  IProjectManager* cProjectManager;
  
  public:
  ComponentCustomTypeResources(IResourceManager* projectResources, IProjectManager* projectManager) {
    cProjectResources = projectResources;
    cProjectManager = projectManager;
    cProjectManager->addProjectListener(this);
  }
    
  AbstractRectangularComponent* createComponent(DOMNodeWrapper* node, IResourceAccessor* resources) {
    return new T(node, resources, cProjectResources, cProjectManager);
  }
  
  void projectOpened(IProject* project) {
    cProjectResources = project->getResourceManager();
  }
};

#endif

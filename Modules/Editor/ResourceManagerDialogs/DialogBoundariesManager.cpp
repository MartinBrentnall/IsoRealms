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
#include "DialogBoundariesManager.h"

DialogBoundariesManager::DialogBoundariesManager(IComponentContainer* container, IResourceAccessor* resources, IResourceManager* resourceManager, IProjectManager* projectManager, IResourceLocator* resourceLocator, IEditingContext* editingContext) : Dialog(container, "Modules/Editor/ResourceManagerDialogs/DialogBoundariesManager", resources) {
  projectManager->addProjectListener(this);
  cBoundariesBrowser = new ComponentResourceBrowser<IBoundaries, NamedResourceIcon<IBoundaries> >(resources, resourceManager, projectManager, resourceLocator, editingContext, 0.0f);
  addComponent("boundariesBrowser", cBoundariesBrowser);
  // TODO: Init resources
}

void DialogBoundariesManager::projectOpened(IProject* project) {
  IResourceManager* mResourceManager = project->getResourceManager();
  std::vector<IBoundaries*> mBoundaries = mResourceManager->getAllBoundaries();
  cBoundariesBrowser->initialise(mBoundaries);
}

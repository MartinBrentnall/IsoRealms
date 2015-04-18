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
#include "DialogCustomTypeManager.h"

DialogCustomTypeManager::DialogCustomTypeManager(IComponentContainer* container, IResourceAccessor* resources, IResourceManager* resourceManager, IProjectManager* projectManager, IResourceLocator* resourceLocator, IEditingContext* editingContext) : Dialog(container, "Modules/Editor/ResourceManagerDialogs/DialogCustomTypeManager", resources) {
  projectManager->addProjectListener(this);
  cCustomTypeBrowser = new ComponentResourceBrowser<IComponentCustomType, NamedResourceIcon<IComponentCustomType> >(resources, resourceManager, projectManager, resourceLocator, editingContext, 0.0f);
  addComponent("customComponentBrowser", cCustomTypeBrowser);
  // TODO: Init resources
}

void DialogCustomTypeManager::projectOpened(IProject* project) {
  IResourceManager* mResourceManager = project->getResourceManager();
  std::vector<IComponentCustomType*> mCustomTypes = mResourceManager->getAllComponentCustomTypes();
  cCustomTypeBrowser->initialise(mCustomTypes);
}

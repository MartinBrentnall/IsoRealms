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
#include "DialogElementTypeManager.h"

DialogElementTypeManager::DialogElementTypeManager(IComponentContainer* container, IResourceAccessor* resources, IResourceManager* resourceManager, IProjectManager* projectManager, IResourceLocator* resourceLocator, IEditingContext* editingContext) : Dialog(container, "Modules/Editor/ResourceManagerDialogs/DialogElementTypeManager", resources) {
  projectManager->addProjectListener(this);
  cElementTypeBrowser = new ComponentResourceBrowser<IElementType, ElementTypeIcon>(resources, resourceManager, projectManager, resourceLocator, editingContext, 0.02f);
  addComponent("elementTypeBrowser", cElementTypeBrowser);
  // TODO: Init resources
}

void DialogElementTypeManager::projectOpened(IProject* project) {
  IResourceManager* mResourceManager = project->getResourceManager();
  std::vector<IElementType*> mElementTypes = mResourceManager->getAllElementTypes();
  cElementTypeBrowser->initialise(mElementTypes);
}

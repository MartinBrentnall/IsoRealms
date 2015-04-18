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
#include "DialogFontManager.h"

DialogFontManager::DialogFontManager(IComponentContainer* container, IResourceAccessor* resources, IResourceManager* resourceManager, IProjectManager* projectManager, IResourceLocator* resourceLocator, IEditingContext* editingContext) : Dialog(container, "Modules/Editor/ResourceManagerDialogs/DialogFontManager", resources) {
  projectManager->addProjectListener(this);
  cFontBrowser = new ComponentResourceBrowser<IFont, FontIcon>(resources, resourceManager, projectManager, resourceLocator, editingContext, .02f);
  addComponent("fontBrowser", cFontBrowser);
  // TODO: Init resources
}

void DialogFontManager::projectOpened(IProject* project) {
  IResourceManager* mResourceManager = project->getResourceManager();
  std::vector<IFont*> mFonts = mResourceManager->getAllFonts();
  cFontBrowser->initialise(mFonts);
}

/*
 * Copyright 2025 Martin Brentnall
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
#include "ProjectMenu.h"

namespace IsoRealms {
  ProjectMenu::ProjectMenu(UIManager& manager, IUIStyle& style, Project& project) : ActionMenu(manager, style, ""),
            cProject(project) {
    refresh();
  }

  void ProjectMenu::refresh() {
    clear();
    std::set<std::string> mCategories;
    std::set<IModule*> mModules = cProject.getModules();

    addItem(std::make_unique<MenuItemAction>("Project Configuration", [this]() {
      UIManager& mManager = getUIManager();
      IUIStyle& mStyle = getStyle();
      mManager.openUI(std::make_unique<PropertiesMenu>(mManager, mStyle, [this]() {
        return cProject.getProperties();
      }, "Project Configuration", 1.0f, 1.0f, 1.0f));
    }));

    for (IModule* mModule : mModules) {
      std::vector<IResourceType*> mResourceTypes = mModule->getResourceTypes();
      for (IResourceType* mResourceType : mResourceTypes) {
        mCategories.insert(mResourceType->getCategory());
      }
    }

    UIManager& mUIManager = getUIManager();
    IUIStyle& mStyle = getStyle();
    for (std::string mCategory : mCategories) {
      addItem(std::make_unique<MenuItemAction>(mCategory, [this, mCategory, &mUIManager, &mStyle]() {
        openUI(std::make_unique<CategoryMenu>(mUIManager, mStyle, cProject, mCategory));
      }));
    }
  }
}

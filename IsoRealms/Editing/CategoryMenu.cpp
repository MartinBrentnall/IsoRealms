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
#include "CategoryMenu.h"

#include "IsoRealms/Project.h"
#include "IsoRealms/ResourceType.h"

namespace IsoRealms {
  CategoryMenu::CategoryMenu(UIManager& manager, IUIStyle& style, Project& project, const std::string& category) : ActionMenu(manager, style) {
    std::set<IModule*> mModules = project.getModules();
    for (IModule* mModule : mModules) {
      std::vector<ResourceType*> mResourceTypes = mModule->getResourceTypes();
      for (ResourceType* mResourceType : mResourceTypes) {
        if (category == mResourceType->getCategory()) {
          std::string mResourceTypeName = mResourceType->getPlural();
          std::string mResourceTypeTooltip = mResourceType->getDescription();
          addItem(std::make_unique<MenuItemAction>(mResourceTypeName, mResourceTypeTooltip, [this, &manager, &style, mResourceType]() {
            manager.openUI(std::make_unique<ResourceTypeMenu>(manager, style, *mResourceType), mResourceType->getPlural());
          }));
        }
      }
    }
  }
}


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
#include "ProjectMenu.h"

#include "IsoRealms/Project/Module.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Project/ResourceType.h"

#include "MenuItemAction.h"
#include "MenuItemLoadModule.h"
#include "MenuItemModule.h"

namespace IsoRealms {
  ProjectMenu::ProjectMenu(UIManager& manager, IUIStyle& style, Project& project) : ActionMenu(manager, style),
            cProject(project) {
    refresh();
  }

  void ProjectMenu::refresh() {
    clear();

    UIManager& mManager = getUIManager();
    IUIStyle& mStyle = getStyle();
    const Metadata& mMetadata = cProject.getMetadata();
    addItem(std::make_unique<MenuItemAction>(mMetadata.getPropertyData("ApplicationConfiguration"), [this, &mManager, &mStyle, &mMetadata]() {
      mManager.openUI(std::make_unique<PropertiesMenu>(mManager, mStyle, cProject, [this](IPropertyMaker& owner) {
        cProject.getProperties(owner);
      }), mMetadata.getPropertyData("ApplicationConfiguration").getName());
    }));

    // Build a map of categories within each module.
    std::map<Module*, std::set<std::string>> mCategoriesByModule;  
    for (const std::unique_ptr<Module>& mModule : cProject.getModules()) {
      std::vector<ResourceType*> mResourceTypes = mModule->getResourceTypes();
      for (ResourceType* mResourceType : mResourceTypes) {
        mCategoriesByModule[mModule.get()].insert(mResourceType->getCategory());
      }
    }

    // Add a menu item for each category within each module.
    for (const std::pair<Module* const, std::set<std::string>>& mCategoryByModule : mCategoriesByModule) {
      Module* mModule = mCategoryByModule.first;
      std::string mModuleName = mModule->getName();
      addItem(std::make_unique<MenuItemModule>(mModuleName, mModule->getDescription()));

      // Add a menu item for each category within the module.
      for (const std::string& mCategory : mCategoryByModule.second) {
        if (!mCategory.empty()) {
          addItem(std::make_unique<MenuItemAction>(mCategory, mModule->getCategoryDescription(mCategory), [this, mModule, mCategory, &mManager, &mStyle]() {
            openUI(std::make_unique<CategoryMenu>(mManager, mStyle, cProject, mModule, mCategory), mCategory);
          }, 1));
        }
      }

      // Add a menu item for each resource type within the module that does not have a category.
      std::vector<ResourceType*> mResourceTypes = mModule->getResourceTypes();
      for (ResourceType* mResourceType : mResourceTypes) {
        if (mResourceType->getCategory().empty()) {
          std::string mResourceTypeName = mResourceType->getPlural();
          std::string mResourceTypeTooltip = mResourceType->getDescription();
          addItem(std::make_unique<MenuItemAction>(mResourceTypeName, mResourceTypeTooltip, [this, &mManager, &mStyle, mResourceType]() {
            mManager.openUI(std::make_unique<ResourceTypeMenu>(mManager, mStyle, *mResourceType), mResourceType->getPlural());
          }, 1));
        }
      }
    }

    // Add a menu item to load a module.
    std::vector<std::string> mUnusedModuleNames = cProject.getUnusedModuleNames();
    if (!mUnusedModuleNames.empty()) {
      addItem(std::make_unique<MenuItemLoadModule>(mMetadata.getPropertyData("ApplicationLoadModule"), []() {
        // TODO: Implement this.
      }));
    }
  }
}

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
#include "MenuItemSpacer.h"

namespace IsoRealms {
  ProjectMenu::ProjectMenu(UIManager& manager, IUIStyle& style, Project& project) : ActionMenu(manager, style),
            cProject(project),
            cPropertyMaker(project.getApplication(), project, project.getMetadata(), *this, manager),
            cDefModuleChooser(project) {
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
      addItem(std::make_unique<MenuItemSpacer>(1.0f));
      addItem(std::make_unique<MenuItemModule>(mModule->getLongName(), mModule->getDescription(), [this, mModule]() {
        cProject.unloadModule(mModule->getName());
        refresh();
      }));

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
    addItem(std::make_unique<MenuItemSpacer>(1.0f));
    std::vector<std::string> mUnusedModuleNames = cProject.getUnusedModuleNames();
    if (!mUnusedModuleNames.empty()) {
      cPropertyMaker.createPropertyOptional("Module", cDefModuleChooser, "Load Module...", []() {
        Utils::renderIconAdd();
        return true;
      }, [this](const std::string& value) {
        cProject.loadModule(value);
      });
    }
  }

  void ProjectMenu::addProperty(std::unique_ptr<IProperty> property) {
    addItem(std::make_unique<MenuItemLoadModule>(*this, std::move(property)));
  }

  void ProjectMenu::openProperties(IResourceData& owner, const std::string& name, std::function<void(IPropertyMaker&)> propertyFetcher) {
    throw std::runtime_error("ProjectMenu::openProperties: Not implemented");
  }

  void ProjectMenu::openProperties(IResourceData& owner, const std::string& name, const Metadata& metadata, std::function<void(IPropertyMaker&)> propertyFetcher) {
    throw std::runtime_error("ProjectMenu::openProperties: Not implemented");
  }

  void ProjectMenu::edit(std::unique_ptr<IPropertyEditor> editor) {
    cEditingProperty = std::move(editor);
  }

  void ProjectMenu::edit(IEditable* editor) {
    throw std::runtime_error("ProjectMenu::edit: Not implemented");
  }

  void ProjectMenu::refreshProperties() {
    refresh();
    setLastSelectedItem();
  }

  IUIStyle& ProjectMenu::getPropertyStyle() {
    return getStyle();
  }

  void ProjectMenu::renderOverlay(IMenuItem& item, IUIStyle& style, float y, float aspectRatio) const {
    ActionMenu::renderOverlay(item, style, y, aspectRatio);
    if (cEditingProperty != nullptr) {
      cEditingProperty->render(style, y, -1.0f * aspectRatio + item.getIndentation(style), aspectRatio);
    }
    if (cClosingProperty != nullptr) {
      cClosingProperty->render(style, y, -1.0f * aspectRatio + item.getIndentation(style), aspectRatio);
    }
  }

  void ProjectMenu::updateOverlay(unsigned int milliseconds) {
    ActionMenu::updateOverlay(milliseconds);
    if (cEditingProperty != nullptr) {
      cEditingProperty->update(milliseconds);
    }
    if (cClosingProperty != nullptr) {
      if (cClosingProperty->update(milliseconds)) {
        cClosingProperty = nullptr;
      }
    }
  }

  bool ProjectMenu::input(IMenuItem& item, UISignalID id, float y) {
    if (cEditingProperty != nullptr) {
      if (cEditingProperty->input(id, getStyle())) {
        getUIManager().getProject().updateLater([this]() {
          cClosingProperty = std::move(cEditingProperty);
          cEditingProperty = nullptr;
        });
      }
      return true;
    }
    return ActionMenu::input(item, id, y);
  }

  bool ProjectMenu::input(IMenuItem& item, sf::Event& event) {
    if (cEditingProperty != nullptr) {
      if (cEditingProperty->input(event, getStyle())) {
        getUIManager().getProject().updateLater([this]() {
          cClosingProperty = std::move(cEditingProperty);
          cEditingProperty = nullptr;
          // refreshProperties(); TODO: Doing this nulls out the closing property... need a less brute-force way of refreshing properties (names and values)
        });
      }
    }
    return ActionMenu::input(item, event);
  }
}

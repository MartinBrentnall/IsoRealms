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
#include "ResourceTypeMenu.h"

#include "Property/IPropertyEditor.h"

namespace IsoRealms {
  ResourceTypeMenu::ResourceTypeMenu(UIManager& manager, IUIStyle& style, IResourceType& resourceType) : Menu(manager, style, resourceType.getPlural(), 1.0f, 1.0f, 1.0f),
            cResourceType(resourceType),
            cRemoveButtonOffset(0.0f),
            cDeleteSelected(false) {
    refresh();
  }

  float ResourceTypeMenu::getWidth(MenuItemResource& item, IUIStyle& style) const {
    return cRemoveButtonOffset + style.getFontSize() * 2.0f;
  }

  void ResourceTypeMenu::renderMenuItem(MenuItemResource& item, IUIStyle& style, float y, float aspectRatio) const {
    item.render(style, y, cRemoveButtonOffset, aspectRatio);
  }
  
  void ResourceTypeMenu::renderOverlay(MenuItemResource& item, IUIStyle& style, float y, float aspectRatio) const {
    if (cConfirmSelection != nullptr) {
      cConfirmSelection->render(style);
    }
    if (cClosedConfirmSelection != nullptr) {
      cClosedConfirmSelection->render(style);
    }
  }

  void ResourceTypeMenu::updateOverlay(unsigned int milliseconds) {
    if (cConfirmSelection != nullptr) {
      cConfirmSelection->update(milliseconds);
    }
    if (cClosedConfirmSelection != nullptr) {
      if (cClosedConfirmSelection->update(milliseconds)) {
        cClosedConfirmSelection = nullptr;
      }
    }
  }

  float ResourceTypeMenu::getSelectionHighlightLeft(MenuItemResource& item, IUIStyle& style, float aspectRatio) const {
    return cDeleteSelected && !item.isAddResource() ? -1.0f * aspectRatio + cRemoveButtonOffset + style.getFontSize()
                                                    : -1.0f * aspectRatio;
  }
  
  float ResourceTypeMenu::getSelectionHighlightRight(MenuItemResource& item, IUIStyle& style, float aspectRatio) const {
    return cDeleteSelected && !item.isAddResource() ? -1.0f * aspectRatio + cRemoveButtonOffset + style.getFontSize()
                                                    : -1.0f * aspectRatio + item.getWidth(style);
  }
  
  bool ResourceTypeMenu::input(MenuItemResource& item, UISignalID id, float y) {
    if (cConfirmSelection != nullptr) {
      return cConfirmSelection->input(id);
    } else switch (id) {
      case UISignalID::MOVE_LEFT: {
        cDeleteSelected = false;
        return true;
      }
      
      case UISignalID::MOVE_RIGHT: {
        cDeleteSelected = true;
        return true;
      }
      
      case UISignalID::CONFIRM: {
        if (cDeleteSelected && !item.isAddResource()) {
          cConfirmSelection = std::make_unique<Choice>(getStyle(), "Are you sure you want to delete \"" + item.getResource()->getName()  + "\"?", std::vector<std::string>{"Cancel", "Delete \"" + item.getResource()->getName() + "\""}, [this, &item](const std::string& choice)->bool {
            if (choice == "Delete \"" + item.getResource()->getName() + "\"") {
              IResource* mResource = item.getResource();
              cResourceType.deleteResource(mResource);
              removeItem(item);
            }
            cClosedConfirmSelection = std::move(cConfirmSelection);
            cConfirmSelection = nullptr;
            return true;
          });
          return true;
        }
      }
        
      default: {
        // Nothing to do
      }
    }
    return item.input(id);
  }
  
  bool ResourceTypeMenu::input(MenuItemResource& item, sf::Event& event) {
    return false;
  }

  void ResourceTypeMenu::selectedItemChanged() {
    // Nothing to do.
  }

  void ResourceTypeMenu::refresh() {
    clear();
    std::set<IResource*> mResources = cResourceType.getResources();
    std::vector<IResource*> mSortedResources;
    IUIStyle& mStyle = getStyle();
    float mFontSize = mStyle.getFontSize();

    for (IResource* mResource : mResources) {
      mSortedResources.emplace_back(mResource);
    }
    std::sort(mSortedResources.begin(), mSortedResources.end(), [](IResource* a, IResource* b) {return a->getName().compare(b->getName()) < 0;});

    for (IResource* mResource : mSortedResources) {
      addResource(mResource);
    }

    std::unique_ptr<MenuItemResource> mMenuItem = std::make_unique<MenuItemResource>("[New " + cResourceType.getSingular() + "...]", nullptr, [this](IResource* resource) {
      IResource* mCreatedResource = cResourceType.createResource();
      std::cout << "CREATING RESOURCE..." << mCreatedResource << std::endl;
      openResourcePropertiesMenu(mCreatedResource);
    }, [](IResource* resource) {
      return true;
    });
    cRemoveButtonOffset = std::max(cRemoveButtonOffset, mMenuItem->getWidth(mStyle) + mFontSize * 2.25f);
    addItem(std::move(mMenuItem));
  }
  
  void ResourceTypeMenu::addResource(IResource* resource) {
    std::string mResourceName = resource->getName();
    std::unique_ptr<MenuItemResource> mMenuItem = std::make_unique<MenuItemResource>(mResourceName, resource, [this](IResource* resource) {
      openResourcePropertiesMenu(resource);
    }, [](IResource* resource) {
      return resource->renderIcon();
    });
    IUIStyle& mStyle = getStyle();
    float mFontSize = mStyle.getFontSize();
    cRemoveButtonOffset = std::max(cRemoveButtonOffset, mMenuItem->getWidth(mStyle) + mFontSize * 2.25f);
    addItem(std::move(mMenuItem));
  }
  
  void ResourceTypeMenu::openResourcePropertiesMenu(IResource* resource) {
    UIManager& mManager = getUIManager();
    IUIStyle& mStyle = getStyle();
    std::string mResourceName = resource->getName();
    mManager.openUI(std::make_unique<PropertiesMenu>(mManager, mStyle, [this, resource]() {
      return resource->getProperties();
    }, mResourceName, 1.0f, 1.0f, 0.5f));
  }
}

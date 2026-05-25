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

#include "IsoRealms/Project/ResourceType.h"

#include "Property/IPropertyEditor.h"

namespace IsoRealms {
  ResourceTypeMenu::ResourceTypeMenu(UIManager& manager, IUIStyle& style, ResourceType& resourceType) : Menu(manager, style),
            cResourceType(resourceType),
            cRemoveButtonOffset(0.0f),
            cDeleteSelected(false) {
    refresh();
  }

  float ResourceTypeMenu::getWidth(MenuItemResource& item, IUIStyle& style) const {
    return cRemoveButtonOffset + style.getFontSize() * 2.0f;
  }

  float ResourceTypeMenu::getHeight(MenuItemResource& item, IUIStyle& style) const {
    return style.getFontSize() * 2.0f;
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
        if (cDeleteSelected) {
          if (item.isResource()) {
            cConfirmSelection = std::make_unique<Choice>(getStyle(), "Are you sure you want to delete \"" + item.getResource()->getName()  + "\"?", std::vector<std::string>{"Cancel", "Delete \"" + item.getResource()->getName() + "\""}, [this, item](const std::string& choice)->bool {
              if (choice == "Delete \"" + item.getResource()->getName() + "\"") {
                IResource* mResource = item.getResource();
                if (mResource->hasReadOnlyReferences()) {
                  cClosedConfirmSelection = std::move(cConfirmSelection);
                  cConfirmSelection = std::make_unique<Choice>(getStyle(), "This resource is referenced by read-only resources.  Deleting it will promote any read-only resources referencing this one and make them writable.", std::vector<std::string>{"Cancel", "Delete \"" + item.getResource()->getName() + "\""}, [this, item](const std::string& choice)->bool {
                    if (choice == "Delete \"" + item.getResource()->getName() + "\"") {
                      IResource* mResource = item.getResource();
                      mResource->overrideReadOnlyReferences();
                      cResourceType.deleteResource(mResource);
                      refresh();
                    }
                    cClosedConfirmSelection = std::move(cConfirmSelection);
                    cConfirmSelection = nullptr;
                    return true;
                  });
                } else {
                  cClosedConfirmSelection = std::move(cConfirmSelection);
                  cConfirmSelection = nullptr;
                  cResourceType.deleteResource(mResource);
                  refresh();
                }
              } else {
                cClosedConfirmSelection = std::move(cConfirmSelection);
                cConfirmSelection = nullptr;
              }
              return true;
            });
          } else if (item.isPlaceHolder()) {
            cResourceType.reloadResource(item.getLabel());
            refresh();
          }
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

  void ResourceTypeMenu::selectedItemChanged(MenuItemResource& item) {
    // Nothing to do.
  }

  bool ResourceTypeMenu::isSelectable(MenuItemResource& item) const {
    return true;
  }

  void ResourceTypeMenu::refresh() {
    
    // Start fresh.
    clear();
    
    // Put all the resources a single container.
    std::vector<std::variant<IResource*, std::string>> mSortedResources;
    cResourceType.forEachResource([&mSortedResources](IResource* resource) {
      mSortedResources.emplace_back(resource);
      return true;
    });
    
    // Also add deleted resources to the container.
    const std::set<std::string> mDeletedResources = cResourceType.getDeletedResources();
    for (const std::string& mDeletedResource : mDeletedResources) {
      mSortedResources.emplace_back(mDeletedResource);
    }

    // Create a function to retrieve the name of a resource (whether a real or deleted one).
    std::function<std::string(const std::variant<IResource*, std::string>&)> mGetResourceNameFunction = [](const std::variant<IResource*, std::string>& variant) -> std::string {
      return std::visit([](auto&& val) -> std::string {
        using TYPE = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<TYPE, IResource*>) {
          return val->getName();
        } else {
          return val;
        }
      }, variant);
    };

    // Sort the container with the resource names.
    std::sort(mSortedResources.begin(), mSortedResources.end(), [&mGetResourceNameFunction](const std::variant<IResource*, std::string>& a, const std::variant<IResource*, std::string>& b) {
      return Utils::naturalCompare(mGetResourceNameFunction(a), mGetResourceNameFunction(b));
    });

    // Create menu items for each resource and deleted resource.
    for (std::variant<IResource*, std::string> const& variant : mSortedResources) {
      std::visit([this](auto const &val) {
        using TYPE = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<TYPE, IResource*>) {
          addResource(val);
        } else {
          addDeletedResource(val);
        }
      }, variant);
    }

    // Create a final menu item that allows creation of a new resource.
    std::unique_ptr<MenuItemResource> mCreateResourceMenuItem = std::make_unique<MenuItemResource>("[New " + cResourceType.getSingular() + "...]", [this](IResource* resource) {
      IResource* mCreatedResource = cResourceType.createResource();
      openResourcePropertiesMenu(mCreatedResource);
    }, [](IResource* resource) {
      return true;
    });
    
    // Expand the "remove resource" button offset (for all items) if necessary.
    IUIStyle& mStyle = getStyle();
    float mFontSize = mStyle.getFontSize();
    cRemoveButtonOffset = std::max(cRemoveButtonOffset, mCreateResourceMenuItem->getWidth(mStyle) + mFontSize * 2.25f);
    
    // Add the final item.
    addItem(std::move(mCreateResourceMenuItem));
  }
  
  void ResourceTypeMenu::addResource(IResource* resource) {
    std::unique_ptr<MenuItemResource> mResourceMenuItem = std::make_unique<MenuItemResource>(resource, [this](IResource* resource) {
      openResourcePropertiesMenu(resource);
    }, [](IResource* resource) {
      return resource->renderIcon();
    });
    
    // Expand the "remove resource" button offset (for all items) if necessary.
    IUIStyle& mStyle = getStyle();
    float mFontSize = mStyle.getFontSize();
    cRemoveButtonOffset = std::max(cRemoveButtonOffset, mResourceMenuItem->getWidth(mStyle) + mFontSize * 2.25f);
    
    // Add the resource menu item.
    addItem(std::move(mResourceMenuItem));
  }
  
  void ResourceTypeMenu::addDeletedResource(const std::string& resource) {
    std::unique_ptr<MenuItemResource> mDeletedResourceMenuItem = std::make_unique<MenuItemResource>(resource, nullptr, [](IResource* resource) {
      Utils::renderIconNone();
      return true;
    });
    
    // Expand the "remove resource" button offset (for all items) if necessary.
    IUIStyle& mStyle = getStyle();
    float mFontSize = mStyle.getFontSize();
    cRemoveButtonOffset = std::max(cRemoveButtonOffset, mDeletedResourceMenuItem->getWidth(mStyle) + mFontSize * 2.25f);
    
    // Add the deleted resource menu item.
    addItem(std::move(mDeletedResourceMenuItem));
  }
  
  void ResourceTypeMenu::openResourcePropertiesMenu(IResource* resource) {
    UIManager& mManager = getUIManager();
    IUIStyle& mStyle = getStyle();
    std::string mResourceName = resource->getName();
    mManager.openUI(std::make_unique<PropertiesMenu>(mManager, mStyle, resource->getResourceData(), [this, resource](IPropertyMaker& owner) {
      resource->getProperties(owner);
    }), mResourceName, LiteralColour(1.0f, 1.0f, 0.5f));
  }
}

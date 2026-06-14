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
#include "ComponentTypeMenu.h"

#include "IsoRealms/Project/ComponentType.h"

#include "Property/IPropertyEditor.h"

namespace IsoRealms {
  ComponentTypeMenu::ComponentTypeMenu(UIManager& manager, IUIStyle& style, ComponentType& resourceType) :
            Menu(manager, style),
            cComponentType(resourceType) {
    refresh();
  }

  float ComponentTypeMenu::getWidth(MenuItemComponent& item, IUIStyle& style) const {
    return cRemoveButtonOffset + style.getFontSize() * 2.0f;
  }

  float ComponentTypeMenu::getHeight(MenuItemComponent& item, IUIStyle& style) const {
    return style.getFontSize() * 2.0f;
  }

  void ComponentTypeMenu::renderMenuItem(MenuItemComponent& item, IUIStyle& style, float y, float aspectRatio) const {
    item.render(style, y, cRemoveButtonOffset, aspectRatio);
  }
  
  void ComponentTypeMenu::renderOverlay(MenuItemComponent& item, IUIStyle& style, float y, float aspectRatio) const {
    if (cConfirmSelection != nullptr) {
      cConfirmSelection->render(style);
    }
    if (cClosedConfirmSelection != nullptr) {
      cClosedConfirmSelection->render(style);
    }
  }

  void ComponentTypeMenu::updateOverlay(unsigned int milliseconds) {
    if (cConfirmSelection != nullptr) {
      cConfirmSelection->update(milliseconds);
    }
    if (cClosedConfirmSelection != nullptr) {
      if (cClosedConfirmSelection->update(milliseconds)) {
        cClosedConfirmSelection = nullptr;
      }
    }
  }

  float ComponentTypeMenu::getSelectionHighlightLeft(MenuItemComponent& item, IUIStyle& style, float aspectRatio) const {
    return cDeleteSelected && !item.isAddComponent() ? -1.0f * aspectRatio + cRemoveButtonOffset + style.getFontSize()
                                                    : -1.0f * aspectRatio;
  }
  
  float ComponentTypeMenu::getSelectionHighlightRight(MenuItemComponent& item, IUIStyle& style, float aspectRatio) const {
    return cDeleteSelected && !item.isAddComponent() ? -1.0f * aspectRatio + cRemoveButtonOffset + style.getFontSize()
                                                    : -1.0f * aspectRatio + item.getWidth(style);
  }
  
  bool ComponentTypeMenu::input(MenuItemComponent& item, UISignalID id, float y) {
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
          if (item.isComponent()) {
            cConfirmSelection = std::make_unique<Choice>(getStyle(), "Are you sure you want to delete \"" + item.getComponent()->getName()  + "\"?", std::vector<std::string>{"Cancel", "Delete \"" + item.getComponent()->getName() + "\""}, [this, item](const std::string& choice)->bool {
              if (choice == "Delete \"" + item.getComponent()->getName() + "\"") {
                IComponent* mComponent = item.getComponent();
                if (mComponent->hasReadOnlyReferences()) {
                  cClosedConfirmSelection = std::move(cConfirmSelection);
                  cConfirmSelection = std::make_unique<Choice>(getStyle(), "This component is referenced by read-only components.  Deleting it will promote any read-only components referencing this one and make them writable.", std::vector<std::string>{"Cancel", "Delete \"" + item.getComponent()->getName() + "\""}, [this, item](const std::string& choice)->bool {
                    if (choice == "Delete \"" + item.getComponent()->getName() + "\"") {
                      IComponent* mComponent = item.getComponent();
                      mComponent->overrideReadOnlyReferences();
                      cComponentType.deleteComponent(mComponent);
                      refresh();
                    }
                    cClosedConfirmSelection = std::move(cConfirmSelection);
                    cConfirmSelection = nullptr;
                    return true;
                  });
                } else {
                  cClosedConfirmSelection = std::move(cConfirmSelection);
                  cConfirmSelection = nullptr;
                  cComponentType.deleteComponent(mComponent);
                  refresh();
                }
              } else {
                cClosedConfirmSelection = std::move(cConfirmSelection);
                cConfirmSelection = nullptr;
              }
              return true;
            });
          } else if (item.isPlaceHolder()) {
            cComponentType.reloadComponent(item.getLabel());
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
  
  bool ComponentTypeMenu::input(MenuItemComponent& item, sf::Event& event) {
    return false;
  }

  void ComponentTypeMenu::selectedItemChanged(MenuItemComponent& item) {
    // Nothing to do.
  }

  bool ComponentTypeMenu::isSelectable(MenuItemComponent& item) const {
    return true;
  }

  void ComponentTypeMenu::refresh() {
    
    // Start fresh.
    clear();
    
    // Put all the components in a single container.
    std::vector<std::variant<IComponent*, std::string>> mSortedComponents;
    cComponentType.forEachComponent([&mSortedComponents](IComponent* component) {
      mSortedComponents.emplace_back(component);
      return true;
    });
    
    // Also add deleted components to the container.
    const std::set<std::string> mDeletedComponents = cComponentType.getDeletedComponents();
    for (const std::string& mDeletedComponent : mDeletedComponents) {
      mSortedComponents.emplace_back(mDeletedComponent);
    }

    // Create a function to retrieve the name of a resource (whether a real or deleted one).
    std::function<std::string(const std::variant<IComponent*, std::string>&)> mGetComponentNameFunction = [](const std::variant<IComponent*, std::string>& variant) -> std::string {
      return std::visit([](auto&& val) -> std::string {
        using TYPE = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<TYPE, IComponent*>) {
          return val->getName();
        } else {
          return val;
        }
      }, variant);
    };

    // Sort the container with the resource names.
    std::sort(mSortedComponents.begin(), mSortedComponents.end(), [&mGetComponentNameFunction](const std::variant<IComponent*, std::string>& a, const std::variant<IComponent*, std::string>& b) {
      return Utils::naturalCompare(mGetComponentNameFunction(a), mGetComponentNameFunction(b));
    });

    // Create menu items for each resource and deleted resource.
    for (std::variant<IComponent*, std::string> const& variant : mSortedComponents) {
      std::visit([this](auto const &val) {
        using TYPE = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<TYPE, IComponent*>) {
          addComponent(val);
        } else {
          addDeletedComponent(val);
        }
      }, variant);
    }

    // Create a final menu item that allows creation of a new resource.
    std::unique_ptr<MenuItemComponent> mCreateResourceMenuItem = std::make_unique<MenuItemComponent>("[New " + cComponentType.getSingular() + "...]", [this](IComponent* component) {
      IComponent* mCreatedComponent = cComponentType.createComponent();
      openComponentPropertiesMenu(mCreatedComponent);
    }, [](IComponent* component) {
      Utils::renderIconAdd();
      return true;
    });
    
    // Expand the "remove resource" button offset (for all items) if necessary.
    IUIStyle& mStyle = getStyle();
    float mFontSize = mStyle.getFontSize();
    cRemoveButtonOffset = std::max(cRemoveButtonOffset, mCreateResourceMenuItem->getWidth(mStyle) + mFontSize * 2.25f);
    
    // Add the final item.
    addItem(std::move(mCreateResourceMenuItem));
  }
  
  void ComponentTypeMenu::addComponent(IComponent* component) {
    std::unique_ptr<MenuItemComponent> mComponentMenuItem = std::make_unique<MenuItemComponent>(component, [this, component](IComponent* selectedComponent) {
      openComponentPropertiesMenu(component);
    }, [component](IComponent* selectedComponent) {
      return component->renderIcon();
    });
    
    // Expand the "remove resource" button offset (for all items) if necessary.
    IUIStyle& mStyle = getStyle();
    float mFontSize = mStyle.getFontSize();
    cRemoveButtonOffset = std::max(cRemoveButtonOffset, mComponentMenuItem->getWidth(mStyle) + mFontSize * 2.25f);
    
    // Add the resource menu item.
    addItem(std::move(mComponentMenuItem));
  }
  
  void ComponentTypeMenu::addDeletedComponent(const std::string& componentName) {
    std::unique_ptr<MenuItemComponent> mDeletedComponentMenuItem = std::make_unique<MenuItemComponent>(componentName, nullptr, [](IComponent* component) {
      Utils::renderIconNone();
      return true;
    });
    
    // Expand the "remove resource" button offset (for all items) if necessary.
    IUIStyle& mStyle = getStyle();
    float mFontSize = mStyle.getFontSize();
    cRemoveButtonOffset = std::max(cRemoveButtonOffset, mDeletedComponentMenuItem->getWidth(mStyle) + mFontSize * 2.25f);
    
    // Add the deleted resource menu item.
    addItem(std::move(mDeletedComponentMenuItem));
  }
  
  void ComponentTypeMenu::openComponentPropertiesMenu(IComponent* component) {
    UIManager& mManager = getUIManager();
    IUIStyle& mStyle = getStyle();
    std::string mComponentName = component->getName();
    mManager.openUI(std::make_unique<PropertiesMenu>(mManager, mStyle, component->getComponentData(), [component](IComponentDefiner& definer) {
      component->define(definer);
    }), mComponentName, LiteralColour(1.0f, 1.0f, 0.5f));
  }
}

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
#pragma once

#include <memory>

#include "IsoRealms/IComponent.h"

#include "Choice.h"
#include "Menu.h"
#include "MenuItemComponent.h"
#include "PropertiesMenu.h"

namespace IsoRealms {
  class ComponentType;

  class ComponentTypeMenu : public Menu<MenuItemComponent> {
    public:
    ComponentTypeMenu(UIManager& manager, IUIStyle& style, ComponentType& resourceType);

    /**************************************\
     * Implements Menu<MenuItemComponent> *
    \**************************************/
    float getWidth(MenuItemComponent& item, IUIStyle& style) const override;
    float getHeight(MenuItemComponent& item, IUIStyle& style) const override;
    void renderMenuItem(MenuItemComponent& item, IUIStyle& style, float y, float aspectRatio) const override;
    void renderOverlay(MenuItemComponent& item, IUIStyle& style, float y, float aspectRatio) const override;
    void updateOverlay(unsigned int milliseconds) override;
    float getSelectionHighlightLeft(MenuItemComponent& item, IUIStyle& style, float aspectRatio) const override;
    float getSelectionHighlightRight(MenuItemComponent& item, IUIStyle& style, float aspectRatio) const override;
    bool input(MenuItemComponent& item, UISignalID id, float y) override;
    bool input(MenuItemComponent& item, sf::Event& event) override;
    void selectedItemChanged(MenuItemComponent& item) override;
    bool isSelectable(MenuItemComponent& item) const override;
    void refresh() override;

    private:
    ComponentType& cComponentType;
    float cRemoveButtonOffset;
    bool cDeleteSelected;

    std::unique_ptr<Choice> cConfirmSelection;
    std::unique_ptr<Choice> cClosedConfirmSelection;

    void addComponent(IComponent* component);
    void addDeletedComponent(const std::string& componentName);
    void openComponentPropertiesMenu(IComponent* component);
  };
}

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

#include "IsoRealms/IResource.h"

#include "Choice.h"
#include "Menu.h"
#include "MenuItemResource.h"
#include "PropertiesMenu.h"

namespace IsoRealms {
  class ResourceType;

  class ResourceTypeMenu : public Menu<MenuItemResource> {
    public:
    ResourceTypeMenu(UIManager& manager, IUIStyle& style, ResourceType& resourceType);

    /*************************************\
     * Implements Menu<MenuItemResource> *
    \*************************************/
    float getWidth(MenuItemResource& item, IUIStyle& style) const override;
    float getHeight(MenuItemResource& item, IUIStyle& style) const override;
    void renderMenuItem(MenuItemResource& item, IUIStyle& style, float y, float aspectRatio) const override;
    void renderOverlay(MenuItemResource& item, IUIStyle& style, float y, float aspectRatio) const override;
    void updateOverlay(unsigned int milliseconds) override;
    float getSelectionHighlightLeft(MenuItemResource& item, IUIStyle& style, float aspectRatio) const override;
    float getSelectionHighlightRight(MenuItemResource& item, IUIStyle& style, float aspectRatio) const override;
    bool input(MenuItemResource& item, UISignalID id, float y) override;
    bool input(MenuItemResource& item, sf::Event& event) override;
    void selectedItemChanged() override;
    bool isSelectable(MenuItemResource& item) const override;
    void refresh() override;

    private:
    ResourceType& cResourceType;
    float cRemoveButtonOffset;
    bool cDeleteSelected;

    std::unique_ptr<Choice> cConfirmSelection;
    std::unique_ptr<Choice> cClosedConfirmSelection;

    void addResource(IResource* resource);
    void addDeletedResource(const std::string& resource);
    void openResourcePropertiesMenu(IResource* resource);
  };
}

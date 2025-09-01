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
#include <set>
#include <vector>

#include "IsoRealms/IModule.h"

#include "Menu.h"
#include "MenuItemAction.h"

namespace IsoRealms {
  class ActionMenu : public Menu<MenuItemAction> {
    public:
    ActionMenu(UIManager& manager, IUIStyle& style);

    /***********************************\
     * Implements Menu<MenuItemAction> *
    \***********************************/
    float getWidth(MenuItemAction& item, IUIStyle& style) const override;
    void renderMenuItem(MenuItemAction& item, IUIStyle& style, float y, float aspectRatio) const override;
    void renderOverlay(MenuItemAction& item, IUIStyle& style, float y, float aspectRatio) const override;
    void updateOverlay(unsigned int milliseconds) override;
    float getSelectionHighlightLeft(MenuItemAction& item, IUIStyle& style, float aspectRatio) const override;
    float getSelectionHighlightRight(MenuItemAction& item, IUIStyle& style, float aspectRatio) const override;
    bool input(MenuItemAction& item, UISignalID id, float y) override;
    bool input(MenuItemAction& item, sf::Event& event) override;
    void selectedItemChanged() override;
    void refresh() override;
  };
}

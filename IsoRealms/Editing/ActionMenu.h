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

#include "Menu.h"
#include "IMenuItem.h"

namespace IsoRealms {
  class ActionMenu : public Menu<IMenuItem> {
    public:
    ActionMenu(UIManager& manager, IUIStyle& style);

    /******************************\
     * Implements Menu<IMenuItem> *
    \******************************/
    float getWidth(IMenuItem& item, IUIStyle& style) const override;
    void renderMenuItem(IMenuItem& item, IUIStyle& style, float y, float aspectRatio) const override;
    void renderOverlay(IMenuItem& item, IUIStyle& style, float y, float aspectRatio) const override;
    void updateOverlay(unsigned int milliseconds) override;
    float getSelectionHighlightLeft(IMenuItem& item, IUIStyle& style, float aspectRatio) const override;
    float getSelectionHighlightRight(IMenuItem& item, IUIStyle& style, float aspectRatio) const override;
    bool input(IMenuItem& item, UISignalID id, float y) override;
    bool input(IMenuItem& item, sf::Event& event) override;
    void selectedItemChanged() override;
    void refresh() override;
  };
}

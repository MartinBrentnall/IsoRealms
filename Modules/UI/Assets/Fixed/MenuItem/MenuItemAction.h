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

#include <GL/glew.h>
#include <string>

#include "IsoRealms.h"

#include "Modules/UI/Assets/Type/IMenuItem.h"

namespace IsoRealms::UI {

  /**
   * Menu item that triggers an action upon confirmation.
   */
  class MenuItemAction final : public IMenuItem {
    public:
    MenuItemAction(IProject& project, Menu& menu);
    MenuItemAction(IProject& project, Menu& menu, JSONObject object);

    /***********************\
     * Scripting Interface *
    \***********************/
    void setValue(const std::string& value);

    /************************\
     * Implements IMenuItem *
    \************************/
    void registerAssets(ResourceAssetRegistry& assets) override;
    void reset() override;
    bool input(sf::Event& event) override;
    void selectTop() override;
    void selectBottom() override;
    void render(float aspectRatio, float y, bool selected, const Menu& menu) const override;
    float getHeight(const Menu& menu) const override;
    float getSelectedY(const Menu& menu) const override;

    /***********************************\
     * Implements IAsset via IMenuItem *
    \***********************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    static const std::string JSON_ID;
    static const std::string JSON_LABEL;
    static const std::string JSON_ON_SELECTION;

    // Constants.
    static const std::string BINDING_TYPE;
    
    // Definition data.
    std::string cDefID;    /// ID of this menu item for binding.
    std::string cDefLabel; /// Label to show for this menu item.
    Action cDefAction;     /// Action that confirming this menu item will trigger.
    
    // Runtime data.
    std::string cRuntimeValue; /// TODO: What's this used for?
    
    // Scripting support.
    LuaBinding<MenuItemAction> cLuaBinding; /// Allows menu item actions to be bound to lua variables.
  };
}

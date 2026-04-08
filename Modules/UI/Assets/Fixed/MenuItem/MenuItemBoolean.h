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
#include <GL/gl.h>

#include "IsoRealms.h"

#include "Modules/UI/Assets/Type/IMenuItem.h"

namespace IsoRealms::UI {
  
  /**
   * Menu item that represents a boolean option or setting.
   */
  class MenuItemBoolean final : public IMenuItem {
    public:
    MenuItemBoolean(const Metadata& metadata, Menu& menu);
    MenuItemBoolean(const Metadata& metadata, Menu& menu, JSONObject object);

    /***********************\
     * Scripting interface *
    \***********************/
    void setValue(bool value);
    bool getValue();
    
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
    std::string getTreeItemLabel() const override;

    /***********************************\
     * Implements IAsset via IMenuItem *
    \***********************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.
    static const std::string JSON_FALSE_LABEL;
    static const std::string JSON_ID;
    static const std::string JSON_LABEL;
    static const std::string JSON_TRUE_LABEL ;
    static const std::string JSON_TYPE;

    // Constants.
    static const std::string BINDING_TYPE;

    // External interfaces.
    const Metadata& cMetadata;
    HatHandler& cHatHandler;

    // Definition data.
    std::string cDefID;         /// ID of this menu item for binding.
    std::string cDefLabel;      /// Label to show for this menu item.
    std::string cDefLabelFalse; /// Label to show when the value of this menu item is false.
    std::string cDefLabelTrue;  /// Label to show when the value of this menu item is true.
    
    // Runtime data.
    bool cRuntimeValue; /// Current value of this item.

    // Scripting support.
    LuaBinding<MenuItemBoolean> cLuaBinding; /// Allows boolean settings to be bound to lua variables.
  };
}

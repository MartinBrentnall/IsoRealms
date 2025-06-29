/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <algorithm>

#include <GL/glew.h>
#include <GL/gl.h>

#include "IsoRealms/DisplayResolution.h"
#include "IsoRealms/IApplication.h"
#include "IsoRealms/IAssetRegistry.h"
#include "IsoRealms/IAssetRemover.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/ProjectCallbackManager.h"
#include "IsoRealms/System.h"

#include "Modules/UI/Assets/Type/IMenuItem.h"

namespace IsoRealms::UI {
  
  /**
   * Menu item that represents a list of display resolutions that are supported
   * in full screen mode.
   */
  class MenuItemDisplayResolution final : public IMenuItem {
    public:
    MenuItemDisplayResolution(IProject& project, Menu& menu);
    MenuItemDisplayResolution(IProject& project, Menu& menu, JSONObject object);

    /***********************\
     * Scripting Interface *
    \***********************/
    void setValue(DisplayResolution resolution);
    DisplayResolution getValue();
    
    /************************\
     * Implements IMenuItem *
    \************************/
    void registerAssets(IAssetRegistry& assets) override;
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
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.
    static const std::string JSON_ID;
    static const std::string JSON_LABEL;
    static const std::string JSON_TYPE;

    // Constants.
    static const std::string BINDING_TYPE;

    // External interfaces.
    ProjectCallbackManager cProjectCallbackManager;

    // Definition data.
    HatHandler& cHatHandler;
    std::string cDefID;    /// ID of this menu item for binding.
    std::string cDefLabel; /// Label to show for this menu item. 
    
    // Runtime data.
    std::vector<DisplayResolution> cRuntimeResolutions; /// List of resolutions that can be selected.
    unsigned int cRuntimeSelectedResolution;            /// Index of the selected resolution.

    // Scripting support.
    LuaBinding<MenuItemDisplayResolution> cLuaBinding; /// Allows resolution menu items to be bound to lua variables.
    
    // Private functions.
    unsigned int getIndex(const DisplayResolution& resolution) const;
    bool left();
    bool right();
  };
}

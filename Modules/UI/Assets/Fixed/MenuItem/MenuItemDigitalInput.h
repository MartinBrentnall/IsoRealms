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

#include <GL/glew.h>
#include <string>

#include "IsoRealms/IAssetRegistry.h"
#include "IsoRealms/IAssetRemover.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Input/AxisMapping.h"
#include "IsoRealms/Input/ButtonMapping.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/Input/HatMapping.h"
#include "IsoRealms/Input/IDigitalInputMapping.h"
#include "IsoRealms/Input/KeyMapping.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

#include "Modules/UI/Assets/Type/IMenuItem.h"

namespace IsoRealms::UI {
  
  /**
   * Menu item that represents a digital input with configurable mappings.
   */
  class MenuItemDigitalInput final : public IMenuItem {
    public:
    MenuItemDigitalInput(IProject* project, Menu* menu, JSONObject object);

    /***********************\
     * Scripting Interface *
    \***********************/
    void addMapping(std::shared_ptr<IDigitalInputMapping> input);
    void clear();
    unsigned int getMappingCount();
    std::shared_ptr<IDigitalInputMapping> getMapping(unsigned int index);
    
    /************************\
     * Implements IMenuItem *
    \************************/
    void registerAssets(IAssetRegistry* assets) override;
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser) override;
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

    private:
    
    // JSON members.
    static const std::string JSON_ID;
    static const std::string JSON_TYPE;

    // Constants.
    static const std::string BINDING_TYPE;

    // Definition data.
    HatHandler& cHatHandler;
    std::string cDefID; /// ID of this menu item for binding.
    
    // Runtime data.
    std::vector<std::shared_ptr<IDigitalInputMapping>> cRuntimeMappings; /// Mappings applied to this digital input.
    unsigned int cRuntimeSelectedMapping;                                /// Index of the selected mapping.
    bool cRuntimeAddingMapping;                                          /// True when awaiting user input from which to apply a new mapping.

    // Scripting support.
    LuaBinding<MenuItemDigitalInput> cLuaBinding; /// Allows digital input settings to be bound to lua variables.
    
    // Private functions.
    bool up();
    bool down();
    bool remove();
    bool confirm();
  };
}

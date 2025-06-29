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
#include <GL/gl.h>

#include "IsoRealms/IAssetRegistry.h"
#include "IsoRealms/IAssetRemover.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/ProjectCallbackManager.h"
#include "IsoRealms/Types.h"

#include "Modules/UI/Assets/Type/IMenuItem.h"

namespace IsoRealms::UI {
  
  /**
   * Menu item that represents a slider setting.
   */
  class MenuItemSlider final : public IMenuItem {
    public:
    MenuItemSlider(IProject& project, Menu& menu);
    MenuItemSlider(IProject& project, Menu& menu, JSONObject object);

    /***********************\
     * Scripting Interface *
    \***********************/
    void setValue(float value);
    float getValue();

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
    static const std::string JSON_MAXIMUM;
    static const std::string JSON_MINIMUM;
    static const std::string JSON_ON_CHANGE;
    static const std::string JSON_STEPS;
    static const std::string JSON_TYPE;

    // Constants.
    static const std::string BINDING_TYPE;
    
    static const float DEFAULT_MAXIMUM;
    static const float DEFAULT_MINIMUM;
    static const int DEFAULT_STEPS;

    // External interfaces.
    ProjectCallbackManager cProjectCallbackManager;
    HatHandler& cHatHandler;

    // Definition data.
    std::string cDefID;            /// ID of this menu item for binding.
    std::string cDefLabel;         /// Label to show for this menu item.
    float cDefMinimum;             /// Minimum allowed value of this slider.
    float cDefMaximum;             /// Maximum allowed value of this slider.
    int cDefSteps;                 /// Number of steps within this slider.
    Action cDefValueChangedAction; /// Action that changing the value of the slider will trigger.
    
    // Runtime data.
    float cRuntimeValue; /// Current value of this slider.

    // Scripting support.
    LuaBinding<MenuItemSlider> cLuaBinding; /// Allows sliders to be bound to lua variables.

    // Private functions.
    void adjustValue(float amount);
  };
}

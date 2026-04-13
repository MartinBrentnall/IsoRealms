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

#include <string>
#include <vector>

#include "IsoRealms.h"

#include "Modules/UI/Assets/Client/MenuItem.h"

namespace IsoRealms::UI {
  class UI;
  
  /**
   * Resource definition for a menu that can be configured to provide various
   * settings and actions.
   */
  class Menu final : public IInputHandler,
                     public IScreen {
    public:
    
    /**********************\
     * Resource Interface *
    \**********************/
    Menu(UI& ui, IResourceData& data);
    Menu(UI& ui, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);  
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    /*********************\
     * Module interfaces *
    \*********************/
    void updateRuntime(unsigned int milliseconds);
    void reset();

    // Interface to be used by menu items.
    IResourceData& getResourceData();
    const IResourceData& getResourceData() const;
    const Font& getFont() const;
    float getFontSize() const;
    float getShadowOffset() const;
    const Colour& getSelectionColour() const;

    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event& event) override;
    void resetInput() override;
    
    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;

    /*********************\
     * Implements IAsset *
    \*********************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.
    static const std::string JSON_COLOUR;
    static const std::string JSON_FONT;
    static const std::string JSON_FONT_SIZE;
    static const std::string JSON_ITEM;
    static const std::string JSON_ON_EXIT;
    static const std::string JSON_OPTIONS;
    static const std::string JSON_SHADOW_OFFSET;
    static const std::string JSON_TYPE;

    static const float DEFAULT_FONT_SIZE;
    static const float DEFAULT_SHADOW_OFFSET;

    // External interfaces.
    IResourceData& cResourceData;

    // Definition data.
    HatHandler& cHatHandler;
    std::vector<std::unique_ptr<MenuItem>> cDefItems; /// Items defined in this menu.
    Action cDefExitAction;                            /// Action to perform when leaving this menu.
    Font cDefFont;                                    /// Font used for rendering items in this menu.
    Colour cDefColour;                                /// Colour used for highlighting selections in this menu.
    float cDefFontSize;                               /// Size of fonts in this menu.
    float cDefShadowOffset;                           /// Shadow offset for rendering items in this menu.
    
    // Runtime data.
    unsigned int cRuntimeSelectedItem; /// Current selected menu item index.
    float cRuntimeScroll;              /// Value used for animating menu scrolling.
    
    // Scripting support.
    LuaBinding<Menu> cLuaBinding; /// Allow menus to be bound to lua variables.

    // Private functions.    
    void down();
    void up();
  };
}

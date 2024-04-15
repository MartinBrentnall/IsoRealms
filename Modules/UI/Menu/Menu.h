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

#include <string>
#include <vector>

#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

#include "Items/MenuItemAction.h"
#include "Items/MenuItemBoolean.h"
#include "Items/MenuItemDigitalInput.h"
#include "Items/MenuItemDisplayResolution.h"
#include "Items/MenuItemFileList.h"
#include "Items/MenuItemSlider.h"
#include "IMenuItem.h"

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
    Menu(IProject* project, UI* ui);
    Menu(IProject* project, UI* ui, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);  
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    // Interface to be used by menu items.
    const Font& getFont() const;
    float getFontSize() const;
    float getShadowOffset() const;
    const Colour& getSelectionColour() const;
    
    /*********************\
     * Scripting Support *
    \*********************/
    void reset();

    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event& event) override;
    
    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;

    /*********************\
     * Implements IAsset *
    \*********************/
    bool renderAssetIcon() const override;

    private:
    
    // DOM strings.
    static const std::string TAG_EXIT;
        
    static const std::string TAG_COLOUR;
    static const std::string TAG_FONT;
    static const std::string TAG_OPTIONS;

    static const std::string ATTRIBUTE_FONT_SIZE;
    static const std::string ATTRIBUTE_SHADOW_OFFSET;
    
    static const float DEFAULT_FONT_SIZE;
    static const float DEFAULT_SHADOW_OFFSET;
      
    // Definition data.
    HatHandler& cHatHandler;
    std::vector<std::unique_ptr<IMenuItem>> cDefItems; /// Items defined in this menu.
    Action cDefExitAction;                             /// Action to perform when leaving this menu.
    Font cDefFont;                                     /// Font used for rendering items in this menu.
    Colour cDefColour;                                 /// Colour used for highlighting selections in this menu.
    float cDefFontSize;                                /// Size of fonts in this menu.
    float cDefShadowOffset;                            /// Shadow offset for rendering items in this menu.
    
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

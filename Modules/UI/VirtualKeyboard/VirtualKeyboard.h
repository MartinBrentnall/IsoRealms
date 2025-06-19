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

#include "IsoRealms/IApplication.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::UI {
  class UI;
  
  /**
   * Resource definition for a virtual keyboard that enables the input of
   * strings via other input methods.
   */
  class VirtualKeyboard final : public IInputHandler,
                                public IScreen,
                                public IString {
    public:
    
    /**********************\
     * Resource interface *
    \**********************/
    VirtualKeyboard(IProject& project, UI& ui, IResourceData& data);
    VirtualKeyboard(IProject& project, UI& ui, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);  
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IResourceData& owner, IAssetBrowser& browser, IAssetRegistry& assets);

    // Scripting interface.
    void reset();

    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event& event) override;
    void resetInput() override;

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    
    /**********************\
     * Implements IString *
    \**********************/
    std::string getValue() const override;
    
    /*********************\
     * Implements IAsset *
    \*********************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:
    
    // Constants.  TODO: Make these configurable!
    static const unsigned int ROWS;
    static const unsigned int COLUMNS;
    static const unsigned int CARET_BLINK_DELAY;
    
    // JSON members.
    static const std::string JSON_FONT;
    static const std::string JSON_ON_CONFIRM;
    static const std::string JSON_SELECTION_COLOUR;

    // External interfaces.
    ProjectCallbackManager cProjectCallbackManager;
    HatHandler& cHatHandler;

    // Definition data.
    Action cDefConfirmAction;   /// Action to be performed upon confirmation of the input.
    Colour cDefSelectionColour; /// Colour used to highlight the selected character.
    Font cDefFont;              /// Font used to render the virtual keyboard.
    
    // Runtime data.
    std::string cRuntimeValue;     /// The value entered using this virtual keyboard.
    unsigned int cRuntimeSelected; /// The index of the selected character on this virtual keyboard.
    bool cRuntimeControllerCaps;   /// Capslock triggered from a controller.
    bool cRuntimeCaretVisible;     /// Controls blinking of the caret.
    int cRuntimeCaretBlinkDelay;   /// Delay until the blink status changes.
      
    // Scripting support.
    LuaBinding<VirtualKeyboard> cLuaBinding; /// Allow virtual keyboards to be bound to lua variables.

    // Private functions.
    void up();
    void down();
    void left();
    void right();
    void confirm();
    void append(char character);
  };
}

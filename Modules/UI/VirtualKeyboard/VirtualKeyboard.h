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

#include "IsoRealms.h"

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
    VirtualKeyboard(UI& ui, IResourceData& data);
    VirtualKeyboard(UI& ui, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(PropertyMaker& owner, const Metadata& metadata);

    /*********************\
     * Module interfaces *
    \*********************/
    void updateRuntime(unsigned int milliseconds);
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
    void getAssetProperties(PropertyMaker& owner) override;
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

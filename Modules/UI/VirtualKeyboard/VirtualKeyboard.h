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
   * Component definition for a virtual keyboard that enables the input of
   * strings via other input methods.
   */
  class VirtualKeyboard final : public IInputHandler,
                                public IScreen,
                                public IString {
    public:
    
    /***********************\
     * Component interface *
    \***********************/
    VirtualKeyboard(UI& ui, IComponentData& data);
    void registerAssets(ComponentAssetRegistry& assets);
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);

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
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;
    std::string getConversionPath() const override;
    bool isConfigurable() const override;

    private:
    
    // Constants.  TODO: Make these configurable!
    inline static const unsigned int ROWS              = 3;
    inline static const unsigned int COLUMNS           = 10;
    inline static const unsigned int CARET_BLINK_DELAY = 200;

    // External interfaces.
    HatHandler& cHatHandler;

    // Definition data.
    Action cDefConfirmAction;   /// Action to be performed upon confirmation of the input.
    Colour cDefSelectionColour; /// Colour used to highlight the selected character.
    Font cDefFont;              /// Font used to render the virtual keyboard.
    
    // Runtime data.
    std::string cRuntimeValue;           /// The value entered using this virtual keyboard.
    unsigned int cRuntimeSelected;       /// The index of the selected character on this virtual keyboard.
    bool cRuntimeControllerCaps = false; /// Capslock triggered from a controller.
    bool cRuntimeCaretVisible;           /// Controls blinking of the caret.
    int cRuntimeCaretBlinkDelay;         /// Delay until the blink status changes.
      
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

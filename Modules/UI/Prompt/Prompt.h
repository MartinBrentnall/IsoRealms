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
#include <sstream>
#include <vector>

#include "IsoRealms.h"

namespace IsoRealms::UI {
  class UI;
  
  /**
   * Component for an interactive prompt with two choices.
   */
  class Prompt final : public IInputHandler,
                       public IScreen {
    public:
    
    /***********************\
     * Component Interface *
    \***********************/    
    Prompt(UI& ui, IComponentData& data);
    void registerAssets(ComponentAssetRegistry& assets);
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);

    /*********************\
     * Module interfaces *
    \*********************/
    void reset();

    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event& event) override;
    void resetInput() override;
    
    /*********************\
     * Implements IScreen *
    \*********************/
    void renderScreen(float scale, float aspectRatio) const override;

    /*********************\
     * Implements IAsset *
    \*********************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    inline static const float DEFAULT_SHADOW_OFFSET = 0.008f;
    inline static const float DEFAULT_TEXT_SIZE     = 0.05f;

    // External interfaces.
    HatHandler& cHatHandler;

    // Definition data.
    Font cDefFont;                /// Font for the prompt to be displayed in.
    float cDefTextSize;           /// Size of the text.
    Colour cDefSelectionColour;   /// Colour applied to the highlighted option.
    float cDefShadowOffset;       /// Offset of the shadow.
    std::string cDefMessage;      /// Prompt message.
    std::string cDefNegativeText; /// Text for the negative response.
    std::string cDefPositiveText; /// Text for the positive response.
    Action cDefNegativeAction;    /// Action to be performed upon a negative response.
    Action cDefPositiveAction;    /// Action to be performed upon a positive response.

    // Runtime data.
    bool cRuntimePositiveHighlighted; /// True if positive response is highlighted, otherwise negative response is highlighted.
    
    // Misc.
    LuaBinding<Prompt> cLuaBinding;
  };
}

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
#include <sstream>
#include <vector>

#include "IsoRealms/IApplication.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::UI {
  class UI;
  
  /**
   * Resource for an interactive prompt with two choices.
   */
  class Prompt final : public IInputHandler,
                       public IScreen {
    public:
    
    /**********************\
     * Resource Interface *
    \**********************/    
    Prompt(IProject& project, UI& ui, IResourceData& data);
    Prompt(IProject& project, UI& ui, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);  
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IAssetBrowser& browser, IAssetRegistry& assets);

    /***********************\
     * Scripting Interface *
    \***********************/
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
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.
    static const std::string JSON_CANCEL_LABEL;
    static const std::string JSON_CONFIRM_LABEL;
    static const std::string JSON_FONT;
    static const std::string JSON_MESSAGE;
    static const std::string JSON_ON_CANCEL;
    static const std::string JSON_ON_CONFIRM;
    static const std::string JSON_SELECTION_COLOUR;
    static const std::string JSON_SHADOW_OFFSET;
    static const std::string JSON_TEXT_SIZE;

    static const float DEFAULT_SHADOW_OFFSET;
    static const float DEFAULT_TEXT_SIZE;

    // External interfaces.
    ProjectCallbackManager cProjectCallbackManager;

    // Definition data.
    HatHandler& cHatHandler;
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

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
#include "Prompt.h"

namespace IsoRealms::UI {
  Prompt::Prompt(UI& ui, IResourceData& data) :
            cHatHandler(data.getProject().getApplication().getHatHandler()),
            cDefFont(data),
            cDefSelectionColour(data, 1.0f, 1.0f, 1.0f),
            cDefNegativeAction(data.getDummyActionContext()),
            cDefPositiveAction(data.getDummyActionContext()),
            cLuaBinding(data.getProject().getLuaState(), this) {
  }
  
  void Prompt::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IInputHandler>(this, "", "Prompts");
    assets.add<IScreen>(this, "", "Prompts");
    assets.add<IBinding>(&cLuaBinding, "", "Prompts");
  }
  
  void Prompt::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool Prompt::renderIcon() const {
    return false;
  }

  void Prompt::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector("font",            cDefFont);
    owner.createPropertyNativeFloat( "textSize",        [this]() {return cDefTextSize;},     [this](float              value) {cDefTextSize     = value;}, DEFAULT_TEXT_SIZE);
    owner.createPropertyNativeFloat( "shadowOffset",    [this]() {return cDefShadowOffset;}, [this](float              value) {cDefShadowOffset = value;}, DEFAULT_SHADOW_OFFSET);
    owner.createPropertyTreeSelector("selectionColour", cDefSelectionColour);
    owner.createPropertyNativeString("message",         [this]() {return cDefMessage;},      [this](const std::string& value) {cDefMessage      = value;});
    owner.createPropertyNativeString("cancelLabel",     [this]() {return cDefNegativeText;}, [this](const std::string& value) {cDefNegativeText = value;});
    owner.createPropertyNativeString("confirmLabel",    [this]() {return cDefPositiveText;}, [this](const std::string& value) {cDefPositiveText = value;});
    owner.createPropertyTreeSelector("onCancel",        cDefNegativeAction);
    owner.createPropertyTreeSelector("onConfirm",       cDefPositiveAction);
  }

  void Prompt::removed() {
    // Nothing to do.
  }
  
  void Prompt::reset() {
    cRuntimePositiveHighlighted = false;
  }

  bool Prompt::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Left:    cRuntimePositiveHighlighted = false;                                              return true;
          case sf::Keyboard::Right:   cRuntimePositiveHighlighted = true;                                               return true;
          case sf::Keyboard::Return: (cRuntimePositiveHighlighted ? cDefPositiveAction : cDefNegativeAction).execute(); return true;
          case sf::Keyboard::Escape:  cDefNegativeAction.execute();                                                     return true;
          default:                                                                                                      break;
        }
        break;
      }

      case sf::Event::JoystickMoved: {
        if (cHatHandler.leftPressed())  {cRuntimePositiveHighlighted = false;}
        if (cHatHandler.rightPressed()) {cRuntimePositiveHighlighted = true;}
        return true;
      }

      case sf::Event::JoystickButtonPressed: {
        if      (event.joystickButton.button == GameControllerButton::CROSS)  {(cRuntimePositiveHighlighted ? cDefPositiveAction : cDefNegativeAction).execute(); return true;}
        else if (event.joystickButton.button == GameControllerButton::CIRCLE) {cDefNegativeAction.execute();                                                      return true;}
        break;
      }

      default: break;
    }
    return false;
  }

  void Prompt::resetInput() {
    // Nothing to do.
  }

  void Prompt::renderScreen(float scale, float aspectRatio) const {
    LiteralColour mWhite(1.0f, 1.0f, 1.0f);
    Utils::shadowPrint( 0.0f,                 0.0f, **cDefFont, cDefTextSize,                                                        mWhite, cDefShadowOffset, IFont::Alignment::CENTER, cDefMessage);
    Utils::shadowPrint(-0.5f, -cDefTextSize * 2.0f, **cDefFont, cDefTextSize, !cRuntimePositiveHighlighted ? **cDefSelectionColour : mWhite, cDefShadowOffset, IFont::Alignment::LEFT,   cDefNegativeText);
    Utils::shadowPrint( 0.5f, -cDefTextSize * 2.0f, **cDefFont, cDefTextSize,  cRuntimePositiveHighlighted ? **cDefSelectionColour : mWhite, cDefShadowOffset, IFont::Alignment::RIGHT,  cDefPositiveText);
  }

  bool Prompt::renderAssetIcon() const {
    return false;
  }

  void Prompt::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Prompt::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool Prompt::isDefaultConfiguration() const {
    return true;
  }
}

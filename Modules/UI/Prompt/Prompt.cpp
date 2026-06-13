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
  
  void Prompt::save(JSONObject object) const {
    object.addFloat(JSON_TEXT_SIZE,      cDefTextSize,     DEFAULT_TEXT_SIZE);
    object.addFloat(JSON_SHADOW_OFFSET,  cDefShadowOffset, DEFAULT_SHADOW_OFFSET);
    object.addString(JSON_MESSAGE,       cDefMessage);
    object.addString(JSON_CANCEL_LABEL,  cDefNegativeText);
    object.addString(JSON_CONFIRM_LABEL, cDefPositiveText);
    cDefSelectionColour.save(object, JSON_SELECTION_COLOUR);
    cDefFont.save(object, JSON_FONT);
    cDefPositiveAction.save(object, JSON_ON_CONFIRM);
    cDefNegativeAction.save(object, JSON_ON_CANCEL);
  }

  void Prompt::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool Prompt::renderIcon() const {
    return false;
  }

  void Prompt::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector(JSON_FONT,             cDefFont);
    owner.createPropertyNativeFloat( JSON_TEXT_SIZE,        [this]() {return cDefTextSize;},     [this](float              value) {cDefTextSize     = value;}, DEFAULT_TEXT_SIZE);
    owner.createPropertyNativeFloat( JSON_SHADOW_OFFSET,    [this]() {return cDefShadowOffset;}, [this](float              value) {cDefShadowOffset = value;}, DEFAULT_SHADOW_OFFSET);
    owner.createPropertyTreeSelector(JSON_SELECTION_COLOUR, cDefSelectionColour);
    owner.createPropertyNativeString(JSON_MESSAGE,          [this]() {return cDefMessage;},      [this](const std::string& value) {cDefMessage      = value;});
    owner.createPropertyNativeString(JSON_CANCEL_LABEL,     [this]() {return cDefNegativeText;}, [this](const std::string& value) {cDefNegativeText = value;});
    owner.createPropertyNativeString(JSON_CONFIRM_LABEL,    [this]() {return cDefPositiveText;}, [this](const std::string& value) {cDefPositiveText = value;});
    owner.createPropertyTreeSelector(JSON_ON_CANCEL,        cDefNegativeAction);
    owner.createPropertyTreeSelector(JSON_ON_CONFIRM,       cDefPositiveAction);
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

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
  Prompt::Prompt(UI& ui, IComponentData& data) :
            cHatHandler(data.getProject().getApplication().getHatHandler()),
            cDefFont(data),
            cDefSelectionColour(data, 1.0f, 1.0f, 1.0f),
            cDefNegativeAction(data.getDummyActionContext()),
            cDefPositiveAction(data.getDummyActionContext()),
            cLuaBinding(data.getProject().getLuaState(), this) {
  }

  void Prompt::define(IComponentDefiner& definer) {
    definer.propertyResource("font",            cDefFont);
    definer.propertyFloat(   "textSize",        [this]() {return cDefTextSize;},     [this](float              value) {cDefTextSize     = value;}, DEFAULT_TEXT_SIZE);
    definer.propertyFloat(   "shadowOffset",    [this]() {return cDefShadowOffset;}, [this](float              value) {cDefShadowOffset = value;}, DEFAULT_SHADOW_OFFSET);
    definer.propertyResource("selectionColour", cDefSelectionColour);
    definer.propertyString(  "message",         [this]() {return cDefMessage;},      [this](const std::string& value) {cDefMessage      = value;});
    definer.propertyString(  "cancelLabel",     [this]() {return cDefNegativeText;}, [this](const std::string& value) {cDefNegativeText = value;});
    definer.propertyString(  "confirmLabel",    [this]() {return cDefPositiveText;}, [this](const std::string& value) {cDefPositiveText = value;});
    definer.propertyResource("onCancel",        cDefNegativeAction);
    definer.propertyResource("onConfirm",       cDefPositiveAction);
  }
  
  void Prompt::publish(ResourcePublisher& publisher) {
    publisher.publish<IInputHandler>(this, "", "Prompts");
    publisher.publish<IScreen>(this, "", "Prompts");
    publisher.publish<IBinding>(&cLuaBinding, "", "Prompts");
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
}

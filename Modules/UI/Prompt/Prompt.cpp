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
  const std::string Prompt::JSON_CANCEL_LABEL     = "cancelLabel";
  const std::string Prompt::JSON_CONFIRM_LABEL    = "confirmLabel";
  const std::string Prompt::JSON_FONT             = "font";
  const std::string Prompt::JSON_MESSAGE          = "message";
  const std::string Prompt::JSON_ON_CANCEL        = "onCancel";
  const std::string Prompt::JSON_ON_CONFIRM       = "onConfirm";
  const std::string Prompt::JSON_SELECTION_COLOUR = "selectionColour";
  const std::string Prompt::JSON_SHADOW_OFFSET    = "shadowOffset";
  const std::string Prompt::JSON_TEXT_SIZE        = "textSize";

  const float Prompt::DEFAULT_SHADOW_OFFSET = 0.008f;
  const float Prompt::DEFAULT_TEXT_SIZE     = 0.05f;

  Prompt::Prompt(IProject& project, UI& ui, IResourceData& data) :
            cHatHandler(project.getApplication().getHatHandler()),
            cDefFont(data),
            cDefSelectionColour(data, 1.0f, 1.0f, 1.0f),
            cDefNegativeAction(data.getDummyActionClient()),
            cDefPositiveAction(data.getDummyActionClient()),
            cLuaBinding(project, this) {
  }
  
  Prompt::Prompt(IProject& project, UI& ui, IResourceData& data, JSONObject object, IOptions& options) :
            Prompt(project, ui, data) {
    cDefTextSize     = object.getFloat(JSON_TEXT_SIZE,     DEFAULT_TEXT_SIZE);
    cDefShadowOffset = object.getFloat(JSON_SHADOW_OFFSET, DEFAULT_SHADOW_OFFSET);
    cDefMessage      = object.getString(JSON_MESSAGE);
    cDefNegativeText = object.getString(JSON_CANCEL_LABEL);
    cDefPositiveText = object.getString(JSON_CONFIRM_LABEL);
    cDefSelectionColour.init(object, JSON_SELECTION_COLOUR);
    cDefFont.init(object, JSON_FONT);
    cDefNegativeAction.init(object, JSON_ON_CANCEL);
    cDefPositiveAction.init(object, JSON_ON_CONFIRM);
  }

  void Prompt::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IInputHandler>(this, "", "System");
    assets.add<IScreen>(this, "", "System");
    assets.add<IBinding>(&cLuaBinding, "", "System");
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

  std::vector<std::unique_ptr<IProperty>> Prompt::getProperties(IResourceData& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Font>>(  "Font",             "TODO", cDefFont));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>(  "Font Size",        "TODO", [this]() {return cDefTextSize;},     [this](float              value) {cDefTextSize     = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>(  "Shadow Offset",    "TODO", [this]() {return cDefShadowOffset;}, [this](float              value) {cDefShadowOffset = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>("Selection Colour", "TODO", cDefSelectionColour));
    mProperties.emplace_back(std::make_unique<PropertyNativeString>( "Message",          "TODO", [this]() {return cDefMessage;},      [this](const std::string& value) {cDefMessage      = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeString>( "Reject Label",     "TODO", [this]() {return cDefNegativeText;}, [this](const std::string& value) {cDefNegativeText = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeString>( "Accept Label",     "TODO", [this]() {return cDefPositiveText;}, [this](const std::string& value) {cDefPositiveText = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("On Rejection",     "TODO", cDefNegativeAction));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("On Acceptance",    "TODO", cDefPositiveAction));
    return mProperties;
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
        if      (event.joystickButton.button == ButtonMapping::CROSS)  {(cRuntimePositiveHighlighted ? cDefPositiveAction : cDefNegativeAction).execute(); return true;}
        else if (event.joystickButton.button == ButtonMapping::CIRCLE) {cDefNegativeAction.execute();                                                      return true;}
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

  std::vector<std::unique_ptr<IProperty>> Prompt::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Prompt::isDefaultConfiguration() const {
    return true;
  }
}

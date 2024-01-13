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
#include "Prompt.h"

#include "IsoRealms/Persistence/DOMNode.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::UI {
  const std::string Prompt::TAG_FALSE     = "False";
  const std::string Prompt::TAG_FONT      = "Font";
  const std::string Prompt::TAG_HIGHLIGHT = "Highlight";
  const std::string Prompt::TAG_TRUE      = "True";

  const std::string Prompt::ATTRIBUTE_FALSE         = "false";
  const std::string Prompt::ATTRIBUTE_SHADOW_OFFSET = "shadowOffset";
  const std::string Prompt::ATTRIBUTE_TEXT          = "text";
  const std::string Prompt::ATTRIBUTE_TEXT_SIZE     = "textSize";
  const std::string Prompt::ATTRIBUTE_TRUE          = "true";

  const float Prompt::DEFAULT_SHADOW_OFFSET = 0.008f;
  const float Prompt::DEFAULT_TEXT_SIZE     = 0.05f;

  Prompt::Prompt(IProject* project, UI* ui) :
            cHatHandler(project->getApplication()->getHatHandler()),
            cDefFont(project),
            cDefSelectionColour(project, 1.0f, 1.0f, 1.0f),
            cDefNegativeAction(project),
            cDefPositiveAction(project),
            cLuaBinding(project, this) {
    project->reset([this]() {
      cRuntimePositiveHighlighted = false;
    });
  }
  
  Prompt::Prompt(IProject* project, UI* ui, DOMNode& node, IOptions* options, IResourceData* data) :
            Prompt(project, ui) {
    cDefTextSize     = node.getFloatAttribute(ATTRIBUTE_TEXT_SIZE,     DEFAULT_TEXT_SIZE);
    cDefShadowOffset = node.getFloatAttribute(ATTRIBUTE_SHADOW_OFFSET, DEFAULT_SHADOW_OFFSET);
    cDefMessage      = node.getAttribute(ATTRIBUTE_TEXT);
    cDefNegativeText = node.getAttribute(ATTRIBUTE_FALSE);
    cDefPositiveText = node.getAttribute(ATTRIBUTE_TRUE);
    cDefSelectionColour.init(node.getNode(TAG_HIGHLIGHT));
    cDefFont.init(node.getNode(TAG_FONT));
    cDefNegativeAction.init(node, TAG_FALSE);
    cDefPositiveAction.init(node, TAG_TRUE);
  }

  void Prompt::registerAssets(IAssetRegistry* assets) {
    assets->add(static_cast<IInputHandler*>(this), "", "System");
    assets->add(static_cast<IScreen*>(this), "", "System");
    assets->add(&cLuaBinding, "", "System");
  }
  
  void Prompt::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(static_cast<IInputHandler*>(this));
    assets->remove(static_cast<IScreen*>(this));
    assets->remove(&cLuaBinding);
  }
  
  void Prompt::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_TEXT_SIZE,     cDefTextSize,     DEFAULT_TEXT_SIZE);
    node->addAttribute(ATTRIBUTE_SHADOW_OFFSET, cDefShadowOffset, DEFAULT_SHADOW_OFFSET);
    node->addAttribute(ATTRIBUTE_TEXT,          cDefMessage);
    node->addAttribute(ATTRIBUTE_FALSE,         cDefNegativeText);
    node->addAttribute(ATTRIBUTE_TRUE,          cDefPositiveText);
    cDefSelectionColour.save(node, TAG_HIGHLIGHT);
    cDefFont.save(node, TAG_FONT);
    cDefPositiveAction.save(node, TAG_TRUE);
    cDefNegativeAction.save(node, TAG_FALSE);
  }
  
  void Prompt::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool Prompt::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> Prompt::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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

  void Prompt::renderScreen(float scale, float aspectRatio) const {
    LiteralColour mWhite(1.0f, 1.0f, 1.0f);
    Utils::shadowPrint( 0.0f,                 0.0f, **cDefFont, cDefTextSize,                                                      mWhite, cDefShadowOffset, IFont::Alignment::CENTER, cDefMessage);
    Utils::shadowPrint(-0.5f, -cDefTextSize * 2.0f, **cDefFont, cDefTextSize, !cRuntimePositiveHighlighted ? cDefSelectionColour : mWhite, cDefShadowOffset, IFont::Alignment::LEFT,   cDefNegativeText);
    Utils::shadowPrint( 0.5f, -cDefTextSize * 2.0f, **cDefFont, cDefTextSize,  cRuntimePositiveHighlighted ? cDefSelectionColour : mWhite, cDefShadowOffset, IFont::Alignment::RIGHT,  cDefPositiveText);
  }

  bool Prompt::renderAssetIcon() const {
    return false;
  }
}

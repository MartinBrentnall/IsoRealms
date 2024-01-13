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
#include "PropertyColour.h"

namespace IsoRealms {
  const std::string PropertyColour::PROPERTY_RED        = "Red";
  const std::string PropertyColour::PROPERTY_GREEN      = "Green";
  const std::string PropertyColour::PROPERTY_BLUE       = "Blue";
  const std::string PropertyColour::PROPERTY_ALPHA      = "Alpha";
  const std::string PropertyColour::PROPERTY_HUE        = "Hue";
  const std::string PropertyColour::PROPERTY_SATURATION = "Saturation";
  const std::string PropertyColour::PROPERTY_LIGHTNESS  = "Lightness";
  
  PropertyColour::PropertyColour(const std::string& label, IColour** selected, IAssetUser<IColour>* client, IAssetBrowser* browser, bool allowNone) :
            PropertyAssetBase(label, selected, client, browser, browser->getAllColours(), allowNone ? std::vector<IAssetMenuItem*>({
              &cOptionNone,
              &cOptionFixed
            })                                                                                  : std::vector<IAssetMenuItem*>({
              &cOptionFixed
            })),
            cOptionNone(this, []() {Utils::renderIconNone();}, "None", "#"),
            cOptionFixed(this) {
  }
  
  void PropertyColour::refresh() {
    refreshItems(cBrowser->getAllColours());
  }
  
  bool PropertyColour::renderIcon(IAssetBrowser* browser, const std::string& id) const {
    return browser->renderColourIcon(id);
  }
  
  IColour* PropertyColour::getAsset(IAssetBrowser* browser, IAssetUser<IColour>* client, const std::string& id) const {
    return browser->getColour(client, id);
  }
  
  PropertyColour::FixedColour::FixedColour(PropertyColour* parent) :
            cParent(parent) {
  }
  
  void PropertyColour::FixedColour::render(float yOffset) const {
    glPushMatrix();
    cParent->transformForIcon(yOffset);
    Utils::renderIconCustom();
    glPopMatrix();
  }
  
  bool PropertyColour::FixedColour::executeAction() {
    cParent->cSpecialEditor = std::make_unique<Editor>(cParent, cParent);
    cParent->cSelection.emplace_back(std::make_unique<AssetMenu::Selection>(nullptr, 0, 1.0f));
    cParent->cWidth = cParent->cSpecialEditor->getWidth(cParent->cIntAppearance);
    cParent->cHeight = cParent->cSpecialEditor->getHeight(cParent->cIntAppearance);
    return false;
  }
  
  float PropertyColour::FixedColour::getWidth() const {
    return cParent->getIconWidth();
  }
  
  std::vector<std::unique_ptr<PropertyAsset::ISelection>> PropertyColour::FixedColour::getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const {
    std::vector<std::unique_ptr<PropertyAsset::ISelection>> mSelection;
    if (id[0] == '#') {
      mSelection.emplace_back(std::make_unique<AssetMenu::Selection>(menu, item, 0.0f));
    }
    return mSelection;
  }  
  
  PropertyColour::Editor::Editor(PropertyColour* parent, ISpecialItemEditorListener* listener) :
            cParent(parent),
            cDefRed(0.5f),
            cDefGreen(0.0f),
            cDefBlue(1.0f),
            cDefAlpha(1.0f),
            cLastKnownHue(Utils::getHue(cDefRed, cDefGreen, cDefBlue)),
            cLastKnownSaturation(Utils::getSaturation(cDefRed, cDefGreen, cDefBlue)),
            cLastKnownLightness(Utils::getLightness(cDefRed, cDefGreen, cDefBlue)),
            cSelectedProperty(0),
            cScroll(0.0f),
            cMenuCallback(listener) {
    cProperties.emplace_back(std::make_unique<PropertyColourChannel>(PROPERTY_RED, [this]() {return cDefRed;}, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefGreen, &cDefBlue, &cDefAlpha, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cDefGreen, &cDefBlue, &cDefAlpha, [this](const float value) {
      cDefRed = value;
      cLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
      cLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
      cLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);
    }));
    cProperties.emplace_back(std::make_unique<PropertyColourChannel>(PROPERTY_GREEN, [this]() {return cDefGreen;}, &cDefRed, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefBlue, &cDefAlpha, &cDefRed, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cDefBlue, &cDefAlpha, [this](const float value) {
      cDefGreen = value;
      cLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
      cLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
      cLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);
    }));
    cProperties.emplace_back(std::make_unique<PropertyColourChannel>(PROPERTY_BLUE, [this]() {return cDefBlue;}, &cDefRed, &cDefGreen, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefAlpha, &cDefRed, &cDefGreen, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cDefAlpha, [this](const float value) {
      cDefBlue = value;
      cLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
      cLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
      cLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);
    }));
    cProperties.emplace_back(std::make_unique<PropertyColourChannel>(PROPERTY_ALPHA, [this]() {return cDefAlpha;}, &cDefRed, &cDefGreen, &cDefBlue, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefRed, &cDefGreen, &cDefBlue, &PropertyColourChannel::MAX_CHANNEL_VALUE, [this](const float value) {
      cDefAlpha = value;
    }));
    cProperties.emplace_back(std::make_unique<PropertyColourHue>(PROPERTY_HUE, [this]() {return cLastKnownHue;}, &cLastKnownSaturation, &cLastKnownLightness, &cDefAlpha, [this](const float value) {
      cLastKnownHue = value;
      Utils::calculateColour(cLastKnownHue, cLastKnownSaturation, cLastKnownLightness, cDefRed, cDefGreen, cDefBlue);
    }));
    cProperties.emplace_back(std::make_unique<PropertyColourSaturation>(PROPERTY_SATURATION, [this]() {return cLastKnownSaturation;}, &cLastKnownHue, &cLastKnownLightness, &cDefAlpha, [this](const float value) {
      cLastKnownSaturation = value;
      Utils::calculateColour(cLastKnownHue, cLastKnownSaturation, cLastKnownLightness, cDefRed, cDefGreen, cDefBlue);
    }));
    cProperties.emplace_back(std::make_unique<PropertyColourLightness>(PROPERTY_LIGHTNESS, [this]() {return cLastKnownLightness;}, &cLastKnownHue, &cLastKnownSaturation, &cDefAlpha, [this](const float value) {
      cLastKnownLightness = value;
      Utils::calculateColour(cLastKnownHue, cLastKnownSaturation, cLastKnownLightness, cDefRed, cDefGreen, cDefBlue);
    }));
  }
  
  float PropertyColour::Editor::getMaxLabelWidth() const {
    float mPropertyNameMaxWidth = 0.0f;
    for (const std::unique_ptr<IProperty>& mProperty : cProperties) {
      mPropertyNameMaxWidth = std::max(mPropertyNameMaxWidth, cParent->getTextWidth(mProperty->getPropertyName() + ":"));
    }
    return mPropertyNameMaxWidth;
  }
  
  bool PropertyColour::Editor::hasOwnPage() {
    return true;
  }
  
  float PropertyColour::Editor::getWidth(IPropertyAppearance* appearance) {
    float mWidth = 0.0f;
    for (const std::unique_ptr<IProperty>& mProperty : cProperties) {
      mWidth = std::max(mWidth, mProperty->getWidth(appearance) + appearance->getTextWidth(mProperty->getPropertyName() + ":"));
    }
    return mWidth + appearance->getLabelPropertySpacing();
  }
  
  float PropertyColour::Editor::getHeight(IPropertyAppearance* appearance) {
    return cProperties.size() * appearance->getLineHeight() - (appearance->getLineHeight() - appearance->getSelectionHighlightHeight());
  }
  
  void PropertyColour::Editor::render(IPropertyAppearance* appearance) {
    glPushMatrix();
    glTranslatef(0.0f, cScroll.animation(), 0.0f);
    
    float mPropertyNameMaxWidth = 0.0f;
    for (unsigned int i = 0; i < cProperties.size(); i++) {
      std::string mPropertyName = cProperties[i]->getPropertyName() + ":";
      mPropertyNameMaxWidth = std::max(mPropertyNameMaxWidth, appearance->getTextWidth(mPropertyName));
      appearance->print(mPropertyName, i, 0.0f);
    }
    for (const std::unique_ptr<IProperty>& mProperty : cProperties) {
      mProperty->render(appearance);
    }
    glPopMatrix();
  }
  
  void PropertyColour::Editor::update(unsigned int milliseconds) {
    cScroll.update(milliseconds);
  }
      
  bool PropertyColour::Editor::input(sf::Event& event) {
    switch (event.key.code) {
      case sf::Keyboard::Up: {
        cSelectedProperty = std::max(cSelectedProperty - 1, 0);
        cScroll = cSelectedProperty * cParent->getLineHeight();
        cMenuCallback->setSpecialItemSelected(cSelectedProperty);
        break;
      };
      
      case sf::Keyboard::Down: {
        cSelectedProperty = std::min(cSelectedProperty + 1, static_cast<int>(cProperties.size() - 1));
        cScroll = cSelectedProperty * cParent->getLineHeight();
        cMenuCallback->setSpecialItemSelected(cSelectedProperty);
        break;
      };
      
      default: {
        cProperties[cSelectedProperty]->input(event);
        break;
      }
    }
    return false;
  }
  
  std::string PropertyColour::Editor::getValue() {
    return "#0.0 0.0 0.0";
  }
}


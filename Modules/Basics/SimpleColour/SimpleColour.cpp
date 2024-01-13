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
#include "SimpleColour.h"

namespace IsoRealms::Basics {
  const std::string SimpleColour::ATTRIBUTE_RED   = "red";
  const std::string SimpleColour::ATTRIBUTE_GREEN = "green";
  const std::string SimpleColour::ATTRIBUTE_BLUE  = "blue";
  const std::string SimpleColour::ATTRIBUTE_ALPHA = "alpha";
  
  const std::string SimpleColour::PROPERTY_RED        = "Red";
  const std::string SimpleColour::PROPERTY_GREEN      = "Green";
  const std::string SimpleColour::PROPERTY_BLUE       = "Blue";
  const std::string SimpleColour::PROPERTY_ALPHA      = "Alpha";
  const std::string SimpleColour::PROPERTY_HUE        = "Hue";
  const std::string SimpleColour::PROPERTY_SATURATION = "Saturation";
  const std::string SimpleColour::PROPERTY_LIGHTNESS  = "Lightness";
  
  SimpleColour::SimpleColour(IProject* project, Basics* basics) :
            cDefRed(0.0f),
            cDefGreen(0.0f),
            cDefBlue(0.0f),
            cDefAlpha(0.0f),
            cRuntimeRed(0.0f),
            cRuntimeGreen(0.0f),
            cRuntimeBlue(0.0f),
            cRuntimeAlpha(0.0f),
            cEditingLastKnownHue(Utils::getHue(cDefRed, cDefGreen, cDefBlue)),
            cEditingLastKnownSaturation(Utils::getSaturation(cDefRed, cDefGreen, cDefBlue)),
            cEditingLastKnownLightness(Utils::getLightness(cDefRed, cDefGreen, cDefBlue)),
            cLuaBinding(project, this),
            cStateNotifier(nullptr) {
    project->reset([this]() {
      cRuntimeRed   = cDefRed;
      cRuntimeGreen = cDefGreen;
      cRuntimeBlue  = cDefBlue;
      cRuntimeAlpha = cDefAlpha;
    });
  }
  
  SimpleColour::SimpleColour(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            SimpleColour(project, basics) {
    cRuntimeRed   = cDefRed   = node.getFloatAttribute(ATTRIBUTE_RED);
    cRuntimeGreen = cDefGreen = node.getFloatAttribute(ATTRIBUTE_GREEN);
    cRuntimeBlue  = cDefBlue  = node.getFloatAttribute(ATTRIBUTE_BLUE);
    cRuntimeAlpha = cDefAlpha = node.getFloatAttribute(ATTRIBUTE_ALPHA);

    cEditingLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
    cEditingLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
    cEditingLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);

    project->init([this, &node](IAssets* resources) {
      cStateNotifier->stateChanged(this);
    });
  }

  void SimpleColour::registerAssets(IAssetRegistry* assets) {
    cStateNotifier = assets->add(this, "", "Simple Colours");
    assets->add(&cLuaBinding,  "", "Simple Colours");
  }

  void SimpleColour::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
    assets->remove(&cLuaBinding);
    cStateNotifier = nullptr;
  }

  void SimpleColour::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_RED,   cDefRed);
    node->addAttribute(ATTRIBUTE_GREEN, cDefGreen);
    node->addAttribute(ATTRIBUTE_BLUE,  cDefBlue);
    node->addAttribute(ATTRIBUTE_ALPHA, cDefAlpha);
  }

  void SimpleColour::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleColour::renderIcon() const {
    return renderAssetIcon();
  }

  std::vector<IProperty*> SimpleColour::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    std::vector<std::unique_ptr<IProperty>> cProperties;
//     cProperties.emplace_back(std::make_unique<PropertyColourChannel>(PROPERTY_RED, [this]() {return cDefRed;}, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefGreen, &cDefBlue, &cDefAlpha, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cDefGreen, &cDefBlue, &cDefAlpha, [this](const float value) {
//       cDefRed = value;
//       cEditingLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
//       cEditingLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
//       cEditingLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);
//     }));
//     cProperties.emplace_back(std::make_unique<PropertyColourChannel>(PROPERTY_GREEN, [this]() {return cDefGreen;}, &cDefRed, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefBlue, &cDefAlpha, &cDefRed, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cDefBlue, &cDefAlpha, [this](const float value) {
//       cDefGreen = value;
//       cEditingLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
//       cEditingLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
//       cEditingLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);
//     }));
//     cProperties.emplace_back(std::make_unique<PropertyColourChannel>(PROPERTY_BLUE, [this]() {return cDefBlue;}, &cDefRed, &cDefGreen, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefAlpha, &cDefRed, &cDefGreen, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cDefAlpha, [this](const float value) {
//       cDefBlue = value;
//       cEditingLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
//       cEditingLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
//       cEditingLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);
//     }));
//     cProperties.emplace_back(std::make_unique<PropertyColourChannel>(PROPERTY_ALPHA, [this]() {return cDefAlpha;}, &cDefRed, &cDefGreen, &cDefBlue, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefRed, &cDefGreen, &cDefBlue, &PropertyColourChannel::MAX_CHANNEL_VALUE, [this](const float value) {
//       cDefAlpha = value;
//     }));
//     cProperties.emplace_back(std::make_unique<PropertyColourHue>(PROPERTY_HUE, [this]() {return cEditingLastKnownHue;}, &cEditingLastKnownSaturation, &cEditingLastKnownLightness, &cDefAlpha, [this](const float value) {
//       cEditingLastKnownHue = value;
//       Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cDefRed, cDefGreen, cDefBlue);
//     }));
//     cProperties.emplace_back(std::make_unique<PropertyColourSaturation>(PROPERTY_SATURATION, [this]() {return cEditingLastKnownSaturation;}, &cEditingLastKnownHue, &cEditingLastKnownLightness, &cDefAlpha, [this](const float value) {
//       cEditingLastKnownSaturation = value;
//       Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cDefRed, cDefGreen, cDefBlue);
//     }));
//     cProperties.emplace_back(std::make_unique<PropertyColourLightness>(PROPERTY_LIGHTNESS, [this]() {return cEditingLastKnownLightness;}, &cEditingLastKnownHue, &cEditingLastKnownSaturation, &cDefAlpha, [this](const float value) {
//       cEditingLastKnownLightness = value;
//       Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cDefRed, cDefGreen, cDefBlue);
//     }));
    return std::vector<IProperty*>();
  }

  void SimpleColour::set() const {
    glColor4f(cRuntimeRed, cRuntimeGreen, cRuntimeBlue, cRuntimeAlpha);
  }

  float SimpleColour::getRed() const {
    return cRuntimeRed;
  }

  float SimpleColour::getGreen() const {
    return cRuntimeGreen;
  }

  float SimpleColour::getBlue() const {
    return cRuntimeBlue;
  }

  float SimpleColour::getAlpha() const {
    return cRuntimeAlpha;
  }

  void SimpleColour::setRed(float value) {
    cRuntimeRed = value;
    cStateNotifier->stateChanged(this);
  }

  void SimpleColour::setGreen(float value) {
    cRuntimeGreen = value;
    cStateNotifier->stateChanged(this);
  }

  void SimpleColour::setBlue(float value) {
    cRuntimeBlue = value;
    cStateNotifier->stateChanged(this);
  }

  void SimpleColour::setAlpha(float value) {
    cRuntimeAlpha = value;
    cStateNotifier->stateChanged(this);
  }
}

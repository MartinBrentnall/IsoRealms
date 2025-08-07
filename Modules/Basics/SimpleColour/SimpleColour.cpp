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
#include "SimpleColour.h"

namespace IsoRealms::Basics {
  const std::string SimpleColour::JSON_RED   = "red";
  const std::string SimpleColour::JSON_GREEN = "green";
  const std::string SimpleColour::JSON_BLUE  = "blue";
  const std::string SimpleColour::JSON_ALPHA = "alpha";

  SimpleColour::SimpleColour(IProject& project, Basics& basics, IResourceData& data) :
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
  }
  
  SimpleColour::SimpleColour(IProject& project, Basics& basics, IResourceData& data, JSONObject object) :
            SimpleColour(project, basics, data) {
    cRuntimeRed   = cDefRed   = object.getFloat(JSON_RED);
    cRuntimeGreen = cDefGreen = object.getFloat(JSON_GREEN);
    cRuntimeBlue  = cDefBlue  = object.getFloat(JSON_BLUE);
    cRuntimeAlpha = cDefAlpha = object.getFloat(JSON_ALPHA);

    cEditingLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
    cEditingLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
    cEditingLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);

    project.init([this](IAssets& resources) {
      cStateNotifier->stateChanged(this);
    });
  }

  void SimpleColour::registerAssets(ResourceAssetRegistry& assets) {
    cStateNotifier = assets.add<IColour>(this, "", "Simple Colours");
    assets.add<IBinding>(&cLuaBinding,  "", "Simple Colours");
  }

  void SimpleColour::save(JSONObject object) const {
    object.addFloat(JSON_RED,   cDefRed);
    object.addFloat(JSON_GREEN, cDefGreen);
    object.addFloat(JSON_BLUE,  cDefBlue);
    object.addFloat(JSON_ALPHA, cDefAlpha);
  }

  void SimpleColour::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleColour::renderIcon() const {
    return renderAssetIcon();
  }

  void SimpleColour::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyColourChannel(metadata.getPropertyData("Red"), [this]() {return cDefRed;}, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefGreen, &cDefBlue, &cDefAlpha, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cDefGreen, &cDefBlue, &cDefAlpha, [this](const float value) {
      cDefRed = value;
      cEditingLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
      cEditingLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
      cEditingLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);
      resetColour();
    });
    owner.createPropertyColourChannel(metadata.getPropertyData("Green"), [this]() {return cDefGreen;}, &cDefRed, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefBlue, &cDefAlpha, &cDefRed, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cDefBlue, &cDefAlpha, [this](const float value) {
      cDefGreen = value;
      cEditingLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
      cEditingLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
      cEditingLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);
      resetColour();
    });
    owner.createPropertyColourChannel(metadata.getPropertyData("Blue"), [this]() {return cDefBlue;}, &cDefRed, &cDefGreen, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefAlpha, &cDefRed, &cDefGreen, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cDefAlpha, [this](const float value) {
      cDefBlue = value;
      cEditingLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
      cEditingLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
      cEditingLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);
      resetColour();
    });
    owner.createPropertyColourChannel(metadata.getPropertyData("Alpha"), [this]() {return cDefAlpha;}, &cDefRed, &cDefGreen, &cDefBlue, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefRed, &cDefGreen, &cDefBlue, &PropertyColourChannel::MAX_CHANNEL_VALUE, [this](const float value) {
      cDefAlpha = value;
      resetColour();
    });
    owner.createPropertyColourHue(metadata.getPropertyData("Hue"), [this]() {return cEditingLastKnownHue;}, &cEditingLastKnownSaturation, &cEditingLastKnownLightness, &cDefAlpha, [this](const float value) {
      cEditingLastKnownHue = value;
      Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cDefRed, cDefGreen, cDefBlue);
      resetColour();
    });
    owner.createPropertyColourSaturation(metadata.getPropertyData("Saturation"), [this]() {return cEditingLastKnownSaturation;}, &cEditingLastKnownHue, &cEditingLastKnownLightness, &cDefAlpha, [this](const float value) {
      cEditingLastKnownSaturation = value;
      Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cDefRed, cDefGreen, cDefBlue);
      resetColour();
    });
    owner.createPropertyColourLightness(metadata.getPropertyData("Lightness"), [this]() {return cEditingLastKnownLightness;}, &cEditingLastKnownHue, &cEditingLastKnownSaturation, &cDefAlpha, [this](const float value) {
      cEditingLastKnownLightness = value;
      Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cDefRed, cDefGreen, cDefBlue);
      resetColour();
    });
  }

  void SimpleColour::reset() {
    resetColour();
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

  void SimpleColour::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void SimpleColour::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool SimpleColour::isDefaultConfiguration() const {
    return true;
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

  void SimpleColour::resetColour() {
    cRuntimeRed   = cDefRed;
    cRuntimeGreen = cDefGreen;
    cRuntimeBlue  = cDefBlue;
    cRuntimeAlpha = cDefAlpha;
    cStateNotifier->stateChanged(this);
  }
}

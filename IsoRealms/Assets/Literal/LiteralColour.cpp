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
#include "LiteralColour.h"

#include "IsoRealms/Editing.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  LiteralColour::LiteralColour() :
            cRed(  0.0f),
            cGreen(0.0f),
            cBlue( 0.0f),
            cAlpha(1.0f) {
  }

  LiteralColour::LiteralColour(const IColour& colour, const float intensity) :
            cRed(  colour.getRed()   * intensity),
            cGreen(colour.getGreen() * intensity),
            cBlue( colour.getBlue()  * intensity),
            cAlpha(colour.getAlpha()) {
  }

  LiteralColour::LiteralColour(const float red, const float green, const float blue, const float alpha) :
            cRed(red),
            cGreen(green),
            cBlue(blue),
            cAlpha(alpha) {
  }

  LiteralColour::LiteralColour(const IColour& a, const IColour& b, const float weight) :
            cRed(  a.getRed()   + (b.getRed()   - a.getRed())   * weight),
            cGreen(a.getGreen() + (b.getGreen() - a.getGreen()) * weight),
            cBlue( a.getBlue()  + (b.getBlue()  - a.getBlue())  * weight),
            cAlpha(a.getAlpha() + (b.getAlpha() - a.getAlpha()) * weight) {
  }

  bool LiteralColour::operator==(const LiteralColour& colour) const {
    return cRed == colour.cRed && cGreen == colour.cGreen && cBlue == colour.cBlue && cAlpha == colour.cAlpha;
  }

  bool LiteralColour::operator!=(const LiteralColour& colour) const {
    return !operator==(colour);
  }

  void LiteralColour::set() const {
    glColor4f(cRed, cGreen, cBlue, cAlpha);
  }

  float LiteralColour::getRed() const {
    return cRed;
  }

  float LiteralColour::getGreen() const {
    return cGreen;
  }

  float LiteralColour::getBlue() const {
    return cBlue;
  }

  float LiteralColour::getAlpha() const {
    return cAlpha;
  }

  void LiteralColour::saveAsset(JSONObject object) const {
    object.addFloat(JSON_RED, cRed);
    object.addFloat(JSON_GREEN, cGreen);
    object.addFloat(JSON_BLUE, cBlue);
    object.addFloat(JSON_ALPHA, cAlpha);
  }

  std::vector<std::unique_ptr<IProperty>> LiteralColour::getAssetProperties() {
    cEditingLastKnownHue = Utils::getHue(cRed, cGreen, cBlue);
    cEditingLastKnownSaturation = Utils::getSaturation(cRed, cGreen, cBlue);
    cEditingLastKnownLightness = Utils::getLightness(cRed, cGreen, cBlue);

    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyColourChannel>("Red", "TODO", [this]() {return cRed;}, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cGreen, &cBlue, &cAlpha, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cGreen, &cBlue, &cAlpha, [this](const float value) {
      cRed = value;
      cEditingLastKnownHue = Utils::getHue(cRed, cGreen, cBlue);
      cEditingLastKnownSaturation = Utils::getSaturation(cRed, cGreen, cBlue);
      cEditingLastKnownLightness = Utils::getLightness(cRed, cGreen, cBlue);
    }));
    mProperties.emplace_back(std::make_unique<PropertyColourChannel>("Green", "TODO", [this]() {return cGreen;}, &cRed, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cBlue, &cAlpha, &cRed, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cBlue, &cAlpha, [this](const float value) {
      cGreen = value;
      cEditingLastKnownHue = Utils::getHue(cRed, cGreen, cBlue);
      cEditingLastKnownSaturation = Utils::getSaturation(cRed, cGreen, cBlue);
      cEditingLastKnownLightness = Utils::getLightness(cRed, cGreen, cBlue);
    }));
    mProperties.emplace_back(std::make_unique<PropertyColourChannel>("Blue", "TODO", [this]() {return cBlue;}, &cRed, &cGreen, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cAlpha, &cRed, &cGreen, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cAlpha, [this](const float value) {
      cBlue = value;
      cEditingLastKnownHue = Utils::getHue(cRed, cGreen, cBlue);
      cEditingLastKnownSaturation = Utils::getSaturation(cRed, cGreen, cBlue);
      cEditingLastKnownLightness = Utils::getLightness(cRed, cGreen, cBlue);
    }));
    mProperties.emplace_back(std::make_unique<PropertyColourChannel>("Alpha", "TODO", [this]() {return cAlpha;}, &cRed, &cGreen, &cBlue, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cRed, &cGreen, &cBlue, &PropertyColourChannel::MAX_CHANNEL_VALUE, [this](const float value) {
      cAlpha = value;
    }));
    mProperties.emplace_back(std::make_unique<PropertyColourHue>("Hue", "TODO", [this]() {return cEditingLastKnownHue;}, &cEditingLastKnownSaturation, &cEditingLastKnownLightness, &cAlpha, [this](const float value) {
      cEditingLastKnownHue = value;
      Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cRed, cGreen, cBlue);
    }));
    mProperties.emplace_back(std::make_unique<PropertyColourSaturation>("Saturation", "TODO", [this]() {return cEditingLastKnownSaturation;}, &cEditingLastKnownHue, &cEditingLastKnownLightness, &cAlpha, [this](const float value) {
      cEditingLastKnownSaturation = value;
      Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cRed, cGreen, cBlue);
    }));
    mProperties.emplace_back(std::make_unique<PropertyColourLightness>("Lightness", "TODO", [this]() {return cEditingLastKnownLightness;}, &cEditingLastKnownHue, &cEditingLastKnownSaturation, &cAlpha, [this](const float value) {
      cEditingLastKnownLightness = value;
      Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cRed, cGreen, cBlue);
    }));
    return mProperties;
  }

  bool LiteralColour::isDefaultConfiguration() const {
    return cRed == 0.0f && cGreen == 0.0f && cBlue == 0.0f && cAlpha == 1.0f;
  }

  const std::string LiteralColour::JSON_ALPHA = "alpha";
  const std::string LiteralColour::JSON_BLUE  = "blue";
  const std::string LiteralColour::JSON_GREEN = "green";
  const std::string LiteralColour::JSON_RED   = "red";
}

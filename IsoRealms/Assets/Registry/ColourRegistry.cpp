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
#include "ColourRegistry.h"

#include "IsoRealms/Project.h"

namespace IsoRealms {
  ColourRegistry::ColourRegistry(Project& project) :
            AssetClientManager(&cLiteral, "Literal") {
  }

  ColourRegistry::Literal::Instance::Instance(const Project& project) :
            cMetadata(project.getApplication().getMetadata("LiteralColour")),
            cRed(  0.0f),
            cGreen(0.0f),
            cBlue( 0.0f),
            cAlpha(1.0f) {
  }

  ColourRegistry::Literal::Instance::Instance(const Project& project, const float red, const float green, const float blue, const float alpha) :
            cMetadata(project.getApplication().getMetadata("LiteralColour")),
            cRed(red),
            cGreen(green),
            cBlue(blue),
            cAlpha(alpha) {
  }

  void ColourRegistry::Literal::Instance::set() const {
    glColor4f(cRed, cGreen, cBlue, cAlpha);
  }

  float ColourRegistry::Literal::Instance::getRed() const {
    return cRed;
  }

  float ColourRegistry::Literal::Instance::getGreen() const {
    return cGreen;
  }

  float ColourRegistry::Literal::Instance::getBlue() const {
    return cBlue;
  }

  float ColourRegistry::Literal::Instance::getAlpha() const {
    return cAlpha;
  }

  void ColourRegistry::Literal::Instance::saveAsset(JSONObject object) const {
    object.addFloat(JSON_RED, cRed);
    object.addFloat(JSON_GREEN, cGreen);
    object.addFloat(JSON_BLUE, cBlue);
    object.addFloat(JSON_ALPHA, cAlpha);
  }

  void ColourRegistry::Literal::Instance::getAssetProperties(PropertyMaker& owner) {
    cEditingLastKnownHue = Utils::getHue(cRed, cGreen, cBlue);
    cEditingLastKnownSaturation = Utils::getSaturation(cRed, cGreen, cBlue);
    cEditingLastKnownLightness = Utils::getLightness(cRed, cGreen, cBlue);

    owner.createPropertyColourChannel(cMetadata.getPropertyData("Red"), [this]() {return cRed;}, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cGreen, &cBlue, &cAlpha, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cGreen, &cBlue, &cAlpha, [this](const float value) {
      cRed = value;
      cEditingLastKnownHue = Utils::getHue(cRed, cGreen, cBlue);
      cEditingLastKnownSaturation = Utils::getSaturation(cRed, cGreen, cBlue);
      cEditingLastKnownLightness = Utils::getLightness(cRed, cGreen, cBlue);
    });
    owner.createPropertyColourChannel(cMetadata.getPropertyData("Green"), [this]() {return cGreen;}, &cRed, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cBlue, &cAlpha, &cRed, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cBlue, &cAlpha, [this](const float value) {
      cGreen = value;
      cEditingLastKnownHue = Utils::getHue(cRed, cGreen, cBlue);
      cEditingLastKnownSaturation = Utils::getSaturation(cRed, cGreen, cBlue);
      cEditingLastKnownLightness = Utils::getLightness(cRed, cGreen, cBlue);
    });
    owner.createPropertyColourChannel(cMetadata.getPropertyData("Blue"), [this]() {return cBlue;}, &cRed, &cGreen, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cAlpha, &cRed, &cGreen, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cAlpha, [this](const float value) {
      cBlue = value;
      cEditingLastKnownHue = Utils::getHue(cRed, cGreen, cBlue);
      cEditingLastKnownSaturation = Utils::getSaturation(cRed, cGreen, cBlue);
      cEditingLastKnownLightness = Utils::getLightness(cRed, cGreen, cBlue);
    });
    owner.createPropertyColourChannel(cMetadata.getPropertyData("Alpha"), [this]() {return cAlpha;}, &cRed, &cGreen, &cBlue, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cRed, &cGreen, &cBlue, &PropertyColourChannel::MAX_CHANNEL_VALUE, [this](const float value) {
      cAlpha = value;
    });
    owner.createPropertyColourHue(cMetadata.getPropertyData("Hue"), [this]() {return cEditingLastKnownHue;}, &cEditingLastKnownSaturation, &cEditingLastKnownLightness, &cAlpha, [this](const float value) {
      cEditingLastKnownHue = value;
      Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cRed, cGreen, cBlue);
    });
    owner.createPropertyColourSaturation(cMetadata.getPropertyData("Saturation"), [this]() {return cEditingLastKnownSaturation;}, &cEditingLastKnownHue, &cEditingLastKnownLightness, &cAlpha, [this](const float value) {
      cEditingLastKnownSaturation = value;
      Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cRed, cGreen, cBlue);
    });
    owner.createPropertyColourLightness(cMetadata.getPropertyData("Lightness"), [this]() {return cEditingLastKnownLightness;}, &cEditingLastKnownHue, &cEditingLastKnownSaturation, &cAlpha, [this](const float value) {
      cEditingLastKnownLightness = value;
      Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cRed, cGreen, cBlue);
    });
  }

  bool ColourRegistry::Literal::Instance::isDefaultConfiguration() const {
    return cRed == 0.0f && cGreen == 0.0f && cBlue == 0.0f && cAlpha == 1.0f;
  }
}

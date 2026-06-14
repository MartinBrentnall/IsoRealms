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
  SimpleColour::SimpleColour(Basics& basics, IComponentData& data) :
            cEditingLastKnownHue(Utils::getHue(cDefRed, cDefGreen, cDefBlue)),
            cEditingLastKnownSaturation(Utils::getSaturation(cDefRed, cDefGreen, cDefBlue)),
            cEditingLastKnownLightness(Utils::getLightness(cDefRed, cDefGreen, cDefBlue)),
            cLuaBinding(data.getProject().getLuaState(), this) {
  }
  
  void SimpleColour::define(IComponentDefiner& definer) {
    definer.propertyColourChannel("red", [this]() {return cDefRed;}, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefGreen, &cDefBlue, &cDefAlpha, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cDefGreen, &cDefBlue, &cDefAlpha, [this](const float value) {
      cDefRed = value;
      cEditingLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
      cEditingLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
      cEditingLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);
      resetColour();
    });
    definer.propertyColourChannel("green", [this]() {return cDefGreen;}, &cDefRed, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefBlue, &cDefAlpha, &cDefRed, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cDefBlue, &cDefAlpha, [this](const float value) {
      cDefGreen = value;
      cEditingLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
      cEditingLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
      cEditingLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);
      resetColour();
    });
    definer.propertyColourChannel("blue", [this]() {return cDefBlue;}, &cDefRed, &cDefGreen, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefAlpha, &cDefRed, &cDefGreen, &PropertyColourChannel::MAX_CHANNEL_VALUE, &cDefAlpha, [this](const float value) {
      cDefBlue = value;
      cEditingLastKnownHue = Utils::getHue(cDefRed, cDefGreen, cDefBlue);
      cEditingLastKnownSaturation = Utils::getSaturation(cDefRed, cDefGreen, cDefBlue);
      cEditingLastKnownLightness = Utils::getLightness(cDefRed, cDefGreen, cDefBlue);
      resetColour();
    });
    definer.propertyColourChannel("alpha", [this]() {return cDefAlpha;}, &cDefRed, &cDefGreen, &cDefBlue, &PropertyColourChannel::MIN_CHANNEL_VALUE, &cDefRed, &cDefGreen, &cDefBlue, &PropertyColourChannel::MAX_CHANNEL_VALUE, [this](const float value) {
      cDefAlpha = value;
      resetColour();
    });
    definer.propertyColourHue("Hue", [this]() {return cEditingLastKnownHue;}, &cEditingLastKnownSaturation, &cEditingLastKnownLightness, &cDefAlpha, [this](const float value) {
      cEditingLastKnownHue = value;
      Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cDefRed, cDefGreen, cDefBlue);
      resetColour();
    });
    definer.propertyColourSaturation("Saturation", [this]() {return cEditingLastKnownSaturation;}, &cEditingLastKnownHue, &cEditingLastKnownLightness, &cDefAlpha, [this](const float value) {
      cEditingLastKnownSaturation = value;
      Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cDefRed, cDefGreen, cDefBlue);
      resetColour();
    });
    definer.propertyColourLightness("Lightness", [this]() {return cEditingLastKnownLightness;}, &cEditingLastKnownHue, &cEditingLastKnownSaturation, &cDefAlpha, [this](const float value) {
      cEditingLastKnownLightness = value;
      Utils::calculateColour(cEditingLastKnownHue, cEditingLastKnownSaturation, cEditingLastKnownLightness, cDefRed, cDefGreen, cDefBlue);
      resetColour();
    });
  }

  void SimpleColour::publish(ResourcePublisher& publisher) {
    cStateNotifier = publisher.publish<IColour>(this, "", "Fixed Colours");
    publisher.publish<IBinding>(&cLuaBinding, "", "Variables/Colours"); // TODO: Localize this.
  }

  bool SimpleColour::renderIcon() const {
    return renderResourceIcon();
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

  void SimpleColour::setRed(float value) {
    cRuntimeRed = value;
    cStateNotifier->stateChanged();
  }

  void SimpleColour::setGreen(float value) {
    cRuntimeGreen = value;
    cStateNotifier->stateChanged();
  }

  void SimpleColour::setBlue(float value) {
    cRuntimeBlue = value;
    cStateNotifier->stateChanged();
  }

  void SimpleColour::setAlpha(float value) {
    cRuntimeAlpha = value;
    cStateNotifier->stateChanged();
  }

  void SimpleColour::resetColour() {
    cRuntimeRed   = cDefRed;
    cRuntimeGreen = cDefGreen;
    cRuntimeBlue  = cDefBlue;
    cRuntimeAlpha = cDefAlpha;
    if (cStateNotifier != nullptr) {
      cStateNotifier->stateChanged();
    }
  }
}

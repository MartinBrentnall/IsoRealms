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
#include "PropertyNativeFloat.h"

namespace IsoRealms {
  PropertyNativeFloat::PropertyNativeFloat(const std::string& name, float* value, bool allowNone, std::function<void()> removeCallback) :
            PropertyInputField(name, Utils::toString(*value)),
            cAllowNone(allowNone),
            cFloatValue(value),
            cRemoveCallback(removeCallback) {
  }
  
  PropertyNativeFloat::PropertyNativeFloat(const std::string& name, std::function<void(const float)> addCallback) :
            PropertyInputField(name, ""),
            cAllowNone(true),
            cFloatValue(nullptr),
            cAddCallback(addCallback) {
  }
  
  bool PropertyNativeFloat::isKeyAllowed(SDL_keysym& keysym) {
    return (keysym.sym == '-' && cCaret == 0 && cValue[0] != '-') || (((keysym.sym == '.' && cValue.find('.') == std::string::npos) || (keysym.sym >= '0' && keysym.sym <= '9')) && cValue.length() < 40);
  }
  
  bool PropertyNativeFloat::confirmValue() {
    float mFloatValue;
    if (cAllowNone && cValue.empty()) {
      mFloatValue = std::numeric_limits<float>::quiet_NaN();
    } else try {
      mFloatValue = std::stof(cValue);
    } catch (std::out_of_range& e) {
      return false;
    } catch (std::invalid_argument& e) {
      return false;
    }
    
    if (cFloatValue != nullptr) {
      *cFloatValue = mFloatValue;
      cValue = std::isnan(mFloatValue) ? "" : Utils::toString(mFloatValue);
      if (std::isnan(mFloatValue)) {
        cRemoveCallback();
      }
    } else if (!std::isnan(mFloatValue)) {
      cAddCallback(mFloatValue);
      cValue = "";
    }
    return true;
  }
}

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
#include "PropertyNativeFloat.h"

#include "IsoRealms/Editing/IComponentAccessManager.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  PropertyNativeFloat::PropertyNativeFloat(const PropertyData& data, IComponentAccessManager& resourceAccessManager, std::function<float()> getter, std::function<bool(float)> validityChecker, std::function<void(float)> setter, std::function<void()> removeFunction) :
            PropertyInputField(data, resourceAccessManager, Utils::toString(getter()), removeFunction),
            cValidityChecker(validityChecker),
            cSetter(setter) {
    while (cValue.length() > 3 && cValue[cValue.length() - 1] == '0' && cValue[cValue.length() - 2] != '.') {
      cValue = cValue.substr(0, cValue.length() - 1);
    }
  }

  bool PropertyNativeFloat::isKeyAllowed(char character, unsigned int caret) {
    return (character == '-' && caret == 0 && cValue[0] != '-') || (((character == '.' && cValue.find('.') == std::string::npos) || (character >= '0' && character <= '9')) && cValue.length() < 40);
  }

  bool PropertyNativeFloat::confirmValue() {
    float mFloatValue;
    try {
      mFloatValue = std::stof(cValue);
    } catch (std::out_of_range& e) {
      return false;
    } catch (std::invalid_argument& e) {
      return false;
    }

    if (!cValidityChecker(mFloatValue)) {
      return false;
    }

    confirmAccess([this, mFloatValue]() {
      confirm();
      cSetter(mFloatValue);
    }, [this]() {
      cancel();
    });
    return true;
  }
}

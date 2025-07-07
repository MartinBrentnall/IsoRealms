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
#include "PropertyNativeUnsignedInteger.h"

#include "IsoRealms/Utils.h"

namespace IsoRealms {
  PropertyNativeUnsignedInteger::PropertyNativeUnsignedInteger(const std::string& name, const std::string& tooltip, std::function<unsigned int()> getter, std::function<bool(unsigned int)> setter, std::function<void()> removeFunction) :
            PropertyInputField(name, tooltip, Utils::toString(getter()), removeFunction),
            cSetter(setter) {
  }

  bool PropertyNativeUnsignedInteger::isKeyAllowed(char character, unsigned int caret) {
    return character >= '0' && character <= '9' && cValue.length() < 10;
  }

  bool PropertyNativeUnsignedInteger::confirmValue() {
    int mIntValue;
    try {
      mIntValue = std::stoi(cValue);
    } catch (std::out_of_range& e) {
      mIntValue = std::numeric_limits<unsigned int>::max();
    } catch (std::invalid_argument& e) {
      return false;
    }
    return cSetter(mIntValue);
  }
}


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
#include "PropertyNativeInteger.h"

#include "IsoRealms/Utils.h"

namespace IsoRealms {
  PropertyNativeInteger::PropertyNativeInteger(const PropertyData& data, IResourceAccessManager& resourceAccessManager, std::function<int()> getter, std::function<void(int)> setter, std::function<bool(int)> validityChecker, std::function<void()> removeFunction) :
            PropertyInputField(data, resourceAccessManager, Utils::toString(getter()), removeFunction),
            cSetter(setter) {
  }
  
  bool PropertyNativeInteger::isKeyAllowed(char character, unsigned int caret) {
    return (character == '-' && caret == 0 && cValue[0] != '-') || (character >= '0' && character <= '9' && cValue.length() < (cValue[0] == '-' ? 11 : 10));
  }
  
  bool PropertyNativeInteger::confirmValue() {
    int mIntValue;
    try {
      mIntValue = std::stoi(cValue);
    } catch (std::out_of_range& e) {
      mIntValue = cValue[0] == '-' ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    } catch (std::invalid_argument& e) {
      return false;
    }
    cSetter(mIntValue);
    return true;
  }
}

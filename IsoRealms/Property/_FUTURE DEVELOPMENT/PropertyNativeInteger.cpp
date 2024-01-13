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
#include "PropertyNativeInteger.h"

namespace IsoRealms {
  PropertyNativeInteger::PropertyNativeInteger(const std::string& name, int* value) :
            PropertyInputField(name, Utils::toString(*value)),
            cIntValue(value) {
  }
  
  bool PropertyNativeInteger::isKeyAllowed(SDL_keysym& keysym) {
    return (keysym.sym == '-' && cCaret == 0 && cValue[0] != '-') || (keysym.sym >= '0' && keysym.sym <= '9' && cValue.length() < (cValue[0] == '-' ? 11 : 10));
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
    *cIntValue = mIntValue;
    cValue = Utils::toString(mIntValue);
    return true;
  }
}

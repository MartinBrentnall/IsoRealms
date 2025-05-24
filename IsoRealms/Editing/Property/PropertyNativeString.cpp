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
#include "PropertyNativeString.h"

namespace IsoRealms {
  PropertyNativeString::PropertyNativeString(const std::string& name, std::function<std::string()> getter, std::function<bool(const std::string&)> setter, std::function<void()> removeFunction) :
            PropertyInputField(name, getter(), removeFunction),
            cSetter(setter) {
  }
  
  bool PropertyNativeString::isKeyAllowed(char character, unsigned int caret) {
    return cValue.length() < 40;
  }
  
  bool PropertyNativeString::confirmValue() {
    return cSetter(cValue);
  }
}

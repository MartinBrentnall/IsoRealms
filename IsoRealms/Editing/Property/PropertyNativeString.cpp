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
#include "PropertyNativeString.h"

namespace IsoRealms {
  PropertyNativeString::PropertyNativeString(const PropertyData& data, IResourceAccessManager& resourceAccessManager, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction) :
            PropertyInputField(data, resourceAccessManager, getter(), removeFunction),
            cValidityChecker(validityChecker),
            cSetter(setter) {
  }
  
  bool PropertyNativeString::isKeyAllowed(char character, unsigned int caret) {
    return cValue.length() < 40;
  }
  
  bool PropertyNativeString::confirmValue() {
    if (!cValidityChecker(cValue)) {
      return false;
    }

    confirmAccess([this]() {
      confirm();
      cSetter(cValue);
    }, [this]() {
      cancel();
    });
    return true;
  }
}

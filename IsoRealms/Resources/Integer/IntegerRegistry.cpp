/*
 * Copyright 2015 Martin Brentnall
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
#include "IntegerRegistry.h"

std::string IntegerRegistry::getSpecialLocation(IInteger* integer) {
  for (unsigned int i = 0; i < cLiteralIntegers.size(); i++) {
    if (cLiteralIntegers[i] == integer) {
      return "#" + Utils::toString(integer->getValue());
    }
  }
  return "";
}

IInteger* IntegerRegistry::getSpecialResource(const std::string& value) {
  if (value[0] == '#') {
    std::string mValue = value.substr(1);
    int mInteger = atoi(mValue.c_str());
    Integer* mIntegerObj = new Integer(mInteger);
    cLiteralIntegers.push_back(mIntegerObj);
    return mIntegerObj;
  }
  return NULL;
}

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
#include "IntegerToString.h"

IntegerToString::IntegerToString(IInteger* value) {
  cValue = value;
}

IInteger* IntegerToString::getInteger() {
  return cValue;
}

std::string IntegerToString::getValue() {
  return Utils::toString(cValue->getValue());
}

void IntegerToString::setValue(const std::string&) {
  // TODO: Try to convert value to int?
  std::cout << "WARNING: Trying to set integer with string value!" << std::endl;
  exit(1);
}

void IntegerToString::initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*) {
  std::cout << "WARNING: Trying to initialise wrapper!" << std::endl;
  exit(1);
}

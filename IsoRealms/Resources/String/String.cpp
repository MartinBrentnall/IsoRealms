/*
 * Copyright 2016 Martin Brentnall
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
#include "String.h"

String::String(const std::string& value) {
  setValue(value);
}

void String::setValue(const std::string& value) {
  cValue = value;
}

std::string String::getValue() {
  return cValue;
}

void String::initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*) {
}

Icon<IString>* String::getResourceIcon(IResourceBrowser<IString>* browser) {
  return nullptr;
}


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
#include "StringRegistry.h"

void StringRegistry::setIntegers(ResourceRegistry<IInteger>* integerRegistry) {
  cIntegers = integerRegistry;
}

IString* StringRegistry::getSpecialResource(const std::string& path) {
  if (path[0] == ':') {
    std::size_t mSplit = path.find('/');
    std::string mType = path.substr(1, mSplit - 1);
    if (mType == "Integer") {
      std::string mIntegerPath = path.substr(mSplit + 1);
      IInteger* mInteger = cIntegers->get(mIntegerPath);
      IntegerToString* mIntegerString = new IntegerToString(mInteger);
      cIntegerStrings.push_back(mIntegerString);
      return mIntegerString;
    } else {
      std::cout << "Unknown type \"" << mType << "\"" << std::endl;
      exit(1);
    }
  }
  return NULL;
}

std::string StringRegistry::getSpecialLocation(IString* string) {
  for (unsigned int i = 0; i < cIntegerStrings.size(); i++) {
    if (string == cIntegerStrings[i]) {
      return ":Integer/" + cIntegers->getLocation(cIntegerStrings[i]->getInteger());
    }
  }
  return ""; 
}


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
#include "ResourceStringTime.h"

ResourceStringTime::ResourceStringTime(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
}

void ResourceStringTime::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  std::string mIntegerPath = node->getAttribute("value");
  cValue = resources->getInteger(mIntegerPath);
}

std::string ResourceStringTime::getValue() {
  int mValue = cValue->getValue();
  int mMilliseconds = mValue % 1000;
  int mSeconds = mValue / 1000;
  int mMinutes = mSeconds / 60;
  mSeconds = mSeconds % 60;
  std::stringstream mStringStream;
  mStringStream << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setw(1) << mMilliseconds;
  return mStringStream.str();
}

void ResourceStringTime::setValue(const std::string&) {
  // Read only; cannot be set
}

void ResourceStringTime::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("value", resourceLocator->getPath(cValue));
}

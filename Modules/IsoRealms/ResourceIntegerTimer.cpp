/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "ResourceIntegerTimer.h"

ResourceIntegerTimer::ResourceIntegerTimer(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cLock = false;
  cMilliseconds = 0;
  resourceRegistry->add(new StringTimer(this), node->getAttribute("name"));
  resourceRegistry->addDynamicElement(this);
}

void ResourceIntegerTimer::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cMilliseconds = node->getIntegerAttribute("value");
}

void ResourceIntegerTimer::setValue(int value) {
  cMilliseconds = value;  
}

int ResourceIntegerTimer::getValue() {
  return cMilliseconds;
}

ResourceIntegerTimer::StringTimer::StringTimer(ResourceIntegerTimer* timer) {
  cTimer = timer;
}

void ResourceIntegerTimer::StringTimer::initialiseResource(DOMNodeWrapper*, IResourceAccessor*) {
  // Nothing to do
}

void ResourceIntegerTimer::StringTimer::setValue(const std::string& value) {
  // Not supported
}

std::string ResourceIntegerTimer::StringTimer::getValue() {
  int mMilliseconds = cTimer->cMilliseconds % 1000;
  int mSeconds = cTimer->cMilliseconds / 1000;
  int mMinutes = mSeconds / 60;
  mSeconds = mSeconds % 60;
  std::stringstream mStringStream;
  mStringStream << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setw(1) << mMilliseconds;
  return mStringStream.str();
}

void ResourceIntegerTimer::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("value", cMilliseconds);
}

void ResourceIntegerTimer::update(unsigned int milliseconds) {
  if (!cLock) {
    if (cMilliseconds > 0) {
      cMilliseconds -= milliseconds;
      if (cMilliseconds <= 0) {
        cMilliseconds = 0;
      }
    }
  }
}

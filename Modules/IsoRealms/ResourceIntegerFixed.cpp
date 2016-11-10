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
#include "ResourceIntegerFixed.h"

ResourceIntegerFixed::ResourceIntegerFixed(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cInitialValue = 0;
  cCurrentValue = 0;
}

void ResourceIntegerFixed::setInitialValue(int value) {
  cInitialValue = value;
}

int ResourceIntegerFixed::getInitialValue() {
  return cInitialValue;
}

void ResourceIntegerFixed::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cInitialValue = node->getIntegerAttribute("value");
  cCurrentValue = cInitialValue;
}

void ResourceIntegerFixed::save(DOMNodeWriter* node, IResourceLocator* resources) {
  if (cInitialValue != 0) {
    node->addAttribute("value", cInitialValue);
  }
}

void ResourceIntegerFixed::setValue(int value) {
  cCurrentValue = value;
}

int ResourceIntegerFixed::getValue() {
  return cCurrentValue;
}

void ResourceIntegerFixed::reset() {
  cCurrentValue = cInitialValue;
}

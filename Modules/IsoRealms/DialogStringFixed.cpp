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

#include "DialogStringFixed.h"

DialogStringFixed::DialogStringFixed(IEditingContext* editingContext, ResourceStringFixed* string, IResourceAccessor* resources, const std::string& resourceName) : DialogOKCancelUndo(editingContext, resources, "String", resourceName) {
  cContent = new RectangularComponent("Modules/IsoRealms/DialogStringFixed", resources);
  cString = string;
  cOriginalValue = cString->getValue();
  cContent->addStringListener(this, "stringFieldValue");
  cContent->setStringValue("stringFieldValue", cOriginalValue);
  addComponent("content", cContent);
}

void DialogStringFixed::undo() {
  cString->setValue(cOriginalValue);
  cContent->setStringValue("stringFieldValue", cOriginalValue);
}

ResourceStringFixed* DialogStringFixed::getResource() {
  return cString;
}

void DialogStringFixed::valueChanged(IValueComponent<std::string>* component, std::string value) {
  cString->setValue(value);
}

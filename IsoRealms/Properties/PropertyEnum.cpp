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
#include "PropertyEnum.h"

PropertyEnum::PropertyEnum(const std::string& name, IPropertyValue<unsigned int>* value, std::vector<std::string> items, IComponentContainer* windowWorkspace) {
  cName = name;
  cValue = value;
  cComponent = new DropDownListComponent(items, value->getValue(), windowWorkspace);
  cComponent->addValueListener(this);
}

std::string PropertyEnum::getPropertyName() {
  return cName;
}

ISizedComponent* PropertyEnum::getPropertyComponent() {
  return cComponent;
}

void PropertyEnum::valueChanged(IValueComponent<unsigned int>* component, unsigned int value) {
  cValue->setValue(value);
  value = cValue->getValue();
  cComponent->setValue(value);
}

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
#include "PropertyInteger.h"

PropertyInteger::PropertyInteger(const std::string& name, IPropertyValue<int>* value) {
  cName = name;
  cValue = value;
  cComponent = new TextFieldComponent(std::to_string(cValue->getValue()), false);
  cComponent->addValueListener(this);
}

std::string PropertyInteger::getPropertyName() {
  return cName;
}

ISizedComponent* PropertyInteger::getPropertyComponent() {
  return cComponent;
}

void PropertyInteger::valueChanged(IValueComponent<std::string>* component, std::string value) {
  try {
    int mValue = std::stoi(value);
    std::cout << "Value: \"" << value << "\" -> " << mValue << std::endl;
    cValue->setValue(mValue);
    mValue = cValue->getValue();
    cComponent->setValue(std::to_string(mValue));
  } catch (std::invalid_argument e) {
    std::cout << "Invalid: \"" << value << "\" -> " << cValue->getValue() << std::endl;
    std::string mOldValue = std::to_string(cValue->getValue());
    cComponent->setValue(mOldValue);
  }
}

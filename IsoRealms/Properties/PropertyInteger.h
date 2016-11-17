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
#ifndef PROPERTY_INTEGER_H
#define PROPERTY_INTEGER_H

#include <functional>
#include <string>

#include <IsoRealms/GUI/TextFieldComponent.h>
#include <IsoRealms/IObjectProperty.h>
#include <IsoRealms/Resources/ElementType/IElement.h>

#include "IPropertyValue.h"

class PropertyInteger:public IObjectProperty,
                      public IValueListener<std::string> {
  private:
  std::string cName;
  IPropertyValue<int>* cValue;
  TextFieldComponent* cComponent;
  
  public:
  PropertyInteger(const std::string&, IPropertyValue<int>*);

  /******************************\
   * Implements IObjectProperty *
  \******************************/
  std::string getPropertyName();
  ISizedComponent* getPropertyComponent();
  
  /******************************************\
   * Implements IValueListener<std::string> *
  \******************************************/
  void valueChanged(IValueComponent<std::string>*, std::string);
};

#endif


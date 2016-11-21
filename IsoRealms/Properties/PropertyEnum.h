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
#ifndef PROPERTY_ENUM_H
#define PROPERTY_ENUM_H

#include <functional>
#include <string>

#include <IsoRealms/GUI/DropDownListComponent.h>
#include <IsoRealms/IObjectProperty.h>
#include <IsoRealms/Resources/ElementType/IElement.h>

#include "IPropertyValue.h"

class PropertyEnum:public IObjectProperty,
                   public IValueListener<unsigned int> {
  private:
  std::string cName;
  IPropertyValue<unsigned int>* cValue;
  DropDownListComponent* cComponent;
  
  public:
  PropertyEnum(const std::string&, IPropertyValue<unsigned int>*, std::vector<std::string>, IComponentContainer*);

  /******************************\
   * Implements IObjectProperty *
  \******************************/
  std::string getPropertyName();
  ISizedComponent* getPropertyComponent();
  
  /*******************************************\
   * Implements IValueListener<unsigned int> *
  \*******************************************/
  void valueChanged(IValueComponent<unsigned int>*, unsigned int);
};

#endif



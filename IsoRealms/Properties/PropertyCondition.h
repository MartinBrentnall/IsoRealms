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
#ifndef PROPERTY_CONDITION_H
#define PROPERTY_CONDITION_H

#include <functional>
#include <string>

#include <IsoRealms/GUI/TextLabelComponent.h>
#include <IsoRealms/IConditionElementIcons.h>
#include <IsoRealms/IObjectProperty.h>
#include <IsoRealms/Condition.h>

#include "ConditionElementIcon.h"
#include "ConditionValueComponent.h"
#include "IPropertyValue.h"

class PropertyCondition:public IObjectProperty {
  private:
  std::string cName;
  IPropertyValue<Condition*>* cValue;
  ConditionValueComponent* cComponent;
  IConditionElementIcons* cConditionElementIcons;
  
  std::vector<ISizedComponent*> getConditionIcons(Condition*);
  
  public:
  PropertyCondition(const std::string&, IPropertyValue<Condition*>*, IConditionElementIcons*);

  /******************************\
   * Implements IObjectProperty *
  \******************************/
  std::string getPropertyName();
  ISizedComponent* getPropertyComponent();
};

#endif


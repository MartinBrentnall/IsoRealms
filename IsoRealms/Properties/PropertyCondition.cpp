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
#include "PropertyCondition.h"

PropertyCondition::PropertyCondition(const std::string& name, IPropertyValue<Condition*>* value, IConditionElementIcons* icons) {
  cName = name;
  cValue = value;
  cConditionElementIcons = icons;
  Condition* mCondition = value->getValue();
  std::vector<ISizedComponent*> mConditionIcons = getConditionIcons(mCondition);
  cComponent = new ConditionValueComponent(mConditionIcons);
}

std::string PropertyCondition::getPropertyName() {
  return cName;
}

ISizedComponent* PropertyCondition::getPropertyComponent() {
  return cComponent;
}

std::vector<ISizedComponent*> PropertyCondition::getConditionIcons(Condition* condition) {
  std::vector<ISizedComponent*> mElementIconComponents;
  if (condition != nullptr) {
    std::set<ConditionElement*> mConditionElements = condition->getConditionElements();
    std::vector<Condition*> mSubConditions = condition->getConditions();
    std::string mOperatorString = condition->isAnd() ? "&" : "|";
    unsigned int mPartsToGo = mConditionElements.size() + mSubConditions.size();
    if (condition->isNegated()) {
      ISizedComponent* mNegationIcon = new TextLabelComponent("!", 0.0f);
      mElementIconComponents.push_back(mNegationIcon);
    }
    ISizedComponent* mBeginCondition = new TextLabelComponent("(", 0.0f);
    mElementIconComponents.push_back(mBeginCondition);
    for (Condition* mSubCondition : mSubConditions) {
      std::vector<ISizedComponent*> mSubConditionIcons = getConditionIcons(mSubCondition);
      for (ISizedComponent* mSubConditionIcon : mSubConditionIcons) {
        mElementIconComponents.push_back(mSubConditionIcon);
      }
      if (--mPartsToGo > 0) {
        ISizedComponent* mOperatorIcon = new TextLabelComponent(mOperatorString, 0.0f);
        mElementIconComponents.push_back(mOperatorIcon);
      }      
    }
    for (ConditionElement* mElement : mConditionElements) {
      I3DModel* mElementIcon = cConditionElementIcons->getConditionElementIcon(mElement);
      ISizedComponent* mElementIconComponent = new ConditionElementIcon(mElementIcon);
      mElementIconComponents.push_back(mElementIconComponent);
      if (--mPartsToGo > 0) {
        ISizedComponent* mOperatorIcon = new TextLabelComponent(mOperatorString, 0.0f);
        mElementIconComponents.push_back(mOperatorIcon);
      }      
    }
    ISizedComponent* mEndCondition = new TextLabelComponent(")", 0.0f);
    mElementIconComponents.push_back(mEndCondition);
  }
  return mElementIconComponents;
}

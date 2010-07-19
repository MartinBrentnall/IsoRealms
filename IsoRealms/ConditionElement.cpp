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
#include "ConditionElement.h"

ConditionElement::ConditionElement(const std::string& name, bool* input, bool negated) {
  cInputName = name;
  cInput = input;
  cNegated = negated;
}

ConditionElement::ConditionElement(const ConditionElement& conditionElement) {
  cInputName = conditionElement.cInputName;
  cInput = conditionElement.cInput;
  cNegated = conditionElement.cNegated; 
}

ConditionElement* ConditionElement::getNegatedElement() {
  ConditionElement* mNegatedElement = new ConditionElement(*this);
  mNegatedElement->cNegated = !cNegated;
  return mNegatedElement;
}

bool ConditionElement::operator==(const ConditionElement& conditionElement) const {
  return cInput == conditionElement.cInput && cNegated == conditionElement.cNegated;
}

bool ConditionElement::operator!=(const ConditionElement& conditionElement) const {
  std::cout << "Input " << cInput << " is: " << *cInput << std::endl;
  return !(*this == conditionElement);
}

bool ConditionElement::isTrue() {
  return *cInput != cNegated;
}

bool ConditionElement::isNegated() {
  return cNegated;
}

bool* ConditionElement::getInputAddress() {
  return cInput;
}

std::string ConditionElement::getName() {
  return cInputName;
}

void ConditionElement::debug() {
  if (cNegated) {
    std::cout << "!";
  }
  std::cout << cInputName;
}





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
#ifndef CONDITION_ELEMENT_H
#define CONDITION_ELEMENT_H

#include <iostream>
#include <string>

#include "DOMNodeWriter.h"

class ConditionElement {
  private:
  std::string cInputName;
  bool* cInput;
  bool cNegated;
  ConditionElement* cNegatedVersion;

  ConditionElement(const ConditionElement&);

  public:
  ConditionElement(const std::string&, bool*, bool = false);

  ConditionElement* getNegatedElement();
  bool operator==(const ConditionElement&) const;
  bool operator!=(const ConditionElement&) const;
  bool isTrue();
  bool isNegated();
  bool* getInputAddress();
  std::string getName();
  void debug();
  void save(DOMNodeWriter*);
};

#endif

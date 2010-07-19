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
#ifndef CONDITION_H
#define CONDITION_H

#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "ConditionElement.h"
#include "DOMNodeWrapper.h"
#include "DOMNodeWriter.h"

class Condition {
  private:
  bool cAnd;
  bool cNegated;
  std::vector<Condition*> cConditions;
  std::set<ConditionElement*> cElements;
  
  void checkForAbsoluteConditions();
  void raiseCondition(int);
  void raiseConditions();
  void checkForConflictingElements();

  public:
  Condition(bool, bool = false);
  Condition(const Condition&);
  Condition(DOMNodeWrapper*, std::vector<ConditionElement*>);
    
  Condition* split(Condition*);
  void compose(Condition*);
  void negate();
  bool isAbsolute() const;
  bool isTrue();
  void save(DOMNodeWriter*);
  
  bool isCompatibleWith(Condition*);
  
  void simplify();
  
  bool operator==(const Condition&) const;
  bool operator!=(const Condition&) const;
  
  void debug(int = 0) const;
};

#endif

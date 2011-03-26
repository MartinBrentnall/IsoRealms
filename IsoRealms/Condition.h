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

  /**
   * Convert all negated conditions into positive conditions.
   * This is the first step in the simplification process.  It ensures that we
   * know that we're not working with negated conditions.
   */
  void convertNegatedConditions();
  
  /**
   * Raise the contents of conditions into this condition where possible.
   * A condition can be raised if it shares the same gate with this one, or
   * it only contains one element.
   * This is the second step in the simplification process.  It ensures that we
   * know that we're working with alternating gates as we recurse through
   * conditions.
   */
  void raiseConditions();

  /**
   * Find opposing elements.
   * 
   * For an OR gate, opposing elements are redundant because at least one of
   * them will always be true, so the opposing elements are removed.
   * 
   * For an AND gate, opposing elements make the condition impossible to
   * satisfy, so everything is removed and this condition is set to literal
   * FALSE (although any negation will remain).
   */
  void checkForConflictingElements();

  /**
   * Find and deal with literal TRUE and FALSE conditions.
   * 
   * For an OR gate:
   *  - A TRUE condition turns this condition into literal TRUE.
   *  - A FALSE condition has no effect and is removed.
   * For an AND gate:
   *  - A TRUE condition has no effect and is removed.
   *  - A FALSE condition turns this condition into literal FALSE.
   */
  void checkForAbsoluteConditions();
  void checkForConflictingConditions();
  void raiseCondition(int);
  void negateEverything();
  void removeDuplicates();

  public:
  Condition(bool, bool = false);
  Condition(const Condition&);
  Condition(DOMNodeWrapper*, std::vector<ConditionElement*>);
    
  std::set<bool*> getInputs();
  Condition* split(Condition*);
  void compose(Condition*);
  void negate();
  bool isAbsolute() const;
  bool isTrue();
  void save(DOMNodeWriter*);
  
  bool isCompatibleWith(Condition*);
  
  void simplify(int = 0);
  
  bool operator==(const Condition&) const;
  bool operator!=(const Condition&) const;

  void debug(const std::string&) const;
  void debug(int = 0) const;
};

#endif

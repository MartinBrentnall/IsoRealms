/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <functional>
#include <iostream>
#include <string>

namespace IsoRealms {
  class JSONObject;
  class IBoolean;
  class IScreen;

  class ConditionElement {
    public:
    class Clause {
      public:
      Clause(ConditionElement& parent, bool negated);

      bool isTrue() const;
      bool isTestTrue() const;
      bool isNegated() const;
      bool operator==(const Clause&) const;
      bool operator!=(const Clause&) const;
      Clause* getNegatedClause() const;
      ConditionElement* getElement() const;
      void save(JSONObject object) const;
      void saveCache(std::ostream& cache, unsigned char elementType) const;
      void debug() const;

      private:
      ConditionElement& cParent;
      bool cNegated;
    };

    ConditionElement(std::function<std::string()> nameFunction, IScreen& icon, IBoolean* input);

    std::string getName() const;
    IBoolean* getInputAddress() const;
    void renderIcon() const;
    Clause* getPositiveClause();
    Clause* getNegativeClause();
    void setTestInput(bool value);
    bool getTestInput() const;

    private:
    static const std::string JSON_INPUT;
    static const std::string JSON_NEGATED;

    std::function<std::string()> cInputNameFunction;
    IBoolean* cInput;
    bool cTestInput;
    IScreen& cIcon;
    Clause cPositiveClause;
    Clause cNegativeClause;
  };
}

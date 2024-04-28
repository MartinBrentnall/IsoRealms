/*
 * Copyright 2023 Martin Brentnall
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

namespace IsoRealms {
  ConditionElement::ConditionElement(const std::string& name, IScreen* icon, IBoolean* input) :
            cInputName(name),
            cInput(input),
            cTestInput(false),
            cIcon(icon),
            cPositiveClause(this, false),
            cNegativeClause(this, true) {
  }

  std::string ConditionElement::getName() const {
    return cInputName;
  }

  IBoolean* ConditionElement::getInputAddress() const {
    return cInput;
  }

  void ConditionElement::renderIcon() const {
    cIcon->renderScreen(1.0f, 1.0f);
  }

  ConditionElement::Clause* ConditionElement::getPositiveClause() {
    return &cPositiveClause;
  }

  ConditionElement::Clause* ConditionElement::getNegativeClause() {
    return &cNegativeClause;
  }

  void ConditionElement::setTestInput(bool value) {
    cTestInput = value;
  }

  bool ConditionElement::getTestInput() const {
    return cTestInput;
  }

  ConditionElement::Clause::Clause(ConditionElement* parent, bool negated) :
            cParent(parent),
            cNegated(negated) {
  }

  bool ConditionElement::Clause::isTrue() const {
    return cParent->cInput->getValue() != cNegated;
  }

  bool ConditionElement::Clause::isTestTrue() const {
    return cParent->cTestInput != cNegated;
  }

  bool ConditionElement::Clause::isNegated() const {
    return cNegated;
  }

  bool ConditionElement::Clause::operator==(const Clause& clause) const {
    return cParent->cInput == clause.cParent->cInput && cNegated == clause.cNegated;
  }

  bool ConditionElement::Clause::operator!=(const Clause& clause) const {
    return !(*this == clause);
  }

  ConditionElement::Clause* ConditionElement::Clause::getNegatedClause() const {
    return this == &cParent->cPositiveClause ? &cParent->cNegativeClause : &cParent->cPositiveClause;
  }

  ConditionElement* ConditionElement::Clause::getElement() const {
    return cParent;
  }

  void ConditionElement::Clause::save(DOMNodeWriter* node) const {
    DOMNodeWriter mElementBranch = node->addBranch("Element");
    mElementBranch.addAttribute("name", cParent->cInputName);
    if (cNegated) {
      mElementBranch.addAttribute("negated", "true");
    }
  }

  void ConditionElement::Clause::saveCache(std::ostream& cache, unsigned char elementType) const {
    cache.write(reinterpret_cast<const char*>(&elementType), sizeof(elementType));
    size_t mLength = cParent->cInputName.length();
    cache.write(reinterpret_cast<const char*>(&mLength),                sizeof(mLength));
    cache.write(reinterpret_cast<const char*>(&cParent->cInputName[0]), mLength);
    cache.write(reinterpret_cast<const char*>(&cNegated),               sizeof(cNegated));
  }

  void ConditionElement::Clause::debug() const {
    if (cNegated) {
      std::cout << "!";
    }
    std::cout << cParent->cInputName;
  }
}

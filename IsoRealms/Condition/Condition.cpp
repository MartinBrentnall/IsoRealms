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
#include "Condition.h"

#include "IsoRealms/IComponentDefiner.h"
#include "IsoRealms/Exception/ArgumentException.h"
#include "IsoRealms/Project/Options.h"

namespace IsoRealms {
  Condition::Condition(bool andGate, bool negated) :
            cDefAnd(andGate),
            cDefNegated(negated) {
  }

  Condition::Condition(const Condition& condition) :
            cDefAnd(condition.cDefAnd),
            cDefNegated(condition.cDefNegated),
            cDefCriteria(condition.cDefCriteria) {
    for (const Condition& mCondition : condition.cDefConditions) {
      cDefConditions.emplace_back(Condition(mCondition));
    }
  }

  Condition::Condition(std::ifstream& cache, std::vector<ConditionElement*> elements, unsigned char conditionType, unsigned char elementType, unsigned char endType) {
    std::map<std::string, ConditionElement*> mElements;
    for (unsigned int i = 0; i < elements.size(); i++) {
      mElements[elements[i]->getName()] = elements[i];
    }

    unsigned char mAttribs;
    cache.read(reinterpret_cast<char*>(&mAttribs), sizeof(mAttribs));
    cDefAnd     = mAttribs & ATTRIB_AND;
    cDefNegated = mAttribs & ATTRIB_NEGATE;

    char mNextEntity;
    cache.read(reinterpret_cast<char*>(&mNextEntity), sizeof(mNextEntity));
    while (mNextEntity != endType) {
      if (mNextEntity == elementType) {
        std::string mName;
        size_t mNameLength;
        cache.read(reinterpret_cast<char*>(&mNameLength), sizeof(mNameLength));
        mName.resize(mNameLength);
        cache.read(&mName[0], mNameLength);
        bool mNegated;
        cache.read(reinterpret_cast<char*>(&mNegated), sizeof(mNegated));

        std::map<std::string, ConditionElement*>::iterator i = mElements.find(mName);
        if (i == mElements.end()) {
          std::cout << "Element \"" << mName << "\" not in list of conditions.  Available conditions:" << std::endl;
          for (unsigned int i = 0; i < elements.size(); i++) {
            std::cout << i << ":      " << elements[i]->getName() << std::endl;
          }
          throw ArgumentException("ERROR: Condition::Condition: Condition element \"" + mName + "\" is not in the specified list of elements.");
        }
        if (mNegated) {
          cDefCriteria.insert(i->second->getNegativeClause());
        } else {
          cDefCriteria.insert(i->second->getPositiveClause());
        }
      } else if (mNextEntity == conditionType) {
        cDefConditions.emplace_back(Condition(cache, elements, conditionType, elementType, endType)); // TODO: Make constructor pass map instead
      }
      cache.read(reinterpret_cast<char*>(&mNextEntity), sizeof(mNextEntity));
    }
  }

  void Condition::define(IComponentDefiner& definer, const std::vector<ConditionElement*>& availableElements) {

    // If we're loading persisted values, clear any existing criteria and conditions.
    if (definer.loadsPersistedValues()) {
      cDefCriteria.clear();
      cDefConditions.clear();
    }

    definer.propertyBoolean("negated", [this]() {return cDefNegated;}, [this](bool value) {cDefNegated = value;});

    // Operator is only applicable if there are multiple criteria or conditions.
    if (!definer.savesPersistedValues() || cDefCriteria.size() + cDefConditions.size() > 1) {
      definer.propertyList("operator",
                           {OPERATOR_AND, OPERATOR_OR},
                           [this]() {return cDefAnd ? OPERATOR_AND : OPERATOR_OR;},
                           [this](const std::string& value) {cDefAnd = value == OPERATOR_AND;},
                           OPERATOR_AND);
    }

    // Handle sub-conditions as an array.
    definer.array("subConditions", cDefConditions, [](const Condition& condition) -> Condition& {return const_cast<Condition&>(condition);}, [&definer, &availableElements](Condition& condition) {
      condition.define(definer, availableElements);
    }, [this]() -> Condition& {
      return cDefConditions.emplace_back(true);
    });

    // TODO: Criteria handling is currently indirect via InputDefinition.  This should be refactored to be more direct.
    // Everything below this point should be refactored to be more direct.
    struct InputDefinition {
      std::string name;
      bool negated = false;
    };
    
    std::vector<InputDefinition> mInputs;
    if (definer.savesPersistedValues()) {
      for (ConditionElement::Clause* mCriteria : cDefCriteria) {
        mInputs.push_back({mCriteria->getElement()->getName(), mCriteria->isNegated()});
      }
    }

    std::map<std::string, ConditionElement*> mElementsByName;
    for (ConditionElement* mElement : availableElements) {
      mElementsByName[mElement->getName()] = mElement;
    }

    definer.array("inputs", mInputs, [](const InputDefinition& input) -> InputDefinition& {return const_cast<InputDefinition&>(input);}, [&definer, &mElementsByName, this](InputDefinition& input) {
      definer.propertyString("input", [ &input]() {return input.name;}, [ &input](const std::string& value) {input.name = value;});
      definer.propertyBoolean("negated", [ &input]() {return input.negated;}, [ &input](bool value) {input.negated = value;});
      if (definer.loadsPersistedValues()) {
        std::map<std::string, ConditionElement*>::iterator mElement = mElementsByName.find(input.name);
        if (mElement == mElementsByName.end()) {
          throw ArgumentException("ERROR: Condition::define: Condition element \"" + input.name + "\" is not in the specified list of elements.");
        }
        cDefCriteria.insert(input.negated ? mElement->second->getNegativeClause() : mElement->second->getPositiveClause());
      }
    }, [&mInputs]() -> InputDefinition& {
      return mInputs.emplace_back();
    });
  }

  void Condition::saveCache(std::ostream& cache, unsigned char conditionType, unsigned char elementType, unsigned char endType) const {
    cache.write(reinterpret_cast<const char*>(&conditionType), sizeof(conditionType));
    unsigned char mAttribs = 0;
    if (cDefAnd)     {mAttribs |= ATTRIB_AND;}
    if (cDefNegated) {mAttribs |= ATTRIB_NEGATE;}
    cache.write(reinterpret_cast<const char*>(&mAttribs), sizeof(mAttribs));
    for (ConditionElement::Clause* mCriteria : cDefCriteria) {
      mCriteria->saveCache(cache, elementType);
    }
    for (unsigned int i = 0; i < cDefConditions.size(); i++) {
      cDefConditions[i].saveCache(cache, conditionType, elementType, endType);
    }
    cache.write(reinterpret_cast<const char*>(&endType), sizeof(endType));
  }

  std::vector<Condition> Condition::getConditions() {
    return cDefConditions;
  }

  std::set<ConditionElement::Clause*> Condition::getConditionElements() {
    return cDefCriteria;
  }

  bool Condition::isNegated() {
    return cDefNegated;
  }

  bool Condition::isAnd() {
    return cDefAnd;
  }

  bool Condition::operator==(const Condition& condition) const {
    Condition mThis(*this);
    Condition mThat(condition);
    mThis.simplify();
    mThat.simplify();
    if (mThis.cDefCriteria.size() != mThat.cDefCriteria.size() || mThis.cDefConditions.size() != mThat.cDefConditions.size() || mThis.cDefNegated != mThat.cDefNegated) {
      return false;
    }

    if (mThis.cDefCriteria.size() + mThis.cDefConditions.size() != 1 && mThis.cDefAnd != mThat.cDefAnd) {
      return false;
    }
    // TODO: This test assumes that simplify has sorted the elements, which simplify doesn't do yet.
    std::set<ConditionElement::Clause*>::iterator mThisElement = mThis.cDefCriteria.begin();
    std::set<ConditionElement::Clause*>::iterator mThatElement = mThat.cDefCriteria.begin();
    while (mThisElement != mThis.cDefCriteria.end()) {
      if (**mThisElement != **mThatElement) {
        return false;
      }
      mThisElement++;
      mThatElement++;
    }
    // TODO: This test assumes that simplify has sorted the conditions, which simplify doesn't do yet.
    for (unsigned int i = 0; i < mThis.cDefConditions.size(); i++) {
      if (mThis.cDefConditions[i] != mThat.cDefConditions[i]) {
        return false;
      }
    }
    return true;
  }

  bool Condition::operator!=(const Condition& condition) const {
    return !(*this == condition);
  }

  std::set<IBoolean*> Condition::getInputs() {
    std::set<IBoolean*> mInputs;
    for (unsigned int i = 0; i < cDefConditions.size(); i++) {
      std::set<IBoolean*> mSubInputs = cDefConditions[i].getInputs();
      mInputs.insert(mSubInputs.begin(), mSubInputs.end());
    }
    for (ConditionElement::Clause* mCriteria : cDefCriteria) {
      IBoolean* mInput = mCriteria->getElement()->getInputAddress();
      mInputs.insert(mInput);
    }
    return mInputs;
  }

  std::vector<Condition> Condition::split(std::optional<Condition>& condition) {
    std::vector<Condition> mSplitConditions;
    if (!condition.has_value()) {
      return mSplitConditions;
    }

    // Create a condition based on this condition AND the split condition
    Condition mCondition(true);
    mCondition.cDefConditions.emplace_back(Condition(*this));
    mCondition.cDefConditions.emplace_back(Condition(condition.value()));

    // Create a condition based on this condition AND the negation of the split condition
    Condition mOpposingCondition(true);
    mOpposingCondition.cDefConditions.emplace_back(Condition(*this));
    mOpposingCondition.cDefConditions.emplace_back(Condition(condition->negate()));

    if (!mCondition.canBe(true) || !mOpposingCondition.canBe(true)) {
      return mSplitConditions;
    }

    mCondition.simplify();
    mOpposingCondition.simplify();
    mSplitConditions.emplace_back(Condition(mCondition));
    mSplitConditions.emplace_back(Condition(mOpposingCondition));
    return mSplitConditions;
  }

  std::set<ConditionElement*> Condition::getAllConditionElements() const {
    std::set<ConditionElement*> mElements;
    for (ConditionElement::Clause* mCriteria : cDefCriteria) {
      mElements.insert(mCriteria->getElement());
    }
    for (const Condition& mSubCondition : cDefConditions) {
      std::set<ConditionElement*> mSubElements = mSubCondition.getAllConditionElements();
      mElements.insert(mSubElements.begin(), mSubElements.end());
    }
    return mElements;
  }

  bool Condition::testInputs(std::vector<ConditionElement*> inputs, unsigned int index, bool value) const {
    if (index == inputs.size()) {
      return isTestTrue() == value;
    }
    inputs[index]->setTestInput(false);
    if (testInputs(inputs, index + 1, value)) {
      return true;
    }
    inputs[index]->setTestInput(true);
    return testInputs(inputs, index + 1, value);
  }

  bool Condition::canBe(bool value) const {
    std::set<ConditionElement*> mInputs = getAllConditionElements();
    std::vector<ConditionElement*> mVectorInputs(mInputs.begin(), mInputs.end());
    return testInputs(mVectorInputs, 0, value);
  }

  void Condition::checkForAbsoluteConditions() {
    for (int i = static_cast<int>(cDefConditions.size()) - 1; i >= 0; i--) {
      if (cDefConditions[i].isAbsolute()) {
        if (cDefConditions[i].isTrue() == cDefAnd) {
          cDefConditions.erase(cDefConditions.begin() + i);
        } else {
          cDefConditions.clear();
          cDefCriteria.clear();
          cDefNegated = !cDefNegated;
          return;
        }
      }
    }
  }

  void Condition::raiseCondition(int index) {
    for (unsigned int i = 0; i < cDefConditions[index].cDefConditions.size(); i++) {
      cDefConditions.emplace_back(Condition(cDefConditions[index].cDefConditions[i]));
    }
    for (ConditionElement::Clause* mCriteria : cDefConditions[index].cDefCriteria) {
      if (cDefConditions[index].cDefNegated) {
        cDefCriteria.insert(mCriteria->getNegatedClause());
      } else {
        cDefCriteria.insert(mCriteria);
      }
    }
    cDefConditions.erase(cDefConditions.begin() + index);
  }

  void Condition::raiseConditions() {
    for (int i = static_cast<int>(cDefConditions.size()) - 1; i >= 0; i--) {
      cDefConditions[i].raiseConditions();
      if ((cDefConditions[i].cDefConditions.empty() && cDefConditions[i].cDefCriteria.size() == 1) || (cDefConditions[i].cDefAnd == cDefAnd && cDefConditions[i].cDefNegated == cDefNegated)) {
        raiseCondition(i);
      }
    }

    if (cDefConditions.size() == 1 && cDefCriteria.empty()) {
      cDefCriteria   = cDefConditions[0].cDefCriteria;
      cDefAnd        = cDefConditions[0].cDefAnd;
      for (unsigned int i = 0; i < cDefConditions[0].cDefConditions.size(); i++) {
        cDefConditions.emplace_back(Condition(cDefConditions[0].cDefConditions[i]));
      }
      cDefConditions.erase(cDefConditions.begin());
    }
  }

  void Condition::checkForConflictingElements() {

    // TODO: This seems to assume AND gate...  for OR gate, conflicting elements should simply be removed
    std::set<IBoolean*> mPositiveElements;
    std::set<IBoolean*> mNegativeElements;
    for (ConditionElement::Clause* mCriteria : cDefCriteria) {
      IBoolean* mAddress = mCriteria->getElement()->getInputAddress();
      std::set<IBoolean*>& mSetToAddTo = mCriteria->isNegated() ? mNegativeElements : mPositiveElements;
      std::set<IBoolean*>& mOtherSet   = mCriteria->isNegated() ? mPositiveElements : mNegativeElements;
      if (mOtherSet.find(mAddress) != mOtherSet.end()) {
        cDefCriteria.clear();
        cDefConditions.clear();
        cDefNegated = !cDefNegated;
        return;
      }
      mSetToAddTo.insert(mAddress);
    }
  }

  void Condition::checkForConflictingConditions() {
    std::vector<ConditionElement::Clause*> cCriteriaToRemove;

    // TODO: We should really check ALL subconditions recursively!
    // TODO: How should this deal with OR conditions?  I've only thought this through using AND so far
    // From here we assume that the gate of the subcondition is different to this one!
    for (unsigned int i = 0; i < cDefConditions.size(); i++) {
      for (ConditionElement::Clause* mSubCriteria : cDefConditions[i].cDefCriteria) {
        for (ConditionElement::Clause* mCriteria : cDefCriteria) {
          if (mCriteria->getElement()->getInputAddress() == mSubCriteria->getElement()->getInputAddress() && mCriteria->isNegated() != mSubCriteria->isNegated()) {
            cCriteriaToRemove.push_back(mSubCriteria);
          }
        }
      }
      for (unsigned int j = 0; j < cCriteriaToRemove.size(); j++) {
        cDefConditions[i].cDefCriteria.erase(cCriteriaToRemove[j]);
      }
      cCriteriaToRemove.clear();
    }
  }

  void Condition::negateEverything() {
    cDefNegated = !cDefNegated;
    cDefAnd = !cDefAnd;
    for (unsigned int i = 0; i < cDefConditions.size(); i++) {
      cDefConditions[i].cDefNegated = !cDefConditions[i].cDefNegated;
    }
    std::set<ConditionElement::Clause*> mNegatedElements;
    for (ConditionElement::Clause* mCriteria : cDefCriteria) {
      ConditionElement::Clause* mNegatedElement = mCriteria->getNegatedClause();
      mNegatedElements.insert(mNegatedElement);
    }
    cDefCriteria = mNegatedElements;
  }

  void Condition::convertNegatedConditions() {
    for (unsigned int i = 0; i < cDefConditions.size(); i++) {
      if (cDefConditions[i].cDefNegated) {
        cDefConditions[i].negateEverything();
      }
    }
    if (cDefNegated) {
      negateEverything();
    }
  }

  void Condition::removeDuplicates() {
    std::vector<ConditionElement::Clause*> mElementsToRemove;
    for (std::set<ConditionElement::Clause*>::iterator i = cDefCriteria.begin(); i != cDefCriteria.end(); i++) {
      for (std::set<ConditionElement::Clause*>::iterator j = i; j != cDefCriteria.end(); j++) {
        if (i != j && **i == **j) {
          mElementsToRemove.push_back(*j);
        }
      }
    }
    for (unsigned int i = 0; i < mElementsToRemove.size(); i++) {
      cDefCriteria.erase(mElementsToRemove[i]);
    }
  }

  void Condition::simplify(int depth) {
    convertNegatedConditions();
    for (unsigned int i = 0; i < cDefConditions.size(); i++) {
      cDefConditions[i].simplify(depth + 1);
    }
    checkForAbsoluteConditions();
    raiseConditions();
    checkForConflictingElements();
    checkForConflictingConditions();
    raiseConditions();
    checkForConflictingConditions();
    checkForAbsoluteConditions();
    removeDuplicates();
  }

  bool Condition::isAbsolute() const {
    return cDefCriteria.empty() && cDefConditions.empty();
  }

  bool Condition::isTrue() const {
    for (ConditionElement::Clause* mCriteria : cDefCriteria) {
      if (cDefAnd != mCriteria->isTrue()) {
        return cDefAnd == cDefNegated;
      }
    }
    for (unsigned int i = 0; i < cDefConditions.size(); i++) {
      if (cDefAnd != cDefConditions[i].isTrue()) {
        return cDefAnd == cDefNegated;
      }
    }
    return cDefAnd != cDefNegated;
  }

  bool Condition::isTestTrue() const {
    for (ConditionElement::Clause* mCriteria : cDefCriteria) {
      if (cDefAnd != mCriteria->isTestTrue()) {
        return cDefAnd == cDefNegated;
      }
    }
    for (unsigned int i = 0; i < cDefConditions.size(); i++) {
      if (cDefAnd != cDefConditions[i].isTestTrue()) {
        return cDefAnd == cDefNegated;
      }
    }
    return cDefAnd != cDefNegated;
  }

  Condition Condition::compose(std::optional<Condition>& condition) {
    if (!condition.has_value()) {
      return cDefAnd ? Condition(*this) : Condition(true);
    }
    Condition mNewCondition(*this);
    if (cDefAnd) {
      mNewCondition.cDefConditions.clear();
      mNewCondition.cDefCriteria.clear();
      mNewCondition.cDefAnd = false;
      mNewCondition.cDefNegated = false;
      mNewCondition.cDefConditions.emplace_back(Condition(*this));
    }
    mNewCondition.cDefConditions.emplace_back(Condition(*condition));
    mNewCondition.simplify();
    return mNewCondition;
  }

  void Condition::add(Condition& condition) {
    cDefConditions.emplace_back(Condition(condition));
  }

  void Condition::add(ConditionElement::Clause* criteria) {
    cDefCriteria.insert(criteria);
  }

  bool Condition::isCompatibleWith(std::optional<Condition>& condition) {
    if (!condition.has_value()) {
      return canBe(true);
    }

    Condition mBothConditions(true);
    mBothConditions.cDefConditions.emplace_back(Condition(*this));
    mBothConditions.cDefConditions.emplace_back(Condition(condition.value()));

    return mBothConditions.canBe(true);
  }

  Condition Condition::negate() {
    Condition mNegatedCondition(*this);
    mNegatedCondition.cDefNegated = !cDefNegated;
    return mNegatedCondition;
  }

  void Condition::debug(const std::string& message) const {
    std::cout << message << std::endl;
    debug();
  }

  void Condition::debug(int depth) const {
    if (depth == 0) {
      std::cout << "    ";
    }
    if (isAbsolute()) {
      std::cout << (cDefAnd != cDefNegated ? "true" : "false");
    } else {
      if (cDefNegated) {
        std::cout << "!";
      }
      std::cout << "(";
      unsigned int mCount = 0;
      for (std::set<ConditionElement::Clause*>::iterator i = cDefCriteria.begin(); i != cDefCriteria.end(); i++) {
        (*i)->debug();
        if (mCount++ != cDefCriteria.size() - 1 || !cDefConditions.empty()) {
          std::cout << " " << (cDefAnd ? "and" : "or") << " ";
        }
      }
      for (unsigned int i = 0; i < cDefConditions.size(); i++) {
        cDefConditions[i].debug(depth + 1);
        if (i != cDefConditions.size() - 1) {
          std::cout << " " << (cDefAnd ? "and" : "or") << " ";
        }
      }
      std::cout << ")";
    }
    if (depth == 0) {
      std::cout << std::endl;
    }
  }
}

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

#include "IsoRealms/Exception/ArgumentException.h"
#include "IsoRealms/Persistence.h"

namespace IsoRealms {
  const char Condition::ATTRIB_AND    = 0x1;
  const char Condition::ATTRIB_NEGATE = 0x2;

  const std::string Condition::JSON_INPUT          = "input";
  const std::string Condition::JSON_INPUTS         = "inputs";
  const std::string Condition::JSON_OPERATOR       = "operator";
  const std::string Condition::JSON_NEGATED        = "negated";
  const std::string Condition::JSON_SUB_CONDITIONS = "subConditions";

  const std::string Condition::OPERATOR_AND = "And";
  const std::string Condition::OPERATOR_OR  = "Or";

  Condition::Condition(bool andGate, bool negated) :
            cAnd(andGate),
            cNegated(negated) {
  }

  Condition::Condition(const Condition& condition) :
            cAnd(condition.cAnd),
            cNegated(condition.cNegated),
            cCriteria(condition.cCriteria) {
    for (const Condition& mCondition : condition.cConditions) {
      cConditions.emplace_back(Condition(mCondition));
    }
  }

  Condition::Condition(JSONObject object, std::vector<ConditionElement*> elements) {
    std::map<std::string, ConditionElement*> mElements;
    for (unsigned int i = 0; i < elements.size(); i++) {
      mElements[elements[i]->getName()] = elements[i];
    }
    cNegated = object.getBoolean(JSON_NEGATED);
    cAnd = object.getString(JSON_OPERATOR) == OPERATOR_AND;

    if (object.hasMember(JSON_SUB_CONDITIONS)) {
      for (JSONValue mSubConditionValue : object.getArray(JSON_SUB_CONDITIONS)) {
        cConditions.emplace_back(Condition(mSubConditionValue.getObject(), elements)); // TODO: Make constructor pass map instead
      }
    }

    if (object.hasMember(JSON_INPUTS)) {
      for (JSONValue mCriteriaValue : object.getArray(JSON_INPUTS)) {
        JSONObject mCriteriaObject = mCriteriaValue.getObject();
        std::string mName = mCriteriaObject.getString(JSON_INPUT);
        bool mNegated = mCriteriaObject.getBoolean(JSON_NEGATED);
        std::map<std::string, ConditionElement*>::iterator i = mElements.find(mName);
        if (i == mElements.end()) {
          std::cout << "Element \"" << mName << "\" not in list of conditions.  Available conditions:" << std::endl;
          for (unsigned int i = 0; i < elements.size(); i++) {
            std::cout << i << ":      " << elements[i]->getName() << std::endl;
          }
          throw ArgumentException("ERROR: Condition::Condition: Condition element \"" + mName + "\" is not in the specified list of elements.");
        }
        cCriteria.insert(mNegated ? i->second->getNegativeClause() : i->second->getPositiveClause());
      }
    }
  }

  Condition::Condition(std::ifstream& cache, std::vector<ConditionElement*> elements, unsigned char conditionType, unsigned char elementType, unsigned char endType) {
    std::map<std::string, ConditionElement*> mElements;
    for (unsigned int i = 0; i < elements.size(); i++) {
      mElements[elements[i]->getName()] = elements[i];
    }

    unsigned char mAttribs;
    cache.read(reinterpret_cast<char*>(&mAttribs), sizeof(mAttribs));
    cAnd     = mAttribs & ATTRIB_AND;
    cNegated = mAttribs & ATTRIB_NEGATE;

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
          cCriteria.insert(i->second->getNegativeClause());
        } else {
          cCriteria.insert(i->second->getPositiveClause());
        }
      } else if (mNextEntity == conditionType) {
        cConditions.emplace_back(Condition(cache, elements, conditionType, elementType, endType)); // TODO: Make constructor pass map instead
      }
      cache.read(reinterpret_cast<char*>(&mNextEntity), sizeof(mNextEntity));
    }
  }

  void Condition::save(JSONObject object) {
    if (cCriteria.size() + cConditions.size() > 1) {
      object.addString(JSON_OPERATOR, cAnd ? OPERATOR_AND : OPERATOR_OR);
    }
    object.addBoolean(JSON_NEGATED, cNegated);

    if (!cCriteria.empty()) {
      JSONArray mCriteriaArray = object.addArray(JSON_INPUTS);
      for (ConditionElement::Clause* mCriteria : cCriteria) {
        JSONObject mCriteriaObject = mCriteriaArray.addObject();
        mCriteria->save(mCriteriaObject);
      }
    }

    if (!cConditions.empty()) {
      JSONArray mSubConditionsArray = object.addArray(JSON_SUB_CONDITIONS);
      for (unsigned int i = 0; i < cConditions.size(); i++) {
        JSONObject mSubConditionObject = mSubConditionsArray.addObject();
        cConditions[i].save(mSubConditionObject);
      }
    }
  }

  void Condition::saveCache(std::ostream& cache, unsigned char conditionType, unsigned char elementType, unsigned char endType) const {
    cache.write(reinterpret_cast<const char*>(&conditionType), sizeof(conditionType));
    unsigned char mAttribs = 0;
    if (cAnd)     {mAttribs |= ATTRIB_AND;}
    if (cNegated) {mAttribs |= ATTRIB_NEGATE;}
    cache.write(reinterpret_cast<const char*>(&mAttribs), sizeof(mAttribs));
    for (ConditionElement::Clause* mCriteria : cCriteria) {
      mCriteria->saveCache(cache, elementType);
    }
    for (unsigned int i = 0; i < cConditions.size(); i++) {
      cConditions[i].saveCache(cache, conditionType, elementType, endType);
    }
    cache.write(reinterpret_cast<const char*>(&endType), sizeof(endType));
  }

  std::vector<Condition> Condition::getConditions() {
    return cConditions;
  }

  std::set<ConditionElement::Clause*> Condition::getConditionElements() {
    return cCriteria;
  }

  bool Condition::isNegated() {
    return cNegated;
  }

  bool Condition::isAnd() {
    return cAnd;
  }

  bool Condition::operator==(const Condition& condition) const {
    Condition mThis(*this);
    Condition mThat(condition);
    mThis.simplify();
    mThat.simplify();
    if (mThis.cCriteria.size() != mThat.cCriteria.size() || mThis.cConditions.size() != mThat.cConditions.size() || mThis.cNegated != mThat.cNegated) {
      return false;
    }

    if (mThis.cCriteria.size() + mThis.cConditions.size() != 1 && mThis.cAnd != mThat.cAnd) {
      return false;
    }
    // TODO: This test assumes that simplify has sorted the elements, which simplify doesn't do yet.
    std::set<ConditionElement::Clause*>::iterator mThisElement = mThis.cCriteria.begin();
    std::set<ConditionElement::Clause*>::iterator mThatElement = mThat.cCriteria.begin();
    while (mThisElement != mThis.cCriteria.end()) {
      if (**mThisElement != **mThatElement) {
        return false;
      }
      mThisElement++;
      mThatElement++;
    }
    // TODO: This test assumes that simplify has sorted the conditions, which simplify doesn't do yet.
    for (unsigned int i = 0; i < mThis.cConditions.size(); i++) {
      if (mThis.cConditions[i] != mThat.cConditions[i]) {
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
    for (unsigned int i = 0; i < cConditions.size(); i++) {
      std::set<IBoolean*> mSubInputs = cConditions[i].getInputs();
      mInputs.insert(mSubInputs.begin(), mSubInputs.end());
    }
    for (ConditionElement::Clause* mCriteria : cCriteria) {
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
    mCondition.cConditions.emplace_back(Condition(*this));
    mCondition.cConditions.emplace_back(Condition(condition.value()));

    // Create a condition based on this condition AND the negation of the split condition
    Condition mOpposingCondition(true);
    mOpposingCondition.cConditions.emplace_back(Condition(*this));
    mOpposingCondition.cConditions.emplace_back(Condition(condition->negate()));

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
    for (ConditionElement::Clause* mCriteria : cCriteria) {
      mElements.insert(mCriteria->getElement());
    }
    for (const Condition& mSubCondition : cConditions) {
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
    for (int i = static_cast<int>(cConditions.size()) - 1; i >= 0; i--) {
      if (cConditions[i].isAbsolute()) {
        if (cConditions[i].isTrue() == cAnd) {
          cConditions.erase(cConditions.begin() + i);
        } else {
          cConditions.clear();
          cCriteria.clear();
          cNegated = !cNegated;
          return;
        }
      }
    }
  }

  void Condition::raiseCondition(int index) {
    for (unsigned int i = 0; i < cConditions[index].cConditions.size(); i++) {
      cConditions.emplace_back(Condition(cConditions[index].cConditions[i]));
    }
    for (ConditionElement::Clause* mCriteria : cConditions[index].cCriteria) {
      if (cConditions[index].cNegated) {
        cCriteria.insert(mCriteria->getNegatedClause());
      } else {
        cCriteria.insert(mCriteria);
      }
    }
    cConditions.erase(cConditions.begin() + index);
  }

  void Condition::raiseConditions() {
    for (int i = static_cast<int>(cConditions.size()) - 1; i >= 0; i--) {
      cConditions[i].raiseConditions();
      if ((cConditions[i].cConditions.empty() && cConditions[i].cCriteria.size() == 1) || (cConditions[i].cAnd == cAnd && cConditions[i].cNegated == cNegated)) {
        raiseCondition(i);
      }
    }

    if (cConditions.size() == 1 && cCriteria.empty()) {
      cCriteria   = cConditions[0].cCriteria;
      cAnd        = cConditions[0].cAnd;
      for (unsigned int i = 0; i < cConditions[0].cConditions.size(); i++) {
        cConditions.emplace_back(Condition(cConditions[0].cConditions[i]));
      }
      cConditions.erase(cConditions.begin());
    }
  }

  void Condition::checkForConflictingElements() {

    // TODO: This seems to assume AND gate...  for OR gate, conflicting elements should simply be removed
    std::set<IBoolean*> mPositiveElements;
    std::set<IBoolean*> mNegativeElements;
    for (ConditionElement::Clause* mCriteria : cCriteria) {
      IBoolean* mAddress = mCriteria->getElement()->getInputAddress();
      std::set<IBoolean*>& mSetToAddTo = mCriteria->isNegated() ? mNegativeElements : mPositiveElements;
      std::set<IBoolean*>& mOtherSet   = mCriteria->isNegated() ? mPositiveElements : mNegativeElements;
      if (mOtherSet.find(mAddress) != mOtherSet.end()) {
        cCriteria.clear();
        cConditions.clear();
        cNegated = !cNegated;
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
    for (unsigned int i = 0; i < cConditions.size(); i++) {
      for (ConditionElement::Clause* mSubCriteria : cConditions[i].cCriteria) {
        for (ConditionElement::Clause* mCriteria : cCriteria) {
          if (mCriteria->getElement()->getInputAddress() == mSubCriteria->getElement()->getInputAddress() && mCriteria->isNegated() != mSubCriteria->isNegated()) {
            cCriteriaToRemove.push_back(mSubCriteria);
          }
        }
      }
      for (unsigned int j = 0; j < cCriteriaToRemove.size(); j++) {
        cConditions[i].cCriteria.erase(cCriteriaToRemove[j]);
      }
      cCriteriaToRemove.clear();
    }
  }

  void Condition::negateEverything() {
    cNegated = !cNegated;
    cAnd = !cAnd;
    for (unsigned int i = 0; i < cConditions.size(); i++) {
      cConditions[i].cNegated = !cConditions[i].cNegated;
    }
    std::set<ConditionElement::Clause*> mNegatedElements;
    for (ConditionElement::Clause* mCriteria : cCriteria) {
      ConditionElement::Clause* mNegatedElement = mCriteria->getNegatedClause();
      mNegatedElements.insert(mNegatedElement);
    }
    cCriteria = mNegatedElements;
  }

  void Condition::convertNegatedConditions() {
    for (unsigned int i = 0; i < cConditions.size(); i++) {
      if (cConditions[i].cNegated) {
        cConditions[i].negateEverything();
      }
    }
    if (cNegated) {
      negateEverything();
    }
  }

  void Condition::removeDuplicates() {
    std::vector<ConditionElement::Clause*> mElementsToRemove;
    for (std::set<ConditionElement::Clause*>::iterator i = cCriteria.begin(); i != cCriteria.end(); i++) {
      for (std::set<ConditionElement::Clause*>::iterator j = i; j != cCriteria.end(); j++) {
        if (i != j && **i == **j) {
          mElementsToRemove.push_back(*j);
        }
      }
    }
    for (unsigned int i = 0; i < mElementsToRemove.size(); i++) {
      cCriteria.erase(mElementsToRemove[i]);
    }
  }

  void Condition::simplify(int depth) {
    convertNegatedConditions();
    for (unsigned int i = 0; i < cConditions.size(); i++) {
      cConditions[i].simplify(depth + 1);
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
    return cCriteria.empty() && cConditions.empty();
  }

  bool Condition::isTrue() const {
    for (ConditionElement::Clause* mCriteria : cCriteria) {
      if (cAnd != mCriteria->isTrue()) {
        return cAnd == cNegated;
      }
    }
    for (unsigned int i = 0; i < cConditions.size(); i++) {
      if (cAnd != cConditions[i].isTrue()) {
        return cAnd == cNegated;
      }
    }
    return cAnd != cNegated;
  }

  bool Condition::isTestTrue() const {
    for (ConditionElement::Clause* mCriteria : cCriteria) {
      if (cAnd != mCriteria->isTestTrue()) {
        return cAnd == cNegated;
      }
    }
    for (unsigned int i = 0; i < cConditions.size(); i++) {
      if (cAnd != cConditions[i].isTestTrue()) {
        return cAnd == cNegated;
      }
    }
    return cAnd != cNegated;
  }

  Condition Condition::compose(std::optional<Condition>& condition) {
    if (!condition.has_value()) {
      return cAnd ? Condition(*this) : Condition(true);
    }
    Condition mNewCondition(*this);
    if (cAnd) {
      mNewCondition.cConditions.clear();
      mNewCondition.cCriteria.clear();
      mNewCondition.cAnd = false;
      mNewCondition.cNegated = false;
      mNewCondition.cConditions.emplace_back(Condition(*this));
    }
    mNewCondition.cConditions.emplace_back(Condition(*condition));
    mNewCondition.simplify();
    return mNewCondition;
  }

  void Condition::add(Condition& condition) {
    cConditions.emplace_back(Condition(condition));
  }

  void Condition::add(ConditionElement::Clause* criteria) {
    cCriteria.insert(criteria);
  }

  bool Condition::isCompatibleWith(std::optional<Condition>& condition) {
    if (!condition.has_value()) {
      return canBe(true);
    }

    Condition mBothConditions(true);
    mBothConditions.cConditions.emplace_back(Condition(*this));
    mBothConditions.cConditions.emplace_back(Condition(condition.value()));

    return mBothConditions.canBe(true);
  }

  Condition Condition::negate() {
    Condition mNegatedCondition(*this);
    mNegatedCondition.cNegated = !cNegated;
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
      std::cout << (cAnd != cNegated ? "true" : "false");
    } else {
      if (cNegated) {
        std::cout << "!";
      }
      std::cout << "(";
      unsigned int mCount = 0;
      for (std::set<ConditionElement::Clause*>::iterator i = cCriteria.begin(); i != cCriteria.end(); i++) {
        (*i)->debug();
        if (mCount++ != cCriteria.size() - 1 || !cConditions.empty()) {
          std::cout << " " << (cAnd ? "and" : "or") << " ";
        }
      }
      for (unsigned int i = 0; i < cConditions.size(); i++) {
        cConditions[i].debug(depth + 1);
        if (i != cConditions.size() - 1) {
          std::cout << " " << (cAnd ? "and" : "or") << " ";
        }
      }
      std::cout << ")";
    }
    if (depth == 0) {
      std::cout << std::endl;
    }
  }
}

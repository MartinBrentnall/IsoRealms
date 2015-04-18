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
#include "Condition.h"

int Condition::cInstanceCount = 0;

Condition::Condition(bool aand, bool negated) {
  cAnd = aand;
  cNegated = negated;
}

Condition::Condition(const Condition& condition) {
  for (std::set<ConditionElement*>::iterator i = condition.cElements.begin(); i != condition.cElements.end(); i++) {
    cElements.insert(*i);
  }
  for (unsigned int i = 0; i < condition.cConditions.size(); i++) {
    cConditions.push_back(new Condition(*(condition.cConditions[i])));
  }
  cNegated = condition.cNegated;
  cAnd = condition.cAnd;
}

Condition::Condition(DOMNodeWrapper* node, std::vector<ConditionElement*> elements) {
  std::map<std::string, ConditionElement*> mElements;
  for (unsigned int i = 0; i < elements.size(); i++) {
    mElements[elements[i]->getName()] = elements[i];
  }
  std::string mNegated = node->getAttribute("negated");
  std::string mType = node->getAttribute("type");
  cNegated = mNegated == "true";
  cAnd = mType != "or";
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Element") {
      std::string mName = mNode->getAttribute("name");
      std::string mNegated = mNode->getAttribute("negated");
      std::map<std::string, ConditionElement*>::iterator i = mElements.find(mName);
      if (i == mElements.end()) {
        std::cout << "Element \"" << mName << "\" not in list of conditions.  Available conditions:" << std::endl;
        for (unsigned int i = 0; i < elements.size(); i++) {
          std::cout << "      " << elements[i]->getName() << std::endl;
        }
        exit(1);
      }
      if (mNegated == "true") {
        cElements.insert(i->second->getNegatedElement());
      } else {
        cElements.insert(i->second);
      }
    } else if (mValueAsString == "Condition") {
      cConditions.push_back(new Condition(mNode, elements)); // TODO: Make constructor pass map instead
    } else {
      // TODO: Throw
    }
  }
}

void Condition::save(DOMNodeWriter* node) {
  DOMNodeWriter* mConditionNode = node->addBranch("Condition");
  if (cElements.size() + cConditions.size() > 1) {
    mConditionNode->addAttribute("type", cAnd ? "and" : "or");
  }
  if (cNegated) {
    mConditionNode->addAttribute("negated", "true");
  }
  for (std::set<ConditionElement*>::iterator i = cElements.begin(); i != cElements.end(); i++) {
    (*i)->save(mConditionNode);
  }
  for (unsigned int i = 0; i < cConditions.size(); i++) {
    cConditions[i]->save(mConditionNode);
  }
}

bool Condition::operator==(const Condition& condition) const {
  Condition mThis(*this);
  Condition mThat(condition);
  mThis.simplify();
  mThat.simplify();
  if (mThis.cElements.size() != mThat.cElements.size() || mThis.cConditions.size() != mThat.cConditions.size() || mThis.cNegated != mThat.cNegated) {
    return false;
  }
  
  if (mThis.cElements.size() + mThis.cConditions.size() != 1 && mThis.cAnd != mThat.cAnd) {
    return false;
  }
  // TODO: This test assumes that simplify has sorted the elements, which simplify doesn't do yet.
  std::set<ConditionElement*>::iterator mThisElement = mThis.cElements.begin();
  std::set<ConditionElement*>::iterator mThatElement = mThat.cElements.begin();
  while (mThisElement != mThis.cElements.end()) {
    if (**mThisElement != **mThatElement) {
      return false;
    }
    mThisElement++;
    mThatElement++;
  }
  // TODO: This test assumes that simplify has sorted the conditions, which simplify doesn't do yet.
  for (unsigned int i = 0; i < mThis.cConditions.size(); i++) {
    if (*(mThis.cConditions[i]) != *(mThat.cConditions[i])) {
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
    std::set<IBoolean*> mSubInputs = cConditions[i]->getInputs();
    mInputs.insert(mSubInputs.begin(), mSubInputs.end());
  }
  for (std::set<ConditionElement*>::iterator i = cElements.begin(); i != cElements.end(); i++) {
    IBoolean* mInput = (*i)->getInputAddress();
    mInputs.insert(mInput);
  }
  return mInputs;
}

std::vector<Condition*> Condition::split(Condition* condition) {
  std::vector<Condition*> mSplitConditions;
  if (condition == NULL) {
    return mSplitConditions;
  }

  // Create a condition based on this condition AND the split condition
  Condition* mCondition = new Condition(true);
  mCondition->cConditions.push_back(new Condition(*this));
  mCondition->cConditions.push_back(new Condition(*condition));
  mCondition->simplify();
  
  // Create a condition based on this condition AND the negation of the split condition
  Condition* mOpposingCondition = new Condition(true);
  mOpposingCondition->cConditions.push_back(new Condition(*this));
  mOpposingCondition->cConditions.push_back(condition->negate());
  mOpposingCondition->simplify();
  
  if (mCondition->isAbsolute() || mOpposingCondition->isAbsolute()) {
    delete mCondition;
    delete mOpposingCondition;
    return mSplitConditions;
  }
  mSplitConditions.push_back(mCondition);
  mSplitConditions.push_back(mOpposingCondition);
  return mSplitConditions;
}

void Condition::checkForAbsoluteConditions() {
  for (int i = cConditions.size() - 1; i >= 0; i--) {
    if (cConditions[i]->isAbsolute()) {
      if (cConditions[i]->isTrue() == cAnd) {
        delete cConditions[i];
        cConditions.erase(cConditions.begin() + i);
      } else {
        for (unsigned int j = 0; j < cConditions.size(); j++) {
          delete cConditions[j];
        }
        cConditions.clear();
        cElements.clear();
        cNegated = !cNegated;
        return;
      }
    }
  }
}

void Condition::raiseCondition(int index) {
  for (unsigned int i = 0; i < cConditions[index]->cConditions.size(); i++) {
    cConditions.push_back(new Condition(*(cConditions[index]->cConditions[i])));
  }
  for (std::set<ConditionElement*>::iterator i = cConditions[index]->cElements.begin(); i != cConditions[index]->cElements.end(); i++) {
    if (cConditions[index]->cNegated) {
      cElements.insert((*i)->getNegatedElement());
    } else {
      cElements.insert(*i);
    }
  }
  delete cConditions[index];
  cConditions.erase(cConditions.begin() + index);
}

void Condition::raiseConditions() {
  for (int i = cConditions.size() - 1; i >= 0; i--) {
    cConditions[i]->raiseConditions();
    if ((cConditions[i]->cConditions.empty() && cConditions[i]->cElements.size() == 1) || (cConditions[i]->cAnd == cAnd && cConditions[i]->cNegated == cNegated)) {
      raiseCondition(i);
    } 
  }
  
  if (cConditions.size() == 1 && cElements.empty()) {
    Condition* mConditionToRaise = cConditions[0];
    cElements   = mConditionToRaise->cElements;
    cAnd        = mConditionToRaise->cAnd;
    cConditions = mConditionToRaise->cConditions;
    // Clear conditions before deletion so the conditions we've raised don't get destroyed.
    mConditionToRaise->cConditions.clear();
    delete mConditionToRaise;
  }
}

void Condition::checkForConflictingElements() {
  
  // TODO: This seems to assume AND gate...  for OR gate, conflicting elements should simply be removed
  std::set<IBoolean*> mPositiveElements;
  std::set<IBoolean*> mNegativeElements;
  for (std::set<ConditionElement*>::iterator i = cElements.begin(); i != cElements.end(); i++) {
    IBoolean* mAddress = (*i)->getInputAddress();
    std::set<IBoolean*>& mSetToAddTo = (*i)->isNegated() ? mNegativeElements : mPositiveElements;
    std::set<IBoolean*>& mOtherSet   = (*i)->isNegated() ? mPositiveElements : mNegativeElements;
    if (mOtherSet.find(mAddress) != mOtherSet.end()) {
      cElements.clear();
      for (int j = cConditions.size() - 1; j >= 0; j--) {
        delete cConditions[j];
      }
      cConditions.clear();
      cNegated = !cNegated;
      return;
    }
    mSetToAddTo.insert(mAddress);
  }
}

void Condition::checkForConflictingConditions() {
  std::vector<ConditionElement*> cElementsToRemove;
  
  // TODO: We should really check ALL subconditions recursively!
  // TODO: How should this deal with OR conditions?  I've only thought this through using AND so far
  // From here we assume that the gate of the subcondition is different to this one!
  for (unsigned int i = 0; i < cConditions.size(); i++) {
    for (std::set<ConditionElement*>::iterator j = cConditions[i]->cElements.begin(); j != cConditions[i]->cElements.end(); j++) {
      for (std::set<ConditionElement*>::iterator k = cElements.begin(); k != cElements.end(); k++) {
        if ((*k)->getInputAddress() == (*j)->getInputAddress() && (*k)->isNegated() != (*j)->isNegated()) {
          cElementsToRemove.push_back(*j);
        }
      }
    }
    for (unsigned int j = 0; j < cElementsToRemove.size(); j++) {
      cConditions[i]->cElements.erase(cElementsToRemove[j]);
    }
    cElementsToRemove.clear();
  }
}

void Condition::negateEverything() {
  cNegated = !cNegated;
  cAnd = !cAnd;
  for (unsigned int i = 0; i < cConditions.size(); i++) {
    cConditions[i]->cNegated = !cConditions[i]->cNegated;
  }
  std::set<ConditionElement*> mNegatedElements;
  for (std::set<ConditionElement*>::iterator i = cElements.begin(); i != cElements.end(); i++) {
    ConditionElement* mNegatedElement = (*i)->getNegatedElement();
    mNegatedElements.insert(mNegatedElement);
  }
  cElements = mNegatedElements;
}

void Condition::convertNegatedConditions() {
  for (unsigned int i = 0; i < cConditions.size(); i++) {
    if (cConditions[i]->cNegated) {
      cConditions[i]->negateEverything();
    }
  }
  if (cNegated) {
    negateEverything();
  }
}

void Condition::removeDuplicates() {
  std::vector<ConditionElement*> mElementsToRemove;
  for (std::set<ConditionElement*>::iterator i = cElements.begin(); i != cElements.end(); i++) {
    for (std::set<ConditionElement*>::iterator j = i; j != cElements.end(); j++) {
      if (i != j && **i == **j) {
        mElementsToRemove.push_back(*j);
      }
    }
  }
  for (unsigned int i = 0; i < mElementsToRemove.size(); i++) {
    cElements.erase(mElementsToRemove[i]);
  }
}

void Condition::simplify(int depth) {
  convertNegatedConditions();
  for (unsigned int i = 0; i < cConditions.size(); i++) {
    cConditions[i]->simplify(depth + 1);
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
  return cElements.empty() && cConditions.empty();
}

bool Condition::isTrue() {
  for (std::set<ConditionElement*>::iterator i = cElements.begin(); i != cElements.end(); i++) {
    if (cAnd != (*i)->isTrue()) {
      return cAnd == cNegated;
    }
  }
  for (unsigned int i = 0; i < cConditions.size(); i++) {
    if (cAnd != cConditions[i]->isTrue()) {
      return cAnd == cNegated;
    }
  }
  return cAnd != cNegated;
}

Condition* Condition::compose(Condition* condition) {
  if (condition == NULL) {
    return cAnd ? new Condition(*this) : new Condition(true);
  }
  Condition* mNewCondition = new Condition(*this);
  if (cAnd) {
    Condition* mThisCondition = new Condition(*this);
    mNewCondition->cConditions.clear();
    mNewCondition->cElements.clear();
    mNewCondition->cAnd = false;
    mNewCondition->cNegated = false;
    mNewCondition->cConditions.push_back(mThisCondition);
  }
  mNewCondition->cConditions.push_back(new Condition(*condition));
  mNewCondition->simplify();
  return mNewCondition;
}

bool Condition::isCompatibleWith(Condition* condition) {
  if (condition == NULL) {
    return !isAbsolute() || isTrue();
  }
  Condition mBothConditions(true);
  mBothConditions.cConditions.push_back(new Condition(*this));
  mBothConditions.cConditions.push_back(new Condition(*condition));
  mBothConditions.simplify();
  return !mBothConditions.isAbsolute() || mBothConditions.isTrue();
}

Condition* Condition::negate() {
  Condition* mNegatedCondition = new Condition(*this);
  mNegatedCondition->cNegated = !cNegated;
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
    for (std::set<ConditionElement*>::iterator i = cElements.begin(); i != cElements.end(); i++) {
      (*i)->debug();
      if (mCount++ != cElements.size() - 1 || !cConditions.empty()) {
        std::cout << " " << (cAnd ? "and" : "or") << " ";
      }
    }
    for (unsigned int i = 0; i < cConditions.size(); i++) {
      cConditions[i]->debug(depth + 1);
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

Condition::~Condition() {
  for (unsigned int i = 0; i < cConditions.size(); i++) {
    delete cConditions[i];
  }
}

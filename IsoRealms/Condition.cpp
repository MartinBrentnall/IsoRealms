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
        std::cout << "Element \"" << mName << "\" not in list of conditions" << std::endl;
        exit(1);
      }
      cElements.insert(i->second);
    } else if (mValueAsString == "Condition") {
      Condition* mCondition = new Condition(mNode, elements); // TODO: Make constructor pass map instead
      cConditions.push_back(mCondition);
    } else {
      // TODO: Throw
    }
  }
  std::cout << "Loaded condition:" << std::endl;
  debug();
}

void Condition::save(DOMNodeWriter* node) {
  // TODO: Save the condition
}

bool Condition::operator==(const Condition& condition) const {
  Condition mThis(*this);
  Condition mThat(condition);
  mThis.simplify();
  mThat.simplify();
  if (mThis.cElements.size() != mThat.cElements.size() || mThis.cConditions.size() != mThat.cConditions.size() || mThis.cNegated != mThat.cNegated || mThis.cAnd != mThat.cAnd) {
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

Condition* Condition::split(Condition* condition) {
//   std::cout << " ========================== Splitting condition:" << std::endl;
//   std::cout << "   This: ";
//   debug();
//   std::cout << "  Split: ";
//   condition->debug();

  // Create a condition based on this condition AND the split condition
  Condition* mCondition = new Condition(true);
  mCondition->cConditions.push_back(new Condition(*this));
  mCondition->cConditions.push_back(condition);
  if (mCondition->isAbsolute() || *mCondition == *this) {
    std::cout << "Condition cannot be split!" << std::endl;
    return NULL;
  }
  
  // Test passed; create a copy of this condition including a negation of the split condition.
  Condition* mOpposingCondition = new Condition(true);
  mOpposingCondition->cConditions.push_back(new Condition(*this));
  
  Condition* mNegatedCondition = new Condition(*condition);
  mNegatedCondition->cNegated = !mNegatedCondition->cNegated;
  mOpposingCondition->cConditions.push_back(mNegatedCondition);

  // Test passed; modify this condition
  cConditions = mCondition->cConditions;
  cElements = mCondition->cElements;
  
//   std::cout << "Result:" << std::endl;
//   debug();
//   mOpposingCondition->debug();

  simplify();
  mOpposingCondition->simplify();
  
/*  std::cout << "Simplified Result:" << std::endl;
  debug();
  mOpposingCondition->debug();*/
  return mOpposingCondition;
}

void Condition::checkForAbsoluteConditions() {
  for (int i = cConditions.size() - 1; i >= 0; i--) {
    if (cConditions[i]->isAbsolute()) {
      if (cConditions[i]->isTrue() == cAnd) {
        cConditions.erase(cConditions.begin() + i);
      } else {
        cConditions.clear();
        cElements.clear();
        cNegated = !cNegated;
        return;
      }
    }
  }
}

void Condition::raiseCondition(int index) {
/*  std::cout << "RAISING " << cConditions[index]->cConditions.size() << " CONDITIONS..." << std::endl;
  debug();*/
  for (unsigned int i = 0; i < cConditions[index]->cConditions.size(); i++) {
    cConditions.push_back(cConditions[index]->cConditions[i]);
  }
/*  std::cout << "RAISING " << cConditions[index]->cElements.size() << " CONDITIONS..." << std::endl;
  debug();*/
  for (std::set<ConditionElement*>::iterator i = cConditions[index]->cElements.begin(); i != cConditions[index]->cElements.end(); i++) {
    if (cConditions[index]->cNegated) {
      ConditionElement* mNegatedElement = (*i)->getNegatedElement();
      cElements.insert(mNegatedElement);
    } else {
      cElements.insert(*i);
    }
  }
/*  std::cout << "SIMPLIFY STAGE 3" << std::endl;
  debug();*/
  cConditions.erase(cConditions.begin() + index);
/*  std::cout << "SIMPLIFY STAGE 4" << std::endl;
  debug();*/
}

void Condition::raiseConditions() {
  for (int i = cConditions.size() - 1; i >= 0; i--) {
    if ((cConditions[i]->cConditions.empty() && cConditions[i]->cElements.size() == 1) ||
        (cConditions[i]->cAnd == cAnd && cConditions[i]->cNegated == cNegated)) {
      raiseCondition(i);
    } 
  }
}

void Condition::checkForConflictingElements() {
  std::set<bool*> mPositiveElements;
  std::set<bool*> mNegativeElements;
  for (std::set<ConditionElement*>::iterator i = cElements.begin(); i != cElements.end(); i++) {
    bool* mAddress = (*i)->getInputAddress();
    std::set<bool*>& mSetToAddTo = (*i)->isNegated() ? mNegativeElements : mPositiveElements;
    std::set<bool*>& mOtherSet   = (*i)->isNegated() ? mPositiveElements : mNegativeElements;
    if (mOtherSet.find(mAddress) != mOtherSet.end()) {
      cElements.clear();
      cConditions.clear();
      cNegated = !cNegated;
      return;
    }
    mSetToAddTo.insert(mAddress);
  }
}

void Condition::simplify() {
  for (unsigned int i = 0; i < cConditions.size(); i++) {
    cConditions[i]->simplify();
  }
  checkForAbsoluteConditions();
  raiseConditions();
  checkForConflictingElements();
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

void Condition::compose(Condition* condition) {
//   std::cout << " ========================== Composing conditions:" << std::endl;
//   debug();
//   condition->debug();

  if (cAnd) {
    Condition* mThisCondition = new Condition(*this);
    cConditions.clear();
    cElements.clear();
    cAnd = false;
    cNegated = false;
    cConditions.push_back(mThisCondition);
  }
  cConditions.push_back(condition);
  
/*  std::cout << "Result:" << std::endl;
  debug();*/
  
  simplify();
/*  std::cout << "Simplified:" << std::endl;
  debug();*/
}

bool Condition::isCompatibleWith(Condition* condition) {
  Condition mThisCondition(*this);
  Condition mBothConditions(true);
  mBothConditions.cConditions.push_back(&mThisCondition);
  mBothConditions.cConditions.push_back(condition);
/*  std::cout << "Compatibility check condition:" << std::endl;
  mBothConditions.debug();*/
  mBothConditions.simplify();
/*  std::cout << "Simplified:" << std::endl;
  mBothConditions.debug();*/
  return !mBothConditions.isAbsolute() && !mBothConditions.isTrue();
}

void Condition::negate() {
  cNegated = !cNegated;
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

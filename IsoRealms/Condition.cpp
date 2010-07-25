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
      if (mNegated == "true") {
        cElements.insert(i->second->getNegatedElement());
      } else {
        cElements.insert(i->second);
      }
    } else if (mValueAsString == "Condition") {
      Condition* mCondition = new Condition(mNode, elements); // TODO: Make constructor pass map instead
      cConditions.push_back(mCondition);
    } else {
      // TODO: Throw
    }
  }
}

void Condition::save(DOMNodeWriter* node) {
  // TODO: Save the condition
}

bool Condition::operator==(const Condition& condition) const {
  Condition mThis(*this);
  Condition mThat(condition);
/*  mThis.debug("Equality Test:");
  mThat.debug("With This:");*/
  mThis.simplify();
  mThat.simplify();
/*  mThis.debug("Simplified Equality Test:");
  mThat.debug("Simplified With This:");*/
  if (mThis.cElements.size() != mThat.cElements.size() || mThis.cConditions.size() != mThat.cConditions.size() || mThis.cNegated != mThat.cNegated) {
/*    std::cout << "Basic difference." << std::endl;*/
    return false;
  }
  
  if (mThis.cElements.size() + mThis.cConditions.size() != 1 && mThis.cAnd != mThat.cAnd) {
/*    std::cout << "Gate or negation difference" << std::endl;*/
    return false;
  }
  // TODO: This test assumes that simplify has sorted the elements, which simplify doesn't do yet.
  std::set<ConditionElement*>::iterator mThisElement = mThis.cElements.begin();
  std::set<ConditionElement*>::iterator mThatElement = mThat.cElements.begin();
  while (mThisElement != mThis.cElements.end()) {
    if (**mThisElement != **mThatElement) {
/*      std::cout << "Element difference." << std::endl;*/
      return false;
    }
    mThisElement++;
    mThatElement++;
  }
  // TODO: This test assumes that simplify has sorted the conditions, which simplify doesn't do yet.
  for (unsigned int i = 0; i < mThis.cConditions.size(); i++) {
    if (*(mThis.cConditions[i]) != *(mThat.cConditions[i])) {
/*      std::cout << "Condition difference." << std::endl;*/
      return false;
    }
  }
/*  std::cout << "Both the same." << std::endl;*/
  return true;
}

bool Condition::operator!=(const Condition& condition) const {
  return !(*this == condition);
}

Condition* Condition::split(Condition* condition) {
  if (condition == NULL) {
    return NULL;
  }
//   std::cout << " ========================== Splitting condition:" << std::endl;
//   debug("   This: ");
//   condition->debug("  Split: ");

  // Create a condition based on this condition AND the split condition
  Condition* mCondition = new Condition(true);
  mCondition->cConditions.push_back(new Condition(*this));
  mCondition->cConditions.push_back(new Condition(*condition));
/*  mCondition->debug("Test Condition for Split");*/
  mCondition->simplify();
/*  mCondition->debug("Simplified Test Condition for Split");*/
  
  Condition* mOpposingCondition = new Condition(true);
  mOpposingCondition->cConditions.push_back(new Condition(*this));
  Condition* mNegatedCondition = new Condition(*condition);
  mNegatedCondition->cNegated = !mNegatedCondition->cNegated;
  mOpposingCondition->cConditions.push_back(mNegatedCondition);
  mOpposingCondition->simplify();
  
  if (mCondition->isAbsolute() || mOpposingCondition->isAbsolute()) {
/*    std::cout << "Condition cannot be split!" << std::endl;*/
    return NULL;
  }

  // Modify this condition
  cConditions = mCondition->cConditions;
  cElements = mCondition->cElements;
  cAnd = mCondition->cAnd;
  
/*  debug("Result:");
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
    cConditions[i]->raiseConditions();
    if ((cConditions[i]->cConditions.empty() && cConditions[i]->cElements.size() == 1) ||
        (cConditions[i]->cAnd == cAnd && cConditions[i]->cNegated == cNegated)) {
      raiseCondition(i);
    } 
  }
  
  if (cConditions.size() == 1 && cElements.empty()) {
    cElements = cConditions[0]->cElements;
    cAnd = cConditions[0]->cAnd;
    cConditions = cConditions[0]->cConditions;
  }
}

void Condition::checkForConflictingElements() {
  
  // TODO: This seems to assume AND gate...  for OR gate, conflicting elements should simply be removed
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
//   std::string mIndent;
//   for (unsigned int i = 0; i < depth; i++) {
//     mIndent += "        ";
//   }

/*  if (depth == 0) {*/
/*    std::cout << std::endl;
    std::cout << "            SIMPLIFY DEBUG ==================================================" << std::endl;*/
/*  }*/
//   debug(mIndent + "Condition to simplify");
  convertNegatedConditions();
//   debug(mIndent + "Converted negative conditions");
  for (unsigned int i = 0; i < cConditions.size(); i++) {
    cConditions[i]->simplify(depth + 1);
  }
//   debug(mIndent + "Performed recursive simplify");
  checkForAbsoluteConditions();
//   debug(mIndent + "Checked for absolute conditions");
  raiseConditions();
//   debug(mIndent + "Raised conditions");
  checkForConflictingElements();
//   debug(mIndent + "Checked conflicting elements");
  checkForConflictingConditions();
//   debug(mIndent + "Checked conflicting conditions");
  raiseConditions();
//   debug(mIndent + "Raised conditions again");
  checkForConflictingConditions();
//  debug(mIndent + "Checked conflicting conditions again");
  checkForAbsoluteConditions();
  
  removeDuplicates();
//  debug(mIndent + "Final result");
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
  if (condition == NULL) {
    return !isAbsolute() || isTrue();
  }
/*  debug("Is this:");
  condition->debug("Compatible with this?");*/
  Condition mThisCondition(*this);
  Condition mTestCondition(*condition);
  Condition mBothConditions(true);
  mBothConditions.cConditions.push_back(&mThisCondition);
  mBothConditions.cConditions.push_back(&mTestCondition);
/*  mBothConditions.debug("Both conditions");*/
  mBothConditions.simplify();
/*  mBothConditions.debug("Both conditions simplified");
  std::cout << "Can the condition vary? " << !mBothConditions.isAbsolute() << std::endl;
  std::cout << "Are both conditions true? " << mBothConditions.isTrue() << std::endl;*/
  return !mBothConditions.isAbsolute() || mBothConditions.isTrue();
}

void Condition::negate() {
  cNegated = !cNegated;
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

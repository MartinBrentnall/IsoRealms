
/*
 * Copyright 2016 Martin Brentnall
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
#include "ConditionComponent.h"

ConditionComponent::ConditionComponent(Condition* condition, IConditionElementIcons* icons, float iconSize) {
  cCondition = condition;
  cConditionElementIcons = icons;
  cIconSize = iconSize;
  cComponents = getConditionIcons(condition);
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    IComponentBoundsCalculator* mCellCalculator = new CellLayout(this, i);
    cComponents[i]->setBoundsCalculator(mCellCalculator);
  }
}

float ConditionComponent::getWidth() {
  float mGridWidth = 0.02f;
  for (ISizedComponent* mComponent : cComponents) {
    mGridWidth += mComponent->getWidth();
  }
  return mGridWidth;
}

float ConditionComponent::getHeight() {
  float mGridHeight = 0.0f;
  for (ISizedComponent* mComponent : cComponents) {
    mGridHeight = std::max(mComponent->getHeight(), mGridHeight);
  }
  return mGridHeight;
}

void ConditionComponent::update(unsigned int ticks) {
  // Nothing to do
}

void ConditionComponent::render() {
  for (ISizedComponent* mComponent : cComponents) {
    mComponent->render();
  }
  float mLeft = getLeft();
  float mBottom = getBottom();
  float mRight = getRight();
  float mTop = getTop();
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINE_LOOP);
  glColor3f(0.45f, 0.0f, 0.9f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glEnd();
}

ConditionComponent::CellLayout::CellLayout(ConditionComponent* parent, unsigned int location) {
  cParent = parent;
  cLocation = location;
}

float ConditionComponent::CellLayout::getLeft() {
  float cLeftOffset = cParent->getLeft() + 0.01;
  for (unsigned int i = 0; i < cLocation; i++) {
    cLeftOffset += cParent->cComponents[i]->getWidth();
  }
  return cLeftOffset;
}

float ConditionComponent::CellLayout::getRight() {
  float cLeftOffset = cParent->getLeft() + 0.01;
  for (unsigned int i = 0; i <= cLocation; i++) {
    cLeftOffset += cParent->cComponents[i]->getWidth();
  }
  return cLeftOffset;
}

float ConditionComponent::CellLayout::getTop() {
  return cParent->getBottom() + cParent->cComponents[cLocation]->getHeight();
}

float ConditionComponent::CellLayout::getBottom() {
  return cParent->getBottom();
}

std::vector<ISizedComponent*> ConditionComponent::getConditionIcons(Condition* condition) {
  std::vector<ISizedComponent*> mElementIconComponents;
  if (condition != nullptr) {
    std::set<ConditionElement*> mConditionElements = condition->getConditionElements();
    std::vector<Condition*> mSubConditions = condition->getConditions();
    std::string mOperatorString = condition->isAnd() ? "&" : "|";
    unsigned int mPartsToGo = mConditionElements.size() + mSubConditions.size();
    if (condition->isNegated()) {
      ISizedComponent* mNegationIcon = new TextLabelComponent("!", 0.0f);
      mElementIconComponents.push_back(mNegationIcon);
    }
    ISizedComponent* mBeginCondition = new TextLabelComponent("(", 0.0f);
    mElementIconComponents.push_back(mBeginCondition);
    for (Condition* mSubCondition : mSubConditions) {
      std::vector<ISizedComponent*> mSubConditionIcons = getConditionIcons(mSubCondition);
      for (ISizedComponent* mSubConditionIcon : mSubConditionIcons) {
        mElementIconComponents.push_back(mSubConditionIcon);
      }
      if (--mPartsToGo > 0) {
        ISizedComponent* mOperatorIcon = new TextLabelComponent(mOperatorString, 0.0f);
        mElementIconComponents.push_back(mOperatorIcon);
      }      
    }
    for (ConditionElement* mElement : mConditionElements) {
      I3DModel* mElementIcon = cConditionElementIcons->getConditionElementIcon(mElement);
      ISizedComponent* mElementIconComponent = new ConditionElementIcon(mElementIcon, cIconSize);
      mElementIconComponents.push_back(mElementIconComponent);
      if (--mPartsToGo > 0) {
        ISizedComponent* mOperatorIcon = new TextLabelComponent(mOperatorString, 0.0f);
        mElementIconComponents.push_back(mOperatorIcon);
      }      
    }
    ISizedComponent* mEndCondition = new TextLabelComponent(")", 0.0f);
    mElementIconComponents.push_back(mEndCondition);
  }
  return mElementIconComponents;
}

Condition* ConditionComponent::getCondition() {
  return cCondition;
}

IConditionElementIcons* ConditionComponent::getConditionElementIcons() {
  return cConditionElementIcons;
}

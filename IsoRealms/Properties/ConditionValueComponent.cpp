
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
#include "ConditionValueComponent.h"

ConditionValueComponent::ConditionValueComponent(std::vector<ISizedComponent*> components) {
  cComponents = components;
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    IComponentBoundsCalculator* mCellCalculator = new CellLayout(this, i);
    cComponents[i]->setBoundsCalculator(mCellCalculator);
  }
}

float ConditionValueComponent::getWidth() {
  float mGridWidth = 0.02f;
  for (ISizedComponent* mComponent : cComponents) {
    mGridWidth += mComponent->getWidth();
  }
  return mGridWidth;
}

float ConditionValueComponent::getHeight() {
  float mGridHeight = 0.0f;
  for (ISizedComponent* mComponent : cComponents) {
    mGridHeight = std::max(mComponent->getHeight(), mGridHeight);
  }
  return mGridHeight;
}

void ConditionValueComponent::update(unsigned int ticks) {
  // Nothing to do
}

void ConditionValueComponent::render() {
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

bool ConditionValueComponent::input(SDL_Event& event) {
  return false; // Nothing to do.
}

ConditionValueComponent::CellLayout::CellLayout(ConditionValueComponent* parent, unsigned int location) {
  cParent = parent;
  cLocation = location;
}

float ConditionValueComponent::CellLayout::getLeft() {
  float cLeftOffset = cParent->getLeft() + 0.01;
  for (unsigned int i = 0; i < cLocation; i++) {
    cLeftOffset += cParent->cComponents[i]->getWidth();
  }
  return cLeftOffset;
}

float ConditionValueComponent::CellLayout::getRight() {
  float cLeftOffset = cParent->getLeft() + 0.01;
  for (unsigned int i = 0; i <= cLocation; i++) {
    cLeftOffset += cParent->cComponents[i]->getWidth();
  }
  return cLeftOffset;
}

float ConditionValueComponent::CellLayout::getTop() {
  return cParent->getBottom() + cParent->cComponents[cLocation]->getHeight();
}

float ConditionValueComponent::CellLayout::getBottom() {
  return cParent->getBottom();
}


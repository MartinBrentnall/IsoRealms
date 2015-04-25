/*
 * Copyright 2015 Martin Brentnall
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
#include "DialogModelSpindizzyJewel.h"

DialogModelSpindizzyJewel::DialogModelSpindizzyJewel(IEditingContext* editingContext, ResourceModelSpindizzyJewel* modelType, IResourceAccessor* resources, const std::string& resourceName) : DialogOKCancelUndo(editingContext, resources, "Spindizzy Jewel Model", resourceName) {
  cModelType = modelType;
  RectangularComponent* mContent = new RectangularComponent("Modules/Spindizzy/DialogModelSpindizzyJewel", resources);
  cResourceSelector = editingContext->getResourceSelector();
  cOriginalFrameColour     = cModelType->getFrameColour();
  cOriginalCycleColours    = cModelType->getCycleColours();
  cColourSelectorFrame     = new ColourSelector(this, cOriginalFrameColour,     0);
  for (unsigned int i = 0; i < cOriginalCycleColours.size(); i++) {
    ColourSelector* mColourSelector = new ColourSelector(this, cOriginalCycleColours[i], i + 1);
    cColourSelectorsCycle.push_back(mColourSelector);
  }
  mContent->setSelectable("frameColour",   cColourSelectorFrame);
//  setSelectable("cycleColours",  cColourSelectorsCycle);
  mContent->addComponent("previewPane", new ModelIcon(cModelType));
}

void DialogModelSpindizzyJewel::undo() {
  cColourSelectorFrame->resourceSelected(cOriginalFrameColour);
  // TODO: Reset number of cycle colours
  for (unsigned int i = 0; i < cColourSelectorsCycle.size(); i++) {
    cColourSelectorsCycle[i]->resourceSelected(cOriginalCycleColours[i]);
  }
}

ResourceModelSpindizzyJewel* DialogModelSpindizzyJewel::getResource() {
  return cModelType;
}

DialogModelSpindizzyJewel::ColourSelector::ColourSelector(DialogModelSpindizzyJewel* parent, IColour* colour, unsigned int which) {
  cWhich = which;
  cParent = parent;
  cColour = colour;
  cBorrowedColour = NULL;
}
    
void DialogModelSpindizzyJewel::ColourSelector::render(SelectableComponent* component) {
  float mLeft =   component->getLeft();
  float mRight =  component->getRight();
  float mTop =    component->getTop();
  float mBottom = component->getBottom();
  cColour->set();
  glBegin(GL_QUADS);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glVertex2f(mLeft,  mTop);
  glEnd();
}

void DialogModelSpindizzyJewel::ColourSelector::selected() {
  cParent->cResourceSelector->addResourceSelectionListener(this);
}

void DialogModelSpindizzyJewel::ColourSelector::deselected() {
  cParent->cResourceSelector->removeResourceSelectionListener(this);
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceOwned(cBorrowedColour);
    cBorrowedColour = NULL;
  }
}

void DialogModelSpindizzyJewel::ColourSelector::resourceSelected(IColour* colour) {
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceReleased(cBorrowedColour);
  }
  cColour = colour;
  cBorrowedColour = colour;
  switch (cWhich) {
    case 0:  cParent->cModelType->setFrameColour(cColour);             break;
    default: cParent->cModelType->setCycleColour(cWhich + 1, cColour); break;
  }
}

DialogModelSpindizzyJewel::ModelIcon::ModelIcon(ResourceModelSpindizzyJewel* modelType) {
  cModelType = modelType;
}

float DialogModelSpindizzyJewel::ModelIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.2f * mAspectRatio;
}

float DialogModelSpindizzyJewel::ModelIcon::getHeight() {
  return 0.2f;
}

void DialogModelSpindizzyJewel::ModelIcon::render() {
  glPushMatrix();
  float mScale = 0.12f; // TODO: Calculate this
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mXLocation = getLeft() + (getRight() - getLeft()) / 2.0f;
  float mYLocation = getBottom() + (getTop() - getBottom()) / 2.0f;
  glTranslatef(mXLocation, mYLocation, 0.0f);
  float mAspectRatio = mScreen->getAspectRatio();
  glScalef(mAspectRatio * mScale, mScale, mScale);
  glColor3f(1.0f, 1.0f, 1.0f);
  cModelType->renderIcon();
  glPopMatrix();
}

void DialogModelSpindizzyJewel::ModelIcon::update(unsigned int milliseconds) {
  // Nothing to do
}

bool DialogModelSpindizzyJewel::ModelIcon::input(SDL_Event& event) {
  return false;
}

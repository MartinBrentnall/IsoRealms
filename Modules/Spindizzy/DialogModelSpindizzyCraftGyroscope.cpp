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
#include "DialogModelSpindizzyCraftGyroscope.h"

DialogModelSpindizzyCraftGyroscope::DialogModelSpindizzyCraftGyroscope(IEditingContext* editingContext, ResourceModelSpindizzyCraftGyroscope* modelType, IResourceAccessor* resources, const std::string& resourceName) : DialogOKCancelUndo(editingContext, resources, "Spindizzy Gyroscope Craft Model", resourceName) {
  cModelType = modelType;
  RectangularComponent* mContent = new RectangularComponent("Modules/Spindizzy/DialogModelSpindizzyCraftGyroscope", resources);
  cResourceSelector = editingContext->getResourceSelector();
  cOriginalColour1       = cModelType->getColour1();
  cOriginalColour2       = cModelType->getColour2();
  cOriginalColour3       = cModelType->getColour3();
  cOriginalColour4       = cModelType->getColour4();
  cOriginalOutlineColour = cModelType->getOutlineColour();
  cColourSelector1       = new ColourSelector(this, cOriginalColour1,       0);
  cColourSelector2       = new ColourSelector(this, cOriginalColour2,       1);
  cColourSelector3       = new ColourSelector(this, cOriginalColour3,       2);
  cColourSelector4       = new ColourSelector(this, cOriginalColour4,       3);
  cColourSelectorOutline = new ColourSelector(this, cOriginalOutlineColour, 4);
  mContent->setSelectable("colour1",       cColourSelector1);
  mContent->setSelectable("colour2",       cColourSelector2);
  mContent->setSelectable("colour3",       cColourSelector3);
  mContent->setSelectable("colour4",       cColourSelector4);
  mContent->setSelectable("outlineColour", cColourSelectorOutline);
  mContent->addComponent("previewPane", new ModelIcon(cModelType));
  addComponent("content", mContent);
}

void DialogModelSpindizzyCraftGyroscope::undo() {
  cColourSelector1->resourceSelected(cOriginalColour1);
  cColourSelector2->resourceSelected(cOriginalColour2);
  cColourSelector3->resourceSelected(cOriginalColour3);
  cColourSelector4->resourceSelected(cOriginalColour4);
  cColourSelectorOutline->resourceSelected(cOriginalOutlineColour);
}

ResourceModelSpindizzyCraftGyroscope* DialogModelSpindizzyCraftGyroscope::getResource() {
  return cModelType;
}

DialogModelSpindizzyCraftGyroscope::ColourSelector::ColourSelector(DialogModelSpindizzyCraftGyroscope* parent, IColour* colour, unsigned int which) {
  cWhich = which;
  cParent = parent;
  cColour = colour;
  cBorrowedColour = NULL;
}
    
void DialogModelSpindizzyCraftGyroscope::ColourSelector::render(SelectableComponent* component) {
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

void DialogModelSpindizzyCraftGyroscope::ColourSelector::selected() {
  cParent->cResourceSelector->addResourceSelectionListener(this);
}

void DialogModelSpindizzyCraftGyroscope::ColourSelector::deselected() {
  cParent->cResourceSelector->removeResourceSelectionListener(this);
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceOwned(cBorrowedColour);
    cBorrowedColour = NULL;
  }
}

void DialogModelSpindizzyCraftGyroscope::ColourSelector::resourceSelected(IColour* colour) {
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceReleased(cBorrowedColour);
  }
  cColour = colour;
  cBorrowedColour = colour;
  switch (cWhich) {
    case 0: cParent->cModelType->setColour1(cColour);       break;
    case 1: cParent->cModelType->setColour2(cColour);       break;
    case 2: cParent->cModelType->setColour3(cColour);       break;
    case 3: cParent->cModelType->setColour4(cColour);       break;
    case 4: cParent->cModelType->setOutlineColour(cColour); break;
  }
}

DialogModelSpindizzyCraftGyroscope::ModelIcon::ModelIcon(ResourceModelSpindizzyCraftGyroscope* modelType) {
  cModelType = modelType;
}

float DialogModelSpindizzyCraftGyroscope::ModelIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.2f * mAspectRatio;
}

float DialogModelSpindizzyCraftGyroscope::ModelIcon::getHeight() {
  return 0.2f;
}

void DialogModelSpindizzyCraftGyroscope::ModelIcon::render() {
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

void DialogModelSpindizzyCraftGyroscope::ModelIcon::update(unsigned int milliseconds) {
  // Nothing to do
}

bool DialogModelSpindizzyCraftGyroscope::ModelIcon::input(SDL_Event& event) {
  return false;
}




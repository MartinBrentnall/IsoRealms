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
#include "DialogModelSpindizzyCraftPyramid.h"

DialogModelSpindizzyCraftPyramid::DialogModelSpindizzyCraftPyramid(IEditingContext* editingContext, ResourceModelSpindizzyCraftPyramid* modelType, IResourceAccessor* resources, const std::string& resourceName) : ResizableDialog(editingContext->getComponentContainer(), "Modules/Spindizzy/DialogModelSpindizzyCraftPyramid", resources) {
  cModelType = modelType;
  cResourceSelector = editingContext->getResourceSelector();
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
  setComponentAction("undoButton", new UndoCommand(this));
  cOriginalTopColour     = cModelType->getTopColour();
  cOriginalSideColour    = cModelType->getSideColour();
  cOriginalOutlineColour = cModelType->getOutlineColour();
  cColourSelectorTop     = new ColourSelector(this, cOriginalTopColour,     0);
  cColourSelectorSide    = new ColourSelector(this, cOriginalSideColour,    1);
  cColourSelectorOutline = new ColourSelector(this, cOriginalOutlineColour, 2);
  setSelectable("topColour",     cColourSelectorTop);
  setSelectable("sideColour"  ,  cColourSelectorSide);
  setSelectable("outlineColour", cColourSelectorOutline);
  addComponent("previewPane", new ModelIcon(cModelType));
}

void DialogModelSpindizzyCraftPyramid::addConfirmationListener(IConfirmationListener* listener) {
  cConfirmationListener = listener;
}

void DialogModelSpindizzyCraftPyramid::undo() {
  cColourSelectorTop->resourceSelected(cOriginalTopColour);
  cColourSelectorSide->resourceSelected(cOriginalSideColour);
  cColourSelectorOutline->resourceSelected(cOriginalOutlineColour);
}

ResourceModelSpindizzyCraftPyramid* DialogModelSpindizzyCraftPyramid::getResource() {
  return cModelType;
}

std::string DialogModelSpindizzyCraftPyramid::getResourceName() {
  return "TODO";
}

DialogModelSpindizzyCraftPyramid::OKCommand::OKCommand(DialogModelSpindizzyCraftPyramid* parent) {
  cParent = parent;
}

void DialogModelSpindizzyCraftPyramid::OKCommand::execute() {
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogConfirmed(cParent);
  }
}

DialogModelSpindizzyCraftPyramid::CancelCommand::CancelCommand(DialogModelSpindizzyCraftPyramid* parent) {
  cParent = parent;
}

void DialogModelSpindizzyCraftPyramid::CancelCommand::execute() {
  cParent->undo();
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogCancelled(cParent);
  }
}

DialogModelSpindizzyCraftPyramid::UndoCommand::UndoCommand(DialogModelSpindizzyCraftPyramid* parent) {
  cParent = parent;
}

void DialogModelSpindizzyCraftPyramid::UndoCommand::execute() {
  cParent->undo();
}

DialogModelSpindizzyCraftPyramid::ColourSelector::ColourSelector(DialogModelSpindizzyCraftPyramid* parent, IColour* colour, unsigned int which) {
  cWhich = which;
  cParent = parent;
  cColour = colour;
  cBorrowedColour = NULL;
}
    
void DialogModelSpindizzyCraftPyramid::ColourSelector::render(SelectableComponent* component) {
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

void DialogModelSpindizzyCraftPyramid::ColourSelector::selected() {
  cParent->cResourceSelector->addResourceSelectionListener(this);
}

void DialogModelSpindizzyCraftPyramid::ColourSelector::deselected() {
  cParent->cResourceSelector->removeResourceSelectionListener(this);
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceOwned(cBorrowedColour);
    cBorrowedColour = NULL;
  }
}

void DialogModelSpindizzyCraftPyramid::ColourSelector::resourceSelected(IColour* colour) {
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceReleased(cBorrowedColour);
  }
  cColour = colour;
  cBorrowedColour = colour;
  switch (cWhich) {
    case 0: cParent->cModelType->setTopColour(cColour);  break;
    case 1: cParent->cModelType->setSideColour(cColour);   break;
    case 2: cParent->cModelType->setOutlineColour(cColour);   break;
  }
}

DialogModelSpindizzyCraftPyramid::ModelIcon::ModelIcon(ResourceModelSpindizzyCraftPyramid* modelType) {
  cModelType = modelType;
}

float DialogModelSpindizzyCraftPyramid::ModelIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.2f * mAspectRatio;
}

float DialogModelSpindizzyCraftPyramid::ModelIcon::getHeight() {
  return 0.2f;
}

void DialogModelSpindizzyCraftPyramid::ModelIcon::render() {
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

void DialogModelSpindizzyCraftPyramid::ModelIcon::update(unsigned int milliseconds) {
  // Nothing to do
}

bool DialogModelSpindizzyCraftPyramid::ModelIcon::input(SDL_Event& event) {
  return false;
}




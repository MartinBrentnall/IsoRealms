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
#include "DialogTexturesSpindizzyLiftsC64.h"

DialogTexturesSpindizzyLiftsC64::DialogTexturesSpindizzyLiftsC64(IEditingContext* editingContext, ResourceTexturesSpindizzyLiftsC64* textureSet, IResourceAccessor* resources) : ResizableDialog(editingContext->getComponentContainer(), "Modules/Spindizzy/DialogTexturesSpindizzyLiftsC64", resources) {
  cSprites = textureSet;
  cResourceSelector = editingContext->getResourceSelector();
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
  setComponentAction("undoButton", new UndoCommand(this));
  cOriginalColour1       = cSprites->getColour1();
  cOriginalColour2       = cSprites->getColour2();
  cOriginalColour3       = cSprites->getColour3();
  cOriginalColourOutline = cSprites->getColourOutline();
  cColourSelector1       = new ColourSelector(this, cOriginalColour1,       0);
  cColourSelector2       = new ColourSelector(this, cOriginalColour2,       1);
  cColourSelector3       = new ColourSelector(this, cOriginalColour3,       2);
  cColourSelectorOutline = new ColourSelector(this, cOriginalColourOutline, 3);
  setSelectable("colour1",       cColourSelector1);
  setSelectable("colour2",       cColourSelector2);
  setSelectable("colour3",       cColourSelector3);
  setSelectable("outlineColour", cColourSelectorOutline);
  std::vector<ITexture*> mTextures = cSprites->getResources();
  for (unsigned int i = 0; i < mTextures.size(); i++) {
    addComponent("previewPane", new TextureIcon(mTextures[i]));
  }
}

void DialogTexturesSpindizzyLiftsC64::addConfirmationListener(IConfirmationListener* listener) {
  cConfirmationListener = listener;
}

void DialogTexturesSpindizzyLiftsC64::undo() {
  cColourSelector1->resourceSelected(cOriginalColour1);
  cColourSelector2->resourceSelected(cOriginalColour2);
  cColourSelector3->resourceSelected(cOriginalColour3);
  cColourSelectorOutline->resourceSelected(cOriginalColourOutline);
}

ResourceTexturesSpindizzyLiftsC64* DialogTexturesSpindizzyLiftsC64::getResource() {
  return cSprites;
}

std::string DialogTexturesSpindizzyLiftsC64::getResourceName() {
  return "TODO";
}

DialogTexturesSpindizzyLiftsC64::OKCommand::OKCommand(DialogTexturesSpindizzyLiftsC64* parent) {
  cParent = parent;
}

void DialogTexturesSpindizzyLiftsC64::OKCommand::execute() {
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogConfirmed(cParent);
  }
}

DialogTexturesSpindizzyLiftsC64::CancelCommand::CancelCommand(DialogTexturesSpindizzyLiftsC64* parent) {
  cParent = parent;
}

void DialogTexturesSpindizzyLiftsC64::CancelCommand::execute() {
  cParent->undo();
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogCancelled(cParent);
  }
}

DialogTexturesSpindizzyLiftsC64::UndoCommand::UndoCommand(DialogTexturesSpindizzyLiftsC64* parent) {
  cParent = parent;
}

void DialogTexturesSpindizzyLiftsC64::UndoCommand::execute() {
  cParent->undo();
}

DialogTexturesSpindizzyLiftsC64::ColourSelector::ColourSelector(DialogTexturesSpindizzyLiftsC64* parent, IColour* colour, unsigned int which) {
  cWhich = which;
  cParent = parent;
  cColour = colour;
  cBorrowedColour = NULL;
}
    
void DialogTexturesSpindizzyLiftsC64::ColourSelector::render(SelectableComponent* component) {
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

void DialogTexturesSpindizzyLiftsC64::ColourSelector::selected() {
  cParent->cResourceSelector->addResourceSelectionListener(this);
}

void DialogTexturesSpindizzyLiftsC64::ColourSelector::deselected() {
  cParent->cResourceSelector->removeResourceSelectionListener(this);
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceOwned(cBorrowedColour);
    cBorrowedColour = NULL;
  }
}

void DialogTexturesSpindizzyLiftsC64::ColourSelector::resourceSelected(IColour* colour) {
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceReleased(cBorrowedColour);
  }
  cColour = colour;
  cBorrowedColour = colour;
  switch (cWhich) {
    case 0: cParent->cSprites->setColour1(cColour);       break;
    case 1: cParent->cSprites->setColour2(cColour);       break;
    case 2: cParent->cSprites->setColour3(cColour);       break;
    case 3: cParent->cSprites->setColourOutline(cColour); break;
  }
}

DialogTexturesSpindizzyLiftsC64::TextureIcon::TextureIcon(ITexture* texture) {
  cTexture = texture;
}

float DialogTexturesSpindizzyLiftsC64::TextureIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.1f * mAspectRatio;
}

float DialogTexturesSpindizzyLiftsC64::TextureIcon::getHeight() {
  return 0.1f;
}

void DialogTexturesSpindizzyLiftsC64::TextureIcon::render() {
  cTexture->set();
  float mLeft = getLeft();
  float mRight = getRight();
  float mTop = getTop();
  float mBottom = getBottom();
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f); glVertex2f(mLeft,  mBottom);
  glTexCoord2f(1.0f, 1.0f); glVertex2f(mRight, mBottom);
  glTexCoord2f(1.0f, 0.0f); glVertex2f(mRight, mTop);
  glTexCoord2f(0.0f, 0.0f); glVertex2f(mLeft,  mTop);
  glEnd();
}

void DialogTexturesSpindizzyLiftsC64::TextureIcon::update(unsigned int milliseconds) {
  // Nothing to do
}

bool DialogTexturesSpindizzyLiftsC64::TextureIcon::input(SDL_Event& event) {
  return false;
}




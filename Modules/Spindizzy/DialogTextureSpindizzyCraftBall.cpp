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
#include "DialogTextureSpindizzyCraftBall.h"

DialogTextureSpindizzyCraftBall::DialogTextureSpindizzyCraftBall(IEditingContext* editingContext, ResourceTextureSpindizzyCraftBall* texture, IResourceAccessor* resources, const std::string& resourceName) : DialogOKCancelUndo(editingContext, resources, "Spindizzy Ball Craft Texture", resourceName) {
  RectangularComponent* mContent = new RectangularComponent("Modules/Spindizzy/DialogTextureSpindizzyCraftBall", resources);
  cTexture = texture;
  cResourceSelector = editingContext->getResourceSelector();
  cColourOriginalFill    = cTexture->getFillColour();
  cColourOriginalShine   = cTexture->getShineColour();
  cColourOriginalOutline = cTexture->getOutlineColour();
  cColourSelectorFill    = new ColourSelector(this, cColourOriginalFill,    0);
  cColourSelectorShine   = new ColourSelector(this, cColourOriginalShine,   1);
  cColourSelectorOutline = new ColourSelector(this, cColourOriginalOutline, 2);
  mContent->setSelectable("fillColour",  cColourSelectorFill);
  mContent->setSelectable("shineColour"  , cColourSelectorShine);
  mContent->setSelectable("outlineColour",   cColourSelectorOutline);
  mContent->addComponent("previewPane", new TextureIcon(cTexture));
  addComponent("content", mContent);
}

void DialogTextureSpindizzyCraftBall::undo() {
  cColourSelectorFill->resourceSelected(cColourOriginalFill);
  cColourSelectorShine->resourceSelected(cColourOriginalShine);
  cColourSelectorOutline->resourceSelected(cColourOriginalOutline);
}

ResourceTextureSpindizzyCraftBall* DialogTextureSpindizzyCraftBall::getResource() {
  return cTexture;
}

DialogTextureSpindizzyCraftBall::ColourSelector::ColourSelector(DialogTextureSpindizzyCraftBall* parent, IColour* colour, unsigned int which) {
  cWhich = which;
  cParent = parent;
  cColour = colour;
  cBorrowedColour = NULL;
}
    
void DialogTextureSpindizzyCraftBall::ColourSelector::render(SelectableComponent* component) {
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

void DialogTextureSpindizzyCraftBall::ColourSelector::selected() {
  cParent->cResourceSelector->addResourceSelectionListener(this);
}

void DialogTextureSpindizzyCraftBall::ColourSelector::deselected() {
  cParent->cResourceSelector->removeResourceSelectionListener(this);
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceOwned(cBorrowedColour);
    cBorrowedColour = NULL;
  }
}

void DialogTextureSpindizzyCraftBall::ColourSelector::resourceSelected(IColour* colour) {
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceReleased(cBorrowedColour);
  }
  cColour = colour;
  cBorrowedColour = colour;
  switch (cWhich) {
    case 0: cParent->cTexture->setFillColour(cColour);  break;
    case 1: cParent->cTexture->setShineColour(cColour);   break;
    case 2: cParent->cTexture->setOutlineColour(cColour);   break;
  }
}

DialogTextureSpindizzyCraftBall::TextureIcon::TextureIcon(ITexture* texture) {
  cTexture = texture;
}

float DialogTextureSpindizzyCraftBall::TextureIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.1f * mAspectRatio;
}

float DialogTextureSpindizzyCraftBall::TextureIcon::getHeight() {
  return 0.1f;
}

void DialogTextureSpindizzyCraftBall::TextureIcon::render() {
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

void DialogTextureSpindizzyCraftBall::TextureIcon::update(unsigned int milliseconds) {
  // Nothing to do
}

bool DialogTextureSpindizzyCraftBall::TextureIcon::input(SDL_Event& event) {
  return false;
}




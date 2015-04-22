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
#include "DialogTexturesSpindizzyBlocksC64.h"

DialogTexturesSpindizzyBlocksC64::DialogTexturesSpindizzyBlocksC64(IEditingContext* editingContext, ResourceTexturesSpindizzyBlocksC64* textureSet, IResourceAccessor* resources, const std::string& resourceName) : DialogOKCancelUndo(editingContext, resources, "Spindizzy Blocks Texture Set", resourceName) {
  RectangularComponent* mContent = new RectangularComponent("Modules/Spindizzy/DialogTexturesSpindizzyBlocksC64", resources);
  cTextureSet = textureSet;
  cResourceSelector = editingContext->getResourceSelector();
  cOriginalFloorColour  = cTextureSet->getFloorColour();
  cOriginalWallColour   = cTextureSet->getWallColour();
  cOriginalGridColour   = cTextureSet->getGridColour();
  cOriginalDetailColour = cTextureSet->getDetailColour();
  cColourSelectorFloor  = new ColourSelector(this, cOriginalFloorColour,  0);
  cColourSelectorWall   = new ColourSelector(this, cOriginalWallColour,   1);
  cColourSelectorGrid   = new ColourSelector(this, cOriginalGridColour,   2);
  cColourSelectorDetail = new ColourSelector(this, cOriginalDetailColour, 3);
  mContent->setSelectable("floorColour",  cColourSelectorFloor);
  mContent->setSelectable("wallColour"  , cColourSelectorWall);
  mContent->setSelectable("gridColour",   cColourSelectorGrid);
  mContent->setSelectable("detailColour", cColourSelectorDetail);
  std::vector<ITexture*> mTextures = cTextureSet->getResources();
  for (unsigned int i = 0; i < mTextures.size(); i++) {
    mContent->addComponent("previewPane", new TextureIcon(mTextures[i]));
  }
  addComponent("content", mContent);
}

void DialogTexturesSpindizzyBlocksC64::undo() {
  cColourSelectorFloor->resourceSelected(cOriginalFloorColour);
  cColourSelectorWall->resourceSelected(cOriginalWallColour);
  cColourSelectorGrid->resourceSelected(cOriginalGridColour);
  cColourSelectorDetail->resourceSelected(cOriginalDetailColour);
}

ResourceTexturesSpindizzyBlocksC64* DialogTexturesSpindizzyBlocksC64::getResource() {
  return cTextureSet;
}

std::string DialogTexturesSpindizzyBlocksC64::getResourceName() {
  return "TODO";
}

DialogTexturesSpindizzyBlocksC64::ColourSelector::ColourSelector(DialogTexturesSpindizzyBlocksC64* parent, IColour* colour, unsigned int which) {
  cWhich = which;
  cParent = parent;
  cColour = colour;
  cBorrowedColour = NULL;
}
    
void DialogTexturesSpindizzyBlocksC64::ColourSelector::render(SelectableComponent* component) {
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

void DialogTexturesSpindizzyBlocksC64::ColourSelector::selected() {
  cParent->cResourceSelector->addResourceSelectionListener(this);
}

void DialogTexturesSpindizzyBlocksC64::ColourSelector::deselected() {
  cParent->cResourceSelector->removeResourceSelectionListener(this);
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceOwned(cBorrowedColour);
    cBorrowedColour = NULL;
  }
}

void DialogTexturesSpindizzyBlocksC64::ColourSelector::resourceSelected(IColour* colour) {
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceReleased(cBorrowedColour);
  }
  cColour = colour;
  cBorrowedColour = colour;
  switch (cWhich) {
    case 0: cParent->cTextureSet->setFloorColour(cColour);  break;
    case 1: cParent->cTextureSet->setWallColour(cColour);   break;
    case 2: cParent->cTextureSet->setGridColour(cColour);   break;
    case 3: cParent->cTextureSet->setDetailColour(cColour); break;
  }
}

DialogTexturesSpindizzyBlocksC64::TextureIcon::TextureIcon(ITexture* texture) {
  cTexture = texture;
}

float DialogTexturesSpindizzyBlocksC64::TextureIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.1f * mAspectRatio;
}

float DialogTexturesSpindizzyBlocksC64::TextureIcon::getHeight() {
  return 0.1f;
}

void DialogTexturesSpindizzyBlocksC64::TextureIcon::render() {
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

void DialogTexturesSpindizzyBlocksC64::TextureIcon::update(unsigned int milliseconds) {
  // Nothing to do
}

bool DialogTexturesSpindizzyBlocksC64::TextureIcon::input(SDL_Event& event) {
  return false;
}




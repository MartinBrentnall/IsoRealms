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
  cColourSelectorFloor  = new ComponentResourceColourSelector(this, cOriginalFloorColour,  cResourceSelector);
  cColourSelectorWall   = new ComponentResourceColourSelector(this, cOriginalWallColour,   cResourceSelector);
  cColourSelectorGrid   = new ComponentResourceColourSelector(this, cOriginalGridColour,   cResourceSelector);
  cColourSelectorDetail = new ComponentResourceColourSelector(this, cOriginalDetailColour, cResourceSelector);
  mContent->setSelectable("floorColour",  cColourSelectorFloor);
  mContent->setSelectable("wallColour"  , cColourSelectorWall);
  mContent->setSelectable("gridColour",   cColourSelectorGrid);
  mContent->setSelectable("detailColour", cColourSelectorDetail);
  std::vector<ITexture*> mTextures = cTextureSet->getResources();
  for (unsigned int i = 0; i < mTextures.size(); i++) {
    mContent->addComponent("previewPane", new ComponentIconTexture(mTextures[i]));
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

void DialogTexturesSpindizzyBlocksC64::selected(ISelector* selector, IColour* colour) {
  if      (selector == cColourSelectorFloor)  {cTextureSet->setFloorColour(colour);}
  else if (selector == cColourSelectorWall)   {cTextureSet->setWallColour(colour);}
  else if (selector == cColourSelectorGrid)   {cTextureSet->setGridColour(colour);}
  else if (selector == cColourSelectorDetail) {cTextureSet->setDetailColour(colour);}
}

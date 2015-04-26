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

DialogTexturesSpindizzyLiftsC64::DialogTexturesSpindizzyLiftsC64(IEditingContext* editingContext, ResourceTexturesSpindizzyLiftsC64* textureSet, IResourceAccessor* resources, const std::string& resourceName) : DialogOKCancelUndo(editingContext, resources, "Spindizzy Lift Texture Set", resourceName) {
  RectangularComponent* mContent = new RectangularComponent("Modules/Spindizzy/DialogTexturesSpindizzyLiftsC64", resources);
  cSprites = textureSet;
  cResourceSelector = editingContext->getResourceSelector();
  cOriginalColour1       = cSprites->getColour1();
  cOriginalColour2       = cSprites->getColour2();
  cOriginalColourOutline = cSprites->getColourOutline();
  cColourSelector1       = new ComponentResourceColourSelector(this, cOriginalColour1,       cResourceSelector);
  cColourSelector2       = new ComponentResourceColourSelector(this, cOriginalColour2,       cResourceSelector);
  cColourSelectorOutline = new ComponentResourceColourSelector(this, cOriginalColourOutline, cResourceSelector);
  mContent->setSelectable("colour1",       cColourSelector1);
  mContent->setSelectable("colour2",       cColourSelector2);
  mContent->setSelectable("outlineColour", cColourSelectorOutline);
  std::vector<ITexture*> mTextures = cSprites->getResources();
  for (unsigned int i = 0; i < mTextures.size(); i++) {
    mContent->addComponent("previewPane", new ComponentIconTexture(mTextures[i]));
  }
  addComponent("content", mContent);
}

void DialogTexturesSpindizzyLiftsC64::undo() {
  cColourSelector1->resourceSelected(cOriginalColour1);
  cColourSelector2->resourceSelected(cOriginalColour2);
  cColourSelectorOutline->resourceSelected(cOriginalColourOutline);
}

ResourceTexturesSpindizzyLiftsC64* DialogTexturesSpindizzyLiftsC64::getResource() {
  return cSprites;
}

void DialogTexturesSpindizzyLiftsC64::selected(ISelector* selector, IColour* colour) {
  if      (selector == cColourSelector1)       {cSprites->setColour1(colour);}
  else if (selector == cColourSelector2)       {cSprites->setColour2(colour);}
  else if (selector == cColourSelectorOutline) {cSprites->setColourOutline(colour);}
}

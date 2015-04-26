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
  cColourSelectorFill    = new ComponentResourceColourSelector(this, cColourOriginalFill,    cResourceSelector);
  cColourSelectorShine   = new ComponentResourceColourSelector(this, cColourOriginalShine,   cResourceSelector);
  cColourSelectorOutline = new ComponentResourceColourSelector(this, cColourOriginalOutline, cResourceSelector);
  mContent->setSelectable("fillColour",  cColourSelectorFill);
  mContent->setSelectable("shineColour"  , cColourSelectorShine);
  mContent->setSelectable("outlineColour",   cColourSelectorOutline);
  mContent->addComponent("previewPane", new ComponentIconTexture(cTexture));
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

void DialogTextureSpindizzyCraftBall::selected(ISelector* selector, IColour* colour) {
  if      (selector == cColourSelectorFill)    {cTexture->setFillColour(colour);}
  else if (selector == cColourSelectorShine)   {cTexture->setShineColour(colour);}
  else if (selector == cColourSelectorOutline) {cTexture->setOutlineColour(colour);}
}

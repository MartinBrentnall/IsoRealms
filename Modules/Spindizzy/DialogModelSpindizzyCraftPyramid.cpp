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

DialogModelSpindizzyCraftPyramid::DialogModelSpindizzyCraftPyramid(IEditingContext* editingContext, ResourceModelSpindizzyCraftPyramid* modelType, IResourceAccessor* resources, const std::string& resourceName) : DialogOKCancelUndo(editingContext, resources, "Spindizzy Pyramid Craft Model", resourceName) {
  cModelType = modelType;
  RectangularComponent* mContent = new RectangularComponent("Modules/Spindizzy/DialogModelSpindizzyCraftPyramid", resources);
  cOriginalTopColour     = cModelType->getTopColour();
  cOriginalSideColour    = cModelType->getSideColour();
  cOriginalOutlineColour = cModelType->getOutlineColour();
  cColourSelectorTop     = new ComponentResourceColourSelector(this, cOriginalTopColour,     editingContext->getResourceSelector());
  cColourSelectorSide    = new ComponentResourceColourSelector(this, cOriginalSideColour,    editingContext->getResourceSelector());
  cColourSelectorOutline = new ComponentResourceColourSelector(this, cOriginalOutlineColour, editingContext->getResourceSelector());
  mContent->setSelectable("topColour",     cColourSelectorTop);
  mContent->setSelectable("sideColour"  ,  cColourSelectorSide);
  mContent->setSelectable("outlineColour", cColourSelectorOutline);
  mContent->addComponent("previewPane", new ComponentIconModel(cModelType));
  addComponent("content", mContent);
}

void DialogModelSpindizzyCraftPyramid::undo() {
  cColourSelectorTop->resourceSelected(cOriginalTopColour);
  cColourSelectorSide->resourceSelected(cOriginalSideColour);
  cColourSelectorOutline->resourceSelected(cOriginalOutlineColour);
}

ResourceModelSpindizzyCraftPyramid* DialogModelSpindizzyCraftPyramid::getResource() {
  return cModelType;
}

void DialogModelSpindizzyCraftPyramid::selected(ISelector* selector, IColour* colour) {
  if      (selector == cColourSelectorTop)     {cModelType->setTopColour(colour);}
  else if (selector == cColourSelectorSide)    {cModelType->setSideColour(colour);}
  else if (selector == cColourSelectorOutline) {cModelType->setOutlineColour(colour);}
}

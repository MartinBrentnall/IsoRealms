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
  cOriginalColour1       = cModelType->getColour1();
  cOriginalColour2       = cModelType->getColour2();
  cOriginalColour3       = cModelType->getColour3();
  cOriginalColour4       = cModelType->getColour4();
  cOriginalOutlineColour = cModelType->getOutlineColour();
  cColourSelector1       = new ComponentResourceColourSelector(this, cOriginalColour1,       editingContext->getResourceSelector());
  cColourSelector2       = new ComponentResourceColourSelector(this, cOriginalColour2,       editingContext->getResourceSelector());
  cColourSelector3       = new ComponentResourceColourSelector(this, cOriginalColour3,       editingContext->getResourceSelector());
  cColourSelector4       = new ComponentResourceColourSelector(this, cOriginalColour4,       editingContext->getResourceSelector());
  cColourSelectorOutline = new ComponentResourceColourSelector(this, cOriginalOutlineColour, editingContext->getResourceSelector());
  mContent->setSelectable("colour1",       cColourSelector1);
  mContent->setSelectable("colour2",       cColourSelector2);
  mContent->setSelectable("colour3",       cColourSelector3);
  mContent->setSelectable("colour4",       cColourSelector4);
  mContent->setSelectable("outlineColour", cColourSelectorOutline);
  mContent->addComponent("previewPane", new ComponentIconModel(cModelType));
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

void DialogModelSpindizzyCraftGyroscope::selected(ISelector* selector, IColour* colour) {
  if      (selector == cColourSelector1)       {cModelType->setColour1(colour);}
  else if (selector == cColourSelector2)       {cModelType->setColour2(colour);}
  else if (selector == cColourSelector3)       {cModelType->setColour3(colour);}
  else if (selector == cColourSelector4)       {cModelType->setColour4(colour);}
  else if (selector == cColourSelectorOutline) {cModelType->setOutlineColour(colour);}
}

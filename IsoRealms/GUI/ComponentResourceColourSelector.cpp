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
#include "ComponentResourceColourSelector.h"

ComponentResourceColourSelector::ComponentResourceColourSelector(IComponentSelectorListener<IColour>* parent, IColour* colour, IResourceSelector* resourceSelector) {
  cParent = parent;
  cResourceSelector = resourceSelector;
  cColour = colour;
  cBorrowedColour = NULL;
}
    
void ComponentResourceColourSelector::render(SelectableComponent* component) {
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

void ComponentResourceColourSelector::selected() {
  cResourceSelector->addResourceSelectionListener(this);
}

void ComponentResourceColourSelector::deselected() {
  cResourceSelector->removeResourceSelectionListener(this);
  if (cBorrowedColour != NULL) {
    cResourceSelector->notifyResourceOwned(cBorrowedColour);
    cBorrowedColour = NULL;
  }
}

void ComponentResourceColourSelector::resourceSelected(IColour* colour) {
  if (cBorrowedColour != NULL) {
    cResourceSelector->notifyResourceReleased(cBorrowedColour);
  }
  cColour = colour;
  cBorrowedColour = colour;
  cParent->selected(this, colour);
}

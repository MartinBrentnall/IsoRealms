/*
 * Copyright 2016 Martin Brentnall
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
#include "ComponentResourceTextureSelector.h"

ComponentResourceTextureSelector::ComponentResourceTextureSelector(IComponentSelectorListener<ITexture>* parent, ITexture* texture, IResourceSelector* resourceSelector) {
  cParent = parent;
  cResourceSelector = resourceSelector;
  cTexture = texture;
}
    
void ComponentResourceTextureSelector::render(SelectableComponent* component) {
  float mLeft =   component->getLeft();
  float mRight =  component->getRight();
  float mTop =    component->getTop();
  float mBottom = component->getBottom();
  cTexture->set();
  glBegin(GL_QUADS);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glVertex2f(mLeft,  mTop);
  glEnd();
}

void ComponentResourceTextureSelector::selected() {
  cResourceSelector->addResourceSelectionListener(this);
}

void ComponentResourceTextureSelector::deselected() {
  cResourceSelector->removeResourceSelectionListener(this);
}

void ComponentResourceTextureSelector::resourceSelected(ITexture* texture) {
  cTexture = texture;
  cParent->selected(this, texture);
}

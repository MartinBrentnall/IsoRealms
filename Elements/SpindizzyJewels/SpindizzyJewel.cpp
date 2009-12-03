/*
 * Copyright 2009 Martin Brentnall
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
#include "SpindizzyJewel.h"

SpindizzyJewel::SpindizzyJewel(IElementFactory* elementFactory, BlockLocation* location, ISimpleModel* jewelModel) : IElement(elementFactory) {
  cModel = jewelModel;
  cLocation = BlockLocation(*location);
//  cCollected = false;
}

void SpindizzyJewel::setModel(ISimpleModel* model) {
  cModel = model;
}

ISimpleModel* SpindizzyJewel::getModel() {
  return cModel;
}

void SpindizzyJewel::renderStatic() {
  // Nothing to do.
}

std::vector<IVisualElement*> SpindizzyJewel::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  mVisualElements.push_back(this);
  return mVisualElements;
}

std::vector<IDynamicElement*> SpindizzyJewel::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
  mDynamicElements.push_back(this);
  return mDynamicElements;
}

std::vector<IInteractiveElement*> SpindizzyJewel::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  return mInteractiveElements;
}

void SpindizzyJewel::save(DOMNodeWriter* node, BlockLocation& location) {
  DOMNodeWriter* mLocationNode = node->addBranch("Location");
  cLocation.saveRelative(mLocationNode, location);  
}

void SpindizzyJewel::update(int milliseconds) {
  cModel->update(milliseconds);
}

void SpindizzyJewel::render() {
  glPushMatrix();
  glTranslatef(cLocation.x, cLocation.y, cLocation.z * IsoRealmsConstants::BLOCK_HEIGHT);
  cModel->render();
  glPopMatrix();
}

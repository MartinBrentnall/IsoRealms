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
#include "SpindizzyGERALD.h"

SpindizzyGERALD::SpindizzyGERALD(IElementFactory* elementFactory, BlockLocation* startLocation, ISimpleModel* geraldModel) : IElement(elementFactory) {
  cGERALDModel = geraldModel;
  cStartLocation = BlockLocation(*startLocation);
  cCurrentLocation = new Vertex(cStartLocation.x + IsoRealmsConstants::BLOCK_RADIUS, cStartLocation.y + IsoRealmsConstants::BLOCK_RADIUS, cStartLocation.z);
}

void SpindizzyGERALD::setModel(ISimpleModel* geraldModel) {
  cGERALDModel = geraldModel;
}

void SpindizzyGERALD::renderStatic() {
  // Nothing to do.
}

std::vector<IVisualElement*> SpindizzyGERALD::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  mVisualElements.push_back(this);
  return mVisualElements;
}

std::vector<IDynamicElement*> SpindizzyGERALD::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
  return mDynamicElements;
}

std::vector<IInteractiveElement*> SpindizzyGERALD::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  return mInteractiveElements;
}

void SpindizzyGERALD::render() {
  glPushMatrix();
  glTranslatef(cCurrentLocation->x, cCurrentLocation->y, cCurrentLocation->z * IsoRealmsConstants::BLOCK_HEIGHT);
  cGERALDModel->render();
  glPopMatrix();
}

void SpindizzyGERALD::save(DOMNodeWriter* node, BlockLocation& relative) {
  cStartLocation.saveRelative(node, relative);  
}


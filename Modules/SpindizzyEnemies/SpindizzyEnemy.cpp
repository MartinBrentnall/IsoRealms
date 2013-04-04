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
#include "SpindizzyEnemy.h"

SpindizzyEnemy::SpindizzyEnemy(IElementType* elementType, BlockLocation* startLocation, I3DModelFactory* modelType) : Element<>(elementType) {
  cStartLocation = BlockLocation(*startLocation);
  cCurrentLocation = new Vertex(cStartLocation.x, cStartLocation.y, cStartLocation.z);
  cEnemyModel = modelType->createModel(cCurrentLocation);
}

void SpindizzyEnemy::renderStatic() {
  // Nothing to do.
}

void SpindizzyEnemy::reset() {
  // TODO: Implement this
}

std::vector<IVisualElement*> SpindizzyEnemy::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  mVisualElements.push_back(this);
  return mVisualElements;
}

std::vector<IDynamicElement*> SpindizzyEnemy::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
  return mDynamicElements;
}

std::vector<IInteractiveElement*> SpindizzyEnemy::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  return mInteractiveElements;
}

void SpindizzyEnemy::render() {
  glPushMatrix();
  cEnemyModel->render();
  glPopMatrix();
}

void SpindizzyEnemy::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& relative) {
  cStartLocation.saveRelative(node, relative);  
}

void SpindizzyEnemy::setDirty() {
  // Nothing to do
}



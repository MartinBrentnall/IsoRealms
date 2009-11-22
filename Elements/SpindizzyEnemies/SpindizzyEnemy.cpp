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

SpindizzyEnemy::SpindizzyEnemy(IElementFactory* elementFactory, BlockLocation* startLocation, ISimpleModel* enemyModel) : IElement(elementFactory) {
  cEnemyModel = enemyModel;
  cStartLocation = BlockLocation(*startLocation);
  cCurrentLocation = new Vertex(cStartLocation.x, cStartLocation.y, cStartLocation.z);
}

SpindizzyEnemy::SpindizzyEnemy(IElementFactory* elementFactory, DOMNodeWrapper* node, BlockLocation* relative) : IElement(elementFactory) {
//  cLocation.set(node); // TODO:
}

void SpindizzyEnemy::setModel(ISimpleModel* enemyModel) {
  cEnemyModel = enemyModel;
}

void SpindizzyEnemy::renderStatic() {
  // Nothing to do.
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
  glTranslatef(cCurrentLocation->x, cCurrentLocation->y, cCurrentLocation->z * IsoRealmsConstants::BLOCK_HEIGHT);
  cEnemyModel->render();
  glPopMatrix();
}

void SpindizzyEnemy::save(DOMNodeWriter* node, BlockLocation& relative) {
  cStartLocation.saveRelative(node, relative);  
}



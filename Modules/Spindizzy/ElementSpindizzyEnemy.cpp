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
#include "ElementSpindizzyEnemy.h"

ElementSpindizzyEnemy::ElementSpindizzyEnemy(IElementType* elementType, BlockLocation* startLocation, I3DModelType* modelType, IElementContainer* container) {
  cEnemyType = elementType;
  cStartLocation = BlockLocation(*startLocation);
  cCurrentLocation = new Vertex(cStartLocation.x, cStartLocation.y, cStartLocation.z);
  cEnemyModel = modelType->createModel(cCurrentLocation);
  cContainer = container;
}

void ElementSpindizzyEnemy::setModelType(I3DModelType* oldModelType, I3DModelType* newModelType) {
  oldModelType->destroyModel(cEnemyModel);
  cEnemyModel = newModelType->createModel(cCurrentLocation);
}

IElementContainer* ElementSpindizzyEnemy::getElementContainer() {
  return cContainer;
}

IElementType* ElementSpindizzyEnemy::getElementType() {
  return cEnemyType;
}

void ElementSpindizzyEnemy::renderEditing() {
  renderRuntime();
}

void ElementSpindizzyEnemy::renderStatic() {
  // Nothing to do.
}

void ElementSpindizzyEnemy::reset() {
  // TODO: Implement this
}

void ElementSpindizzyEnemy::renderRuntime() {
  glPushMatrix();
  cEnemyModel->render();
  glPopMatrix();
}

void ElementSpindizzyEnemy::updateRuntime(unsigned int milliseconds) {
  // TODO: Implement this
}

void ElementSpindizzyEnemy::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& relative) {
  cStartLocation.saveRelative(node, relative);  
}

void ElementSpindizzyEnemy::setDirty() {
  // Nothing to do
}

bool ElementSpindizzyEnemy::initElement(unsigned int pass) {
  return true;
}

IElementBounds* ElementSpindizzyEnemy::getBounds() {
  return this;
}

void ElementSpindizzyEnemy::processCursorAppearance(ILayerEditingContext* editingContext, Vertex& location) {
  Element::processCursorAppearance(editingContext, location);
}

void ElementSpindizzyEnemy::processCursorMovement(ILayerEditingContext* editingContext, Vertex& start, Vertex& end) {
  Element::processCursorMovement(editingContext, start, end);
}

PickedElement* ElementSpindizzyEnemy::pickElement(Vertex& start, Vertex& end) {
  return Element::pickElement(start, end);
}

float ElementSpindizzyEnemy::getWest() {
  return cStartLocation.x;
}

float ElementSpindizzyEnemy::getEast() {
  return cStartLocation.x;
}

float ElementSpindizzyEnemy::getSouth() {
  return cStartLocation.y;
}

float ElementSpindizzyEnemy::getNorth() {
  return cStartLocation.y;
}

float ElementSpindizzyEnemy::getBottom() {
  return cStartLocation.z;
}

float ElementSpindizzyEnemy::getTop() {
  return cStartLocation.z;
}

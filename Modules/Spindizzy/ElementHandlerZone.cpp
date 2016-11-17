/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "ElementHandlerZone.h"

ElementHandlerZone::ElementHandlerZone(IModuleElementHandlerZone* moduleInterface) {
  cSingleZone = true;
  cZone = nullptr;
  cModuleInterface = moduleInterface;
}

void ElementHandlerZone::setSingleZone(bool singleZone) {
  cSingleZone = singleZone;
}

void ElementHandlerZone::setCurrentZone(ElementSpindizzyZone* zone) {
  cZone = zone;
}

void ElementHandlerZone::setValue() {
  cModuleInterface->setArgumentValue(this);
}

void ElementHandlerZone::unsetValue() {
  cModuleInterface->setArgumentValue(nullptr);
}

void ElementHandlerZone::addElement(ElementSpindizzyZone* element) {
  cElements.addElement(element);
}

void ElementHandlerZone::removeElement(ElementSpindizzyZone* element) {
  cElements.removeElement(element);
}

bool ElementHandlerZone::isEmpty() {
  return cElements.isEmpty();
}

void ElementHandlerZone::renderEditing() {
  cElements.renderEditing();
}

bool ElementHandlerZone::renderSelectionHighlight() {
  return false;
}

void ElementHandlerZone::renderRuntime() {
  if (cSingleZone && !cModuleInterface->isOverview()) {
    cZone->renderRuntime();
  } else {
    cElements.renderRuntime();
  }
}

void ElementHandlerZone::updateRuntime(unsigned int ticks) {
  cElements.updateRuntime(ticks);
}

std::string ElementHandlerZone::getTypeName() {
  return ""; // TODO
}
  
std::vector<IObjectProperty*> ElementHandlerZone::getProperties() {
  return std::vector<IObjectProperty*>();
}

IElementType* ElementHandlerZone::getElementType() {
  return nullptr;
}

void ElementHandlerZone::renderStatic() {
  // Nothing to do.
}

void ElementHandlerZone::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  cElements.save(node, resourceLocator, location);
}

void ElementHandlerZone::setDirty() {
  // Nothing to do?
}

IElementBounds* ElementHandlerZone::getBounds() {
  return cElements.getBounds();
}

bool ElementHandlerZone::initElement(unsigned int pass) {
  return cElements.init(pass);
}

bool ElementHandlerZone::isSelectable() {
  return false;
}

void ElementHandlerZone::cursorMoved(ILayerEditingContext* editingContext, Vertex& start, Vertex& end) {
  cElements.cursorMoved(editingContext, start, end);
}

void ElementHandlerZone::cursorAppeared(ILayerEditingContext* editingContext, Vertex& location) {
  cElements.cursorAppeared(editingContext, location);
}

PickedElement* ElementHandlerZone::pickElement(Vertex& start, Vertex& end) {
  return cElements.pickElement(start, end);
}

IElementContainer* ElementHandlerZone::getElementContainer() {
  return nullptr;
}

void ElementHandlerZone::reset() {
  cElements.reset();
}

bool ElementHandlerZone::isImplicit() {
  return true;
}

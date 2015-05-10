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
#include "ElementHandlerItem.h"

ElementHandlerItem::ElementHandlerItem(ISpindizzyJewelSet* moduleInterface) {
  cModuleInterface = moduleInterface;
}

unsigned int ElementHandlerItem::getCount() {
  unsigned int mCount = 0;
  for (unsigned int i = 0; i < cElements.size(); i++) {
    if (!cElements[i]->isCollected()) {
      mCount++;
    }
  }
  return mCount;
}

void ElementHandlerItem::setValue() {
  cModuleInterface->setArgumentValue(this);
}

void ElementHandlerItem::unsetValue() {
  cModuleInterface->setArgumentValue(nullptr);
}

void ElementHandlerItem::addElement(ElementSpindizzyItem* element) {
  cElements.addElement(element);
}

void ElementHandlerItem::removeElement(ElementSpindizzyItem* element) {
  cElements.removeElement(element);
}

bool ElementHandlerItem::isEmpty() {
  return cElements.isEmpty();
}

void ElementHandlerItem::renderRuntime() {
  cElements.renderRuntime();
}

void ElementHandlerItem::updateRuntime(unsigned int ticks) {
  cElements.updateRuntime(ticks);
}

IElementType* ElementHandlerItem::getElementType() {
  return nullptr;
}

void ElementHandlerItem::renderStatic() {
  // Nothing to do.
}

void ElementHandlerItem::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  // TODO: ?
}

void ElementHandlerItem::setDirty() {
  // Nothing to do?
}

IElementBounds* ElementHandlerItem::getBounds() {
  return cElements.getBounds();
}

bool ElementHandlerItem::initElement(unsigned int pass) {
  return cElements.init(pass);
}

bool ElementHandlerItem::isSelectable() {
  return false;
}

void ElementHandlerItem::cursorMoved(ILayerEditingContext* editingContext, Vertex& start, Vertex& end) {
  cElements.cursorMoved(editingContext, start, end);
}

void ElementHandlerItem::cursorAppeared(ILayerEditingContext* editingContext, Vertex& location) {
  cElements.cursorAppeared(editingContext, location);
}

PickedElement* ElementHandlerItem::pickElement(Vertex& start, Vertex& end) {
  return cElements.pickElement(start, end);
}

IElementContainer* ElementHandlerItem::getElementContainer() {
  return nullptr;
}

void ElementHandlerItem::reset() {
  cElements.reset();
}

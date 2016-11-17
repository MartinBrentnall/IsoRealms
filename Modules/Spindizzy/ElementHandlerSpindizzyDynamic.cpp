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
#include "ElementHandlerSpindizzyDynamic.h"

ElementHandlerSpindizzyDynamic::ElementHandlerSpindizzyDynamic(IModuleElementHandlerSpindizzyDynamic* moduleInterface) {
  cVisibility = 0.0f;
  cActive = false;
  cModuleInterface = moduleInterface;
}

void ElementHandlerSpindizzyDynamic::reset() {
  cElements.reset();
}

void ElementHandlerSpindizzyDynamic::setActive(bool active) {
  cActive = active;
}

void ElementHandlerSpindizzyDynamic::setValue() {
  cModuleInterface->setArgumentValue(this);
}

void ElementHandlerSpindizzyDynamic::unsetValue() {
  cModuleInterface->setArgumentValue(nullptr);
}

void ElementHandlerSpindizzyDynamic::addElement(IElementSpindizzyDynamic* element) {
  cElements.addElement(element);
}

void ElementHandlerSpindizzyDynamic::removeElement(IElementSpindizzyDynamic* element) {
  cElements.removeElement(element);
}

bool ElementHandlerSpindizzyDynamic::isEmpty() {
  return cElements.isEmpty();
}

void ElementHandlerSpindizzyDynamic::renderEditing() {
  cElements.renderEditing();
}

bool ElementHandlerSpindizzyDynamic::renderSelectionHighlight() {
  return false;
}

void ElementHandlerSpindizzyDynamic::renderRuntime() {
  if (cVisibility > 0.0f) {
    glEnable(GL_BLEND);
    glColor4f(1.0f, 1.0f, 1.0f, cVisibility);
    cElements.renderRuntime();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  }
}

void ElementHandlerSpindizzyDynamic::updateRuntime(unsigned int ticks) {
  if (cActive) {
    if (cVisibility < 1.0f) {
      cVisibility += ticks / 500.0f;
      if (cVisibility > 1.0f) {
        cVisibility = 1.0f;
      }
    }
    cElements.updateRuntime(ticks);
  } else {
    if (cVisibility > 0.0f) {
      cVisibility -= ticks / 500.0f;
      if (cVisibility < 0.0f) {
        cVisibility = 0.0f;
      }
    }
  }
}

std::string ElementHandlerSpindizzyDynamic::getTypeName() {
  return ""; // TODO
}
  
std::vector<IObjectProperty*> ElementHandlerSpindizzyDynamic::getProperties() {
  return std::vector<IObjectProperty*>();
}

IElementType* ElementHandlerSpindizzyDynamic::getElementType() {
  return nullptr;
}

void ElementHandlerSpindizzyDynamic::renderStatic() {
  // Nothing to do.
}

void ElementHandlerSpindizzyDynamic::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  cElements.save(node, resourceLocator, location);
}

void ElementHandlerSpindizzyDynamic::setDirty() {
  // Nothing to do?
}

IElementBounds* ElementHandlerSpindizzyDynamic::getBounds() {
  return cElements.getBounds();
}

bool ElementHandlerSpindizzyDynamic::isSelectable() {
  return false;
}

void ElementHandlerSpindizzyDynamic::cursorMoved(ILayerEditingContext* editingContext, Vertex& start, Vertex& end) {
  cElements.cursorMoved(editingContext, start, end);
}

void ElementHandlerSpindizzyDynamic::cursorAppeared(ILayerEditingContext* editingContext, Vertex& location) {
  cElements.cursorAppeared(editingContext, location);
}

PickedElement* ElementHandlerSpindizzyDynamic::pickElement(Vertex& start, Vertex& end) {
  return cElements.pickElement(start, end);
}

bool ElementHandlerSpindizzyDynamic::initElement(unsigned int pass) {
  return cElements.init(pass);
}

IElementContainer* ElementHandlerSpindizzyDynamic::getElementContainer() {
  return nullptr;
}

bool ElementHandlerSpindizzyDynamic::isImplicit() {
  return true;
}

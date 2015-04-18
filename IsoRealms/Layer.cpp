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
#include "Layer.h"

Layer::Layer(DOMNodeWrapper* node, Resources* resources) {
  BlockLocation* mIdentity = new BlockLocation();
  resources->loadElement(node, mIdentity, this);
}

void Layer::updateEditing(unsigned int milliseconds) {
  cRootElement->updateEditing(milliseconds);
}

void Layer::updateRuntime(unsigned int milliseconds) {
  cRootElement->updateRuntime(milliseconds);
}

void Layer::renderRuntime() {
  cRootElement->renderStatic();
  cRootElement->renderRuntime();
}

void Layer::renderEditing() {
  cRootElement->renderStatic();
  cRootElement->renderEditing();
}

void Layer::input(SDL_Event& event) {
  cRootElement->input(event);
}

void Layer::initRuntime() {
  cRootElement->initRuntime();
}

void Layer::save(DOMNodeWriter* node, IResourceLocator* resources) {
  IElementType* mElementType = cRootElement->getElementType();
  node->addAttribute("rootElementType", resources->getPath(mElementType));
  BlockLocation* mIdentity = new BlockLocation();
  cRootElement->save(node, resources, *mIdentity);
}

void Layer::staticChanged() {
  cRootElement->staticChanged();
}

void Layer::addElement(IElement* element) {
  cRootElement = element;
}

void Layer::removeElement(IElement* element) {
  if (cRootElement == element) {
    cRootElement = NULL;
  }
}

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
#include "ResourceModelElement.h"

ResourceModelElement::ResourceModelElement(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
}

I3DModel* ResourceModelElement::createModel(Vertex* location, float scale) {
  return new ModelElement(&cElement, location, scale);
}

void ResourceModelElement::destroyModel(I3DModel* model) {
  delete model;
}

void ResourceModelElement::save(DOMNodeWriter* node, IResourceLocator* resources) {
  cElement->save(node, resources, cIdentity);
  node->addAttribute("independent", true);
}

void ResourceModelElement::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  resources->loadElement(node, &cIdentity, this);
  Vertex* mVertex = new Vertex(0.0f, 0.0f, 0.0f);
  cSampleModel = new ModelElement(&cElement, mVertex, 1.0f);
}

void ResourceModelElement::addElement(IElement* element) {
  cElement = element;
}

void ResourceModelElement::removeElement(IElement* element) {
  cElement = NULL;
}

void ResourceModelElement::addArgumentValue(IArgument* argument) {
  // Ignore it
}

void ResourceModelElement::setArguments() {
  // Not supported
}

void ResourceModelElement::unsetArguments() {
  // Nothing to do
}

BlockArea* ResourceModelElement::getCoverage() {
  return nullptr;
}

void ResourceModelElement::setDirty() {
  // Nothing to do
}

void ResourceModelElement::updateIcon(unsigned int milliseconds) {
  cSampleModel->update(milliseconds);
}

void ResourceModelElement::renderIcon() {
  glTranslatef(0.0f, -0.5f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  cSampleModel->render();
}


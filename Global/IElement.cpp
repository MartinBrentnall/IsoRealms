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
#include "IElement.h"

IElement::IElement(IElementFactory* elementFactory) {
  cElementFactory = elementFactory;
  cElementContainer = NULL;
}

IElementSet* IElement::getElementSet() {
  return cElementFactory->getElementSet();
}

IElementFactory* IElement::getElementFactory() {
  return cElementFactory;
}

void IElement::signalElementDirty() {
  if (cElementContainer) {
    cElementContainer->elementDirty(this);
  }
}

void IElement::setElementContainer(IElementContainer* elementContainer) {
  cElementContainer = elementContainer;
}

void IElement::removed() {
  // Nothing to do.
}

void IElement::added() {
  // Nothing to do.
}

bool IElement::initElement() {
  return true;
}


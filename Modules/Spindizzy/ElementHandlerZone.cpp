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
  cSingleZone = false;
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
  cElements.push_back(element);
}

void ElementHandlerZone::renderEditing() {
  for (unsigned int i = 0; i < cElements.size(); i++) {
    cElements[i]->renderEditing();
  }
}

void ElementHandlerZone::renderRuntime() {
  if (cSingleZone && !cModuleInterface->isOverview()) {
    cZone->renderRuntime();
  } else {
    for (unsigned int i = 0; i < cElements.size(); i++) {
      cElements[i]->renderRuntime();
    }
  }
}

void ElementHandlerZone::updateRuntime(unsigned int ticks) {
  for (unsigned int i = 0; i < cElements.size(); i++) {
    cElements[i]->updateRuntime(ticks);
  }
}

IElementType* ElementHandlerZone::getElementType() {
  return nullptr;
}

void ElementHandlerZone::renderStatic() {
  // Nothing to do.
}

void ElementHandlerZone::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  // TODO: ?
}

void ElementHandlerZone::setDirty() {
  // Nothing to do?
}

IElementBounds* ElementHandlerZone::getBounds() {
  float mWest   = std::numeric_limits<float>::max();
  float mEast   = std::numeric_limits<float>::lowest();
  float mSouth  = std::numeric_limits<float>::max();
  float mNorth  = std::numeric_limits<float>::lowest();
  float mBottom = std::numeric_limits<float>::max();
  float mTop    = std::numeric_limits<float>::lowest();
  for (unsigned int i = 0; i < cElements.size(); i++) {
    IElementBounds* mElementBounds = cElements[i]->getBounds();
    if (mElementBounds != nullptr) {
      mWest   = std::min(mWest,   mElementBounds->getWest());
      mEast   = std::max(mEast,   mElementBounds->getEast());
      mSouth  = std::min(mSouth,  mElementBounds->getSouth());
      mNorth  = std::max(mNorth,  mElementBounds->getNorth());
      mBottom = std::min(mBottom, mElementBounds->getBottom());
      mTop    = std::max(mTop,    mElementBounds->getTop());
    }
  }
  return new ElementBounds(mWest, mEast, mSouth, mNorth, mBottom, mTop);
}

bool ElementHandlerZone::initElement(unsigned int pass) {
  bool mSuccess = true;
  for (unsigned int i = 0; i < cElements.size(); i++) {
    if (!cElements[i]->initElement(pass)) {
      mSuccess = false;
    }
  }
  return mSuccess;
}

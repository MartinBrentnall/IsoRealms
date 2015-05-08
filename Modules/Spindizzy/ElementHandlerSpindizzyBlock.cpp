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
#include "ElementHandlerSpindizzyBlock.h"

ElementHandlerSpindizzyBlock::ElementHandlerSpindizzyBlock(ISpindizzyBlockSet* moduleInterface, IElementContainer* container) {
  cModuleInterface = moduleInterface;
  cContainer = container;
}

unsigned int ElementHandlerSpindizzyBlock::getClueCount() {
  unsigned int mClueCount = 0;
  for (unsigned int i = 0; i < cClues.size(); i++) {
    if (!cClues[i]->isActive()) {
      mClueCount++;
    }
  }
  return mClueCount;
}

I3DModel* ElementHandlerSpindizzyBlock::getClue(unsigned int index) {
  unsigned int mClueCount = 0;
  for (unsigned int i = 0; i < cClues.size(); i++) {
    if (!cClues[i]->isActive()) {
      if (mClueCount == index) {
        return cClues[i]->getModel();
      }
      mClueCount++;
    }
  }
  return nullptr;
}

void ElementHandlerSpindizzyBlock::setValue() {
  cModuleInterface->setArgumentValue(this);
}

void ElementHandlerSpindizzyBlock::unsetValue() {
  cModuleInterface->setArgumentValue(nullptr);
}

void ElementHandlerSpindizzyBlock::addElement(ElementSpindizzyBlock* element) {
  cElements.addElement(element);
  Condition* mCondition = element->getCondition();
  if (mCondition != nullptr) {
    std::set<IBoolean*> mInputs = mCondition->getInputs();
    for (std::set<IBoolean*>::iterator i = mInputs.begin(); i != mInputs.end(); i++) {
      SpindizzyBlockState* mBlockState = cModuleInterface->getBlockState(*i);
      bool mExists = false;
      for (unsigned int j = 0; j < cClues.size(); j++) {
        if (cClues[j] == mBlockState) {
          mExists = true;
          break;
        }
      }
      if (!mExists) {
        cClues.push_back(mBlockState);
      }
    }
  }
}

void ElementHandlerSpindizzyBlock::removeElement(ElementSpindizzyBlock* element) {
  cElements.removeElement(element);
}

bool ElementHandlerSpindizzyBlock::isEmpty() {
  return cElements.isEmpty();
}

void ElementHandlerSpindizzyBlock::renderRuntime() {
  cElements.renderRuntime();
}

void ElementHandlerSpindizzyBlock::updateRuntime(unsigned int ticks) {
  cElements.updateRuntime(ticks);
}

IElementType* ElementHandlerSpindizzyBlock::getElementType() {
  return nullptr;
}

void ElementHandlerSpindizzyBlock::renderStatic() {
  cElements.renderStatic();
}

void ElementHandlerSpindizzyBlock::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  // TODO: ?
}

void ElementHandlerSpindizzyBlock::setDirty() {
  // Nothing to do?
}

IElementBounds* ElementHandlerSpindizzyBlock::getBounds() {
  return cElements.getBounds();
}

bool ElementHandlerSpindizzyBlock::initElement(unsigned int pass) {
  return cElements.init(pass);
}

void ElementHandlerSpindizzyBlock::setArguments() {
  cContainer->setArguments();
}

void ElementHandlerSpindizzyBlock::unsetArguments() {
  cContainer->unsetArguments();
}

void ElementHandlerSpindizzyBlock::setDirty(IElement* element) {
  cContainer->setDirty(this);
}

IElementContainer* ElementHandlerSpindizzyBlock::getElementContainer() {
  return cContainer;
}

bool ElementHandlerSpindizzyBlock::isSelectable() {
  return false;
}

void ElementHandlerSpindizzyBlock::cursorMoved(ILayerEditingContext* editingContext, Vertex& start, Vertex& end) {
  cElements.cursorMoved(editingContext, start, end);
}

void ElementHandlerSpindizzyBlock::cursorAppeared(ILayerEditingContext* editingContext, Vertex& location) {
  cElements.cursorAppeared(editingContext, location);
}

PickedElement* ElementHandlerSpindizzyBlock::pickElement(Vertex& start, Vertex& end) {
  return cElements.pickElement(start, end);
}

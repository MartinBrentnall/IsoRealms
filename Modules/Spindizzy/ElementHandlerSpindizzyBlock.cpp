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

ElementHandlerSpindizzyBlock::ElementHandlerSpindizzyBlock(ISpindizzyBlockSet* moduleInterface) {
  cModuleInterface = moduleInterface;
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
  cElements.push_back(element);
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

void ElementHandlerSpindizzyBlock::renderRuntime() {
  for (unsigned int i = 0; i < cElements.size(); i++) {
    cElements[i]->renderRuntime();
  }
}

void ElementHandlerSpindizzyBlock::updateRuntime(unsigned int ticks) {
  for (unsigned int i = 0; i < cElements.size(); i++) {
    cElements[i]->updateRuntime(ticks);
  }
}

IElementType* ElementHandlerSpindizzyBlock::getElementType() {
  return nullptr;
}

void ElementHandlerSpindizzyBlock::renderStatic() {
  for (unsigned int i = 0; i < cElements.size(); i++) {
    cElements[i]->renderStatic();
  }
}

void ElementHandlerSpindizzyBlock::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  // TODO: ?
}

void ElementHandlerSpindizzyBlock::setDirty() {
  // Nothing to do?
}

IElementBounds* ElementHandlerSpindizzyBlock::getBounds() {
  return nullptr;
}

bool ElementHandlerSpindizzyBlock::initElement(unsigned int pass) {
  bool mSuccess = true;
  for (unsigned int i = 0; i < cElements.size(); i++) {
    if (!cElements[i]->initElement(pass)) {
      mSuccess = false;
    }
  }
  return mSuccess;
}

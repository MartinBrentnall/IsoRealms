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
#include "ElementHandler.h"

ElementHandler::ElementHandler() {
  cUpdateStatic = false;
  cSpawnThreads = false;
  cStaticBounds = new ElementBounds(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  cDirty = false;
}

std::vector<IElement*> ElementHandler::getElements() {
  return cElements;
}

void ElementHandler::setMultiThreaded(bool multiThreaded) {
  cSpawnThreads = multiThreaded;
}

void ElementHandler::addElement(IElement* element) {
  cElements.push_back(element);
  if (element->isDynamicRuntime()) {
    cDynamicElementsRuntime.push_back(element);
  }
  if (element->isDynamicEditing()) {
    cDynamicElementsEditing.push_back(element);
  }
  if (element->isVisualRuntime()) {
    cVisualElementsRuntime.push_back(element);
  }
  if (element->isVisualEditing()) {
    cVisualElementsEditing.push_back(element);
  }
}

void ElementHandler::removeElement(IElement* element) {
  for (unsigned int i = 0; i < cElements.size(); i++) {
    if (cElements[i] == element) {
      cElements.erase(cElements.begin() + i);
    }
  }
  for (unsigned int i = 0; i < cDynamicElementsRuntime.size(); i++) {
    if (cDynamicElementsRuntime[i] == element) {
      cDynamicElementsRuntime.erase(cDynamicElementsRuntime.begin() + i);
    }
  }
  for (unsigned int i = 0; i < cDynamicElementsEditing.size(); i++) {
    if (cDynamicElementsEditing[i] == element) {
      cDynamicElementsEditing.erase(cDynamicElementsEditing.begin() + i);
    }
  }
  for (unsigned int i = 0; i < cVisualElementsRuntime.size(); i++) {
    if (cVisualElementsRuntime[i] == element) {
      cVisualElementsRuntime.erase(cVisualElementsRuntime.begin() + i);
    }
  }
  for (unsigned int i = 0; i < cVisualElementsEditing.size(); i++) {
    if (cVisualElementsEditing[i] == element) {
      cVisualElementsEditing.erase(cVisualElementsEditing.begin() + i);
    }
  }
}

void ElementHandler::setAllDirty() {
  cDirtyElements = cElements;
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    cDirtyElements[i]->setDirty();
  }
  cDirty = true;
}

void ElementHandler::setDirty(IElement* element) {
  if (!contains(element)) {
    std::cout << "WARNING: Specified dirty element is not a member of this container!  Did you forget to set the cursor's container?" << std::endl;
    return;
  }
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    if (cDirtyElements[i] == element) {
      return;
    }
  }
  // TODO: Does order matter?
  cDirtyElements.push_back(element);
}

bool ElementHandler::isDirty() {
  return !cDirtyElements.empty() || cDirty;
}

bool ElementHandler::contains(IElement* element) {
  for (unsigned int i = 0; i < cElements.size(); i++) {
    if (element == cElements[i]) {
      return true;
    }
  }
  return false;
}

void ElementHandler::updateEditing(unsigned int milliseconds) {
  for (unsigned int i = 0; i < cDynamicElementsEditing.size(); i++) {
    cDynamicElementsEditing[i]->updateEditing(milliseconds);
  }
}

void ElementHandler::updateRuntime(unsigned int milliseconds) {
  for (unsigned int i = 0; i < cDynamicElementsRuntime.size(); i++) {
    cDynamicElementsRuntime[i]->updateRuntime(milliseconds);
  }
}

int ElementHandler::getIndex(IElement* element) {
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    if (cDirtyElements[i] == element) {
      return i;
    }
  }
  // TODO: Throw exception
  return -1;
}

int startThread(void* elementHandler) {
  ElementHandler* mElementHandler = static_cast<ElementHandler*>(elementHandler);
  mElementHandler->initThread();
  return 0;
}

void ElementHandler::initThread() {
  unsigned int mInitPass = 0;
  
//  while (!cDirtyElements.empty()) {
    initElementHandler(mInitPass, cEditing);
    mInitPass++;
//  }

  while (SDL_mutexP(cElementQueueMutex) == -1);
  cLiveThreads--;
  if (cLiveThreads == 0) {
    SDL_CondSignal(cThreadInitComplete);
  }
  SDL_mutexV(cElementQueueMutex);
}

void ElementHandler::initElementHandler(unsigned int pass, bool editing) {
  while (SDL_mutexP(cElementQueueMutex) == -1);
  while (!cDirtyElementsRemaining.empty()) {
    IElement* mElement = getElementToInit();
    if (SDL_mutexV(cElementQueueMutex) == -1) {
      std::cout << "Couldn't unlock mutex" << std::endl;
      exit(-1);
    }
    if (mElement->initElement(pass)) {
      while (SDL_mutexP(cElementQueueMutex) == -1);
      cCleanElements.push_back(mElement);
      if (SDL_mutexV(cElementQueueMutex) == -1) {
        std::cout << "Couldn't unlock mutex" << std::endl;
        exit(-1);
      }
    }
    while (SDL_mutexP(cElementQueueMutex) == -1);
  }
  if (SDL_mutexV(cElementQueueMutex) == -1) {
    std::cout << "Couldn't unlock mutex" << std::endl;
    exit(-1);
  }
}

bool ElementHandler::initMultiThreaded() {
  cElementQueueMutex = SDL_CreateMutex();
  SDL_mutexP(cElementQueueMutex);
  cThreadInitComplete = SDL_CreateCond();
  unsigned int mCPUCores = System::getCPUCores();
  while (!cDirtyElements.empty()) {
    cDirtyElementsRemaining = cDirtyElements;
    cLiveThreads = mCPUCores;
    for (unsigned int i = 0; i < mCPUCores; i++) {
      SDL_CreateThread(startThread, this);
    }
    SDL_CondWait(cThreadInitComplete, cElementQueueMutex);
    for (unsigned int i = 0; i < cCleanElements.size(); i++) {
      int mIndexToRemove = getElementIndex(cCleanElements[i]);
      cDirtyElements.erase(cDirtyElements.begin() + mIndexToRemove);
    }
    cCleanElements.clear();
  }
  staticChanged();
  return cDirtyElements.empty();
}

bool ElementHandler::initSingleThreaded() {
  int mPass = 0;
  while (!cDirtyElements.empty()) {
    cDirtyElementsRemaining = cDirtyElements;
    while (!cDirtyElementsRemaining.empty()) {
      IElement* mElement = getElementToInit();
      if (mElement->initElement(mPass)) {
        cCleanElements.push_back(mElement);
      }
    }
    for (unsigned int i = 0; i < cCleanElements.size(); i++) {
      int mIndexToRemove = getElementIndex(cCleanElements[i]);
      cDirtyElements.erase(cDirtyElements.begin() + mIndexToRemove);
    }
    cCleanElements.clear();
    mPass++;
  }
  staticChanged();
  if (cDirtyElements.empty()) {
    cDirty = false;
  }
  return cDirtyElements.empty();
}

bool ElementHandler::init(unsigned int pass, bool editing) {  
  if (!cDirtyElements.empty()) {
    cEditing = editing;
    if (cSpawnThreads) {
      return initMultiThreaded();
    } else {
      return initSingleThreaded();
    }
  }
  return true;
}

void ElementHandler::staticChanged() {
  cUpdateStatic = true;
}

void ElementHandler::updateStatic() {
  glDeleteLists(cDisplayList, 1);
  cDisplayList = glGenLists(1);
  glNewList(cDisplayList, GL_COMPILE);
  for (int i = cElements.size() - 1; i >= 0; i--) {
    cElements[i]->renderStatic();
  }
  glEndList();

  updateStaticBounds();
  
//   if (editing) {
//     glDeleteLists(cEditingDisplayList, 1);
//     cEditingDisplayList = glGenLists(1);
//     glNewList(cEditingDisplayList, GL_COMPILE);
//     for (unsigned int i = 0; i < cElements.size(); i++) {
//       cElements[i]->renderStatic();
//     }
//     glEndList();
//   }
}

void ElementHandler::renderStatic() {
  if (cUpdateStatic) {
    updateStatic();
    cUpdateStatic = false;
  }
  glCallList(cDisplayList);
}

void ElementHandler::renderRuntime() {
  for (unsigned int i = 0; i < cVisualElementsRuntime.size(); i++) {
    cVisualElementsRuntime[i]->renderRuntime();
  }
}

void ElementHandler::renderEditing() {
  if (isDirty()) {
    init(0, true);
  }
  for (unsigned int i = 0; i < cVisualElementsEditing.size(); i++) {
    cVisualElementsEditing[i]->renderEditing();
  }
}

void ElementHandler::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  for (unsigned int i = 0; i < cElements.size(); i++) {
    DOMNodeWriter* mNode = node;
    if (!resourceLocator->isImplicit(cElements[i])) {
      mNode = node->addBranch("Element");
      IElementType* mElementType = cElements[i]->getElementType();
      mNode->addAttribute("type", resourceLocator->getPath(mElementType));
    }
    cElements[i]->save(mNode, resourceLocator, location);
  }
}

void ElementHandler::cursorAppeared(ILayerEditingContext* editingContext, Vertex& location) {
  for (IElement* mElement : cElements) {
    mElement->processCursorAppearance(editingContext, location);
  }
}

void ElementHandler::cursorMoved(ILayerEditingContext* editingContext, Vertex& start, Vertex& end) {
  for (IElement* mElement : cElements) {
    mElement->processCursorMovement(editingContext, start, end);
  }
}

int ElementHandler::getElementIndex(IElement* element) {
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    if (cDirtyElements[i] == element) {
      return i;
    }
  }
  // TODO: Throw exception
  return -1;
}

IElement* ElementHandler::getElementToInit() {
  IElement* mDirtyElement = NULL;
  if (!cDirtyElementsRemaining.empty()) {
    mDirtyElement = cDirtyElementsRemaining[0];
    cDirtyElementsRemaining.erase(cDirtyElementsRemaining.begin());
  }
  return mDirtyElement;
}

void ElementHandler::updateStaticBounds() {
  delete cStaticBounds;
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
  cStaticBounds = new ElementBounds(mWest, mEast, mSouth, mNorth, mBottom, mTop);
}

IElementBounds* ElementHandler::getStaticBounds() {
  return cStaticBounds;
}

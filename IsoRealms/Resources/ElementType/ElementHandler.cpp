#include "ElementHandler.h"

ElementHandler::ElementHandler() {
  cUpdateStatic = false;
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
  // TODO: Implement this
}

void ElementHandler::setAllDirty() {
  cDirtyElements = cElements;
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    cDirtyElements[i]->setDirty();
  }
}

void ElementHandler::setDirty(IElement* element) {
  if (!contains(element)) {
    std::cout << "WARNING: Specified dirty element is not a member of this Zone!  Did you forget to set the cursor's Zone?" << std::endl;
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
  return !cDirtyElements.empty();
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

bool ElementHandler::init(unsigned int pass, bool editing) {
  std::vector<IElement*> mCleanElements;
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    if (cDirtyElements[i]->initElement(pass)) {
      mCleanElements.push_back(cDirtyElements[i]);
    }
  }
  
  for (unsigned int i = 0; i < mCleanElements.size(); i++) {
    int mIndexToRemove = getIndex(mCleanElements[i]);
    cDirtyElements.erase(cDirtyElements.begin() + mIndexToRemove);
  }
  
  if (cDirtyElements.empty()) {

    // Game rendering
    updateStatic();

    // Editor-only rendering
    if (editing) {
      glDeleteLists(cEditingDisplayList, 1);
      cEditingDisplayList = glGenLists(1);
      glNewList(cEditingDisplayList, GL_COMPILE);
      for (unsigned int i = 0; i < cElements.size(); i++) {
        cElements[i]->renderStatic();
      }
      glEndList();
    }
  }
  return cDirtyElements.empty();
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

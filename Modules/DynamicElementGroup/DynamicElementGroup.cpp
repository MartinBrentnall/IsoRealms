#include "DynamicElementGroup.h"

void DynamicElementGroup::setActive(bool active) {
  cActive = active;
}

void DynamicElementGroup::addElement(IElement* element) {
  cElementHandler.addElement(element);
}

void DynamicElementGroup::removeElement(IElement* element) {
// TODO  cElementHandler.removeElement(element):
}

IPlugin* DynamicElementGroup::getElementSet() {
  return NULL;
}

IElementType* DynamicElementGroup::getElementType() {
  return NULL;
}

void DynamicElementGroup::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
}

bool DynamicElementGroup::initElement(unsigned int pass) {
  return cElementHandler.init(pass, false);
}

void DynamicElementGroup::renderStatic() {
}

void DynamicElementGroup::input(SDL_Event& event) {
  // TODO: Implement this
}

void DynamicElementGroup::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  cElementHandler.save(node, resourceLocator, location);
}

void DynamicElementGroup::setDirty() {
  cElementHandler.setAllDirty();
}

void DynamicElementGroup::renderRuntime() {
  if (cVisibility > 0.0f) {
    if (cVisibility < 1.0f) {
      glEnable(GL_BLEND);
      glColor4f(1.0f, 1.0f, 1.0f, cVisibility);
    }
    cElementHandler.renderStatic();
    cElementHandler.renderRuntime();
    if (cVisibility < 1.0f) {
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
  }
}

void DynamicElementGroup::renderEditing() {
  cElementHandler.renderStatic();
  cElementHandler.renderEditing();
}

void DynamicElementGroup::updateRuntime(unsigned int milliseconds) {
  if (cActive) {
    if (cVisibility < 1.0f) {
      cVisibility += milliseconds / 500.0f;
      if (cVisibility > 1.0f) {
        cVisibility = 1.0f;
      }
    }
    cElementHandler.updateRuntime(milliseconds);
  } else if (cVisibility > 0.0f) {
    cVisibility -= milliseconds / 500.0f;
    if (cVisibility < 0.0f) {
      cVisibility = 0.0f;
    }
  }
}

void DynamicElementGroup::updateEditing(unsigned int milliseconds) {
  cElementHandler.updateEditing(milliseconds);
}

bool DynamicElementGroup::isVisualRuntime() {
  return true;
}

bool DynamicElementGroup::isVisualEditing() {
  return true;
}

bool DynamicElementGroup::isDynamicRuntime() {
  return true;
}

bool DynamicElementGroup::isDynamicEditing() {
  return true;
}

bool DynamicElementGroup::isInteractive() {
  return true;
}

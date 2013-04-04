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

void DynamicElementGroup::update(unsigned int ticks) {
  if (cActive) {
    if (cVisibility < 1.0f) {
      cVisibility += ticks / 500.0f;
      if (cVisibility > 1.0f) {
        cVisibility = 1.0f;
      }
    }
    cElementHandler.update(ticks);
  } else if (cVisibility > 0.0f) {
    cVisibility -= ticks / 500.0f;
    if (cVisibility < 0.0f) {
      cVisibility = 0.0f;
    }
  }
}

void DynamicElementGroup::render() {
  if (cVisibility > 0.0f) {
    if (cVisibility < 1.0f) {
      glEnable(GL_BLEND);
      glColor4f(1.0f, 1.0f, 1.0f, cVisibility);
    }
    cElementHandler.renderStatic();
    cElementHandler.renderDynamic();
    if (cVisibility < 1.0f) {
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
  }
}

IPlugin* DynamicElementGroup::getElementSet() {
  return NULL;
}

IElementType* DynamicElementGroup::getElementType() {
  return NULL;
}

bool DynamicElementGroup::initElement(unsigned int pass) {
  return cElementHandler.init(pass, false);
}

void DynamicElementGroup::renderStatic() {
}

void DynamicElementGroup::renderStaticEditing() {
}

bool DynamicElementGroup::input(SDL_Event& event) {
  // TODO: Implement this
  return false;
}

std::vector<IVisualElement*> DynamicElementGroup::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  mVisualElements.push_back(this);
  return mVisualElements;
}

std::vector<IDynamicElement*> DynamicElementGroup::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
//  mDynamicElements.push_back(this);
  return mDynamicElements;
}

std::vector<IDynamicElement*> DynamicElementGroup::getDynamicElementsRuntime() {
  std::vector<IDynamicElement*> mDynamicElements;
  mDynamicElements.push_back(this);
  return mDynamicElements;
}

std::vector<IInteractiveElement*> DynamicElementGroup::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  mInteractiveElements.push_back(this);
  return mInteractiveElements;
}

void DynamicElementGroup::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  cElementHandler.save(node, resourceLocator, location);
}

void DynamicElementGroup::setDirty() {
  cElementHandler.setAllDirty();
}

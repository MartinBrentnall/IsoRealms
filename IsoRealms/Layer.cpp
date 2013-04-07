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

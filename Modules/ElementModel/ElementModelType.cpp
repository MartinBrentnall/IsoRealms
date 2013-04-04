#include "ElementModelType.h"

I3DModel* ElementModelType::createModel(Vertex* location, float scale) {
  return new ElementModel(&cElement, location, scale);
}

void ElementModelType::destroyModel(I3DModel* model) {
  delete model;
}

void ElementModelType::save(DOMNodeWriter* node, IResourceLocator* resources) {
  cElement->save(node, resources, cIdentity);
  node->addAttribute("independent", true);
}

void ElementModelType::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  resources->loadElement(node, &cIdentity, this);
}

void ElementModelType::addElement(IElement* element) {
  cElement = element;
}

void ElementModelType::removeElement(IElement* element) {
  cElement = NULL;
}

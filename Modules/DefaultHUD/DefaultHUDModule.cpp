#include "DefaultHUDModule.h"

void DefaultHUDModule::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  runtimeContext->add(this, "HUD", node);
}

void DefaultHUDModule::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cResourceAccessor = resources;
}

IPlugin* DefaultHUDModule::getElementSet() {
  return this;
}

IElement* DefaultHUDModule::getElement(DOMNodeWrapper* node, BlockLocation* location, IElementContainer* container) {
  return new DefaultHUD(node, cResourceAccessor, this);
}

void DefaultHUDModule::configureElement() {
  // TODO
}

void DefaultHUDModule::renderEditingPreview() {
  // TODO
}

void DefaultHUDModule::renderIcon() {
  // TODO
}

void DefaultHUDModule::updateIcon(unsigned int milliseconds) {
  // TODO
}

void DefaultHUDModule::destroy(IElement* element) {
  delete element;
}

IElementHandler* DefaultHUDModule::getElementHandler() {
  return NULL;
}

extern "C" IPlugin* create() {
  return new DefaultHUDModule();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

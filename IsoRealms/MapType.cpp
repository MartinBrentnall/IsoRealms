#include "MapType.h"

MapType::MapType(bool editing, IProject* project, IResources* resources) {
  cProject = project;
  cEditing = editing;
  cResources = resources;
}

void MapType::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
}

IPlugin* MapType::getElementSet() {
  return NULL;
}

void MapType::loadInstance(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  std::string mInstanceName = node->getAttribute("instance");
  Map* mMapInstance = new Map(cEditing, cProject, cResources);
  cNamedInstances[mInstanceName] = mMapInstance;
  IArgumentSource* mArgumentSource = new ArgumentSourceCustom<IMap>(mMapInstance);
  runtimeContext->add(mMapInstance, mInstanceName, node);
  runtimeContext->registerArgument("Map", mInstanceName, mArgumentSource);
}

IElement* MapType::getElement(DOMNodeWrapper* node, BlockLocation* location, IElementContainer* container) {
  std::string mInstanceName = node->getAttribute("instance");
  if (mInstanceName != "") {
    return cNamedInstances[mInstanceName];
  } else {
    return new Map(node, cEditing, cProject, cResources);
  }
}

void MapType::configureElement() {
  // TODO: Implement this
}

bool MapType::input(SDL_Event& event) {
  // TODO: Implement this
  return false;
}

void MapType::renderEditingPreview() {
  // TODO: Implement this
}

void MapType::renderIcon() {
  // TODO: Implement this
}

void MapType::updateIcon(unsigned int) {
  // TODO: Implement this
}

void MapType::destroy(IElement* element) {
  delete element;
}

IElementHandler* MapType::getElementHandler() {
  return NULL;
}

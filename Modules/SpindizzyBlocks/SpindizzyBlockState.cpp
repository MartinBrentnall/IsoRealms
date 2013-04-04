#include "SpindizzyBlockState.h"

SpindizzyBlockState::SpindizzyBlockState() {
}

void SpindizzyBlockState::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cName = node->getAttribute("name");
  bool* mState = new bool(false);
  cState = new ConditionElement(cName, mState);
  cClueModelLocation.x = node->getFloatAttribute("x");
  cClueModelLocation.y = node->getFloatAttribute("y");
  cClueModelLocation.z = node->getFloatAttribute("z");
  std::string mModelPath = node->getAttribute("model");
  cClueModel = resources->getModel(mModelPath, &cClueModelLocation);
}

bool* SpindizzyBlockState::getInputAddress() {
  return cState->getInputAddress();
}

I3DModel* SpindizzyBlockState::getModel() {
  return cClueModel;
}

ConditionElement* SpindizzyBlockState::getConditionElement() {
  return cState;
}

void SpindizzyBlockState::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("name", cName);
  node->addAttribute("model", resourceLocator->getPath(cClueModel));
  node->addAttribute("x", cClueModelLocation.x);
  node->addAttribute("y", cClueModelLocation.y);
  node->addAttribute("z", cClueModelLocation.z);
}

void SpindizzyBlockState::setActive(bool value) {
  bool* mState = cState->getInputAddress();
  *mState = value;
}

bool SpindizzyBlockState::isActive() {
  bool* mState = cState->getInputAddress();
  return *mState;
}


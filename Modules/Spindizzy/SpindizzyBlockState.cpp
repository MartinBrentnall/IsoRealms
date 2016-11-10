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
#include "SpindizzyBlockState.h"

SpindizzyBlockState::SpindizzyBlockState() {
}

void SpindizzyBlockState::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cName = node->getAttribute("name");
  cClueModelLocation.x = node->getFloatAttribute("x");
  cClueModelLocation.y = node->getFloatAttribute("y");
  cClueModelLocation.z = node->getFloatAttribute("z");
  cScale = node->getFloatAttribute("scale");
  std::string mModelPath = node->getAttribute("model");
  std::string mStatePath = node->getAttribute("state");
  cClueModel = resources->getModel(mModelPath, &cClueModelLocation, cScale);
  IBoolean* mState = resources->getBoolean(mStatePath);
  cState = new ConditionElement(cName, mState);
}

IBoolean* SpindizzyBlockState::getInputAddress() {
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
  node->addAttribute("state", resourceLocator->getPath(cState->getInputAddress()));
  node->addAttribute("scale", cScale);
}

bool SpindizzyBlockState::isActive() {
  IBoolean* mState = cState->getInputAddress();
  return mState->getValue();
}


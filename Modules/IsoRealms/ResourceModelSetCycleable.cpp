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
#include "ResourceModelSetCycleable.h"

ResourceModelSetCycleable::ResourceModelSetCycleable(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cModelSetName = node->getAttribute("name");
  unsigned int mIndex = 0;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mTag = mNode->getNodeName();
    if (mTag == "Model") {
      cCycleableModels.push_back(new ModelCycleableOffset(this, mIndex));
      cModelTypes.push_back(nullptr);
      mIndex++;
    }
  }
  IArgumentValue* mArgumentValue = new ArgumentValueCustomType<ResourceModelSetCycleable>(this);
  resourceRegistry->add(mArgumentValue, "ModelSetCycleable", cModelSetName);
}

void ResourceModelSetCycleable::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  unsigned int mIndex = 0;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mTag = mNode->getNodeName();
    if (mTag == "Model") {
      cModelTypes[mIndex] = resources->getModelType(mNode->getAttribute("name"));
      cCycleableModels[mIndex]->updateModel(cModelTypes[i], mIndex);
      mIndex++;
    }
  }
}

void ResourceModelSetCycleable::save(DOMNodeWriter* node, IResourceLocator* resources) {
  node->addAttribute("name", cModelSetName);
  for (I3DModelType* mModelType : cModelTypes) {
    DOMNodeWriter* mModelBranch = node->addBranch("Model");
    std::string mModelTypePath = resources->getPath(mModelType);
    mModelBranch->addAttribute("name", mModelTypePath);
  }
}

void ResourceModelSetCycleable::saveCache(DOMNodeWriter* cache) {
  // Nothing to do
}

std::vector<std::string> ResourceModelSetCycleable::getResourceNames() {
  std::vector<std::string> mNames;
  for (unsigned int i = 0; i < cCycleableModels.size(); i++) {
    mNames.push_back(std::to_string(i));
  }
  return mNames;
}

bool ResourceModelSetCycleable::contains(I3DModelType* modelType) {
  for (ModelCycleableOffset* mCycleableModelType : cCycleableModels) {
    if (mCycleableModelType == modelType) {
      return true;
    }
  }
  return false;
}

std::string ResourceModelSetCycleable::stripMemberName(const std::string& name) {
  return name.substr(0, name.rfind('_'));
}

I3DModelType* ResourceModelSetCycleable::getResource(const std::string& name) {
  return cCycleableModels[atoi(name.c_str())];
}

std::vector<I3DModelType*> ResourceModelSetCycleable::getResources() {
  std::vector<I3DModelType*> mCycleableModels;
  for (ModelCycleableOffset* mCycleableModelType : cCycleableModels) {
    mCycleableModels.push_back(mCycleableModelType);
  }
  return mCycleableModels;
}

void ResourceModelSetCycleable::next() {
  if (++cCycleIndex == cModelTypes.size()) {
    cCycleIndex = 0;
  }
}

void ResourceModelSetCycleable::previous() {
  if (cCycleIndex-- == 0) {
    cCycleIndex = cModelTypes.size() - 1;
  }
}

ResourceModelSetCycleable::ModelCycleableOffset::ModelCycleableOffset(ResourceModelSetCycleable* parent, unsigned int offset) {
  cParent = parent;
  cOffset = offset;
}

void ResourceModelSetCycleable::ModelCycleableOffset::updateModel(I3DModelType* modelType, unsigned int index) {
  for (ModelCycleable* mCycleableModel : cModels) {
    mCycleableModel->setModel(modelType, index);
  }
}

unsigned int ResourceModelSetCycleable::ModelCycleableOffset::getCycleIndex() {
  return (cParent->cCycleIndex + cOffset) % cParent->cModelTypes.size();
}

void ResourceModelSetCycleable::ModelCycleableOffset::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  // cParent->cModelTypes[cOffset] = resources->getModelType(node->getAttribute("model"));
}

I3DModel* ResourceModelSetCycleable::ModelCycleableOffset::createModel(Vertex* location, float scale) {
  return new ModelCycleable(this, cParent->cModelTypes, location, scale);
}

void ResourceModelSetCycleable::ModelCycleableOffset::destroyModel(I3DModel* model) {
  delete model;
}

void ResourceModelSetCycleable::ModelCycleableOffset::renderIcon() {
  // TODO
}

void ResourceModelSetCycleable::ModelCycleableOffset::updateIcon(unsigned int milliseconds) {
  // TODO
}

/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#include "ResourceModelScriptable.h"

ResourceModelScriptable::ResourceModelScriptable(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Instance") {
      std::string mName = mNode->getAttribute("name");
      ModelScriptable* mInstance = createModel(nullptr, 1.0f);
      IArgumentValue* mArgumentValue = new ArgumentValueCustomType<ModelScriptable>(mInstance);
      resourceRegistry->add(mInstance, mName);
      resourceRegistry->add(mArgumentValue, "ModelScriptable", mName);
      cResources.push_back(mInstance);
    }
  }
}

void ResourceModelScriptable::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resourceAccessor) {
//   runtimeContext->add(this, "Factory");
//   for (int i = 0; i < node->getChildCount(); i++) {
//     DOMNodeWrapper *mNode = node->getChild(i);
//     std::string mValueAsString = mNode->getNodeName();
//     if (mValueAsString == "Model") {
//       ModelScriptable* mModel = createModel(NULL, 1.0f);
//       std::string mName = mNode->getAttribute("name");
//       IArgumentSource* mArgumentSource = new ArgumentSourceCustom<ModelScriptable>(mModel);
//       runtimeContext->add(mModel, mName, mNode);
//       runtimeContext->registerArgument("Model", mName, mArgumentSource);
//     }
//   }
}

ModelScriptable* ResourceModelScriptable::createModel(Vertex* location, float scale) {
  return new ModelScriptable();
}

void ResourceModelScriptable::destroyModel(I3DModel* model) {
  delete model;
}

void ResourceModelScriptable::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cResources.size(); i++) {
    DOMNodeWriter* mResourceNode = node->addBranch("Instance");
    std::string mResourceName = resourceLocator->getPath(cResources[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mResourceNode->addAttribute("name", mResourceName);
  }
}

void ResourceModelScriptable::updateIcon(unsigned int milliseconds) {
  // TODO
}

void ResourceModelScriptable::renderIcon() {
  // TODO: Sample
}

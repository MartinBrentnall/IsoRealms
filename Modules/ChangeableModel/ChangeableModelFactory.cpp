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
#include "ChangeableModelFactory.h"

void ChangeableModelFactory::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  runtimeContext->add(this, "Factory");
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Model") {
      ChangeableModel* mModel = createModel(NULL, 1.0f);
      std::string mName = mNode->getAttribute("name");
      IArgumentSource* mArgumentSource = new ArgumentSourceCustom<ChangeableModel>(mModel);
      runtimeContext->add(mModel, mName, mNode);
      runtimeContext->registerArgument("Model", mName, mArgumentSource);
    }
  }
}

ChangeableModel* ChangeableModelFactory::createModel(Vertex* location, float scale) {
  ChangeableModel* mChangeableModel = new ChangeableModel();
  cResources.push_back(mChangeableModel);
  return mChangeableModel;
}

void ChangeableModelFactory::destroyModel(I3DModel* model) {
  delete model;
}

void ChangeableModelFactory::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cResources.size(); i++) {
    DOMNodeWriter* mResourceNode = node->addBranch("Model");
    std::string mResourceName = resourceLocator->getPath(cResources[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mResourceNode->addAttribute("name", mResourceName);
  }
}

void ChangeableModelFactory::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  // Nothing to do
}

extern "C" IPlugin* create() {
  return new ChangeableModelFactory();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

/*
 * Copyright 2009 Martin Brentnall
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
#include "SpindizzyCraftGyroscopeModelFactory.h"

I3DModel* SpindizzyCraftGyroscopeModelFactory::createModel(Vertex* location, float scale) {
  return new SpindizzyCraftGyroscopeModel(location);
}

void SpindizzyCraftGyroscopeModelFactory::destroyModel(I3DModel* model) {
  delete model;
}

void SpindizzyCraftGyroscopeModelFactory::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
// TODO: SAVE COLOURS!!!  cProperties->save(node);
}

void SpindizzyCraftGyroscopeModelFactory::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
/* TODO LOAD COLOURS!!!!     for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    ITexture* mTexture = resourceAccessor->getTexture(mNode);
  }*/
}

extern "C" IPlugin* create() {
  return new SingleResourceTypeModule<SpindizzyCraftGyroscopeModelFactory>("ModelType");
}

extern "C" void destroy(IPlugin* ballModel) {
  delete ballModel;
}

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
#include "ResourceLayerHUD.h"

ResourceLayerHUD::ResourceLayerHUD(IElementRelationManager* manager, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cManager = manager;
}

void ResourceLayerHUD::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  // Nothing to do.
}

ILayer* ResourceLayerHUD::getLayer(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resourceAccessor, bool editing, bool asTemplate) {
  LayerHUD* mLayerHUD = new LayerHUD(node, cache, resourceAccessor, this, cManager);
  //mLayerRegistry->add(mLayerHUD)
  return mLayerHUD;
}

std::string ResourceLayerHUD::getInstanceName(ILayer* layer) {
  return "<default>"; // TODO: Support instanced HUD's?
}

void ResourceLayerHUD::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator) {
  // Nothing to do.
}

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
#include "ResourceModelSpindizzyCraftBall.h"

ResourceModelSpindizzyCraftBall::ResourceModelSpindizzyCraftBall(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cCamera = NULL;
}

I3DModel* ResourceModelSpindizzyCraftBall::createModel(Vertex* location, float scale) {
  return new ModelSpindizzyCraftBall(location, cCamera);
}

void ResourceModelSpindizzyCraftBall::destroyModel(I3DModel* ballModel) {
  delete ballModel;
}

void ResourceModelSpindizzyCraftBall::updateIcon(unsigned int milliseconds) {
  // TODO
}

void ResourceModelSpindizzyCraftBall::renderIcon() {
  // TODO: Sample
}

void ResourceModelSpindizzyCraftBall::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  std::string mCameraPath = resourceLocator->getPath(cCamera);
  node->addAttribute("camera", mCameraPath);
}

void ResourceModelSpindizzyCraftBall::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mCameraPath = node->getAttribute("camera");
  cCamera = resourceAccessor->getCamera(mCameraPath);
}

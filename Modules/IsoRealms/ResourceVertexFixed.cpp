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
#include "ResourceVertexFixed.h"

ResourceVertexFixed::ResourceVertexFixed(IDummyModule* dummy, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  IArgumentValue* mArgumentValue = new ArgumentValueCustomType<ResourceVertexFixed>(this);
  resourceRegistry->add(mArgumentValue, "Vertex", node->getAttribute("name"));
}

IVertex* ResourceVertexFixed::getVertex() {
  return &cVertex;
}

double ResourceVertexFixed::getX() {
  return cVertex.x;
}

double ResourceVertexFixed::getY() {
  return cVertex.y;
}

double ResourceVertexFixed::getZ() {
  return cVertex.z;
}

void ResourceVertexFixed::setX(double x) {
  cVertex.x = x;
}

void ResourceVertexFixed::setY(double y) {
  cVertex.y = y;
}

void ResourceVertexFixed::setZ(double z) {
  cVertex.z = z;
}

void ResourceVertexFixed::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
}

void ResourceVertexFixed::save(DOMNodeWriter* node, IResourceLocator* resources) {
}

void ResourceVertexFixed::saveCache(DOMNodeWriter* cache) {
  // Nothing to do
}

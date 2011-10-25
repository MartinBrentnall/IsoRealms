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
#include "BlockProperties.h"

BlockProperties::BlockProperties(ISpindizzyTextureSet** textureSet) {
  cTextureSet = textureSet;
}

ISpindizzyTexture* BlockProperties::getTopSurfaceTexture() {
  return (*cTextureSet)->getTexture(cTopSurfaceTexture);
}

ISpindizzyTexture* BlockProperties::getBottomSurfaceTexture() {
  return (*cTextureSet)->getTexture(cBottomSurfaceTexture);
}

ISpindizzyTexture* BlockProperties::getNorthSurfaceTexture() {
  return (*cTextureSet)->getTexture(cNorthSurfaceTexture);
}

ISpindizzyTexture* BlockProperties::getEastSurfaceTexture() {
  return (*cTextureSet)->getTexture(cEastSurfaceTexture);
}

ISpindizzyTexture* BlockProperties::getSouthSurfaceTexture() {
  return (*cTextureSet)->getTexture(cSouthSurfaceTexture);
}

ISpindizzyTexture* BlockProperties::getWestSurfaceTexture() {
  return (*cTextureSet)->getTexture(cWestSurfaceTexture);
}

void BlockProperties::setTopSurfaceTexture(const std::string& name) {
  cTopSurfaceTexture = name;
}

void BlockProperties::setBottomSurfaceTexture(const std::string& name) {
  cBottomSurfaceTexture = name;
}

void BlockProperties::setNorthSurfaceTexture(const std::string& name) {
  cNorthSurfaceTexture = name;
}

void BlockProperties::setEastSurfaceTexture(const std::string& name) {
  cEastSurfaceTexture = name;
}

void BlockProperties::setSouthSurfaceTexture(const std::string& name) {
  cSouthSurfaceTexture = name;
}

void BlockProperties::setWestSurfaceTexture(const std::string& name) {
  cWestSurfaceTexture = name;
}

void BlockProperties::saveProperty(DOMNodeWriter* node, const std::string& name, const std::string& texture) {
  if (texture != "") {
    DOMNodeWriter* mNode = node->addBranch(name); // TODO: Dealloc
    mNode->addText(texture);
  }
}

void BlockProperties::save(DOMNodeWriter* node) {
  saveProperty(node, "TopSurfaceTexture",    cTopSurfaceTexture);
  saveProperty(node, "BottomSurfaceTexture", cBottomSurfaceTexture);
  saveProperty(node, "NorthSurfaceTexture",  cNorthSurfaceTexture);
  saveProperty(node, "EastSurfaceTexture",   cEastSurfaceTexture);
  saveProperty(node, "SouthSurfaceTexture",  cSouthSurfaceTexture);
  saveProperty(node, "WestSurfaceTexture",   cWestSurfaceTexture);
}

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

BlockProperties::BlockProperties(ITextureSet** textureSet) {
  cTextureSet = textureSet;
}

ITexture* BlockProperties::getTopSurfaceTexture() {
  return (*cTextureSet)->getTexture(cTopSurfaceTexture);
}

ITexture* BlockProperties::getBottomSurfaceTexture() {
  return (*cTextureSet)->getTexture(cBottomSurfaceTexture);
}

ITexture* BlockProperties::getNorthSurfaceTextureTop() {
  return (*cTextureSet)->getTexture(cNorthSurfaceTextureTop);
}

ITexture* BlockProperties::getEastSurfaceTextureTop() {
  return (*cTextureSet)->getTexture(cEastSurfaceTextureTop);
}

ITexture* BlockProperties::getSouthSurfaceTextureTop() {
  return (*cTextureSet)->getTexture(cSouthSurfaceTextureTop);
}

ITexture* BlockProperties::getWestSurfaceTextureTop() {
  return (*cTextureSet)->getTexture(cWestSurfaceTextureTop);
}

ITexture* BlockProperties::getNorthSurfaceTextureBottom() {
  return (*cTextureSet)->getTexture(cNorthSurfaceTextureBottom);
}

ITexture* BlockProperties::getEastSurfaceTextureBottom() {
  return (*cTextureSet)->getTexture(cEastSurfaceTextureBottom);
}

ITexture* BlockProperties::getSouthSurfaceTextureBottom() {
  return (*cTextureSet)->getTexture(cSouthSurfaceTextureBottom);
}

ITexture* BlockProperties::getWestSurfaceTextureBottom() {
  return (*cTextureSet)->getTexture(cWestSurfaceTextureBottom);
}

void BlockProperties::setTopSurfaceTexture(const std::string& name) {
  cTopSurfaceTexture = name;
}

void BlockProperties::setBottomSurfaceTexture(const std::string& name) {
  cBottomSurfaceTexture = name;
}

void BlockProperties::setNorthSurfaceTextureTop(const std::string& name) {
  cNorthSurfaceTextureTop = name;
}

void BlockProperties::setEastSurfaceTextureTop(const std::string& name) {
  cEastSurfaceTextureTop = name;
}

void BlockProperties::setSouthSurfaceTextureTop(const std::string& name) {
  cSouthSurfaceTextureTop = name;
}

void BlockProperties::setWestSurfaceTextureTop(const std::string& name) {
  cWestSurfaceTextureTop = name;
}

void BlockProperties::setNorthSurfaceTextureBottom(const std::string& name) {
  cNorthSurfaceTextureBottom = name;
}

void BlockProperties::setEastSurfaceTextureBottom(const std::string& name) {
  cEastSurfaceTextureBottom = name;
}

void BlockProperties::setSouthSurfaceTextureBottom(const std::string& name) {
  cSouthSurfaceTextureBottom = name;
}

void BlockProperties::setWestSurfaceTextureBottom(const std::string& name) {
  cWestSurfaceTextureBottom = name;
}

void BlockProperties::saveProperty(DOMNodeWriter* node, const std::string& name, const std::string& texture) {
  if (texture != "") {
    DOMNodeWriter* mNode = node->addBranch(name); // TODO: Dealloc
    mNode->addText(texture);
  }
}

void BlockProperties::save(DOMNodeWriter* node) {
  saveProperty(node, "TopSurfaceTexture",         cTopSurfaceTexture);
  saveProperty(node, "BottomSurfaceTexture",      cBottomSurfaceTexture);
  saveProperty(node, "NorthSurfaceTextureTop",    cNorthSurfaceTextureTop);
  saveProperty(node, "EastSurfaceTextureTop",     cEastSurfaceTextureTop);
  saveProperty(node, "SouthSurfaceTextureTop",    cSouthSurfaceTextureTop);
  saveProperty(node, "WestSurfaceTextureTop",     cWestSurfaceTextureTop);
  saveProperty(node, "NorthSurfaceTextureBottom", cNorthSurfaceTextureBottom);
  saveProperty(node, "EastSurfaceTextureBottom",  cEastSurfaceTextureBottom);
  saveProperty(node, "SouthSurfaceTextureBottom", cSouthSurfaceTextureBottom);
  saveProperty(node, "WestSurfaceTextureBottom",  cWestSurfaceTextureBottom);
}

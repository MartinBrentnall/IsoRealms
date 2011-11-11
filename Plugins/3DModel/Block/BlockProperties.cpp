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

ITexture* BlockProperties::getTopSurfaceTexture() {
  return cTopSurfaceTexture;
}

ITexture* BlockProperties::getBottomSurfaceTexture() {
  return cBottomSurfaceTexture;
}

ITexture* BlockProperties::getNorthSurfaceTextureTop() {
  return cNorthSurfaceTextureTop;
}

ITexture* BlockProperties::getEastSurfaceTextureTop() {
  return cEastSurfaceTextureTop;
}

ITexture* BlockProperties::getSouthSurfaceTextureTop() {
  return cSouthSurfaceTextureTop;
}

ITexture* BlockProperties::getWestSurfaceTextureTop() {
  return cWestSurfaceTextureTop;
}

ITexture* BlockProperties::getNorthSurfaceTextureBottom() {
  return cNorthSurfaceTextureBottom;
}

ITexture* BlockProperties::getEastSurfaceTextureBottom() {
  return cEastSurfaceTextureBottom;
}

ITexture* BlockProperties::getSouthSurfaceTextureBottom() {
  return cSouthSurfaceTextureBottom;
}

ITexture* BlockProperties::getWestSurfaceTextureBottom() {
  return cWestSurfaceTextureBottom;
}

void BlockProperties::setTopSurfaceTexture(ITexture* texture) {
  cTopSurfaceTexture = texture;
}

void BlockProperties::setBottomSurfaceTexture(ITexture* texture) {
  cBottomSurfaceTexture = texture;
}

void BlockProperties::setNorthSurfaceTextureTop(ITexture* texture) {
  cNorthSurfaceTextureTop = texture;
}

void BlockProperties::setEastSurfaceTextureTop(ITexture* texture) {
  cEastSurfaceTextureTop = texture;
}

void BlockProperties::setSouthSurfaceTextureTop(ITexture* texture) {
  cSouthSurfaceTextureTop = texture;
}

void BlockProperties::setWestSurfaceTextureTop(ITexture* texture) {
  cWestSurfaceTextureTop = texture;
}

void BlockProperties::setNorthSurfaceTextureBottom(ITexture* texture) {
  cNorthSurfaceTextureBottom = texture;
}

void BlockProperties::setEastSurfaceTextureBottom(ITexture* texture) {
  cEastSurfaceTextureBottom = texture;
}

void BlockProperties::setSouthSurfaceTextureBottom(ITexture* texture) {
  cSouthSurfaceTextureBottom = texture;
}

void BlockProperties::setWestSurfaceTextureBottom(ITexture* texture) {
  cWestSurfaceTextureBottom = texture;
}

void BlockProperties::save(DOMNodeWriter* node) {
  cTopSurfaceTexture->save(node, "TopSurfaceTexture");
  cBottomSurfaceTexture->save(node, "BottomSurfaceTexture");
  cNorthSurfaceTextureTop->save(node, "NorthSurfaceTextureTop");
  cEastSurfaceTextureTop->save(node, "EastSurfaceTextureTop");
  cSouthSurfaceTextureTop->save(node, "SouthSurfaceTextureTop");
  cWestSurfaceTextureTop->save(node, "WestSurfaceTextureTop");
  cNorthSurfaceTextureBottom->save(node, "NorthSurfaceTextureBottom");
  cEastSurfaceTextureBottom->save(node, "EastSurfaceTextureBottom");
  cSouthSurfaceTextureBottom->save(node, "SouthSurfaceTextureBottom");
  cWestSurfaceTextureBottom->save(node, "WestSurfaceTextureBottom");
}

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
#include "RuntimeContext.h"

RuntimeContext::RuntimeContext(IMap* map, RegistryProxy<ICommand, CommandProxy>* commandGateway, RegistryProxy<IColour, ColourProxy>* colourGateway, RegistryProxy<ITexture, TextureProxy>* textureGateway, bool editing, IScriptSource* scriptSource, IColourSource* colourSource, ITextureSource* textureSource) {
  cMap = map;
  cCommandGateway = commandGateway;
  cColourGateway = colourGateway;
  cTextureGateway = textureGateway;
  cEditing = editing;
  cScriptSource = scriptSource;
  cColourSource = colourSource;
  cTextureSource = textureSource;
}

IMap* RuntimeContext::getMap() {
  return cMap;
}

bool RuntimeContext::isEditing() {
  return cEditing;
}

std::string RuntimeContext::getLocation(ICommand* command) {
  return cCommandGateway->getLocation(command);
}

void RuntimeContext::add(ICommand* command, const std::string& name) {
  cCommandGateway->add(command, name);
}

void RuntimeContext::add(IColour* colour, const std::string& name) {
  cColourGateway->add(colour, name);
}

void RuntimeContext::add(ITexture* texture, const std::string& name) {
  cTextureGateway->add(texture, name);
}

void RuntimeContext::remove(ICommand* command) {
  cCommandGateway->remove(command);
}

Script* RuntimeContext::getScript(DOMNodeWrapper* node) {
  return cScriptSource->getScript(node);
}

IColour* RuntimeContext::getColour(DOMNodeWrapper* node) {
  return cColourSource->getColour(node);
}

ITexture* RuntimeContext::getTexture(DOMNodeWrapper* node) {
  return cTextureSource->getTexture(node);
}

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

RuntimeContext::RuntimeContext(IResources* resources, std::vector<std::string> entityPath) {
  cResources = resources;
  cEntityPath = entityPath;
}

IProject* RuntimeContext::getProject() {
  return cResources->getProject();
}

bool RuntimeContext::isEditing() {
  return cResources->isEditing();
}

std::string RuntimeContext::getLocation(ICommand* command) {
  std::cout << "TODO: Implement \"getLocation()\" for command resources" << std::endl;
//  return getDirectory(cScriptRegistry, cEntityPath)->getLocation(command);
  return "";
}

void RuntimeContext::add(ICommand* command, const std::string& name) {
  cResources->add(command, cEntityPath, name);
}

void RuntimeContext::add(IColour* colour, const std::string& name) {
  cResources->add(colour, cEntityPath, name);
}

void RuntimeContext::add(ITexture* texture, const std::string& name) {
  cResources->add(texture, cEntityPath, name);
}

void RuntimeContext::add(I3DModelFactory* modelFactory, const std::string& name) {
  cResources->add(modelFactory, cEntityPath, name);
}

void RuntimeContext::add(ISound* sound, const std::string& name) {
  cResources->add(sound, cEntityPath, name);
}

void RuntimeContext::add(IInteger* value, const std::string& name) {
  cResources->add(value, cEntityPath, name);
}

void RuntimeContext::remove(ICommand* command) {
  std::cout << "TODO: Implement \"remove()\" for command resources" << std::endl;
  // getDirectory(cScriptRegistry, cEntityPath)->remove(command);
}

IScript* RuntimeContext::getLuaScript(DOMNodeWrapper* name) {
  return cResources->getLuaScript(name);
}

Script* RuntimeContext::getScript(DOMNodeWrapper* node) {
  return cResources->getScript(node);
}

IColour* RuntimeContext::getColour(DOMNodeWrapper* node) {
  return cResources->getColour(node);
}

ITexture* RuntimeContext::getTexture(DOMNodeWrapper* node) {
  return cResources->getTexture(node);
}

I3DModel* RuntimeContext::getModel(DOMNodeWrapper* node, Vertex* location) {
  return cResources->getModel(node, location);
}

I3DModel* RuntimeContext::getModel(const std::string& name, Vertex* location) {
  return cResources->getModel(name, location);
}

ISound* RuntimeContext::getSound(DOMNodeWrapper* node) {
  return cResources->getSound(node);
}

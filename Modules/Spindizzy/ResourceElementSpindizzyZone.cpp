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
#include "ResourceElementSpindizzyZone.h"

ResourceElementSpindizzyZone::ResourceElementSpindizzyZone(ISpindizzyZoneModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cModuleInterface = module;
}

ISpindizzyZoneModule* ResourceElementSpindizzyZone::getSpindizzyZoneInterface() {
  return cModuleInterface;
}

bool ResourceElementSpindizzyZone::isOverview() {
  return cModuleInterface->isOverview();
}

IColour* ResourceElementSpindizzyZone::getVisitedColour() {
  return cVisitedColour;
}

IColour* ResourceElementSpindizzyZone::getUnvisitedColour() {
  return cUnvisitedColour;
}

IBoundaries* ResourceElementSpindizzyZone::getBoundaries() {
  return cBoundaries;
}

void ResourceElementSpindizzyZone::setZoneArgument(ElementSpindizzyZone* zone) {
  cModuleInterface->setArgumentValue(zone);
}

void ResourceElementSpindizzyZone::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  std::string mPathVisitedColour = node->getAttribute("visitedColour");
  std::string mPathUnvisitedColour = node->getAttribute("unvisitedColour");
  std::string mPathFlagModel = node->getAttribute("flagModel");
  std::string mPathBoundaries = node->getAttribute("boundaries");
  cVisitedColour = resources->getColour(mPathVisitedColour);
  cUnvisitedColour = resources->getColour(mPathUnvisitedColour);
  cFlagModelType = resources->getModelType(mPathFlagModel);
  cBoundaries = resources->getBoundaries(mPathBoundaries);
  cBoundaries->registerArgumentValuesBoundaries(this);
}

void ResourceElementSpindizzyZone::save(DOMNodeWriter* node, IResourceLocator* locator) {
}

void ResourceElementSpindizzyZone::loadElement(DOMNodeWrapper* node, BlockLocation* location, IElementContainer* container, IResourceAccessor* resources) {
  BlockLocation* mLocation = new BlockLocation();
  mLocation->setRelative(node, *location);
  ElementSpindizzyZone* mSpindizzyZone = new ElementSpindizzyZone(this, node, mLocation, resources, cFlagModelType, container);
  cContent.push_back(mSpindizzyZone);
  ElementHandlerZone* mZoneElementHandler = cModuleInterface->getZoneElementHandler(container);
  mZoneElementHandler->addElement(mSpindizzyZone);
}

void ResourceElementSpindizzyZone::configureElement() {
}

void ResourceElementSpindizzyZone::setEditingContext(BlockLocation*, IComponentContainer*) {
}

void ResourceElementSpindizzyZone::renderEditingPreview() {
}

bool ResourceElementSpindizzyZone::input(SDL_Event& event) {
  // TODO: This is apparently for editor input!
  return false;
}

void ResourceElementSpindizzyZone::updateIcon(unsigned int milliseconds) {
  // TODO: Render some kind of icon
}

void ResourceElementSpindizzyZone::renderIcon() {
  // TODO: Render some kind of icon
}

void ResourceElementSpindizzyZone::setDirty(IElement* element) {
}

void ResourceElementSpindizzyZone::destroy(IElement* element) {
  delete element;
}

ResourceElementSpindizzyZone::~ResourceElementSpindizzyZone() {
  // TODO: Remove zone content
}

IArgumentValue* ResourceElementSpindizzyZone::getArgumentValue(const std::string& argument) {
  return cModuleInterface->getArgumentValue(argument);
}

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
#include "ResourceElementHUDString.h"

ResourceElementHUDString::ResourceElementHUDString(IElementRelationManager* manager, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) : ResourceElementHUDAbstract(manager) {
}

IElement* ResourceElementHUDString::createHUDElement(DOMNodeWrapper* node, DOMNodeWrapper* cache, BlockLocation* location, IResourceAccessor* resources, bool asTemplate, HUDComponentPosition* elementTransformer) {
  return new ElementHUDString(this, elementTransformer);
}

void ResourceElementHUDString::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  cText      = resources->getString(node->getAttribute("value"));
  cFont      = resources->getFont(  node->getAttribute("font"));
  std::string mAlignment = node->getAttribute("alignment");
  cAlignment = mAlignment == "Left"  ? IFont::LEFT
             : mAlignment == "Right" ? IFont::RIGHT
             :                         IFont::CENTER;
}

void ResourceElementHUDString::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator) {
  node->addAttribute("font",  resourceLocator->getPath(cFont));
  node->addAttribute("value", resourceLocator->getPath(cText));
}

void ResourceElementHUDString::configureElement() {
  // TODO:
}

void ResourceElementHUDString::destroy(IElement* element) {
  // TODO
}

void ResourceElementHUDString::removeElement(IElement* element) {
  // TODO
}

Vertex* ResourceElementHUDString::editorCursorStopped(Vertex* location) {
  return nullptr;
}

bool ResourceElementHUDString::inputEdit(SDL_Event& event, ILayerEditingContext* editingContext) {
  return false;// TODO
}

void ResourceElementHUDString::renderEditingPreview(Vertex& location) {
  // TODO
}

void ResourceElementHUDString::updateEditingPreview(unsigned int milliseconds) {
  // TODO
}

void ResourceElementHUDString::renderIcon() {
  // TODO
}

void ResourceElementHUDString::updateIcon(unsigned int) {
  // TODO
}

IElementType* ResourceElementHUDString::getElementType() {
  return this;
}

IFont* ResourceElementHUDString::getFont() {
  return cFont;
}

std::string ResourceElementHUDString::getValue() {
  return cText->getValue();
}

IFont::Alignment ResourceElementHUDString::getAlignment() {
  return cAlignment;
}

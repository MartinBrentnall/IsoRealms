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
  cBoundaries = new Boundaries();
  cBoundaries->registerArgumentValuesBoundaries(this);
  resourceRegistry->add(cBoundaries, node->getAttribute("name"));
  cStartLocation = nullptr;
  cEditingZone = nullptr;
  cModuleInterface->addSpindizzyZoneThemeListener(this);
}

ElementSpindizzyZone* ResourceElementSpindizzyZone::getElement(IElement* element) {
  for (ElementSpindizzyZone* mElement : cContent) {
    if (mElement == element) {
      return mElement;
    }
  }
  return nullptr;
}

void ResourceElementSpindizzyZone::applyDefaultTheme() {
  cModuleInterface->applyDefaultTheme();
}

void ResourceElementSpindizzyZone::spindizzyZoneThemeSelected(ISpindizzyZoneTheme* spindizzyZoneTheme) {
  for (ElementSpindizzyZone* mFocusedZone : cFocusedZones) {
    mFocusedZone->setTheme(spindizzyZoneTheme);
  }
}

void ResourceElementSpindizzyZone::spindizzyZoneThemeEdited(ISpindizzyZoneTheme* spindizzyZoneTheme) {
  for (ElementSpindizzyZone* mZone : cContent) {
    mZone->spindizzyZoneThemeEdited(spindizzyZoneTheme);
  }
}

void ResourceElementSpindizzyZone::zoneGainedFocus(ElementSpindizzyZone* zone) {
  cFocusedZones.insert(zone);
}

void ResourceElementSpindizzyZone::zoneLostFocus(ElementSpindizzyZone* zone) {
  cFocusedZones.erase(zone);
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

void ResourceElementSpindizzyZone::setDefaultTheme(ISpindizzyZoneTheme* theme) {
  cModuleInterface->setTheme(theme);
}

void ResourceElementSpindizzyZone::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  std::string mPathVisitedColour = node->getAttribute("visitedColour");
  std::string mPathUnvisitedColour = node->getAttribute("unvisitedColour");
  std::string mPathFlagModel = node->getAttribute("flagModel");
  cVisitedColour = resources->getColour(mPathVisitedColour);
  cUnvisitedColour = resources->getColour(mPathUnvisitedColour);
  cFlagModelType = resources->getModelType(mPathFlagModel);
  BlockLocation mZoneLocation(0, 0, 0);
  BlockArea* mZoneArea = new BlockArea(mZoneLocation, mZoneLocation);
  cSampleZone = new ElementSpindizzyZone(this, mZoneArea);
}

Icon<IElementType>* ResourceElementSpindizzyZone::getResourceIcon(IResourceBrowser<IElementType>* browser) {
  return nullptr;
}

void ResourceElementSpindizzyZone::save(DOMNodeWriter* node, IResourceLocator* locator) {
  node->addAttribute("visitedColour", locator->getPath(cVisitedColour));
  node->addAttribute("unvisitedColour", locator->getPath(cUnvisitedColour));
  node->addAttribute("flagModel", locator->getPath(cFlagModelType));
}

void ResourceElementSpindizzyZone::saveCache(DOMNodeWriter* cache) {
  // Nothing to do
}

void ResourceElementSpindizzyZone::loadElement(DOMNodeWrapper* node, DOMNodeWrapper* cache, BlockLocation* location, IElementContainer* container, IResourceAccessor* resources, bool asTemplate) {
  if (!asTemplate) {
    BlockLocation* mLocation = new BlockLocation();
    mLocation->setRelative(node, *location);
    ElementSpindizzyZone* mSpindizzyZone = new ElementSpindizzyZone(this, node, cache, mLocation, resources, cFlagModelType, container);
    cContent.push_back(mSpindizzyZone);
    ElementHandlerZone* mZoneElementHandler = cModuleInterface->getZoneElementHandler(container);
    mZoneElementHandler->addElement(mSpindizzyZone);
    container->updateElement(mZoneElementHandler);
  }
}

void ResourceElementSpindizzyZone::configureElement() {
}

void ResourceElementSpindizzyZone::setEditingContext(BlockLocation*, IComponentContainer*) {
}

void ResourceElementSpindizzyZone::renderEditingPreview(Vertex& location) {
  if (cEditingZone == nullptr) {
    BlockLocation mZoneLocation(0, 0, 0);
    BlockArea* mZoneArea = new BlockArea(mZoneLocation, mZoneLocation);
    cEditingZone = new ElementSpindizzyZone(this, mZoneArea);
    cStartLocation = nullptr;
  }
  if (cStartLocation != nullptr) {
    bool mValidZone = !intersectsExistingZone(cEditingZone);
    cEditingZone->renderPreview(*cStartLocation, location, mValidZone);
  } else {
    glTranslatef(location.x, location.y, location.z);
    BlockLocation mCurrentBlockLocation(location.x, location.y, location.z * 2.0);
    bool mValidZone = !intersectsExistingZone(mCurrentBlockLocation);
    cEditingZone->renderEditing(mValidZone);
  }
}

void ResourceElementSpindizzyZone::updateEditingPreview(unsigned int milliseconds) {
  // TODO
}

bool ResourceElementSpindizzyZone::keyDown(SDLKey& key, ILayerEditingContext* editingContext) {
  switch (key) {
    case SDLK_SPACE: {
      IElementContainer* mContainer = editingContext->getElementContainer();
      ElementHandlerZone* mHandler = cModuleInterface->getZoneElementHandler(mContainer);
      if (cStartLocation == nullptr) {
        Vertex* mLocation = editingContext->getLocation();
        BlockLocation mStartLocation(mLocation->x, mLocation->y, mLocation->z * 2.0);
        if (!intersectsExistingZone(mStartLocation)) {
          if (cEditingZone != nullptr) {
            delete cEditingZone;
          }
          BlockArea* mZoneArea = new BlockArea(mStartLocation, mStartLocation);
          ISpindizzyZoneTheme* mSelectedZoneTheme = cModuleInterface->getSelectedZoneTheme();
          cEditingZone = new ElementSpindizzyZone(this, mZoneArea, mContainer, mSelectedZoneTheme);
          cStartLocation = new Vertex(*mLocation);
        }
      } else {
        if (!intersectsExistingZone(cEditingZone)) { 
          mHandler->addElement(cEditingZone);
          cContent.push_back(cEditingZone);
          cEditingZone->updateFlagModel(cFlagModelType);
          mContainer->updateElement(mHandler);
          mContainer->setDirty(mHandler);
          cStartLocation = nullptr;
          BlockLocation mIdentityBlockLocation(0, 0, 0);
          BlockArea* mZoneArea = new BlockArea(mIdentityBlockLocation, mIdentityBlockLocation);
          cEditingZone = new ElementSpindizzyZone(this, mZoneArea);
          editingContext->staticChanged();
        }
      }
    }

    default: {
      return false;
    }
  }
  return false;
}

bool ResourceElementSpindizzyZone::intersectsExistingZone(ElementSpindizzyZone* zone) {
  for (ElementSpindizzyZone* mZone : cContent) {
    if (mZone->intersects(zone)) {
      return true;
    }
  }
  return false;
}

bool ResourceElementSpindizzyZone::intersectsExistingZone(BlockLocation& location) {
  for (ElementSpindizzyZone* mZone : cContent) {
    if (mZone->intersects(location)) {
      return true;
    }
  }
  return false;
}

bool ResourceElementSpindizzyZone::inputEdit(SDL_Event& event, ILayerEditingContext* editingContext) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      if (keyDown(event.key.keysym.sym, editingContext)) {
        delete cEditingZone;
        cEditingZone = nullptr;
        return true;
      }
    }
  }
  return false;
}

void ResourceElementSpindizzyZone::updateIcon(unsigned int milliseconds) {
  // TODO: Render some kind of icon
}

void ResourceElementSpindizzyZone::renderIcon() {
  glTranslatef(0.0f, 0.3f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  // TODO: Scale the icon
  glScalef(1.3f, 1.3f, 1.3f);
  glColor3f(1.0f, 1.0f, 1.0f);
  cSampleZone->renderEditing();
}

void ResourceElementSpindizzyZone::setDirty(IElement* element) {
}

void ResourceElementSpindizzyZone::destroy(IElement* element) {
  delete element;
}

void ResourceElementSpindizzyZone::removeElement(IElement* element) {
  ElementSpindizzyZone* mZone = getElement(element);
  IElementContainer* mContainer = mZone->getElementContainer();
  ElementHandlerZone* mHandler = cModuleInterface->getZoneElementHandler(mContainer);
  mHandler->removeElement(mZone);
  if (mHandler->isEmpty()) {
    mContainer->removeElement(mHandler);
    cModuleInterface->removeElementHandlerZone(mContainer);
  }
}

Vertex* ResourceElementSpindizzyZone::editorCursorStopped(Vertex* location) {
  Vertex* mGridLocation = new Vertex();
  mGridLocation->x = std::round(location->x);
  mGridLocation->y = std::round(location->y);
  mGridLocation->z = std::round(location->z * 2.0) * 0.5;
  return mGridLocation;
}

IArgumentValue* ResourceElementSpindizzyZone::getArgumentValue(const std::string& argument) {
  return cModuleInterface->getArgumentValue(argument);
}

ResourceElementSpindizzyZone::~ResourceElementSpindizzyZone() {
  delete cSampleZone;
  for (unsigned int i = 0; i < cContent.size(); i++) {
    removeElement(cContent[i]);
    destroy(cContent[i]);
  }
}


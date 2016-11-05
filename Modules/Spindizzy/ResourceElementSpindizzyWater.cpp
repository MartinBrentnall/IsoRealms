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
#include "ResourceElementSpindizzyWater.h"

ResourceElementSpindizzyWater::ResourceElementSpindizzyWater(ISpindizzyBlockSet* elementSet, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cModuleInterface = elementSet;
  cStartWaterLocation = nullptr;
  cSampleWater = nullptr;
}

void ResourceElementSpindizzyWater::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mTexturePath = node->getAttribute("texture");
  cTexture = resourceAccessor->getTexture(mTexturePath);
}

void ResourceElementSpindizzyWater::configureElement() {
  // Nothing to do.
}

IElement* ResourceElementSpindizzyWater::getElement() {
  return nullptr;
}

void ResourceElementSpindizzyWater::loadElement(DOMNodeWrapper* node, BlockLocation* zoneLocation, IElementContainer* container, IResourceAccessor* resources, bool asTemplate) {
  if (!asTemplate) {
    BlockLocation mStartLocation;
    BlockLocation mEndLocation;
    mStartLocation.setRelative(node, *zoneLocation);
    mEndLocation.setRelative(node, mStartLocation, "width", "length", "height");
    mEndLocation.z++;
    ElementSpindizzyWater* mLoadedWater = new ElementSpindizzyWater(this, &mStartLocation, &mEndLocation, &cTexture, container);
    cContent.push_back(mLoadedWater);
    cModuleInterface->registerSurfaceProvider(mLoadedWater, false);
    cModuleInterface->setDirty();
    container->addElement(mLoadedWater);
  }
}

void ResourceElementSpindizzyWater::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
}

bool ResourceElementSpindizzyWater::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
//       if (cStartWaterLocation == nullptr) {
//         cStartWaterLocation = new BlockLocation(*cEditingLocation);
//       } else {
//         ElementSpindizzyWater* mNewBlock = new ElementSpindizzyWater(this, cStartWaterLocation, cEditingLocation, &cTexture, nullptr);
//         cContent.push_back(mNewBlock);
// //        addElement(mNewBlock);
//         delete cStartWaterLocation;
//         cStartWaterLocation = nullptr;
//       }
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

ElementSpindizzyWater* ResourceElementSpindizzyWater::getElement(IElement* element) {
  for (ElementSpindizzyWater* mElement : cContent) {
    if (mElement == element) {
      return mElement;
    }
  }
  return nullptr;
}

bool ResourceElementSpindizzyWater::inputEdit(SDL_Event& event, ILayerEditingContext* editingContext) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void ResourceElementSpindizzyWater::renderEditingPreview(Vertex& location) {
  glTranslatef(location.x, location.y, location.z);
  if (cStartWaterLocation != nullptr) {
//     float x  = (cEditingLocation->x > cStartWaterLocation->x ? cEditingLocation->x    : cStartWaterLocation->x) + IsoRealmsConstants::BLOCK_RADIUS;
//     float xs = (cEditingLocation->x > cStartWaterLocation->x ? cStartWaterLocation->x : cEditingLocation->x)    - IsoRealmsConstants::BLOCK_RADIUS;
//     float y  = (cEditingLocation->y > cStartWaterLocation->y ? cEditingLocation->y    : cStartWaterLocation->y) + IsoRealmsConstants::BLOCK_RADIUS;
//     float ys = (cEditingLocation->y > cStartWaterLocation->y ? cStartWaterLocation->y : cEditingLocation->y)    - IsoRealmsConstants::BLOCK_RADIUS;
//     float z  = (cEditingLocation->z > cStartWaterLocation->z ? cEditingLocation->z    : cStartWaterLocation->z) * IsoRealmsConstants::BLOCK_HEIGHT;
//   
//     glBindTexture(GL_TEXTURE_2D, 0);
//     glColor3f(1.0, 0.5f, 0.5f);
//     glBegin(GL_LINES);
//     glVertex3f(xs, ys, z);   glVertex3f(x,  ys, z);
//     glVertex3f(x,  ys, z);   glVertex3f(x,  y,  z);
//     glVertex3f(x,  y,  z);   glVertex3f(xs, y,  z);
//     glVertex3f(xs, y,  z);   glVertex3f(xs, ys, z);
//     glEnd();
  } else {
    cSampleWater->renderStatic();
  }
}

void ResourceElementSpindizzyWater::renderIcon() {
  glTranslatef(0.0f, 0.3f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  // TODO: Scale the icon
  glScalef(1.3f, 1.3f, 1.3f);
  glColor3f(1.0f, 1.0f, 1.0f);
  if (cSampleWater == nullptr) {
    BlockLocation mIdentityBlockLocation(0, 0, 0);
    cSampleWater = new ElementSpindizzyWater(this, &mIdentityBlockLocation, &mIdentityBlockLocation, &cTexture, nullptr);
    cSampleWater->createSampleSurfaces();
  }
  cSampleWater->renderStatic();
}

void ResourceElementSpindizzyWater::updateIcon(unsigned int) {
  // Nothing to do.
}

void ResourceElementSpindizzyWater::destroy(IElement* element) {
  delete element;
}

void ResourceElementSpindizzyWater::removeElement(IElement* element) {
  ElementSpindizzyWater* mWater = getElement(element);
  IElementContainer* mContainer = mWater->getElementContainer();
  mContainer->removeElement(mWater);
  cModuleInterface->unregisterSurfaceProvider(mWater);
}

Vertex* ResourceElementSpindizzyWater::editorCursorStopped(Vertex* location) {
  Vertex* mGridLocation = new Vertex();
  mGridLocation->x = std::round(location->x);
  mGridLocation->y = std::round(location->y);
  mGridLocation->z = std::round(location->z * 2.0) * 0.5;
  return mGridLocation;
}

ISpindizzyBlockSet* ResourceElementSpindizzyWater::getSpindizzyBlockInterface() {
  return cModuleInterface;
}

BlockTypeProperties* ResourceElementSpindizzyWater::getBlockTypeProperties() {
  return nullptr;
}

void ResourceElementSpindizzyWater::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("texture", resourceLocator->getPath(cTexture));
}

void ResourceElementSpindizzyWater::configureBlock(DOMNodeWrapper*) {
  // Nothing to do.
}

void ResourceElementSpindizzyWater::resourcePendingDestruction(ITexture* destroyee, ITexture* replacement) {
  if (cTexture == destroyee) {
    cTexture = replacement;
    cModuleInterface->staticChanged();
  }
}

void ResourceElementSpindizzyWater::resourceChanged(ITexture* texture) {
  if (cTexture == texture) {
    cModuleInterface->staticChanged();
  }
}

ResourceElementSpindizzyWater::~ResourceElementSpindizzyWater() {
  if (cStartWaterLocation != nullptr) {
    delete cStartWaterLocation;
  }
  delete cSampleWater;
  for (unsigned int i = 0; i < cContent.size(); i++) {
    removeElement(cContent[i]);
    destroy(cContent[i]);
  }  
}


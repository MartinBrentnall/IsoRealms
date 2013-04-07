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
#include "SpindizzyWaterType.h"

SpindizzyWaterType::SpindizzyWaterType(ISpindizzyBlockSet* elementSet) : ISpindizzyBlockType(elementSet) {
  cStartWaterLocation = NULL;
  cSampleWater = NULL;
}

void SpindizzyWaterType::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mTexturePath = node->getAttribute("texture");
  cTexture = resourceAccessor->getTexture(mTexturePath);
}

void SpindizzyWaterType::configureElement() {
  // Nothing to do.
}

IElement* SpindizzyWaterType::getElement() {
  return NULL;
}

IElement* SpindizzyWaterType::getElement(DOMNodeWrapper* node, BlockLocation* zoneLocation, IElementContainer* container) {
  BlockLocation mStartLocation;
  BlockLocation mEndLocation;
  mStartLocation.setRelative(node, *zoneLocation);
  mEndLocation.setRelative(node, mStartLocation, "width", "length", "height");
  mEndLocation.z++;
  SpindizzyWater* mLoadedWater = new SpindizzyWater(this, &mStartLocation, &mEndLocation, &cTexture);
  cContent.push_back(mLoadedWater);
  ISpindizzyBlockSet* mSurfaceProcessor = getElementSet();
  mSurfaceProcessor->registerSurfaceProvider(mLoadedWater);
  mSurfaceProcessor->setDirty();
  return mLoadedWater;
}

IElementHandler* SpindizzyWaterType::getElementHandler() {
  ISpindizzyBlockSet* mBlockSet = getElementSet();
  return mBlockSet->createHandler();
}

void SpindizzyWaterType::unregisterSurfaces(ISurfaceProcessor* surfaceProcessor) {
  for (unsigned int i = 0; i < cContent.size(); i++) {
    surfaceProcessor->unregisterSurfaceProvider(cContent[i]);
  }
}

void SpindizzyWaterType::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cEditingLocation = editingLocation;
}

bool SpindizzyWaterType::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cStartWaterLocation == NULL) {
        cStartWaterLocation = new BlockLocation(*cEditingLocation);
      } else {
        SpindizzyWater* mNewBlock = new SpindizzyWater(this, cStartWaterLocation, cEditingLocation, &cTexture);
        cContent.push_back(mNewBlock);
        addElement(mNewBlock);
        delete cStartWaterLocation;
        cStartWaterLocation = NULL;
      }
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool SpindizzyWaterType::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void SpindizzyWaterType::renderEditingPreview() {
  if (cStartWaterLocation != NULL) {
    float x  = (cEditingLocation->x > cStartWaterLocation->x ? cEditingLocation->x    : cStartWaterLocation->x) + IsoRealmsConstants::BLOCK_RADIUS;
    float xs = (cEditingLocation->x > cStartWaterLocation->x ? cStartWaterLocation->x : cEditingLocation->x)    - IsoRealmsConstants::BLOCK_RADIUS;
    float y  = (cEditingLocation->y > cStartWaterLocation->y ? cEditingLocation->y    : cStartWaterLocation->y) + IsoRealmsConstants::BLOCK_RADIUS;
    float ys = (cEditingLocation->y > cStartWaterLocation->y ? cStartWaterLocation->y : cEditingLocation->y)    - IsoRealmsConstants::BLOCK_RADIUS;
    float z  = (cEditingLocation->z > cStartWaterLocation->z ? cEditingLocation->z    : cStartWaterLocation->z) * IsoRealmsConstants::BLOCK_HEIGHT;
  
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0, 0.5f, 0.5f);
    glBegin(GL_LINES);
    glVertex3f(xs, ys, z);   glVertex3f(x,  ys, z);
    glVertex3f(x,  ys, z);   glVertex3f(x,  y,  z);
    glVertex3f(x,  y,  z);   glVertex3f(xs, y,  z);
    glVertex3f(xs, y,  z);   glVertex3f(xs, ys, z);
    glEnd();
  }
}

void SpindizzyWaterType::renderIcon() {
  glTranslatef(0.0f, 0.3f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  // TODO: Scale the icon
  glScalef(1.3f, 1.3f, 1.3f);
  glColor3f(1.0f, 1.0f, 1.0f);
  if (cSampleWater == NULL) {
    BlockLocation mIdentityBlockLocation(0, 0, 0);
    cSampleWater = new SpindizzyWater(this, &mIdentityBlockLocation, &mIdentityBlockLocation, &cTexture);
    cSampleWater->createSampleSurfaces();
  }
  cSampleWater->renderStatic();
}

void SpindizzyWaterType::updateIcon(unsigned int) {
  // Nothing to do.
}

void SpindizzyWaterType::destroy(IElement* element) {
  delete element;
}

BlockTypeProperties* SpindizzyWaterType::getBlockTypeProperties() {
  return NULL;
}

void SpindizzyWaterType::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("texture", resourceLocator->getPath(cTexture));
}

void SpindizzyWaterType::configureBlock(DOMNodeWrapper*) {
  // Nothing to do.
}

void SpindizzyWaterType::resourcePendingDestruction(ITexture* destroyee, ITexture* replacement) {
  if (cTexture == destroyee) {
    cTexture = replacement;
    ISpindizzyBlockSet* mBlockSet = getElementSet();
    mBlockSet->staticChanged();
  }
}

void SpindizzyWaterType::resourceChanged(ITexture* texture) {
  if (cTexture == texture) {
    ISpindizzyBlockSet* mBlockSet = getElementSet();
    mBlockSet->staticChanged();
  }
}

SpindizzyWaterType::~SpindizzyWaterType() {
  if (cStartWaterLocation != NULL) {
    delete cStartWaterLocation;
  }
  delete cSampleWater;
  for (unsigned int i = 0; i < cContent.size(); i++) {
    removeElement(cContent[i]);
    delete cContent[i];
  }  
}


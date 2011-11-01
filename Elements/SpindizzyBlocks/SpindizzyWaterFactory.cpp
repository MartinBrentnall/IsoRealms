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
#include "SpindizzyWaterFactory.h"

SpindizzyWaterFactory::SpindizzyWaterFactory(ITextureSet** textureSet, ISpindizzyBlockSet* elementSet) : ISpindizzyBlockFactory(elementSet) {
  cStartWaterLocation = NULL;
  cSampleWater = NULL;
  cTextureSet = textureSet;
}

void SpindizzyWaterFactory::configureElement() {
  // Nothing to do.
}

IElement* SpindizzyWaterFactory::getElement(DOMNodeWrapper* node, BlockLocation* zoneLocation, IElementContainer* elementContainer) {
  BlockLocation mStartLocation;
  BlockLocation mEndLocation;
  // TODO: Should throw something if these are not specified!
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Location") {
      mStartLocation.setRelative(mNode, *zoneLocation);
    } else if (mValueAsString == "Size") {
      mEndLocation.setRelative(mNode, mStartLocation);
    }
  }
  mEndLocation.z++;
  SpindizzyWater* mLoadedWater = new SpindizzyWater(this, &mStartLocation, &mEndLocation, cTextureSet);
  cContent.push_back(mLoadedWater);
  return mLoadedWater;
}

void SpindizzyWaterFactory::signalAllElementsDirty() {
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->signalElementDirty();
  }
}

void SpindizzyWaterFactory::unregisterSurfaces(ISurfaceProcessor* surfaceProcessor) {
  for (unsigned int i = 0; i < cContent.size(); i++) {
    surfaceProcessor->unregisterSurfaceProvider(cContent[i]);
  }
}

void SpindizzyWaterFactory::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cEditingLocation = editingLocation;
}

bool SpindizzyWaterFactory::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cStartWaterLocation == NULL) {
        cStartWaterLocation = new BlockLocation(*cEditingLocation);
      } else {
        SpindizzyWater* mNewBlock = new SpindizzyWater(this, cStartWaterLocation, cEditingLocation, cTextureSet);
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

bool SpindizzyWaterFactory::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void SpindizzyWaterFactory::renderEditingPreview() {
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

void SpindizzyWaterFactory::renderIcon() {
  glTranslatef(0.0f, 0.3f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  // TODO: Scale the icon
  glScalef(1.3f, 1.3f, 1.3f);
  glColor3f(1.0f, 1.0f, 1.0f);
  if (cSampleWater == NULL) {
    BlockLocation mIdentityBlockLocation(0, 0, 0);
    cSampleWater = new SpindizzyWater(this, &mIdentityBlockLocation, &mIdentityBlockLocation, cTextureSet);
  }
  cSampleWater->renderStatic();
}

void SpindizzyWaterFactory::updateIcon(int) {
  // Nothing to do.
}

std::string SpindizzyWaterFactory::getName() {
  return "Water";
}

BlockTypeProperties* SpindizzyWaterFactory::getBlockTypeProperties() {
  return NULL;
}

void SpindizzyWaterFactory::save(DOMNodeWriter*) {
  // Nothing to do.
}

void SpindizzyWaterFactory::configureBlock(DOMNodeWrapper*, ICommandRegistry*) {
  // Nothing to do.
}

ITexture** SpindizzyWaterFactory::getSurfaceTexture() {
  return NULL; // TODO: Implement this
}

ITexture** SpindizzyWaterFactory::getSplitNETexture() {
  return NULL; // TODO: Implement this
}

ITexture** SpindizzyWaterFactory::getSplitNWTexture() {
  return NULL; // TODO: Implement this
}

ITexture** SpindizzyWaterFactory::getWestWallTexture() {
  return NULL; // TODO: Implement this
}

ITexture** SpindizzyWaterFactory::getEastWallTexture() {
  return NULL; // TODO: Implement this
}

ITexture** SpindizzyWaterFactory::getSouthWallTexture() {
  return NULL; // TODO: Implement this
}

ITexture** SpindizzyWaterFactory::getNorthWallTexture() {
  return NULL; // TODO: Implement this
}

SpindizzyWaterFactory::~SpindizzyWaterFactory() {
  if (cStartWaterLocation != NULL) {
    delete cStartWaterLocation;
  }
  delete cSampleWater;
  for (unsigned int i = 0; i < cContent.size(); i++) {
    removeElement(cContent[i]);
    delete cContent[i];
  }  
}


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
#include "ResourceElementSpindizzyBlock.h"

ResourceElementSpindizzyBlock::ResourceElementSpindizzyBlock(ISpindizzyBlockSet* elementSet, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cModuleInterface = elementSet;
  cConfigurationComponent = nullptr;
  cStartBlockLocation = nullptr;
  cBlockProperties = new SpindizzyBlockProperties();
  cSampleBlock = nullptr;
}

void ResourceElementSpindizzyBlock::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  cBlockTypeProperties.configure(node, resourceAccessor);
}

IElement* ResourceElementSpindizzyBlock::getElement() {
  return nullptr;
}

void ResourceElementSpindizzyBlock::loadElement(DOMNodeWrapper* node, BlockLocation* zoneLocation, IElementContainer* container, IResourceAccessor* resources) {
  BlockLocation mStartLocation;
  BlockLocation mEndLocation;
  // TODO: Should throw something if these are not specified!
  cBlockProperties->reset();
  mStartLocation.setRelative(node, *zoneLocation);
  mEndLocation.setRelative(node, mStartLocation, "width", "length", "height");
  std::vector<ConditionElement*> mElements = cModuleInterface->getConditionElements();
  cBlockProperties->setup(node, mElements);
  bool mAddition = mStartLocation.z <= mEndLocation.z;
  (mAddition ? mStartLocation.z : mEndLocation.z)++;
  mEndLocation.x--;
  mEndLocation.y--;
  bool mIndependent = node->getBooleanAttribute("independent");
  ElementSpindizzyBlock* mLoadedBlock = createBlock(&mStartLocation, &mEndLocation, cBlockProperties, mAddition, container);
  cContent.push_back(mLoadedBlock);
  if (mIndependent) {
    mLoadedBlock->createSampleSurfaces();
  } else {
    cModuleInterface->registerSurfaceProvider(mLoadedBlock);
    cModuleInterface->setDirty();
  }
  ElementHandlerSpindizzyBlock* mHandler = cModuleInterface->getElementHandlerSpindizzyBlock(container);  
  mHandler->addElement(mLoadedBlock);
}

// void ResourceElementSpindizzyBlock::unregisterSurfaces(ISurfaceProcessor* surfaceProcessor) {
//   for (unsigned int i = 0; i < cContent.size(); i++) {
//     surfaceProcessor->unregisterSurfaceProvider(cContent[i]);
//   }
// }

ISpindizzyBlockSet* ResourceElementSpindizzyBlock::getSpindizzyBlockInterface() {
  return cModuleInterface;
}

BlockTypeProperties* ResourceElementSpindizzyBlock::getBlockTypeProperties() {
  return &cBlockTypeProperties;
}

void ResourceElementSpindizzyBlock::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  cBlockTypeProperties.save(node, resourceLocator);
}

void ResourceElementSpindizzyBlock::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cComponentContainer = componentContainer;
  cEditingLocation = editingLocation;
}

bool ResourceElementSpindizzyBlock::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cStartBlockLocation == nullptr) {
        cStartBlockLocation = new BlockLocation(*cEditingLocation);
      } else {
        ElementSpindizzyBlock* mNewBlock = createBlock(cStartBlockLocation, cEditingLocation, cBlockProperties, true, nullptr);
        cContent.push_back(mNewBlock);
//        addElement(mNewBlock);
        delete cStartBlockLocation;
        cStartBlockLocation = nullptr;
      }
      return true;
    }

    case SDLK_DELETE: {
      if (cStartBlockLocation == nullptr) {
        cStartBlockLocation = new BlockLocation(*cEditingLocation);
      } else {
        ElementSpindizzyBlock* mNewBlock = createBlock(cStartBlockLocation, cEditingLocation, cBlockProperties, false, nullptr);
        cContent.push_back(mNewBlock);
//        addElement(mNewBlock);
        delete cStartBlockLocation;
        cStartBlockLocation = nullptr;
      }
    }

    default: {
      return false;
    }
  }
  return false;
}

void ResourceElementSpindizzyBlock::configureElement() {
  if (cConfigurationComponent == nullptr) {
    cConfigurationComponent = new SpindizzyBlockConfigurationComponent(cComponentContainer, this, cBlockProperties, this);
    cComponentContainer->addComponent(cConfigurationComponent);
  }
}

bool ResourceElementSpindizzyBlock::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void ResourceElementSpindizzyBlock::renderEditingPreview() {
  if (cStartBlockLocation != nullptr) {
    float mEast  = (cEditingLocation->x > cStartBlockLocation->x ? cEditingLocation->x    : cStartBlockLocation->x) + IsoRealmsConstants::BLOCK_RADIUS;
    float mWest  = (cEditingLocation->x > cStartBlockLocation->x ? cStartBlockLocation->x : cEditingLocation->x)    - IsoRealmsConstants::BLOCK_RADIUS;
    float mNorth = (cEditingLocation->y > cStartBlockLocation->y ? cEditingLocation->y    : cStartBlockLocation->y) + IsoRealmsConstants::BLOCK_RADIUS;
    float mSouth = (cEditingLocation->y > cStartBlockLocation->y ? cStartBlockLocation->y : cEditingLocation->y)    - IsoRealmsConstants::BLOCK_RADIUS;
    float mBottom = (cEditingLocation->z > cStartBlockLocation->z ? cStartBlockLocation->z : cEditingLocation->z)    * IsoRealmsConstants::BLOCK_HEIGHT - IsoRealmsConstants::BLOCK_HEIGHT;
    float mTopSouthWest = (cEditingLocation->z > cStartBlockLocation->z ? cEditingLocation->z    : cStartBlockLocation->z) * IsoRealmsConstants::BLOCK_HEIGHT;
    float mTopSouthEast = mTopSouthWest;
    float mTopNorthEast = mTopSouthWest;
    float mTopNorthWest = mTopSouthWest;
  
    int mXSlope = cBlockProperties->getXSlope();
    if (mXSlope > 0) {
      int mHeightOffset = mXSlope * ((mWest - mEast)) * IsoRealmsConstants::BLOCK_HEIGHT;
      mTopSouthEast -= mHeightOffset;
      mTopNorthEast -= mHeightOffset;
    } else if (mXSlope < 0) {
      int mHeightOffset = mXSlope * ((mWest - mEast)) * IsoRealmsConstants::BLOCK_HEIGHT;
      mTopNorthWest += mHeightOffset;
      mTopSouthWest += mHeightOffset;
    }
  
    int mYSlope = cBlockProperties->getYSlope();
    if (mYSlope > 0) {
      int mHeightOffset = mYSlope * ((mSouth - mNorth)) * IsoRealmsConstants::BLOCK_HEIGHT;
      mTopNorthWest -= mHeightOffset;
      mTopNorthEast -= mHeightOffset;
    } else if (mYSlope < 0) {
      int mHeightOffset = mYSlope * ((mSouth - mNorth)) * IsoRealmsConstants::BLOCK_HEIGHT;
      mTopSouthEast += mHeightOffset;
      mTopSouthWest += mHeightOffset;
    }
  
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0, 0.5f, 0.5f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(mWest, mSouth, mTopSouthWest);
    glVertex3f(mEast, mSouth, mTopSouthEast);
    glVertex3f(mEast, mSouth, mBottom);
    glVertex3f(mWest, mSouth, mBottom);
    glVertex3f(mWest, mNorth, mBottom);
    glVertex3f(mEast, mNorth, mBottom);
    glVertex3f(mEast, mNorth, mTopNorthEast);
    glVertex3f(mWest, mNorth, mTopNorthWest);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(mWest, mSouth, mTopSouthWest);  glVertex3f(mWest, mSouth, mBottom);
    glVertex3f(mWest, mNorth, mTopNorthWest);  glVertex3f(mWest, mNorth, mBottom);
    glVertex3f(mEast, mSouth, mBottom);        glVertex3f(mEast, mNorth, mBottom);
    glVertex3f(mEast, mSouth, mTopSouthEast);  glVertex3f(mEast, mNorth, mTopNorthEast);
    glEnd();
  }
}

void ResourceElementSpindizzyBlock::renderIcon() {
  glTranslatef(0.0f, 0.3f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  // TODO: Scale the icon
  glScalef(1.3f, 1.3f, 1.3f);
  glColor3f(1.0f, 1.0f, 1.0f);
  if (cSampleBlock == nullptr) {
    BlockLocation mIdentityBlockLocation(0, 0, 0);
    cSampleBlock = createBlock(&mIdentityBlockLocation, &mIdentityBlockLocation, cBlockProperties, true, nullptr);
    cSampleBlock->createSampleSurfaces();
  }
  cSampleBlock->renderStatic();
}

void ResourceElementSpindizzyBlock::updateIcon(unsigned int milliseconds) {
  // Nothing to do.
}

void ResourceElementSpindizzyBlock::componentClosed(IHUDComponent* component) {
  if (component == cConfigurationComponent) {
    cConfigurationComponent = nullptr;
  }
}

void ResourceElementSpindizzyBlock::destroy(IElement* element) {
  delete element;
}

ElementSpindizzyBlock* ResourceElementSpindizzyBlock::createBlock(BlockLocation* startLocation, BlockLocation* endLocation, SpindizzyBlockProperties* blockProperties, bool addition, IElementContainer* container) {
  return new ElementSpindizzyBlock(this, startLocation, endLocation, blockProperties, addition, container);
}

void ResourceElementSpindizzyBlock::resourcePendingDestruction(ITexture* destroyee, ITexture* replacement) {
  if (cBlockTypeProperties.resourcePendingDestruction(destroyee, replacement)) {
    cModuleInterface->staticChanged();
  }
}

void ResourceElementSpindizzyBlock::resourceChanged(ITexture* texture) {
  if (cBlockTypeProperties.contains(texture)) {
    cModuleInterface->staticChanged();
  }
}

ResourceElementSpindizzyBlock::~ResourceElementSpindizzyBlock() {
  if (cStartBlockLocation != nullptr) {
    delete cStartBlockLocation;
  }
  delete cSampleBlock;
  delete cBlockProperties;
  if (cConfigurationComponent != nullptr) {
    cComponentContainer->removeComponent(cConfigurationComponent);
    delete cConfigurationComponent;
  }
  for (unsigned int i = 0; i < cContent.size(); i++) {
//    removeElement(cContent[i]);
    delete cContent[i];
  }
}


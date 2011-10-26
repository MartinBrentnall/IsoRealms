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
#include "SpindizzyBlockFactory.h"

SpindizzyBlockFactory::SpindizzyBlockFactory(std::string name, ISpindizzyTextureSet** textureSet, ISpindizzyBlockSet* elementSet, DOMNodeWrapper* node, ICommandRegistry* commandRegistry) : ISpindizzyBlockFactory(elementSet) {
  cFactoryName = name;
  cConfigurationComponent = NULL;
  cStartBlockLocation = NULL;
  cBlockProperties = new SpindizzyBlockProperties();
  cSampleBlock = NULL;
  cSpindizzyTextureSet = textureSet;
  cBlockTypeProperties.configure(node, commandRegistry, textureSet);
}

IElement* SpindizzyBlockFactory::getElement(DOMNodeWrapper* node, BlockLocation* zoneLocation, IElementContainer* elementContainer) {
  BlockLocation mStartLocation;
  BlockLocation mEndLocation;
  // TODO: Should throw something if these are not specified!
  cBlockProperties->reset();
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Location") {
      mStartLocation.setRelative(mNode, *zoneLocation);
    } else if (mValueAsString == "Size") {
      mEndLocation.setRelative(mNode, mStartLocation);
    } else if (mValueAsString == "Split") {
      cBlockProperties->setup(mNode);
    } else if (mValueAsString == "Slope") {
      cBlockProperties->setupSlope(mNode);
    } else if (mValueAsString == "Condition") {
      ISpindizzyBlockSet* mBlockSet = getElementSet();
      std::vector<ConditionElement*> mElements = mBlockSet->getConditionElements();
      cBlockProperties->setupCondition(mNode, mElements);
    }
  }
  bool mAddition = mStartLocation.z <= mEndLocation.z;
  (mAddition ? mStartLocation.z : mEndLocation.z)++;
  mEndLocation.x--;
  mEndLocation.y--;
  SpindizzyBlock* mLoadedBlock = createBlock(&mStartLocation, &mEndLocation, cSpindizzyTextureSet, cBlockProperties, mAddition);
  cContent.push_back(mLoadedBlock);
  registerElement(mLoadedBlock, elementContainer);
  return mLoadedBlock;
}

void SpindizzyBlockFactory::signalAllElementsDirty() {
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->cacheSurfaces();
  }
}

void SpindizzyBlockFactory::unregisterSurfaces(ISurfaceProcessor* surfaceProcessor) {
  for (unsigned int i = 0; i < cContent.size(); i++) {
    surfaceProcessor->unregisterSurfaceProvider(cContent[i]);
  }
}

BlockTypeProperties* SpindizzyBlockFactory::getBlockTypeProperties() {
  return &cBlockTypeProperties;
}

void SpindizzyBlockFactory::save(DOMNodeWriter* node) {
  DOMNodeWriter* mBlockTypeNode = node->addBranch("BlockType");
  mBlockTypeNode->addAttribute("name", cFactoryName);
  cBlockTypeProperties.save(mBlockTypeNode);
}

void SpindizzyBlockFactory::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cComponentContainer = componentContainer;
  cEditingLocation = editingLocation;
}

bool SpindizzyBlockFactory::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cStartBlockLocation == NULL) {
        cStartBlockLocation = new BlockLocation(*cEditingLocation);
      } else {
        SpindizzyBlock* mNewBlock = createBlock(cStartBlockLocation, cEditingLocation, cSpindizzyTextureSet, cBlockProperties, true);
        cContent.push_back(mNewBlock);
        addElement(mNewBlock);
        delete cStartBlockLocation;
        cStartBlockLocation = NULL;
      }
      return true;
    }

    case SDLK_DELETE: {
      if (cStartBlockLocation == NULL) {
        cStartBlockLocation = new BlockLocation(*cEditingLocation);
      } else {
        SpindizzyBlock* mNewBlock = createBlock(cStartBlockLocation, cEditingLocation, cSpindizzyTextureSet, cBlockProperties, false);
        cContent.push_back(mNewBlock);
        addElement(mNewBlock);
        delete cStartBlockLocation;
        cStartBlockLocation = NULL;
      }
    }

    default: {
      return false;
    }
  }
  return false;
}

void SpindizzyBlockFactory::configureElement() {
  if (cConfigurationComponent == NULL) {
    cConfigurationComponent = new SpindizzyBlockConfigurationComponent(cComponentContainer, this, cBlockProperties, cSpindizzyTextureSet, this);
    cComponentContainer->addComponent(cConfigurationComponent);
  }
}

bool SpindizzyBlockFactory::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void SpindizzyBlockFactory::renderEditingPreview() {
  if (cStartBlockLocation != NULL) {
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

void SpindizzyBlockFactory::renderIcon() {
  glTranslatef(0.0f, 0.3f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  // TODO: Scale the icon
  glScalef(1.3f, 1.3f, 1.3f);
  glColor3f(1.0f, 1.0f, 1.0f);
  if (cSampleBlock == NULL) {
    BlockLocation mIdentityBlockLocation(0, 0, 0);
    cSampleBlock = createBlock(&mIdentityBlockLocation, &mIdentityBlockLocation, cSpindizzyTextureSet, cBlockProperties, true);
  }
  cSampleBlock->renderStatic();
}

void SpindizzyBlockFactory::updateIcon(int milliseconds) {
  // Nothing to do.
}

void SpindizzyBlockFactory::componentClosed(IHUDComponent* component) {
  if (component == cConfigurationComponent) {
    cConfigurationComponent = NULL;
  }
}

std::string SpindizzyBlockFactory::getName() {
  return cFactoryName;
}

SpindizzyBlock* SpindizzyBlockFactory::createBlock(BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** textureSet, SpindizzyBlockProperties* blockProperties, bool addition) {
  return new SpindizzyBlock(this, startLocation, endLocation, blockProperties, addition);
}

SpindizzyBlockFactory::~SpindizzyBlockFactory() {
  if (cStartBlockLocation != NULL) {
    delete cStartBlockLocation;
  }
  delete cSampleBlock;
  delete cBlockProperties;
  if (cConfigurationComponent != NULL) {
    cComponentContainer->removeComponent(cConfigurationComponent);
    delete cConfigurationComponent;
  }
  for (unsigned int i = 0; i < cContent.size(); i++) {
    removeElement(cContent[i]);
    delete cContent[i];
  }
}


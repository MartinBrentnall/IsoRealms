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

SpindizzyBlockFactory::SpindizzyBlockFactory(std::string name, ISpindizzyTextureSet** textureSet, IElementSet* elementSet) : ISpindizzyBlockFactory(elementSet) {
  cFactoryName = name;
  cConfigurationComponent = NULL;
  cStartBlockLocation = NULL;
  cBlockProperties = new SpindizzyBlockProperties();
  cSampleBlock = NULL;
  cSpindizzyTextureSet = textureSet;
}

IElement* SpindizzyBlockFactory::getElement(DOMNodeWrapper* node, BlockLocation* zoneLocation) {
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
    }
  }
  bool mAddition = mStartLocation.z <= mEndLocation.z;
  (mAddition ? mStartLocation.z : mEndLocation.z)++;
  AbstractSpindizzyBlock* mLoadedBlock = createBlock(&mStartLocation, &mEndLocation, cSpindizzyTextureSet, cBlockProperties, mAddition);
  cContent.push_back(mLoadedBlock);
  return mLoadedBlock;
}

void SpindizzyBlockFactory::signalAllElementsDirty() {
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->signalElementDirty();
  }
}

void SpindizzyBlockFactory::setEditingInfo(BlockLocation* editingLocation, IElementGateway* gateway, IComponentContainer* componentContainer) {
  cComponentContainer = componentContainer;
  cGateway = gateway;
  cEditingLocation = editingLocation;
}

bool SpindizzyBlockFactory::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cStartBlockLocation == NULL) {
        cStartBlockLocation = new BlockLocation(*cEditingLocation);
      } else {
        AbstractSpindizzyBlock* mNewBlock = createBlock(cStartBlockLocation, cEditingLocation, cSpindizzyTextureSet, cBlockProperties, true);
        cContent.push_back(mNewBlock);
        cGateway->pushElement(mNewBlock);
        delete cStartBlockLocation;
        cStartBlockLocation = NULL;
      }
      return true;
    }

    case SDLK_DELETE: {
      if (cStartBlockLocation == NULL) {
        cStartBlockLocation = new BlockLocation(*cEditingLocation);
      } else {
        AbstractSpindizzyBlock* mNewBlock = createBlock(cStartBlockLocation, cEditingLocation, cSpindizzyTextureSet, cBlockProperties, false);
        cContent.push_back(mNewBlock);
        cGateway->pushElement(mNewBlock);
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
    float x  = (cEditingLocation->x > cStartBlockLocation->x ? cEditingLocation->x    : cStartBlockLocation->x) + IsoRealmsConstants::BLOCK_RADIUS;
    float xs = (cEditingLocation->x > cStartBlockLocation->x ? cStartBlockLocation->x : cEditingLocation->x)    - IsoRealmsConstants::BLOCK_RADIUS;
    float y  = (cEditingLocation->y > cStartBlockLocation->y ? cEditingLocation->y    : cStartBlockLocation->y) + IsoRealmsConstants::BLOCK_RADIUS;
    float ys = (cEditingLocation->y > cStartBlockLocation->y ? cStartBlockLocation->y : cEditingLocation->y)    - IsoRealmsConstants::BLOCK_RADIUS;
    float z  = (cEditingLocation->z > cStartBlockLocation->z ? cEditingLocation->z    : cStartBlockLocation->z) * IsoRealmsConstants::BLOCK_HEIGHT;
    float zs = (cEditingLocation->z > cStartBlockLocation->z ? cStartBlockLocation->z : cEditingLocation->z)    * IsoRealmsConstants::BLOCK_HEIGHT - IsoRealmsConstants::BLOCK_HEIGHT;
    float zn = zs;
    float ze = zs;
    float zw = zs;
  
    int mXSlope = cBlockProperties->getXSlope();
    if (mXSlope > 0) {
      ze += mXSlope * (cEditingLocation->x) * IsoRealmsConstants::BLOCK_HEIGHT;
      zn += mXSlope * (cEditingLocation->x) * IsoRealmsConstants::BLOCK_HEIGHT;
    } else if (mXSlope < 0) {
      zw -= mXSlope * (cEditingLocation->x) * IsoRealmsConstants::BLOCK_HEIGHT;
      zs -= mXSlope * (cEditingLocation->x) * IsoRealmsConstants::BLOCK_HEIGHT;
    }
  
    int mYSlope = cBlockProperties->getYSlope();
    if (mYSlope > 0) {
      zw += mYSlope * (cEditingLocation->y) * IsoRealmsConstants::BLOCK_HEIGHT;
      zn += mYSlope * (cEditingLocation->y) * IsoRealmsConstants::BLOCK_HEIGHT;
    } else if (mYSlope < 0) {
      ze -= mYSlope * (cEditingLocation->y) * IsoRealmsConstants::BLOCK_HEIGHT;
      zs -= mYSlope * (cEditingLocation->y) * IsoRealmsConstants::BLOCK_HEIGHT;
    }
  
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0, 0.5f, 0.5f);
    glBegin(GL_LINES);
    glVertex3f(xs, ys, zs); glVertex3f(x,  ys, ze);
    glVertex3f(x,  ys, ze); glVertex3f(x,  y,  zn);
    glVertex3f(x,  y,  zn); glVertex3f(xs, y,  zw);
    glVertex3f(xs, y,  zw); glVertex3f(xs, ys, zs);
  
    glVertex3f(xs, ys, z);   glVertex3f(x,  ys, z);
    glVertex3f(x,  ys, z);   glVertex3f(x,  y,  z);
    glVertex3f(x,  y,  z);   glVertex3f(xs, y,  z);
    glVertex3f(xs, y,  z);   glVertex3f(xs, ys, z);
  
    glVertex3f(x,  ys, ze); glVertex3f(x,  ys, z);
    glVertex3f(x,  y,  zn); glVertex3f(x,  y,  z);
    glVertex3f(xs, y,  zw); glVertex3f(xs, y,  z);
    glVertex3f(xs, ys, zs); glVertex3f(xs, ys, z);
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

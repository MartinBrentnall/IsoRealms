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
#include "ResourceElementSpindizzyLift.h"

ResourceElementSpindizzyLift::ResourceElementSpindizzyLift(ISpindizzyLiftSet* elementSet, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cModuleInterface = elementSet;
  cProperties = cModuleInterface->getSpindizzyLiftProperties();
  cInsertLocation = nullptr;
  cFirstRange = nullptr;
}

I3DModelType* ResourceElementSpindizzyLift::getModelType() {
  return cModelType;
}

void ResourceElementSpindizzyLift::setModelType(I3DModelType* modelType) {
  cSampleLift->setModelType(cModelType, modelType);
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->setModelType(cModelType, modelType);
  }
  cModelType = modelType;
}

void ResourceElementSpindizzyLift::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mModelPath = node->getAttribute("model");
  std::string mStatePath = node->getAttribute("state");
  BlockLocation mIdentityLocation(0, 0, 0);
  cModelType = resourceAccessor->getModelType(mModelPath);
  cState = resourceAccessor->getBoolean(mStatePath);
  cSampleLift = new ElementSpindizzyLift(this, &mIdentityLocation, cModelType, cProperties, 0, 0, nullptr);
  cConfigurationComponent = nullptr;
}

bool ResourceElementSpindizzyLift::isActive() {
  return cState->getValue();
}

void ResourceElementSpindizzyLift::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModelType));
  node->addAttribute("state", resourceLocator->getPath(cState));
}

ISpindizzyLiftSet* ResourceElementSpindizzyLift::getSpindizzyLiftInterface() {
  return cModuleInterface;
}

void ResourceElementSpindizzyLift::loadElement(DOMNodeWrapper* node, BlockLocation* relative, IElementContainer* container, IResourceAccessor* resources, bool asTemplate, bool independent) {
  if (!asTemplate) {
    cProperties->reset();
    BlockLocation mStartLocation;
    mStartLocation.setRelative(node, *relative);
    int mLiftTop = node->getIntegerAttribute("top");
    int mLiftBottom = node->getIntegerAttribute("bottom");
    cProperties->setup(node);
    ElementSpindizzyLift* mLoadedLift = new ElementSpindizzyLift(this, &mStartLocation, cModelType, cProperties, mLiftBottom, mLiftTop, container);
    cContent.push_back(mLoadedLift);
    ElementHandlerSpindizzyDynamic* mDynamicElementHandler = cModuleInterface->getDynamicElementHandler(container);
    mDynamicElementHandler->addElement(mLoadedLift);
  }
}

bool ResourceElementSpindizzyLift::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
//       if (cInsertLocation == nullptr) {
//         cInsertLocation = new BlockLocation(*cEditingLocation);
//       } else if (cFirstRange == nullptr) {
//         cFirstRange = new int(cEditingLocation->z);
//       } else {
//         int mTopRange = *cFirstRange > cEditingLocation->z ? *cFirstRange : cEditingLocation->z;
//         int mBottomRange = *cFirstRange > cEditingLocation->z ? cEditingLocation->z : *cFirstRange;
//         ElementSpindizzyLift* mLiftElement = new ElementSpindizzyLift(this, cInsertLocation, cModelType, cProperties, mBottomRange, mTopRange, nullptr);
// //        addElement(mLiftElement);
//         cContent.push_back(mLiftElement);
//         delete cInsertLocation;
//         cInsertLocation = nullptr;
//         delete cFirstRange;
//         cFirstRange = nullptr;
//       }
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

void ResourceElementSpindizzyLift::configureElement() {
  if (cConfigurationComponent == nullptr) {
    cConfigurationComponent = new SpindizzyLiftConfigurationComponent(cComponentContainer, cProperties);
    cComponentContainer->addComponent(cConfigurationComponent);
  }
}

bool ResourceElementSpindizzyLift::inputEdit(SDL_Event& event, ILayerEditingContext* editingContext) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void ResourceElementSpindizzyLift::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cComponentContainer = componentContainer;
}

void ResourceElementSpindizzyLift::renderArrowLines() {
//   float mLineRadius = IsoRealmsConstants::BLOCK_RADIUS * 0.5;
//   float mArrowOffset = IsoRealmsConstants::BLOCK_HEIGHT * 0.5;
//   glBegin(GL_LINES);
//   int mTop = cFirstRange != nullptr ? (*cFirstRange > cEditingLocation->z ? *cFirstRange : cEditingLocation->z) : cEditingLocation->z;
//   int mBottom = cFirstRange != nullptr ? (*cFirstRange > cEditingLocation->z ? cEditingLocation->z : *cFirstRange) : cEditingLocation->z;
//   glVertex3f(0, 0, mTop * IsoRealmsConstants::BLOCK_HEIGHT);
//   glVertex3f(0, 0, mBottom * IsoRealmsConstants::BLOCK_HEIGHT);
// 
//   // Top point
//   glVertex3f( 0,           0, mTop * IsoRealmsConstants::BLOCK_HEIGHT);
//   glVertex3f( mLineRadius, 0, mTop * IsoRealmsConstants::BLOCK_HEIGHT - mArrowOffset);
//   glVertex3f( 0,           0, mTop * IsoRealmsConstants::BLOCK_HEIGHT);
//   glVertex3f(-mLineRadius, 0, mTop * IsoRealmsConstants::BLOCK_HEIGHT - mArrowOffset);
// 
//   // Bottom point
//   glVertex3f( 0,           0, mBottom * IsoRealmsConstants::BLOCK_HEIGHT);
//   glVertex3f( mLineRadius, 0, mBottom * IsoRealmsConstants::BLOCK_HEIGHT + mArrowOffset);
//   glVertex3f( 0,           0, mBottom * IsoRealmsConstants::BLOCK_HEIGHT);
//   glVertex3f(-mLineRadius, 0, mBottom * IsoRealmsConstants::BLOCK_HEIGHT + mArrowOffset);
//   glEnd();
}

ElementSpindizzyLift* ResourceElementSpindizzyLift::getElement(IElement* element) {
  for (ElementSpindizzyLift* mElement : cContent) {
    if (mElement == element) {
      return mElement;
    }
  }
  return nullptr;
}

void ResourceElementSpindizzyLift::renderEditingPreview(Vertex& location) {
  glTranslatef(location.x, location.y, location.z);
  if (cInsertLocation != nullptr) {
    glPushMatrix();
    glTranslatef(cInsertLocation->x, cInsertLocation->y, cInsertLocation->z * IsoRealmsConstants::BLOCK_HEIGHT + (IsoRealmsConstants::BLOCK_HEIGHT * 0.05));
    cSampleLift->renderStatic();
    if (cSampleLift->isVisualRuntime()) {
      cSampleLift->renderRuntime();
    }
    glPopMatrix();
    glPushMatrix();
    glTranslatef(cInsertLocation->x, cInsertLocation->y, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0, 1.0, 0.0);
    glLineWidth(3.0);
    glLineStipple(1, 255);
    glEnable(GL_LINE_STIPPLE);
    renderArrowLines();
    glColor3f(1.0, 0.0, 0.0);
    glDisable(GL_LINE_STIPPLE);
    glLineWidth(5.0);
    renderArrowLines();
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(1.0);
    glPopMatrix();
  } else {
    glColor3f(1.0f, 1.0f, 1.0f);
    cSampleLift->renderRuntime();
  }
}

void ResourceElementSpindizzyLift::renderIcon() {
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  // TODO: Scale the icon
//  glScalef(0.7f, 0.7f, 0.7f);
  glColor3f(1.0f, 1.0f, 1.0f);
  cSampleLift->renderStatic();
  if (cSampleLift->isVisualRuntime()) {
    cSampleLift->renderRuntime();
  }
}

void ResourceElementSpindizzyLift::updateIcon(unsigned int milliseconds) {
  // Nothing to do.
}

void ResourceElementSpindizzyLift::destroy(IElement* element) {
  delete element;
}

void ResourceElementSpindizzyLift::removeElement(IElement* element) {
  ElementSpindizzyLift* mLift = getElement(element);
  IElementContainer* mContainer = mLift->getElementContainer();
  ElementHandlerSpindizzyDynamic* mItemHandler = cModuleInterface->getDynamicElementHandler(mContainer);
  mItemHandler->removeElement(mLift);
  if (mItemHandler->isEmpty()) {
    mContainer->removeElement(mItemHandler);
    cModuleInterface->removeElementHandlerSpindizzyDynamic(mContainer);
  }
}

Vertex* ResourceElementSpindizzyLift::editorCursorStopped(Vertex* location) {
  Vertex* mGridLocation = new Vertex();
  mGridLocation->x = std::round(location->x);
  mGridLocation->y = std::round(location->y);
  mGridLocation->z = std::round(location->z * 2.0) * 0.5;
  return mGridLocation;
}

ResourceElementSpindizzyLift::~ResourceElementSpindizzyLift() {
  delete cSampleLift;
  for (unsigned int i = 0; i < cContent.size(); i++) {
    removeElement(cContent[i]);
    destroy(cContent[i]);
  }
}

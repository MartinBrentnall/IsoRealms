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

void ResourceElementSpindizzyLift::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resourceAccessor) {
  std::string mModelPath = node->getAttribute("model");
  std::string mStatePath = node->getAttribute("state");
  BlockLocation mIdentityLocation(0, 0, 0);
  cModelType = resourceAccessor->getModelType(mModelPath);
  cState = resourceAccessor->getBoolean(mStatePath);
  cSampleLift = new ElementSpindizzyLift(this, &mIdentityLocation, cModelType, cProperties, 0, 0, nullptr);
  cConfigurationComponent = nullptr;
}

Icon<IElementType>* ResourceElementSpindizzyLift::getResourceIcon(IResourceBrowser<IElementType>* browser) {
  return nullptr;
}

bool ResourceElementSpindizzyLift::isActive() {
  return cState->getValue();
}

void ResourceElementSpindizzyLift::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModelType));
  node->addAttribute("state", resourceLocator->getPath(cState));
}

void ResourceElementSpindizzyLift::saveCache(DOMNodeWriter* cache) {
  // Nothing to do
}

ISpindizzyLiftSet* ResourceElementSpindizzyLift::getSpindizzyLiftInterface() {
  return cModuleInterface;
}

void ResourceElementSpindizzyLift::loadElement(DOMNodeWrapper* node, DOMNodeWrapper* cache, BlockLocation* relative, IElementContainer* container, IResourceAccessor* resources, bool asTemplate) {
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

bool ResourceElementSpindizzyLift::keyDown(SDLKey& key, ILayerEditingContext* editingContext) {
  switch (key) {
    case SDLK_SPACE: {
      Vertex* mLocation = editingContext->getLocation();
      IElementContainer* mElementContainer = editingContext->getElementContainer();
      if (cInsertLocation == nullptr) {
        cInsertLocation = new BlockLocation(std::round(mLocation->x), std::round(mLocation->y), std::round(mLocation->z * 2.0));
        editingContext->setCursorRestriction(mElementContainer);
      } else if (cFirstRange == nullptr) {
        cFirstRange = new int(std::round(mLocation->z * 2.0));
      } else {
        editingContext->removeCursorRestriction(mElementContainer);
        int mSecondRange = std::round(mLocation->z * 2.0);
        int mTopRange    = *cFirstRange > mSecondRange ? *cFirstRange : mSecondRange;
        int mBottomRange = *cFirstRange > mSecondRange ? mSecondRange : *cFirstRange;
        ElementSpindizzyLift* mLiftElement = new ElementSpindizzyLift(this, cInsertLocation, cModelType, cProperties, mBottomRange, mTopRange, mElementContainer);
        ElementHandlerSpindizzyDynamic* mHandler = cModuleInterface->getDynamicElementHandler(mElementContainer);
        mHandler->addElement(mLiftElement);
        mElementContainer->setDirty(mHandler);
        cContent.push_back(mLiftElement);
        delete cInsertLocation;
        cInsertLocation = nullptr;
        delete cFirstRange;
        cFirstRange = nullptr;
      }
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
      return keyDown(event.key.keysym.sym, editingContext);
    }
  }
  return false;
}

void ResourceElementSpindizzyLift::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cComponentContainer = componentContainer;
}

void ResourceElementSpindizzyLift::renderArrowLines(float rangeA, float rangeB) {
  float mLineRadius = IsoRealmsConstants::BLOCK_RADIUS * 0.5;
  float mArrowOffset = IsoRealmsConstants::BLOCK_HEIGHT * 0.5;
  float mTop    = rangeA > rangeB ? rangeA : rangeB;
  float mBottom = rangeA > rangeB ? rangeB : rangeA;
  glBegin(GL_LINES);
  glVertex3f(0, 0, mTop);
  glVertex3f(0, 0, mBottom);

  // Top point
  glVertex3f( 0,           0, mTop);
  glVertex3f( mLineRadius, 0, mTop - mArrowOffset);
  glVertex3f( 0,           0, mTop);
  glVertex3f(-mLineRadius, 0, mTop - mArrowOffset);

  // Bottom point
  glVertex3f( 0,           0, mBottom);
  glVertex3f( mLineRadius, 0, mBottom + mArrowOffset);
  glVertex3f( 0,           0, mBottom);
  glVertex3f(-mLineRadius, 0, mBottom + mArrowOffset);
  glEnd();
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
  if (cInsertLocation != nullptr) {
    glPushMatrix();
//    glTranslatef(location.x, location.y, location.z);
    if (cFirstRange == nullptr) {
      glColor3f(1.0f, 0.5f, 0.5f);
    } else {
      glColor3f(0.5f, 1.0f, 0.5f);
    }
    glLineWidth(3.0);
    glBegin(GL_LINES);
    Utils::renderVolumeLines(location.x - IsoRealmsConstants::BLOCK_RADIUS, location.x + IsoRealmsConstants::BLOCK_RADIUS,
                             location.y - IsoRealmsConstants::BLOCK_RADIUS, location.y + IsoRealmsConstants::BLOCK_RADIUS,
                             location.z,                                    location.z);
    glEnd();
//    cSampleLift->renderStatic();
//    if (cSampleLift->isVisualRuntime()) {
//      cSampleLift->renderRuntime();
//    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(cInsertLocation->x, cInsertLocation->y, cInsertLocation->z * IsoRealmsConstants::BLOCK_HEIGHT + (IsoRealmsConstants::BLOCK_HEIGHT * 0.05));
//    cSampleLift->renderStatic(); TODO
    if (cSampleLift->isVisualRuntime()) {
      cSampleLift->renderRuntime();
    }
    glPopMatrix();
    glPushMatrix();
    glTranslatef(cInsertLocation->x, cInsertLocation->y, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0, 1.0, 0.0);
    glLineStipple(1, 255);
    glEnable(GL_LINE_STIPPLE);    
    float mRangeA = location.z;
    float mRangeB = cFirstRange != nullptr ? *cFirstRange * IsoRealmsConstants::BLOCK_HEIGHT : location.z;
    renderArrowLines(mRangeA, mRangeB);
    glColor3f(1.0, 0.0, 0.0);
    glDisable(GL_LINE_STIPPLE);
    glLineWidth(5.0);
    renderArrowLines(mRangeA, mRangeB);
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(1.0);
    glPopMatrix();
  } else {
    glPushMatrix();
    glTranslatef(location.x, location.y, location.z);
    glColor3f(1.0f, 1.0f, 1.0f);
//    cSampleLift->renderStatic(); TODO
    if (cSampleLift->isVisualRuntime()) {
      cSampleLift->renderRuntime();
    }
    glPopMatrix();
  }
}

void ResourceElementSpindizzyLift::updateEditingPreview(unsigned int milliseconds) {
  // TODO
}

void ResourceElementSpindizzyLift::renderIcon() {
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  // TODO: Scale the icon
//  glScalef(0.7f, 0.7f, 0.7f);
  glColor3f(1.0f, 1.0f, 1.0f);
//  cSampleLift->renderStatic(); TODO
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

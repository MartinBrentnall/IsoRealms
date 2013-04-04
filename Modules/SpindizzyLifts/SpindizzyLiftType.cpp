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
#include "SpindizzyLiftType.h"

SpindizzyLiftType::SpindizzyLiftType(ISpindizzyLiftSet* elementSet, SpindizzyLiftProperties* properties) : ISpindizzyLiftType(elementSet) {
  cProperties = properties;
  cInsertLocation = NULL;
  cFirstRange = NULL;
}

void SpindizzyLiftType::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mModelPath = node->getAttribute("model");
  cState = node->getBooleanAttribute("active");
  BlockLocation mIdentityLocation(0, 0, 0);
  cModelType = resourceAccessor->getModelType(mModelPath);
  cSampleLift = new SpindizzyLift(this, &mIdentityLocation, cModelType, cProperties, 0, 0);
  cSampleVisualElements = cSampleLift->getVisualElements();
  cConfigurationComponent = NULL;
}

void SpindizzyLiftType::setActive(bool active) {
  cState = active;
  std::cout << "Setting lift active: " << active << std::endl;
}

bool SpindizzyLiftType::isActive() {
  return cState;
}

void SpindizzyLiftType::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModelType));
  if (cState) {
    node->addAttribute("active", "true");
  }
}

IElement* SpindizzyLiftType::getElement(DOMNodeWrapper* node, BlockLocation* relative, IElementContainer* container) {
  cProperties->reset();
  BlockLocation mStartLocation;
  mStartLocation.setRelative(node, *relative);
  int mLiftTop = node->getIntegerAttribute("top");
  int mLiftBottom = node->getIntegerAttribute("bottom");
  cProperties->setup(node);
  SpindizzyLift* mLoadedLift = new SpindizzyLift(this, &mStartLocation, cModelType, cProperties, mLiftBottom, mLiftTop);
  cContent.push_back(mLoadedLift);
  ISpindizzyLiftSet* mLiftSet = getElementSet();
  mLiftSet->registerElement(container, mLoadedLift);
  return mLoadedLift;
}

IElementHandler* SpindizzyLiftType::getElementHandler() {
  ISpindizzyLiftSet* mSpindizzyLiftSet = getElementSet();
  return mSpindizzyLiftSet->createHandler();
}

bool SpindizzyLiftType::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cInsertLocation == NULL) {
        cInsertLocation = new BlockLocation(*cEditingLocation);
      } else if (cFirstRange == NULL) {
        cFirstRange = new int(cEditingLocation->z);
      } else {
        int mTopRange = *cFirstRange > cEditingLocation->z ? *cFirstRange : cEditingLocation->z;
        int mBottomRange = *cFirstRange > cEditingLocation->z ? cEditingLocation->z : *cFirstRange;
        SpindizzyLift* mLiftElement = new SpindizzyLift(this, cInsertLocation, cModelType, cProperties, mBottomRange, mTopRange);
        addElement(mLiftElement);
        cContent.push_back(mLiftElement);
        delete cInsertLocation;
        cInsertLocation = NULL;
        delete cFirstRange;
        cFirstRange = NULL;
      }
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

void SpindizzyLiftType::configureElement() {
  if (cConfigurationComponent == NULL) {
    cConfigurationComponent = new SpindizzyLiftConfigurationComponent(cComponentContainer, cProperties);
    cComponentContainer->addComponent(cConfigurationComponent);
  }
}

bool SpindizzyLiftType::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void SpindizzyLiftType::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cEditingLocation = editingLocation;  
  cComponentContainer = componentContainer;
}

void SpindizzyLiftType::renderArrowLines() {
  float mLineRadius = IsoRealmsConstants::BLOCK_RADIUS * 0.5;
  float mArrowOffset = IsoRealmsConstants::BLOCK_HEIGHT * 0.5;
  glBegin(GL_LINES);
  int mTop = cFirstRange != NULL ? (*cFirstRange > cEditingLocation->z ? *cFirstRange : cEditingLocation->z) : cEditingLocation->z;
  int mBottom = cFirstRange != NULL ? (*cFirstRange > cEditingLocation->z ? cEditingLocation->z : *cFirstRange) : cEditingLocation->z;
  glVertex3f(0, 0, mTop * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(0, 0, mBottom * IsoRealmsConstants::BLOCK_HEIGHT);

  // Top point
  glVertex3f( 0,           0, mTop * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f( mLineRadius, 0, mTop * IsoRealmsConstants::BLOCK_HEIGHT - mArrowOffset);
  glVertex3f( 0,           0, mTop * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(-mLineRadius, 0, mTop * IsoRealmsConstants::BLOCK_HEIGHT - mArrowOffset);

  // Bottom point
  glVertex3f( 0,           0, mBottom * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f( mLineRadius, 0, mBottom * IsoRealmsConstants::BLOCK_HEIGHT + mArrowOffset);
  glVertex3f( 0,           0, mBottom * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(-mLineRadius, 0, mBottom * IsoRealmsConstants::BLOCK_HEIGHT + mArrowOffset);
  glEnd();
}

void SpindizzyLiftType::renderEditingPreview() {
  if (cInsertLocation != NULL) {
    glPushMatrix();
    glTranslatef(cInsertLocation->x, cInsertLocation->y, cInsertLocation->z * IsoRealmsConstants::BLOCK_HEIGHT + (IsoRealmsConstants::BLOCK_HEIGHT * 0.05));
    cSampleLift->renderStatic();
    for (unsigned int i = 0; i < cSampleVisualElements.size(); i++) {
      cSampleVisualElements[i]->render();
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
  }
}

void SpindizzyLiftType::renderIcon() {
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  // TODO: Scale the icon
//  glScalef(0.7f, 0.7f, 0.7f);
  glColor3f(1.0f, 1.0f, 1.0f);
  cSampleLift->renderStatic();
  for (unsigned int i = 0; i < cSampleVisualElements.size(); i++) {
    cSampleVisualElements[i]->render();
  }
}

void SpindizzyLiftType::updateIcon(int milliseconds) {
  // Nothing to do.
}

void SpindizzyLiftType::destroy(IElement* element) {
  delete element;
}

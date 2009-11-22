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
#include "SpindizzyLiftFactory.h"

SpindizzyLiftFactory::SpindizzyLiftFactory(IElementSet* elementSet, ISpindizzyTextureSet::TextureType texture) : IElementFactory(elementSet) {
  cTexture = texture;
  cInsertLocation = NULL;
  cFirstRange = NULL;
  BlockLocation mIdentityLocation(0, 0, 0);
  cSampleLift = new SpindizzyLift(this, &mIdentityLocation, NULL, 0, 0);
  cSampleVisualElements = cSampleLift->getVisualElements();
}

std::string getName() {
  return ""; // TODO: Return the actual lift type
}

IElement* SpindizzyLiftFactory::getElement(DOMNodeWrapper* node, BlockLocation* relative) {
  // TODO: Do this!
  return NULL;
}

bool SpindizzyLiftFactory::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cInsertLocation == NULL) {
        cInsertLocation = new BlockLocation(*cEditingLocation);
      } else if (cFirstRange == NULL) {
        cFirstRange = new int(cEditingLocation->z);
      } else {
        int mTopRange = *cFirstRange > cEditingLocation->z ? *cFirstRange : cEditingLocation->z;
        int mBottomRange = *cFirstRange > cEditingLocation->z ? cEditingLocation->z : *cFirstRange;
        ISpindizzyTexture* mTexture = cTextureSet->getTexture(cTexture);
        SpindizzyLift* mLiftElement = new SpindizzyLift(this, cInsertLocation, mTexture, mBottomRange, mTopRange);
        cGateway->pushElement(mLiftElement);
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

void SpindizzyLiftFactory::setTextureSet(ISpindizzyTextureSet* textureSet) {
  cTextureSet = textureSet;
  ISpindizzyTexture* mTexture = cTextureSet->getTexture(cTexture);
  cSampleLift->setTexture(mTexture);
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->setTexture(mTexture);
  }
}

void SpindizzyLiftFactory::configureElement() {
  // Nothing to do.
}

bool SpindizzyLiftFactory::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void SpindizzyLiftFactory::setEditingInfo(BlockLocation* editingLocation, IElementGateway* gateway, IComponentContainer* cComponentContainer) {
  cGateway = gateway;
  cEditingLocation = editingLocation;  
}

void SpindizzyLiftFactory::renderArrowLines() {
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

void SpindizzyLiftFactory::renderEditingPreview() {
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

void SpindizzyLiftFactory::renderIcon() {
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  // TODO: Scale the icon
//  glScalef(0.7f, 0.7f, 0.7f);
  glColor3f(1.0f, 1.0f, 1.0f);
  cSampleLift->renderStatic();
  for (unsigned int i = 0; i < cSampleVisualElements.size(); i++) {
    cSampleVisualElements[i]->render();
  }
}

void SpindizzyLiftFactory::updateIcon(int milliseconds) {
  // Nothing to do.
}

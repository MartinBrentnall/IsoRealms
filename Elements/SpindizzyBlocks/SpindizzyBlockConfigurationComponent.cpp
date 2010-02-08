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
#include "SpindizzyBlockConfigurationComponent.h"

SpindizzyBlockConfigurationComponent::SpindizzyBlockConfigurationComponent(IComponentContainer* componentContainer, SpindizzyBlockFactory* blockFactory, SpindizzyBlockProperties* blockProperties, ISpindizzyTextureSet** textureSet, IComponentCloseListener* listener) : RectangleComponent(componentContainer, new std::string("Block Editor"), 0.0f, 0.0f, 0.25f, 0.5f) {
  cBlockProperties = blockProperties;
  cBlockFactory = blockFactory;
  cTextureSet = textureSet;
  cCloseListener = listener;
  cSampleBlock = NULL;
  updateSampleBlock();
}

void SpindizzyBlockConfigurationComponent::updateSampleBlock() {
  if (cSampleBlock != NULL) {
    delete cSampleBlock;
  }
  BlockLocation mIdentityLocation(0, 0, 0);
  cSampleBlock = cBlockFactory->createBlock(&mIdentityLocation, &mIdentityLocation, cTextureSet, cBlockProperties, true);
}

void SpindizzyBlockConfigurationComponent::updateContent(int milliseconds) {
}

void SpindizzyBlockConfigurationComponent::renderContent() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mScreenAspectRatio = mScreenConfiguration->getAspectRatio();
  float mXLocation = getLeft() + (getRight() - getLeft()) / 2.0f;
  float mYLocation = getBottom() + (getTop() - getBottom()) / 2.0f;
  glBindTexture(GL_TEXTURE_2D, 0);
  glColor3f(0.45f, 0.0f, 0.9f);
  glBegin(GL_LINE_LOOP);
  glVertex2f(getLeft() + 0.01f * mScreenAspectRatio, getBottom() + 0.01f);
  glVertex2f(getLeft() + 0.07f * mScreenAspectRatio, getBottom() + 0.01f);
  glVertex2f(getLeft() + 0.07f * mScreenAspectRatio, getBottom() + 0.07f);
  glVertex2f(getLeft() + 0.01f * mScreenAspectRatio, getBottom() + 0.07f);
  glEnd();
  if (cBlockProperties->isSteppedBottom()) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(getLeft() + 0.01f * mScreenAspectRatio, getBottom() + 0.01f);
    glVertex2f(getLeft() + 0.07f * mScreenAspectRatio, getBottom() + 0.07f);
    glVertex2f(getLeft() + 0.07f * mScreenAspectRatio, getBottom() + 0.01f);
    glVertex2f(getLeft() + 0.01f * mScreenAspectRatio, getBottom() + 0.07f);
    glEnd();
  }
  glPushMatrix();
  float mScale = 0.2f;
  glTranslatef(mXLocation, mYLocation, 0.0f);
  glScalef(mScreenAspectRatio * mScale, mScale, mScale);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  glColor3f(1.0f, 1.0f, 1.0f);
  cSampleBlock->renderStatic();
  std::vector<IVisualElement*> mVisualElements = cSampleBlock->getVisualElements();
  for (unsigned int i = 0; i < mVisualElements.size(); i++) {
    mVisualElements[i]->render();
  }
  cBlockProperties->renderConfiguration();
  glPopMatrix();
}

bool SpindizzyBlockConfigurationComponent::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_ESCAPE: { 
      close();
      cCloseListener->componentClosed(this);
      return true;
    }

    case SDLK_UP: {
      cBlockProperties->selectNorth();
      return true;
    }

    case SDLK_DOWN: {
      cBlockProperties->selectSouth();
      return true;
    }

    case SDLK_LEFT: {
      cBlockProperties->selectWest();
      return true;
    }

    case SDLK_RIGHT: {
      cBlockProperties->selectEast();
      return true;
    }

    case SDLK_PAGEUP: {
      cBlockProperties->raiseSelectedCorner();
      updateSampleBlock();
      return true;
    }

    case SDLK_PAGEDOWN: {
      cBlockProperties->lowerSelectedCorner();
      updateSampleBlock();
      return true;
    }

    case SDLK_s: {
      cBlockProperties->toggleSplit();
      updateSampleBlock();
      return true;
    }

    case SDLK_x: {
      cBlockProperties->toggleSteppedBottom();
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool SpindizzyBlockConfigurationComponent::inputContent(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}


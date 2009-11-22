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
#include "ChooseElementsComponent.h"

ChooseElementsComponent::ChooseElementsComponent(IComponentContainer* componentContainer, ElementSetRegistry* elementSetRegistry) {
  cSelectedSet = 0;
  cAnimation = cSelectedSet;
  std::vector<std::string*> mAvailableElementSets = elementSetRegistry->getElementSets();
  ICommand* mCloseCommand = new CloseCommand(this);
  for (unsigned int i = 0; i < mAvailableElementSets.size(); i++) {
    IElementSet* mElementSet = elementSetRegistry->getElementSet(mAvailableElementSets[i]);
    cElementRows.push_back(new ChooseElementsComponentRow(mCloseCommand, mElementSet));
  }
  cComponentContainer = componentContainer;
}

void ChooseElementsComponent::addElementSelectionListener(IElementSelectionListener* listener) {
  for (unsigned int i = 0; i < cElementRows.size(); i++) {
    cElementRows[i]->addElementSelectionListener(listener);
  }
}

void ChooseElementsComponent::update(int milliseconds) {
  cAnimation += (cSelectedSet - cAnimation) * (0.02f * milliseconds);
  for (unsigned int i = 0; i < cElementRows.size(); i++) {
    cElementRows[i]->update(milliseconds);
  }
}

void ChooseElementsComponent::render() {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();
  glDisable(GL_DEPTH_TEST);

  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();

  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();

  glScalef(0.1f * mAspectRatio, 0.1f, 0.1f);
  glTranslatef(0.0f, cAnimation * 3.0f, 0.0f);
  for (unsigned int i = 0; i < cElementRows.size(); i++) {
    cElementRows[i]->render();
    glTranslatef(0.0f, -3.0f, 0.0f);
  }
  glLoadIdentity();
  glScalef(0.1f * mAspectRatio, 0.1f, 0.1f);
  glBegin(GL_LINE_LOOP);
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex2f(-1.1f,  1.1f);
  glVertex2f(-1.1f, -1.1f);
  glVertex2f( 1.1f, -1.1f);
  glVertex2f( 1.1f,  1.1f);
  glEnd();
  glLoadIdentity();

  glEnable(GL_DEPTH_TEST);
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
}

bool ChooseElementsComponent::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_UP: {
      if (cSelectedSet > 0) {
        cSelectedSet--;
      }
      return true;
    }
  
    case SDLK_DOWN: {
      cSelectedSet++;
      if (cSelectedSet >= cElementRows.size()) {
        cSelectedSet = cElementRows.size() - 1;
      }
      return true;
    }
  
    case SDLK_ESCAPE: {
      close();
      return true;
    }

    default: {
      return false;
    }
  }
}

bool ChooseElementsComponent::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  float mX = mScreen->getXLocation(event.button.x) / mAspectRatio;
  float mY = mScreen->getYLocation(event.button.y);
  float mRowTop = -0.1f + cAnimation * 0.3f; // TODO: Needs row selection taking into consideration
  float mRowBottom = 0.1f + cAnimation * 0.3f; // TODO: Needs row selection taking into consideration
  for (unsigned int i = 0; i < cElementRows.size(); i++) {
    if (mY > mRowTop && mY < mRowBottom) {
      cSelectedSet = i;
      if (cElementRows[i]->testClick(mX)) {
        cComponentContainer->bringComponentToFront(this);
        return true;
      } else {
        return false;
      }
    }
    mRowTop -= 0.3f;
    mRowBottom -= 0.3f;
    // TODO: Move top and bottom by correct amount
  }
  return false;
}

bool ChooseElementsComponent::input(SDL_Event& event) {
  if (cElementRows.size() == 0) {
    // TODO: Should never happen; don't allow this component to be loaded without elements!
    close();
    return false;
  }
  if (cElementRows[cSelectedSet]->input(event)) {
    return true;
  }
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }

    case SDL_MOUSEBUTTONDOWN: {
      return mouseButtonDown(event);
    }
  }
  return false;
}

void ChooseElementsComponent::close() {
  cComponentContainer->removeComponent(this);
}

bool ChooseElementsComponent::contains(float x, float y) {
  return true;
}

ChooseElementsComponent::CloseCommand::CloseCommand(ChooseElementsComponent* parent) {
  cParent = parent;
}

void ChooseElementsComponent::CloseCommand::execute() {
  cParent->close();
}


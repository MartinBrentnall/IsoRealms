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
#include "ChooseElementsComponentRow.h"

ChooseElementsComponentRow::ChooseElementsComponentRow(ICommand* closeCommand, IElementSet* elementSet) {
  cCloseCommand = closeCommand;
  cSelectedElement = 0;
  cAnimation = cSelectedElement;
  cElements = elementSet->getElementFactories();
}

void ChooseElementsComponentRow::addElementSelectionListener(IElementSelectionListener* listener) {
  cElementSelectionListeners.push_back(listener);
}

bool ChooseElementsComponentRow::testClick(float x) {
  float mColumnLeft = -0.1f + cAnimation * -0.3f; // TODO: Needs row selection taking into consideration
  float mColumnRight = 0.1f + cAnimation * -0.3f; // TODO: Needs row selection taking into consideration
  for (unsigned int i = 0; i < cElements.size(); i++) {
    if (x > mColumnLeft && x < mColumnRight) {
      cSelectedElement = i;
      return true;
    }
    mColumnLeft += 0.3f;
    mColumnRight += 0.3f;
  }
  return true;
}

void ChooseElementsComponentRow::update(int milliseconds) {
  cAnimation += (cSelectedElement - cAnimation) * (0.02f * milliseconds);
  for (unsigned int i = 0; i < cElements.size(); i++) {
    cElements[i]->updateIcon(milliseconds);
  }
}

void ChooseElementsComponentRow::render() {
  glTranslatef(cAnimation * -3.0f, 0.0f, 0.0f);
  for (unsigned int i = 0; i < cElements.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    glColor4f(0.05f, 0.0f, 0.1f, 0.85f);
    glVertex2f(-1.0f,  1.0f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f( 1.0f, -1.0f);
    glVertex2f( 1.0f,  1.0f);
    glEnd();
    glDisable(GL_BLEND);
  
    glBegin(GL_LINE_LOOP);
    glColor3f(0.8f, 0.6f, 1.0f);
    glVertex2f(-1.0f,  1.0f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f( 1.0f, -1.0f);
    glVertex2f( 1.0f,  1.0f);
    glEnd();

    glPushMatrix();
    cElements[i]->renderIcon();
    glPopMatrix();

    glTranslatef(3.0f, 0.0f, 0.0f);
  }
  glTranslatef(cAnimation * 3.0f + cElements.size() * -3.0f, 0.0f, 0.0f);
}

bool ChooseElementsComponentRow::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_LEFT: {
      if (cSelectedElement > 0) {
        cSelectedElement--;
      }
      return true;
    }

    case SDLK_RIGHT: {
      cSelectedElement++;
      if (cSelectedElement >= cElements.size()) {
        cSelectedElement = cElements.size() - 1;
      }
      return true;
    }

    case SDLK_RETURN: {
      for (unsigned int i = 0; i < cElementSelectionListeners.size(); i++) {
        cElementSelectionListeners[i]->elementSelected(cElements[cSelectedElement]);
      }
      cCloseCommand->execute();
      return true;
    }

    case SDLK_c: {
      cElements[cSelectedElement]->configureElement();
      return true;
    }

    default: {
      return false;
    }
  }
}

bool ChooseElementsComponentRow::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

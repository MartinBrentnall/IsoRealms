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
#include "EditorCursor.h"

EditorCursor::EditorCursor() {
//  cEditMapPointer = map;
//  cZoneBrush = new ZoneBrush(cEditMapPointer, this);
  cElementBrush = NULL;
  set(0, 0, 0);
//   cEditZonePointer = cEditMapPointer->getZone(*this);
//   cEditZonePointer->restrainLocation(this);
}

void EditorCursor::render() {
  float mXLocation = (x - 1.0f) * BLOCK_SIZE + BLOCK_RADIUS;
  float mYLocation = (y - 1.0f) * BLOCK_SIZE + BLOCK_RADIUS;
  float mZLocation = (z - 1.0f) * BLOCK_HEIGHT;
  float mXSize = x * BLOCK_SIZE + BLOCK_RADIUS;
  float mYSize = y * BLOCK_SIZE + BLOCK_RADIUS;
  float mZSize = z * BLOCK_HEIGHT;

  glBindTexture(GL_TEXTURE_2D, 0);
  glColor3f(1.0, 0.0, 1.0);
  glBegin(GL_LINES);
  glVertex3f(mXSize,      mYSize,      mZLocation); glVertex3f(mXLocation,  mYSize,      mZLocation);
  glVertex3f(mXLocation,  mYSize,      mZLocation); glVertex3f(mXLocation,  mYLocation,  mZLocation);
  glVertex3f(mXLocation,  mYLocation,  mZLocation); glVertex3f(mXSize,      mYLocation,  mZLocation);
  glVertex3f(mXSize,      mYLocation,  mZLocation); glVertex3f(mXSize,      mYSize,      mZLocation);

  glVertex3f(mXSize,      mYSize,      mZSize);   glVertex3f(mXLocation,  mYSize,      mZSize);
  glVertex3f(mXLocation,  mYSize,      mZSize);   glVertex3f(mXLocation,  mYLocation,  mZSize);
  glVertex3f(mXLocation,  mYLocation,  mZSize);   glVertex3f(mXSize,      mYLocation,  mZSize);
  glVertex3f(mXSize,      mYLocation,  mZSize);   glVertex3f(mXSize,      mYSize,      mZSize);

  glVertex3f(mXLocation,  mYSize,      mZLocation); glVertex3f(mXLocation,  mYSize,      mZSize);
  glVertex3f(mXLocation,  mYLocation,  mZLocation); glVertex3f(mXLocation,  mYLocation,  mZSize);
  glVertex3f(mXSize,      mYLocation,  mZLocation); glVertex3f(mXSize,      mYLocation,  mZSize);
  glVertex3f(mXSize,      mYSize,      mZLocation); glVertex3f(mXSize,      mYSize,      mZSize);
  glEnd();
  glColor3f(1.0, 1.0, 1.0);
//  cZoneBrush->render();
  if (cElementBrush != NULL) {
    cElementBrush->renderEditingPreview();
  }
}

void EditorCursor::setElementType(IElementType* elementType) {
  cElementBrush = elementType;
}

bool EditorCursor::input(SDL_Event& event) {
//  if (cEditZonePointer == NULL) {
//     if (cZoneBrush->input(event)) {
//       return true;
//     }
//   } else if (cElementBrush != NULL) {
//     if (cElementBrush->input(event)) {
//       return true;
//     }
//  }

  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }

  return false;
}

bool EditorCursor::keyDown(SDLKey& key) {
  switch (key) {       
    case SDLK_LEFT: {
      x--;
      return true;
    }

    case SDLK_RIGHT: {
      x++;
      return true;
    }

    case SDLK_UP: {
      y++;
      return true;
    }

    case SDLK_DOWN: {
      y--;
      return true;
    }

    case SDLK_PAGEUP: {
      z++;
      return true;
    }

    case SDLK_PAGEDOWN: {
      z--;
      return true;
    }

    case SDLK_ESCAPE: {
      break;
    }

    case SDLK_RETURN: {
      break;
    }
    
    case SDLK_DELETE: {
    }

    default: {
      return false;
    }
  }
  return false;
}

void EditorCursor::moveToCamera() {
  glTranslatef(-x * BLOCK_SIZE, -y * BLOCK_SIZE, -z * BLOCK_HEIGHT);
}

// IElementContainer* EditorCursor::pushElement(IElement* element) {
//   cEditZonePointer->pushElement(element);
//   return cEditZonePointer;
// }
// 
// IElementContainer* EditorCursor::pushMapElement(IElement* element) {
//   cEditMapPointer->pushElement(element);
//   return cEditMapPointer;
// }
// 
IElement* EditorCursor::popElement() {
//  return cEditZonePointer != NULL ? cEditZonePointer->popElement() : NULL;
  return NULL;
}

void EditorCursor::setDirty(IElement* element) {
  // TODO: Do we know for sure that the element is in the current zone?
//   cEditZonePointer->setDirty(element);
//   cEditMapPointer->zoneChanged(cEditZonePointer);
}

// Map* EditorCursor::getMap() {
//   return cEditMapPointer;
// }

EditorCursor::~EditorCursor() {
//  delete cZoneBrush;
}


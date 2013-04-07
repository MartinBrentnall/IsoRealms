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
#include "ZoneBrush.h"

ZoneBrush::ZoneBrush(IMap* map, BlockLocation* editorCursorLocation) {
  cEditMapPointer = map;
  cEditorCursor = editorCursorLocation;
  cStartDrawing = NULL;
}

bool ZoneBrush::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cEditMapPointer->getZone(*cEditorCursor) == NULL) {
        if (cStartDrawing == NULL) {
          cStartDrawing = new BlockLocation(*cEditorCursor);
        } else {
          BlockArea mNewZoneArea(*cStartDrawing, *cEditorCursor);
          if (cEditMapPointer->overlaps(mNewZoneArea)) {
            // TODO: Some alert (e.g. sound) that the operation is not valid.
            break;
          }
          BlockLocation mEndDrawing = *cEditorCursor;
//          ColourScheme *mColourScheme = cColourEditor.getColourScheme();
          Zone *mNewZone = new Zone(*cStartDrawing, mEndDrawing);
          cEditMapPointer->addZone(mNewZone);
          cStartDrawing = NULL;
        }
        return true;
      } else {
        // TODO: Some alert (e.g. sound) that the operation is not valid.
      }
      break;
    }

    case SDLK_ESCAPE: {
      if (cStartDrawing != NULL) {
        cStartDrawing = NULL;
        return true;
      }
      break;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool ZoneBrush::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void ZoneBrush::render() {
  if (cStartDrawing != NULL) {
    BlockLocation location;
    BlockLocation dimension;

    BlockArea mBlockArea(*cStartDrawing, *cEditorCursor);
    if (cEditMapPointer->overlaps(mBlockArea)) {
      glColor3f(1.0f, 0.0f, 0.0f);
    } else {
      glColor3f(0.0f, 1.0f, 1.0f);
    }

    location.x = cStartDrawing->x < cEditorCursor->x ? cStartDrawing->x : cEditorCursor->x;
    location.y = cStartDrawing->y < cEditorCursor->y ? cStartDrawing->y : cEditorCursor->y;
    location.z = cStartDrawing->z < cEditorCursor->z ? cStartDrawing->z : cEditorCursor->z;
    dimension.x = cStartDrawing->x < cEditorCursor->x ? cEditorCursor->x : cStartDrawing->x;
    dimension.y = cStartDrawing->y < cEditorCursor->y ? cEditorCursor->y : cStartDrawing->y;
    dimension.z = cStartDrawing->z < cEditorCursor->z ? cEditorCursor->z : cStartDrawing->z;

    float x = (dimension.x) * BLOCK_SIZE + BLOCK_RADIUS;
    float y = (dimension.y) * BLOCK_SIZE + BLOCK_RADIUS;
    float z = (dimension.z) * BLOCK_HEIGHT;
    float xs = (location.x - 1) * BLOCK_SIZE + BLOCK_RADIUS;
    float ys = (location.y - 1)* BLOCK_SIZE + BLOCK_RADIUS;
    float zs = (location.z - 1) * BLOCK_HEIGHT;
  
    dimension.z--;
    dimension.z *= (int) BLOCK_HEIGHT;
  
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_LINES);
    glVertex3f(xs, ys, z); glVertex3f(x,  ys, z);
    glVertex3f(x,  ys, z); glVertex3f(x,  y,  z);
    glVertex3f(x,  y,  z); glVertex3f(xs, y,  z);
    glVertex3f(xs, y,  z); glVertex3f(xs, ys, z);
  
    glVertex3f(xs, ys, zs);   glVertex3f(x,  ys, zs);
    glVertex3f(x,  ys, zs);   glVertex3f(x,  y,  zs);
    glVertex3f(x,  y,  zs);   glVertex3f(xs, y,  zs);
    glVertex3f(xs, y,  zs);   glVertex3f(xs, ys, zs);
  
    glVertex3f(x,  ys, z); glVertex3f(x,  ys, zs);
    glVertex3f(x,  y,  z); glVertex3f(x,  y,  zs);
    glVertex3f(xs, y,  z); glVertex3f(xs, y,  zs);
    glVertex3f(xs, ys, z); glVertex3f(xs, ys, zs);
    glEnd();
  }
}

ZoneBrush::~ZoneBrush() {
  if (cStartDrawing != NULL) {
    delete cStartDrawing;
  }
}


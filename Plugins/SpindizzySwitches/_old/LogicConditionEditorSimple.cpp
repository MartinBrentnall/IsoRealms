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
#include "LogicConditionEditorSimple.h"

bool LogicConditionEditorSimple::processInputEvent(SDL_Event& event) {
}

ConditionEditor::ConditionEditor() {
  cNot = false;
  cSymbolA = SWITCH_NONE;
  cSymbolB = SWITCH_NONE;
  cFocus = 0;
}

Condition* ConditionEditor::getCondition() {
  if (cSymbolA == SWITCH_NONE && cSymbolB == SWITCH_NONE) {
    return NULL;
  }
  
//  Condition* mCondition = new Condition(cSymbolA, cSymbolB, cNot);
//  return mCondition;
  return NULL;
}

void ConditionEditor::move(int x, int y, int z) {
  cFocus += x;
  cFocus = restrict(0, 2, cFocus);

  if (y != 0) {
    switch (cFocus) {
      case NOT: cNot = !cNot;                                             break;
      case SYMBOL_A: cSymbolA += y; cSymbolA = restrict(0, 11, cSymbolA); break;
      case SYMBOL_B: cSymbolB += y; cSymbolB = restrict(0, 11, cSymbolB); break;
    }
  }
}

void LogicConditionEditorSimple::render() {
  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);

  if (cNot) {
    cTextures->setTexture(SWITCH_NONE);
    glBegin(GL_QUADS);
    if (cTextures->isBackwards(cSymbolA)) {
      glTexCoord2f(0.0, 1.0); glVertex3f(-1 +     BLOCK_RADIUS, 0 - BLOCK_RADIUS, 0.0);
      glTexCoord2f(0.0, 0.0); glVertex3f(-1 +     BLOCK_RADIUS,     BLOCK_RADIUS, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(-1 + 0 - BLOCK_RADIUS,     BLOCK_RADIUS, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(-1 + 0 - BLOCK_RADIUS, 0 - BLOCK_RADIUS, 0.0);
    } else {
      glTexCoord2f(1.0, 0.0); glVertex3f(-1 +     BLOCK_RADIUS, 0 - BLOCK_RADIUS, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(-1 +     BLOCK_RADIUS,     BLOCK_RADIUS, 0.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-1 + 0 - BLOCK_RADIUS,     BLOCK_RADIUS, 0.0);
      glTexCoord2f(0.0, 0.0); glVertex3f(-1 + 0 - BLOCK_RADIUS, 0 - BLOCK_RADIUS, 0.0);
    }
    glEnd();
  }

  cTextures->setTexture(cSymbolA);
  glBegin(GL_QUADS);
  if (cTextures->isBackwards(cSymbolA)) {
    glTexCoord2f(0.0, 1.0); glVertex3f(    BLOCK_RADIUS, 0 - BLOCK_RADIUS, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(    BLOCK_RADIUS,     BLOCK_RADIUS, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0 - BLOCK_RADIUS,     BLOCK_RADIUS, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0 - BLOCK_RADIUS, 0 - BLOCK_RADIUS, 0.0);
  } else {
    glTexCoord2f(1.0, 0.0); glVertex3f(    BLOCK_RADIUS, 0 - BLOCK_RADIUS, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(    BLOCK_RADIUS,     BLOCK_RADIUS, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(0 - BLOCK_RADIUS,     BLOCK_RADIUS, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0 - BLOCK_RADIUS, 0 - BLOCK_RADIUS, 0.0);
  }
  glEnd();

  cTextures->setTexture(cSymbolB);
  glBegin(GL_QUADS);
  if (cTextures->isBackwards(cSymbolA)) {
    glTexCoord2f(0.0, 1.0); glVertex3f(1 +     BLOCK_RADIUS, 0 - BLOCK_RADIUS, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(1 +     BLOCK_RADIUS,     BLOCK_RADIUS, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1 + 0 - BLOCK_RADIUS,     BLOCK_RADIUS, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1 + 0 - BLOCK_RADIUS, 0 - BLOCK_RADIUS, 0.0);
  } else {
    glTexCoord2f(1.0, 0.0); glVertex3f(1 +     BLOCK_RADIUS, 0 - BLOCK_RADIUS, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1 +     BLOCK_RADIUS,     BLOCK_RADIUS, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1 + 0 - BLOCK_RADIUS,     BLOCK_RADIUS, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(1 + 0 - BLOCK_RADIUS, 0 - BLOCK_RADIUS, 0.0);
  }
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_TRIANGLES);
  glVertex3f((cFocus - 1),                0 - BLOCK_RADIUS,       0.0);
  glVertex3f((cFocus - 1) + BLOCK_RADIUS, 0 - BLOCK_RADIUS * 3.0, 0.0);
  glVertex3f((cFocus - 1) - BLOCK_RADIUS, 0 - BLOCK_RADIUS * 3.0, 0.0);
  glEnd();


  glEnable(GL_CULL_FACE);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);
}

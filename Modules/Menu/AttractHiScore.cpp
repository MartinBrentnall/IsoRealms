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
#include "AttractHiScore.h"

AttractHiScore::AttractHiScore(IFont* font) {
  cFont = font;
  init();
}

void AttractHiScore::init() {
  cFinished = false;
  cFade = 0.0;
  cStage = 0;
  cDelay = 0;
}

void AttractHiScore::update(int ticks) {
  switch (cStage) {
    case 0: {
      cFade += ticks / 1000.0;
      if (cFade >= 1.0) {
        cFade = 1.0;
        cStage = 1;
      }
      break;
    }

    case 1: {
      cDelay += ticks;
      if (cDelay >= 2000) {
        cStage = 2;
      }
      break;
    }

    case 2: {
      cFade -= ticks / 1000.0;
      if (cFade <= 0.0) {
        cFade = 0.0;
        cFinished = true;
      }
      break;
    }
  }
}

void AttractHiScore::render() {
  glEnable(GL_BLEND);
  glColor4f(0.0f, 0.0f, 0.0f, cFade);
  cFont->print(-0.79f, 0.79f, 0.035f, IFont::LEFT,  "Pos.");
  cFont->print(-0.59f, 0.79f, 0.035f, IFont::LEFT,  "Name");
  cFont->print( 0.01f, 0.79f, 0.035f, IFont::LEFT,  "Score");
  cFont->print( 0.81f, 0.79f, 0.035f, IFont::RIGHT, "Time");
  glColor4f(1.0f, 1.0f, 1.0f, cFade);
  cFont->print(-0.8f, 0.8f, 0.035f, IFont::LEFT, "Pos.");
  cFont->print(-0.6f, 0.8f, 0.035f, IFont::LEFT, "Name");
  cFont->print( 0.0f, 0.8f, 0.035f, IFont::LEFT, "Score");
  cFont->print( 0.8f, 0.8f, 0.035f, IFont::RIGHT, "Time");
  for (int i = 0; i < 20; i ++) {
    glColor4f(0.0f, 0.0f, 0.0f, cFade);
    cFont->print(-0.79f, 0.71f - (i * (0.08)), 0.025f, IFont::LEFT,  "%d", i + 1);
    cFont->print(-0.59f, 0.71f - (i * (0.08)), 0.025f, IFont::LEFT,  "No-one yet");
    cFont->print( 0.01f, 0.71f - (i * (0.08)), 0.025f, IFont::LEFT,  "10,000");
    cFont->print( 0.81f, 0.71f - (i * (0.08)), 0.025f, IFont::RIGHT, "59:59.999");
    glColor4f(1.0f, 1.0f, 1.0f, cFade);
    cFont->print(-0.8f, 0.72f - (i * (0.08)), 0.025f, IFont::LEFT,  "%d", i + 1);
    cFont->print(-0.6f, 0.72f - (i * (0.08)), 0.025f, IFont::LEFT,  "No-one yet");
    cFont->print( 0.0f, 0.72f - (i * (0.08)), 0.025f, IFont::LEFT,  "10,000");
    cFont->print( 0.8f, 0.72f - (i * (0.08)), 0.025f, IFont::RIGHT, "59:59.999");
  }
  glDisable(GL_BLEND);
}

bool AttractHiScore::hasFinished() {
  return cFinished;
}

void AttractHiScore::frontEndActive(bool active) {
}

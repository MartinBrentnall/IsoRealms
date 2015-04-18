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
#include "AttractTitle.h"

AttractTitle::AttractTitle(IFont* font) {
  cFont = font;
  init();
}

void AttractTitle::init() {
  cFinished = false;
  cFade = 0.0;
  cStage = 0;
  cDelay = 0;
}

void AttractTitle::update(int ticks) {
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
      if (!cFrontEndActive) {
        cDelay += ticks;
        if (cDelay >= 2000) {
          cStage = 2;
        }
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

void AttractTitle::render() {
  glEnable(GL_BLEND);
  glColor4f(0.0f, 0.0f, 0.0f, cFade);
  cFont->print(0.01f, 0.59f, 0.07f, 1, "Iso Realms");
  glColor4f(1.0f, 1.0f, 1.0, cFade);
  cFont->print(0.0f, 0.6f, 0.07f, 1, "Iso Realms");
  glDisable(GL_BLEND);
}

bool AttractTitle::hasFinished() {
  return cFinished;
}

void AttractTitle::frontEndActive(bool active) {
  cFrontEndActive = active;
  cDelay = 0;
  if (cStage == 2) {
    cFade = 1.0;
    cStage = 1;
  }
}

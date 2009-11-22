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
#include "AttractIntro.h"

AttractIntro::AttractIntro() {
  IFontEngine* mFontEngine = GlobalConfiguration::getFontEngine();

  // TODO: Get font from XML!
  cFont = mFontEngine->getFont("Menu");
  init();
}

void AttractIntro::init() {
  cFinished = false;
  cFade = 0.0;
  cStage = 0;
  cDelay = 0;
}

void AttractIntro::update(int ticks) {
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

void AttractIntro::render() {
  glColor3f(cFade, cFade, cFade);
  cFont->print(0.0f, 0.0f, 0.05f, 1, "Introducing...");
}

bool AttractIntro::hasFinished() {
  return cFinished;
}

void AttractIntro::frontEndActive(bool active) {
}

extern "C" IAttract* create() {
  return new AttractIntro();
}

extern "C" void destroy(IAttract* attract) {
  delete attract;
}

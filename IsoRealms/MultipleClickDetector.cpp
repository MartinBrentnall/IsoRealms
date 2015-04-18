/*
 * Copyright 2015 Martin Brentnall
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
#include "MultipleClickDetector.h"

const unsigned int MultipleClickDetector::CLICK_INTERVAL_THRESHOLD = 400;
const unsigned int MultipleClickDetector::SINGLE_CLICK = 1;
const unsigned int MultipleClickDetector::DOUBLE_CLICK = 2;
const unsigned int MultipleClickDetector::TRIPLE_CLICK = 3;

MultipleClickDetector::MultipleClickDetector() {
  cClicks = 0;
  cTimeSinceLastClick = 0;
}

void MultipleClickDetector::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      if (event.button.button != cButton) {
        cClicks = 0;
      }
      cClicks++;
      cButton = event.button.button;
      cTimeSinceLastClick = 0;
      break;
    }

    case SDL_MOUSEMOTION: {
      cClicks = 0;
      break;
    }
  }
}

void MultipleClickDetector::update(unsigned int milliseconds) {
  if (cClicks > 0) {
    cTimeSinceLastClick += milliseconds;
    if (cTimeSinceLastClick > CLICK_INTERVAL_THRESHOLD) {
      cClicks = 0;
    }
  }
}

unsigned int MultipleClickDetector::getClicks() {
  if (cClicks == 0) {
    std::cout << "WARNING: Obtaining clicks when click detector did not register any clicks!" << std::endl;
  } else if (cTimeSinceLastClick != 0) {
    std::cout << "WARNING: Obtaining clicks possibly in different cycle to click input" << std::endl;
  }
  return cClicks;
}

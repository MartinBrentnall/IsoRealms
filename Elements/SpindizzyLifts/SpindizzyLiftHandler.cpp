/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "SpindizzyLiftHandler.h"

SpindizzyLiftHandler::SpindizzyLiftHandler(ISpindizzyLiftSet* spindizzyLiftSet, IElementContainer* elementContainer) {
  cElementContainer = elementContainer;
  cSpindizzyLiftSet = spindizzyLiftSet;
  cVisibility = 0.0f;
  cReinit = true;
}

void SpindizzyLiftHandler::elementAdded(SpindizzyLift* lift) {
  cLifts.push_back(lift);
}

void SpindizzyLiftHandler::render() {
  glEnable(GL_BLEND);
  glColor4f(1.0f, 1.0f, 1.0f, cVisibility);
  DefaultElementHandler<SpindizzyLift>::render();
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void SpindizzyLiftHandler::resetLifts() {
  for (unsigned int i = 0; i < cLifts.size(); i++) {
    cLifts[i]->reset();
  }
}

void SpindizzyLiftHandler::updateRuntime(int ticks) {
  IZone* mZone = cSpindizzyLiftSet->getCurrentZone();
  // TODO: Dynamic casts are BAD
  IElementContainer* mCurrentZone = dynamic_cast<IElementContainer*>(mZone);
  if (mCurrentZone == cElementContainer) {
    if (cReinit) {
      resetLifts();
      cReinit = false;
    }
    if (cVisibility < 1.0f) {
      cVisibility += ticks / 500.0f;
      if (cVisibility > 1.0f) {
        cVisibility = 1.0f;
      }
    }
    DefaultElementHandler<SpindizzyLift>::update(ticks);
  } else {
    if (cVisibility > 0.0f) {
      cVisibility -= ticks / 500.0f;
      if (cVisibility < 0.0f) {
        cElementContainer->setHandlerActive(this, false);
        cVisibility = 0.0f;
      }
    }
    cReinit = true;
  }
}

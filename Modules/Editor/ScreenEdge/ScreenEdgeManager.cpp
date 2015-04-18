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
#include "ScreenEdgeManager.h"

void ScreenEdgeManager::update(unsigned int update) {
  cScreenEdgeBottom.update(update);
  cScreenEdgeLeft.update(update);
  cScreenEdgeRight.update(update);
  cScreenEdgeTop.update(update);
}

void ScreenEdgeManager::render() {
  cScreenEdgeBottom.render();
  cScreenEdgeLeft.render();
  cScreenEdgeRight.render();
  cScreenEdgeTop.render();
}

bool ScreenEdgeManager::input(SDL_Event& event) {
  return cScreenEdgeBottom.input(event)
      || cScreenEdgeLeft.input(event)
      || cScreenEdgeRight.input(event)
      || cScreenEdgeTop.input(event);
}

bool ScreenEdgeManager::contains(float x, float y) {
  return cScreenEdgeBottom.contains(x, y)
      || cScreenEdgeLeft.contains(x, y)
      || cScreenEdgeRight.contains(x, y)
      || cScreenEdgeTop.contains(x, y);
}

void ScreenEdgeManager::add(Dialog* resizableDialog, AbstractRectangularComponent* icon) {
  cScreenEdgeRight.add(resizableDialog, icon);
}


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
#ifndef SCREEN_EDGE_MANAGER_H
#define SCREEN_EDGE_MANAGER_H

#include <IsoRealms/IHUDComponent.h>

#include "ScreenEdgeBottom.h"
#include "ScreenEdgeLeft.h"
#include "ScreenEdgeRight.h"
#include "ScreenEdgeTop.h"

class ScreenEdgeManager:public IHUDComponent {
  private:
  ScreenEdgeBottom cScreenEdgeBottom;
  ScreenEdgeLeft cScreenEdgeLeft;
  ScreenEdgeRight cScreenEdgeRight;
  ScreenEdgeTop cScreenEdgeTop;
  
  public:
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
  bool contains(float, float);
  void add(Dialog*, AbstractRectangularComponent*);
};

#endif

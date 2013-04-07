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
#ifndef ZONE_BRUSH_H
#define ZONE_BRUSH_H

#include <SDL/SDL.h>

#include "BlockArea.h"
#include "BlockLocation.h"
#include "IsoRealmsConstants.h"
#include "Map.h"
#include "Zone.h"

class ZoneBrush:public IsoRealmsConstants {
  private:
  IMap* cEditMapPointer;
  BlockLocation* cEditorCursor;
  BlockLocation* cStartDrawing;

  bool keyDown(SDLKey&);

  public:

  /**
   * Construct a ZoneBrush for the specified Map.  The ZoneBrush is located
   * according to the specified BlockLocation pointer.
   * 
   * @param Map*  The map that this brush will draw in.
   * @param BlockLocation*  The location of the brush.
   */
  ZoneBrush(IMap*, BlockLocation*);

  bool input(SDL_Event&);
  void render();

  ~ZoneBrush();
};

#endif

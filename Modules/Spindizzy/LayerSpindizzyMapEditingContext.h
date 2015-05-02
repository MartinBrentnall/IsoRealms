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
#ifndef LAYER_SPINDIZZY_MAP_EDITING_CONTEXT_H
#define LAYER_SPINDIZZY_MAP_EDITING_CONTEXT_H

#include "CameraLayerMapEditing.h"

class LayerSpindizzyMapEditingContext {
  private:
  static const float SPEED_FAST;
  static const float SPEED_NORMAL;
  static const float SPEED_SLOW;

  bool cActiveLeft;
  bool cActiveRight;
  bool cActiveUp;
  bool cActiveDown;
  bool cActiveHigher;
  bool cActiveLower;
  bool cActiveSlow;
  bool cActiveFast;
  Vertex cLocation;
  Vertex cMomentum;
  CameraLayerMapEditing cCameraEditing;
  
  bool isMovingWest();
  bool isMovingEast();
  bool isMovingSouth();
  bool isMovingNorth();
  float getMovementSpeed();
  bool keyDown(SDLKey&, SDLMod&);
  bool keyUp(SDLKey&, SDLMod&);
  
  public:
  LayerSpindizzyMapEditingContext();
  
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
};

#endif

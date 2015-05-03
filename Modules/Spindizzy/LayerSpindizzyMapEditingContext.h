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

#include <stack>

#include <IsoRealms/Collision.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>
#include <IsoRealms/Struct/SpatialContainer2D.h>

#include "CameraLayerMapEditing.h"
#include "ILayerSpindizzyMap.h"

class LayerSpindizzyMapEditingContext : public ILayerEditingContext {
  private:
  static const float SPEED_FAST;
  static const float SPEED_NORMAL;
  static const float SPEED_SLOW;
  static const float STOP_THRESHOLD;

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
  IElementType* cElementType;
  ILayerSpindizzyMap* cMap;
  SpatialContainer2D<IElement> cElements;
  std::stack<IElementContainer*> cSelectedElementContainers;
  
  bool isMovingWest();
  bool isMovingEast();
  bool isMovingSouth();
  bool isMovingNorth();
  float getMovementSpeed();
  bool keyDown(SDLKey&, SDLMod&);
  bool keyUp(SDLKey&, SDLMod&);
  void processCursorMovement(Vertex&, Vertex&);
  void processCursorAppearance(Vertex&);
  
  public:
  LayerSpindizzyMapEditingContext(ILayerSpindizzyMap*);
  
  void init();
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
  void setElementType(IElementType*);
  void addElement(IElement*);
  
  /**********************************\
   * Implemens ILayerEditingContext *
  \**********************************/
  IElementContainer* getElementContainer();
  Vertex* getLocation();
  float getAngle();
  void selectElementContainer(IElementContainer*);
  void deselectElementContainer(IElementContainer*);
};

#endif

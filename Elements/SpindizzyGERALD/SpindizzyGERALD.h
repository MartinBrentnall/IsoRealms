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
#ifndef SPINDIZZY_GERALD_H
#define SPINDIZZY_GERALD_H

#include <iostream>
#include <iomanip>
#include <cmath>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Element.h>
#include <IsoRealms/IInteractiveElement.h>
#include <IsoRealms/IMap.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/KeyStates.h>
#include <IsoRealms/Vertex.h>

#include "../../Plugins/3DModel/ISimpleModel.h"
#include "../../Plugins/3DModel/ISimpleModelFactory.h"
#include "../../Plugins/Camera/ICamera.h"
#include "../../Plugins/Collectables/ICollectables.h"
#include "../../Plugins/CollidableSurfaceRegistry/ICollidableSurfaceRegistry.h"
#include "../../Plugins/CommandRegistry/IUserCommand.h"
#include "../../Plugins/LocationAwareness/ILocationAwareness.h"
#include "../../Plugins/ZoneContext/IZoneContext.h"

class SpindizzyGERALD:public Element<>, 
                      public ICollector,
                      public IDynamicElement,
                      public IInteractiveElement,
                      public IVisualElement {
  private:
  static const float CRAFT_ACCELERATION;
  static const float GRAVITY_STRENGTH;
  // TODO: Need to define an "initialisation scheme" somewhere
  static const unsigned int INIT_REGISTER_BLOCKS;
  static const unsigned int INIT_PROCESS_BLOCKS;
  static const unsigned int INIT_REGISTER_SURFACES;
  static const unsigned int INIT_USE_SURFACES;

  SDLKey cNorthKey;
  SDLKey cSouthKey;
  SDLKey cEastKey;
  SDLKey cWestKey;

  IMap* cMap;
  IZone* cZone;
  BlockLocation cStartLocation;
  Vertex cLocation;
  Vertex cMomentum;
  ISimpleModel* cGERALDModel;
  ICamera* cCamera;
  ICollectables* cCollectables;
  ICollidableSurfaceRegistry* cCollidableSurfaceRegistry;
  IZoneContext* cZoneContext;
  IRollableSurface* cCurrentSurface;
  IRollableSurface* cRespawnSurface;
  int cRespawnX;
  int cRespawnY;
  float cMapBottom;
  std::vector<IUserCommand*> cFallenCommands;

  SDLKey rotateKey(const SDLKey&);
  void keyDown(SDLKey&);

  public:
  SpindizzyGERALD(IElementFactory*, BlockLocation*, ISimpleModelFactory*, ICollectables*, ICollidableSurfaceRegistry*, ILocationAwareness*, IZoneContext*, ICamera*);

  void checkCurrentZoneEvents(Vertex&, Vertex&);
  void checkMapZoneEvents(IZone*, Vertex&, Vertex&);
  void updateZoneContext(Vertex&, Vertex&);

  void setCamera(ICamera*);
  void setModel(ISimpleModelFactory*);
  void setCollectables(ICollectables*);
  void setZoneContext(IZoneContext*);

  /*************************\
   * Implements ICollector *
  \*************************/
  void collected(ICollectable*);

  /***********************\
   * Implements IElement *
  \***********************/
  void renderStatic();
  void setRuntimeContext(IMap*);
  bool initElement(unsigned int);
  std::vector<IVisualElement*> getVisualElements();
  std::vector<IDynamicElement*> getDynamicElements();
  std::vector<IDynamicElement*> getDynamicElementsRuntime();
  std::vector<IInteractiveElement*> getInteractiveElements();
  void save(DOMNodeWriter*, BlockLocation&);

  /********************************\
   * Implementeds IDynamicElement *
  \********************************/
  void update(int);

  /**********************************\
   * Implements IInteractiveElement *
  \**********************************/
  bool input(SDL_Event&);

  /*******************************\
   * Implementeds IVisualElement *
  \*******************************/
  void render();
};

#endif

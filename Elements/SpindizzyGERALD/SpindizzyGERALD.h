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

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stack>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Element.h>
#include <IsoRealms/IMap.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/KeyStates.h>
#include <IsoRealms/MiscFunctions.h>
#include <IsoRealms/Script.h>
#include <IsoRealms/Vertex.h>

#include "../../Plugins/3DModel/ISimpleModel.h"
#include "../../Plugins/3DModel/ISimpleModelFactory.h"
#include "../../Plugins/Camera/ICamera.h"
#include "../../Plugins/Collectables/ICollectables.h"
#include "../../Plugins/CollidableSurfaceRegistry/ICollidableSurfaceRegistry.h"
#include "../../Plugins/LocationAwareness/ILocationAwareness.h"
#include "../../Plugins/ZoneContext/IZoneContext.h"

#include "ISpindizzyGERALDFactory.h"
#include "ISpindizzyGERALDSet.h"

class SpindizzyGERALD:public Element<ISpindizzyGERALDSet, ISpindizzyGERALDFactory>, 
                      public ICollector,
                      public IDynamicElement,
                      public IVisualElement {
  private:
  static const float CRAFT_ACCELERATION;
  static const float GRAVITY_STRENGTH;
  // TODO: Need to define an "initialisation scheme" somewhere
  static const unsigned int INIT_REGISTER_BLOCKS;
  static const unsigned int INIT_PROCESS_BLOCKS;
  static const unsigned int INIT_REGISTER_SURFACES;
  static const unsigned int INIT_USE_SURFACES;
  static const unsigned int BOUNCE_CONTROL_TIME;
  
  struct RespawnData {
    IRollableSurface* cSurface;
    int cX;
    int cY;
  };

  IMap* cMap;
  IZone* cZone;

  // User inputs
  bool* cMovingNorth;
  bool* cMovingEast;
  bool* cMovingSouth;
  bool* cMovingWest;
  bool* cThrust;
  
  // Definition data
  BlockLocation cStartLocation;
  Vertex cLocation;
  Vertex cMomentum;
  
  // Supported plugins
  ISimpleModel* cGERALDModel;
  ICamera* cCamera;
  ICollectables* cCollectables;
  ICollidableSurfaceRegistry* cCollidableSurfaceRegistry;
  IZoneContext* cZoneContext;
  
  // Death and respawn handling
  std::stack<RespawnData*> cRespawnSurfaceStack;
  RespawnData* cRespawnData;
  bool cRespawning;
  Vertex cDestroyLocation;
  
  // Craft limits
  float cMapBottom;
  float cFallLimit;

  // Event scripts
  Script* cFallScript;
  Script* cFallLimitScript;

  // Physics
  bool cJumpedFromRamp;
  float cPeakHeight;
  float cSurfaceLeaveVerticalMomentum;
  float cRespawnAnimation;
  bool cCycleBounce;
  int cFastEvents;
  
  // Surfaces
  IRollableSurface* cCurrentSurface;
  ICollidableWallSurface* cLockNorth;
  ICollidableWallSurface* cLockEast;
  ICollidableWallSurface* cLockSouth;
  ICollidableWallSurface* cLockWest;

  bool isMovingNorth();
  bool isMovingEast();
  bool isMovingSouth();
  bool isMovingWest();
  bool isValidEvent(ICollisionData*);
  void pollEvent(float&);
  void getNewLocation(float, Vertex*, Vertex*);
  ICollisionData* pollSlideEvent(Vertex&, Vertex&);
  ICollisionData* pollCollisionEvent(Vertex&, Vertex&);
  bool processEvent(ICollisionData&);
  void updateLocation(Vertex&);
  void updateRespawnLocation();
  void updateRespawnData();
  void updateAlive(int ticks);
  void updateDead(int ticks);
  RespawnData* getRespawnData();
  void checkFall();
  void destroy();
  void respawn();
  void discoverZone(ICollidableWallSurface*);

  public:
  SpindizzyGERALD(ISpindizzyGERALDFactory*, BlockLocation*, ISimpleModelFactory*, ICollectables*, ICollidableSurfaceRegistry*, ILocationAwareness*, IZoneContext*, ICamera*, float, Script*, Script*, IMap*);

  void checkCurrentZoneEvents(Vertex&, Vertex&);
  void checkMapZoneEvents(IZone*, Vertex&, Vertex&);

  void setCamera(ICamera*);
  void setModel(ISimpleModelFactory*);
  void setCollectables(ICollectables*);
  void setZoneContext(IZoneContext*);

  void stop();

  /*************************\
   * Implements ICollector *
  \*************************/
  void collected(ICollectable*);

  /***********************\
   * Implements IElement *
  \***********************/
  void renderStatic();
  bool initElement(unsigned int);
  std::vector<IVisualElement*> getVisualElements();
  std::vector<IDynamicElement*> getDynamicElements();
  std::vector<IDynamicElement*> getDynamicElementsRuntime();
  void save(DOMNodeWriter*, BlockLocation&);

  /********************************\
   * Implementeds IDynamicElement *
  \********************************/
  void update(int);

  /*******************************\
   * Implementeds IVisualElement *
  \*******************************/
  void render();
};

#endif

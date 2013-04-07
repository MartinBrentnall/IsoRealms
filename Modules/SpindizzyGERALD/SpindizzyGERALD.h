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
#include <IsoRealms/IMap.h>
#include <IsoRealms/IProject.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/MiscFunctions.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/Collectables/ICollectables.h>
#include <IsoRealms/Resources/ElementType/Element.h>
#include <IsoRealms/Resources/SurfaceRegistry/ICollidableSurfaceRegistry.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "ISpindizzyGERALDSet.h"
#include "ISpindizzyGERALDType.h"

class SpindizzyGERALD:public Element<ISpindizzyGERALDSet, ISpindizzyGERALDType>, 
                      public ICollector,
		      public IResource {
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

  IMap** cMap;
  IZone* cZone;

  // User inputs
  bool* cPressedForward;
  bool* cPressedRight;
  bool* cPressedBackward;
  bool* cPressedLeft;
  bool* cThrust;
  
  // Definition data
  BlockLocation cStartLocation;
  Vertex* cLocation;
  Vertex cMomentum;
  
  // Supported plugins
  I3DModel* cGERALDModel;
  ICollectables* cCollectables;
  ICollidableSurfaceRegistry* cCollidableSurfaceRegistry;
  
  // Death and respawn handling
  std::stack<RespawnData*> cRespawnSurfaceStack;
  RespawnData* cRespawnData;
  bool cRespawning;
  Vertex cDestroyLocation;
  
  // Craft limits
  float cMapBottom;
  float cFallLimit;

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

  void readData(DOMNodeWrapper*);
  void initInstance(IMap**, IResourceAccessor*);
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
  void updateAlive(unsigned int ticks);
  void updateDead(unsigned int ticks);
  RespawnData* getRespawnData();
  void checkFall();
  void respawn();
  void discoverZone(ICollidableWallSurface*);

  public:
  SpindizzyGERALD(ISpindizzyGERALDType*, IMap**, IResourceAccessor*, DOMNodeWrapper*);
  SpindizzyGERALD(ISpindizzyGERALDType*, IMap**, IResourceAccessor*, BlockLocation*);
  SpindizzyGERALD(ISpindizzyGERALDType*, IMap**, IResourceAccessor*);

  void checkCurrentZoneEvents(Vertex&, Vertex&);
  void checkMapZoneEvents(IZone*, Vertex&, Vertex&);
  void saveInstance(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setResources(IResourceAccessor*);
  
  Vertex* getLocation();
  
  /*************\
   * Lua API's *
  \*************/
  void destroy();
  void stop();

  /*************************\
   * Implements ICollector *
  \*************************/
  void collected(ICollectable*);

  /***********************\
   * Implements IElement *
  \***********************/
  void renderStatic();
  void renderRuntime();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  bool initElement(unsigned int);
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty();
  
  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
};

#endif

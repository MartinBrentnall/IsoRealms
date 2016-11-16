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
#ifndef ELEMENT_SPINDIZZY_CRAFT_H
#define ELEMENT_SPINDIZZY_CRAFT_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stack>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IProject.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/MiscFunctions.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/Boundaries//IBoundaries.h>
#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/ElementType/Element.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "ISpindizzyGERALDSet.h"
#include "ISpindizzyGERALDType.h"
#include "ICollisionData.h"
#include "ICollidableWallSurface.h"
#include "IRollableSurface.h"

class ElementSpindizzyCraft:public Element,
                            public IElementBounds,
                            public IBoundaryPenetrator,
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
  
  static const float STEP_REACH_NORMAL;
  static const float STEP_REACH_BOUNCE;
  
  struct RespawnData {
    IRollableSurface* cSurface;
    int cX;
    int cY;
  };

  // Interfaces
  ISpindizzyGERALDType* cCraftType;
  
  IElementContainer* cContainer;
  
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
  IBoundaries* cBoundaries;
  
  // Death and respawn handling
  std::stack<RespawnData*> cRespawnSurfaceStack;
  RespawnData* cRespawnData;
  bool cAppeared;
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
  float cStepReach;
  
  // Surfaces
  IRollableSurface* cCurrentSurface;
  ICollidableWallSurface* cLockNorth;
  ICollidableWallSurface* cLockEast;
  ICollidableWallSurface* cLockSouth;
  ICollidableWallSurface* cLockWest;

  void readData(DOMNodeWrapper*);
  void initInstance(IResourceAccessor*);
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
  ElementSpindizzyCraft(ISpindizzyGERALDType*, IResourceAccessor*, DOMNodeWrapper*);
  ElementSpindizzyCraft(ISpindizzyGERALDType*, IResourceAccessor*, BlockLocation*);
  ElementSpindizzyCraft(ISpindizzyGERALDType*);

  void setModelType(I3DModelType*, I3DModelType*);

  void setElementContainer(IElementContainer*);
  IElementContainer* getElementContainer();
  
  void setModel(I3DModelType*);
  
//  void checkCurrentZoneEvents(Vertex&, Vertex&);
//  void checkMapZoneEvents(IZone*, Vertex&, Vertex&);
  void saveInstance(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  
  Vertex* getLocation();
  
  /*************\
   * Lua API's *
  \*************/
  void destroy();
  void stop();

  /**********************************\
   * Implements IBoundaryPenetrator *
  \**********************************/
  void entered(IBoundary*);
  void setArguments();
  void unsetArguments();

  /************************************\
   * Implements IObjectWithProperties *
  \************************************/
  std::string getTypeName();
  std::vector<IObjectProperty*> getProperties();
  
  /***********************\
   * Implements IElement *
  \***********************/
  IElementType* getElementType();
  void renderStatic();
  void renderRuntime();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  bool initElement(unsigned int);
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty();
  IElementBounds* getBounds();
  void reset();
  
  /*****************************\
   * Implements IElementBounds *
  \*****************************/
  float getWest();
  float getEast();
  float getSouth();
  float getNorth();
  float getBottom();
  float getTop();
  
  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
};

#endif

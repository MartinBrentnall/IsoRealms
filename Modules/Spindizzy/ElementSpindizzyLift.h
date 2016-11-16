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
#ifndef ELEMENT_SPINDIZZY_LIFT_H
#define ELEMENT_SPINDIZZY_LIFT_H

#include <cmath>
#include <GL/glew.h>
#include <iomanip>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Collision.h>
#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/Properties/PropertyInteger.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/ElementType/Element.h>

#include "ICollidableSurfaceElement.h"
#include "IElementSpindizzyDynamic.h"
#include "IRollableSurface.h"
#include "ISpindizzyLiftSet.h"
#include "ISpindizzyLiftType.h"
#include "LiftSurfaceCollisionEvent.h"
#include "SpindizzyLiftProperties.h"

class ElementSpindizzyLift:public ICollidableSurfaceElement,
                           public IRollableSurface,
                           public IElementSpindizzyDynamic,
                           public IElementBounds {
  private:
  
  enum LiftState {
    MOVING_UP,
    MOVING_DOWN,
    PAUSED_TOP,
    PAUSED_BOTTOM
  };
    
  // Interfaces
  ISpindizzyLiftType* cLiftType;
  
  IElementContainer* cContainer;
  
  // Definition values
  BlockLocation cLocation;
  I3DModel* cLiftModel;
  int cBottom;
  int cTop;
  unsigned int cTopDelay;
  unsigned int cBottomDelay;
  unsigned int cUpSpeed;
  unsigned int cDownSpeed;
  
  // Runtime values
  struct LiftValues {
    Vertex cLocation;
    LiftState cState;
    int cDelay;
  };
  LiftValues cLiftValues;
  
  // Editor data
  std::vector<IObjectProperty*> cProperties;
  
  void renderEditingArrow();
  void executeLiftMovedScript();
  Vertex* getBoundaryCrossingPoint(Vertex&, Vertex&, float*, float);
  
  public:
  ElementSpindizzyLift(ISpindizzyLiftType*, BlockLocation*, I3DModelType*, SpindizzyLiftProperties*, int, int, IElementContainer*);

  void setModelType(I3DModelType*, I3DModelType*);
  LiftValues getZLocationAfter(int);
  IElementContainer* getElementContainer();  
  
  /***************************************\
   * Implements IElementSpindizzyDynamic *
  \***************************************/
  void reset();
  void processCursorMovement(ILayerEditingContext*, Vertex&, Vertex&);
  void processCursorAppearance(ILayerEditingContext*, Vertex&);
  PickedElement* pickElement(Vertex&, Vertex&);
  
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
  void renderEditing();
  void renderRuntime();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  bool initElement(unsigned int);
  void setDirty();
  IElementBounds* getBounds();

  /*******************************\
   * Implements IRollableSurface *
  \*******************************/
  bool contains(Vertex&, float);
  ICollisionData* getCollision(Vertex&, Vertex&, float);
  ICollisionData* getRollingEvent(Vertex&, Vertex&);
  float getHeightAt(float, float);
  float getXAcceleration(float, float);
  float getYAcceleration(float, float);
  void notifyContact();
  void notifyImpact();
  float getSurfaceFriction();
  float getSurfaceGrip();
  float getSurfaceBounce();
  IRollableSurface::RespawnPossibility getRespawnPossibility();
  bool isRespawnPossibleNow();
  void getRestingLocation(Vertex&);
  BlockArea* getCoverage();

  /******************\
   * IElementBounds *
  \******************/
  float getWest();
  float getEast();
  float getSouth();
  float getNorth();
  float getBottom();
  float getTop();
};

#endif

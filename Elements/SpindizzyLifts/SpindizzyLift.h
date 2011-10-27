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
#ifndef SPINDIZZY_LIFT_H
#define SPINDIZZY_LIFT_H

#include <cmath>
#include <GL/gl.h>
#include <iomanip>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Collision.h>
#include <IsoRealms/Element.h>
#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>

#include "../../Plugins/CollidableSurfaceRegistry/IRollableSurface.h"
#include "../../Plugins/3DModel/ISimpleModel.h"
#include "../../Plugins/3DModel/ISimpleModelFactory.h"

#include "ISpindizzyLiftFactory.h"
#include "ISpindizzyLiftSet.h"
#include "LiftSurfaceCollisionEvent.h"
#include "SpindizzyLiftProperties.h"

class SpindizzyLift:public Element<ISpindizzyLiftSet, ISpindizzyLiftFactory>,
                           IDynamicElement,
                           IRollableSurface,
                           IVisualElement {
  private:
  
  enum LiftState {
    MOVING_UP,
    MOVING_DOWN,
    PAUSED_TOP,
    PAUSED_BOTTOM
  };
    
  // Definition values
  BlockLocation cLocation;
  ISimpleModel* cLiftModel;
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
  
  void renderEditingArrow();
  void executeLiftMovedScript();
  Vertex* getBoundaryCrossingPoint(Vertex&, Vertex&, float*, float);
  
  public:
  SpindizzyLift(ISpindizzyLiftFactory*, BlockLocation*, ISimpleModelFactory*, SpindizzyLiftProperties*, int, int);

  void setModel(ISimpleModelFactory*, ISimpleModelFactory*);

  LiftValues getZLocationAfter(int);
  void reset();
  
  /***********************\
   * Implements IElement *
  \***********************/
  void renderStatic();
  void renderStaticEditing();
  std::vector<IVisualElement*> getVisualElements();
  std::vector<IDynamicElement*> getDynamicElements();
  std::vector<IInteractiveElement*> getInteractiveElements();
  void save(DOMNodeWriter*, BlockLocation&);
  bool initElement(unsigned int, bool);

  /*******************************\
   * Implements IRollableSurface *
  \*******************************/
  bool contains(Vertex&);
  ICollisionData* getCollision(Vertex&, Vertex&);
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

  /******************************\
   * Implements IDynamicElement *
  \******************************/
  void update(int);

  /*****************************\
   * Implements IVisualElement *
  \*****************************/
  void render();
};

#endif

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

#include "../../Global/BlockLocation.h"
#include "../../Global/Element.h"
#include "../../Global/IInteractiveElement.h"
#include "../../Global/IMap.h"
#include "../../Global/IsoRealmsConstants.h"
#include "../../Global/IVisualElement.h"
#include "../../Global/KeyStates.h"
#include "../../Global/Vertex.h"

#include "../../Plugins/3DModel/ISimpleModel.h"
#include "../../Plugins/3DModel/ISimpleModelFactory.h"
#include "../../Plugins/Camera/ICamera.h"
#include "../../Plugins/Collectables/ICollectables.h"
#include "../../Plugins/LocationAwareness/ILocationAwareness.h"
#include "../../Plugins/ZoneContext/IZoneContext.h"

class SpindizzyGERALD:public Element<>, 
                      public ICollector,
                      public IDynamicElement,
                      public IInteractiveElement,
                      public IVisualElement {
  private:
  static const float CRAFT_ACCELERATION;

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
  IZoneContext* cZoneContext;

  SDLKey rotateKey(const SDLKey&);
  void keyDown(SDLKey&);

  public:
  SpindizzyGERALD(IElementFactory*, BlockLocation*, ISimpleModelFactory*, ICollectables*, ILocationAwareness*, IZoneContext*, ICamera*);

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

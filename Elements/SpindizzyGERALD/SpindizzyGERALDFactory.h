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
#ifndef SPINDIZZY_GERALD_FACTORY_H
#define SPINDIZZY_GERALD_FACTORY_H

#include <GL/gl.h>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/ElementFactory.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IElementGateway.h>
#include <IsoRealms/IVisualElement.h>

#include "../../Plugins/LocationAwareness/ILocationAwareness.h"
#include "../../Plugins/Camera/ICamera.h"
#include "../../Plugins/Collectables/ICollectables.h"
#include "../../Plugins/CollidableSurfaceRegistry/ICollidableSurfaceRegistry.h"
#include "../../Plugins/3DModel/ISimpleModel.h"
#include "../../Plugins/3DModel/ISimpleModelFactory.h"
#include "../../Plugins/ZoneContext/IZoneContext.h"

#include "ISpindizzyGERALDFactory.h"
#include "ISpindizzyGERALDSet.h"
#include "SpindizzyGERALD.h"

class SpindizzyGERALDFactory:public ISpindizzyGERALDFactory {
  private:
  std::vector<SpindizzyGERALD*> cContent;
  ISimpleModelFactory* cGERALDModelFactory;
  ICamera* cCamera;
  ICollectables* cCollectables;
  ICollidableSurfaceRegistry* cCollidableSurfaceRegistry;
  ILocationAwareness* cLocationAwareness;
  IZoneContext* cZoneContext;
  SpindizzyGERALD* cSampleGERALD;
  std::vector<IVisualElement*> cSampleGERALDVisuals;
  BlockLocation* cEditingLocation;
  float cFallLimit;
  Script* cFallScript;
  Script* cFallLimitScript;

  IElement* getElement();

  bool keyDown(SDLKey&);

  void loadFallLimitConfiguration(DOMNodeWrapper*, ICommandRegistry*);
  void loadRespawnConfiguration(DOMNodeWrapper*, ICommandRegistry*);

  public:
  SpindizzyGERALDFactory(ISpindizzyGERALDSet*, ISimpleModelFactory*, ILocationAwareness*, IZoneContext*);

  void setModel(ISimpleModelFactory*);
  void setCamera(ICamera*);
  void setCollectables(ICollectables*);
  void setCollidableSurfaceRegistry(ICollidableSurfaceRegistry*);
  void setLocationAwareness(ILocationAwareness*);
  void setZoneContext(IZoneContext*);
  void loadConfiguration(DOMNodeWrapper*, ICommandRegistry*);
  void save(DOMNodeWriter*);
  void stop();

  /******************************\
   * Implements IElementFactory *
  \******************************/
  std::string getName();
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  bool input(SDL_Event&);
  void configureElement();
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void renderEditingPreview();
  void updateIcon(int);
  void renderIcon();
};

#endif

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
#ifndef SPINDIZZY_GERALD_TYPE_H
#define SPINDIZZY_GERALD_TYPE_H

#include <GL/glew.h>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IScript.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/LuaSupport/ArgumentSourceCustom.h>
#include <IsoRealms/LuaSupport/ArgumentSourceLocal.h>
#include <IsoRealms/LuaSupport/ArgumentValue.h>
#include <IsoRealms/LuaSupport/IArgumentGenerator.h>
#include <IsoRealms/Resources/Collectables/ICollectables.h>
#include <IsoRealms/Resources/ElementType/ElementType.h>
#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/Float/Float.h>
#include <IsoRealms/Resources/SurfaceRegistry/ICollidableSurfaceRegistry.h>

#include "ISpindizzyGERALDSet.h"
#include "ISpindizzyGERALDType.h"
#include "SpindizzyGERALD.h"

class SpindizzyGERALDType:public ISpindizzyGERALDType,
                          public IArgumentGenerator,
			  public IArgumentLocator {
  private:
  std::string cName;
  std::map<std::string, SpindizzyGERALD*> cNamedInstances;
  std::vector<SpindizzyGERALD*> cContent;
  IResourceAccessor* cResources;
  I3DModelFactory* cModelType;
  ICamera* cCamera;
  ICollectables* cCollectables;
  ICollidableSurfaceRegistry* cCollidableSurfaceRegistry;
  IMap* cMap;
  SpindizzyGERALD* cSampleGERALD;
  BlockLocation* cEditingLocation;
  IScript* cRespawnScript;
  IScript* cFallImpactScript;
  IScript* cZoneEnteredScript;
  IScript* cZoneExitedScript;
  ArgumentValue<IZone> cArgumentZoneEntered;
  ArgumentValue<IZone> cArgumentZoneExited;
  ArgumentValue<IFloat> cArgumentFallDistance;
  ArgumentValue<SpindizzyGERALD> cArgumentCraft;
  
  bool keyDown(SDLKey&);

  public:
  SpindizzyGERALDType(ISpindizzyGERALDSet*, IRuntimeContext*);
  
  SpindizzyGERALD* createInstance(const std::string&);
  std::string getInstanceName(SpindizzyGERALD*);

  void saveInstances(DOMNodeWriter*, IResourceLocator*);
  void save(DOMNodeWriter*, IResourceLocator*);
  void stop();

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /******************************\
   * Implements IResourceSource *
  \******************************/
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  
  /***************************\
   * Implements IElementType *
  \***************************/
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  IElementHandler* getElementHandler();
  bool input(SDL_Event&);
  void configureElement();
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void renderEditingPreview();
  void updateIcon(unsigned int);
  void renderIcon();
  void destroy(IElement*);
  
  /*********************************\
   * Implements IArgumentGenerator *
  \*********************************/
  std::string getPath(IArgumentValue*);
  IArgumentSource* getArgument(DOMNodeWrapper*);
  
  /***********************************\
   * Implements ISpindizzyGERALDType *
  \***********************************/
  I3DModel* createModel(Vertex*);
  void collect(SpindizzyGERALD*, Vertex&, Vertex&);
  ICollisionData* getNextEvent(Vertex&, Vertex&, IRollableSurface*);
  IRollableSurface* getSurfaceAt(Vertex&);
  ICamera* getCamera();
  void executeRespawnScript();
  void executeFallImpactScript(float, SpindizzyGERALD*);
  void executeZoneEnteredScript(IZone*);
  void executeZoneExitedScript(IZone*);
};

#endif

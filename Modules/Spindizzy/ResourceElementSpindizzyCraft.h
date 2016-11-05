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
#ifndef RESOURCE_ELEMENT_SPINDIZZY_CRAFT_H
#define RESOURCE_ELEMENT_SPINDIZZY_CRAFT_H

#include <GL/glew.h>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/LuaSupport/ArgumentValue.h>
#include <IsoRealms/LuaSupport/ArgumentValueLocal.h>
#include <IsoRealms/Resources/Boundaries/IBoundaryHandler.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/Float/Float.h>
#include <IsoRealms/Resources/Script/IArgumentLocator.h>
#include <IsoRealms/Resources/Script/IArgumentValueRegistry.h>
#include <IsoRealms/Resources/Script/IScriptCall.h>

#include "ISpindizzyGERALDSet.h"
#include "ISpindizzyGERALDType.h"
#include "ElementSpindizzyCraft.h"

class ResourceElementSpindizzyCraft:public ISpindizzyGERALDType,
                                    public IArgumentValueRegistry,
                                    public IArgumentLocator {
  private:
  ISpindizzyGERALDSet* cModuleInterface;
    
  std::string cName;
  std::map<std::string, ElementSpindizzyCraft*> cNamedInstances;
  std::vector<ElementSpindizzyCraft*> cContent;
  IResourceAccessor* cResources;
  I3DModelType* cModelType;
  ICamera* cCamera;
  std::vector<IBoundaryHandler*> cBoundaryHandlers;
  ElementSpindizzyCraft* cSampleGERALD;
  IScriptCall* cRespawnScript;
  IScriptCall* cFallImpactScript;
//   ArgumentValue<IZone> cArgumentZoneEntered;
//   ArgumentValue<IZone> cArgumentZoneExited;
  ArgumentValueLocal<IFloat> cArgumentFallDistance;
  ArgumentValueLocal<ElementSpindizzyCraft> cArgumentCraft;
  
  bool keyDown(SDLKey&);

  ElementSpindizzyCraft* getElement(IElement*);
  
  public:
  ResourceElementSpindizzyCraft(ISpindizzyGERALDSet*, DOMNodeWrapper*, IResourceRegistry*);
  
  I3DModelType* getModelType();
  void setModelType(I3DModelType*);

  ElementSpindizzyCraft* createInstance(const std::string&);
  std::string getInstanceName(ElementSpindizzyCraft*);

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
  void loadElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*, IResourceAccessor*, bool);
  bool inputEdit(SDL_Event&, ILayerEditingContext*);
  void configureElement();
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void renderEditingPreview(Vertex&);
  void updateIcon(unsigned int);
  void renderIcon();
  void destroy(IElement*);
  void removeElement(IElement*);
  Vertex* editorCursorStopped(Vertex*);
  
  /*********************************\
   * Implements IArgumentGenerator *
  \*********************************/
  std::string getPath(IArgumentValue*);
  IArgumentValue* getArgumentValue(DOMNodeWrapper*);
  
  /***********************************\
   * Implements ISpindizzyGERALDType *
  \***********************************/
  ISpindizzyGERALDSet* getSpindizzyCraftInterface();
  I3DModel* createModel(Vertex*);
  void notifyMovement(ElementSpindizzyCraft*, Vertex&, Vertex&);
  void notifyAppearance(ElementSpindizzyCraft*, Vertex&);
  void notifyDisappearance(ElementSpindizzyCraft*, Vertex&);
  ICollisionData* getNextEvent(Vertex&, Vertex&, IRollableSurface*, float);
  IRollableSurface* getSurfaceAt(Vertex&, float);
  ICamera* getCamera();
  void executeRespawnScript();
  void executeFallImpactScript(float, ElementSpindizzyCraft*);
  
  virtual ~ResourceElementSpindizzyCraft();
};

#endif

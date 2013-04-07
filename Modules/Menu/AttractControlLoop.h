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
#ifndef ATTRACT_CONTROL_LOOP_H
#define ATTRACT_CONTROL_LOOP_H

#include <vector>
#include <iostream>
#include <string>
#include <dlfcn.h>

#include "AttractSceneManager.h"
#include "IFrontEnd.h"
#include "StartAttractSceneCommand.h"
#include "TerminateAttractSceneCommand.h"

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Hacks.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/InitException.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/Project.h>
#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/Resources/Registry.h>
#include <IsoRealms/Resources/Resources.h>
#include <IsoRealms/System.h>

class AttractControlLoop:public IPlugin,
                         public IElementType,
                         public IElement,
                         public IController {
  private:
  std::map<IAttract*, std::vector<ICommand*> > cSceneEndCommands;
  std::vector<ICommand*> cFrontEndStartCommands; // TODO: Implement
  std::vector<ICommand*> cFrontEndEndCommands; // TODO: Implement
  std::map<std::string, IAttract*> cAttractServices;
  AttractSceneManager cAttractSceneManager;
  std::vector<ICommand*> cInitCommands;
  IFrontEnd* cFrontEnd;
  PluginRegistry cPluginRegistry;
  IFont* cFont;
  bool cFrontEndActive;
  std::map<std::string, int> cLayers;
  Project* cRunningProject;

  /**
   * Parse the commands to be performed on an event.
   *
   * @param DOMNodeWrapper*
   * @returns  The events.
   */
  std::vector<ICommand*> parseEventCommands(DOMNodeWrapper*);

  public:
  AttractControlLoop();
  bool checkActiveInput(int);

  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /***************************\
   * Implements IElementType *
  \***************************/
  IPlugin* getElementSet();
  IElement* getElement();
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void configureElement();
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(unsigned int);
  void destroy(IElement*);
  IElementHandler* getElementHandler();
  
  /***********************\
   * Implements IElement * 
  \***********************/
  IElementType* getElementType();
  bool initElement(unsigned int);
  void renderStatic();
  void renderStaticEditing();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty();
  
  void renderRuntime();
  void renderEditing();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  void input(SDL_Event&);
  bool isVisualRuntime();
  bool isVisualEditing();
  bool isDynamicRuntime();
  bool isDynamicEditing();
  bool isInteractive();  
  
  /**************************\
   * Implements IController *
  \**************************/
  void startProject(const std::string&);
};

#endif

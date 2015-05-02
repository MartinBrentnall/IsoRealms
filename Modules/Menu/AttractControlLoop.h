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

#include "AttractDemo.h"
#include "AttractFadeOut.h"
#include "AttractHelp.h"
#include "AttractHiScore.h"
#include "AttractIntro.h"
#include "AttractSceneManager.h"
#include "AttractTitle.h"
#include "FrontEnd.h"
#include "IFrontEnd.h"
#include "StartAttractSceneCommand.h"
#include "TerminateAttractSceneCommand.h"

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Hacks.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/InitException.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/Project.h>
#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/Resources/Registry.h>
#include <IsoRealms/Resources/Resources.h>
#include <IsoRealms/System.h>

class AttractControlLoop:public IModule,
                         public ILayerType,
                         public ILayer,
                         public IController {
  private:
  std::map<IAttract*, std::vector<ICommand*> > cSceneEndCommands;
  std::vector<ICommand*> cFrontEndStartCommands; // TODO: Implement
  std::vector<ICommand*> cFrontEndEndCommands; // TODO: Implement
  std::map<std::string, IAttract*> cAttractServices;
  AttractSceneManager cAttractSceneManager;
  std::vector<ICommand*> cInitCommands;
  IFrontEnd* cFrontEnd;
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

  IFrontEnd* createFrontEnd(DOMNodeWrapper*, const std::string&);
  IAttract* createAttract(const std::string&);
  
  public:
  AttractControlLoop();
  bool checkActiveInput(int);

  void createResources(DOMNodeWrapper*, IResourceRegistry*);
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  void load(DOMNodeWrapper*, IResourceRegistry*);
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /*************************\
   * Implements ILayerType *
  \*************************/
  ILayer* getLayer(DOMNodeWrapper*, IResourceAccessor*);
  
  /*********************\
   * Implements ILayer * 
  \*********************/
  void resourceSelected(IElementType*);
  ILayerType* getLayerType();
  void initRuntime();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void staticChanged();
  void renderRuntime();
  void renderEditing();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  void input(SDL_Event&);
  
  /**************************\
   * Implements IController *
  \**************************/
  void startProject(const std::string&);
};

#endif

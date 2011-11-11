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

#include "../../IControlLoop.h"
#include "../../IEngineArguments.h"

#include <IsoRealms/CommandProxy.h>
#include <IsoRealms/DOMNodeWrapper.h>
#include <IsoRealms/Hacks.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IFont.h>
#include <IsoRealms/InitException.h>
#include <IsoRealms/IPluginSupport.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/PlugSocket.h>
#include <IsoRealms/Registry.h>
#include <IsoRealms/System.h>

class AttractControlLoop:public IControlLoop,
                         public IPluginSupport {
  private:
  std::map<IAttract*, std::vector<ICommand*> > cSceneEndCommands;
  std::vector<ICommand*> cFrontEndStartCommands; // TODO: Implement
  std::vector<ICommand*> cFrontEndEndCommands; // TODO: Implement
  std::map<std::string, IAttract*> cAttractServices;
  AttractSceneManager cAttractSceneManager;
  Registry<ICommand, CommandProxy> cCommandRegistry;
  Registry<IColour, ColourProxy> cColourRegistry;
  Registry<ITexture, TextureProxy> cTextureRegistry;
  std::vector<ICommand*> cInitCommands;
  IFrontEnd* cFrontEnd;
  PluginRegistry cPluginRegistry;
  std::vector<PlugSocket*> cFontSocket;
  IFont* cFont;
  bool cFrontEndActive;
  std::map<std::string, int> cLayers;

  /**
   * Parse the commands to be performed on an event.
   *
   * @param DOMNodeWrapper*
   * @returns  The events.
   */
  std::vector<ICommand*> parseEventCommands(DOMNodeWrapper*);

  public:
  AttractControlLoop(DOMNodeWrapper*, IEngineArguments*);
  bool checkActiveInput(int);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /***************************\
   * Implements IControlLoop *
  \***************************/
  void input(SDL_Event&);
  void execute(int);
};

#endif

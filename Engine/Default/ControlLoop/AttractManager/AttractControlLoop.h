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

#include "../../../../Global/DOMNodeWrapper.h"
#include "../../../../Global/Hacks.h"
#include "../../../../Global/ICommand.h"
#include "../../../../Global/InitException.h"
#include "../../../../Global/System.h"

using namespace std;

class AttractControlLoop:public IControlLoop {
  private:
  map<IAttract*, vector<ICommand*> > cSceneEndCommands;
  vector<ICommand*> cFrontEndStartCommands; // TODO: Implement
  vector<ICommand*> cFrontEndEndCommands; // TODO: Implement
  map<string, IAttract*> cAttractServices;
  AttractSceneManager cAttractSceneManager;
  vector<ICommand*> cInitCommands;
  IFrontEnd* cFrontEnd;
  bool cFrontEndActive;
  map<string, int> cLayers;

  /**
   * Parse the commands to be performed on an event.
   *
   * @param DOMNodeWrapper*
   * @returns  The events.
   */
  vector<ICommand*> parseEventCommands(DOMNodeWrapper*);

  public:
  AttractControlLoop(DOMNodeWrapper*);
  bool checkActiveInput(int);

  /**************************************************************************\
   * Implemented methods of IControlLoop.h                                  *
  \**************************************************************************/
  void input(SDL_Event&);
  void execute(int);
};

#endif

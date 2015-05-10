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
#ifndef ENGINE_H
#define ENGINE_H

#include <dlfcn.h>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL/SDL.h>

#include <IsoRealms/CommandManager.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/InitException.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IEngine.h>
#include <IsoRealms/Input/KeyStates.h>
#include <IsoRealms/Project.h>
#include <IsoRealms/System.h>

#include "EngineArguments.h"
#include "TerminateEngineCommand.h"

/**
 * TODO: Documentation below is wrong!
 * 
 * This engine implementation supports a configuration file specifying the
 * following information:
 * <br>
 * Any number of attract services (implementations of IAttract).
 * Exactly one front-end service (implementation of IFrontEnd).
 * instance(s) of TODO
 * An order in which to cycle through attraction services.
 * Other miscellaneous configuration options.
 * <br>
 * The engine will cycle through the IAttract implementations in the order
 * specified by the configuration file.  The attract service may be interupted
 * by any form of active input (e.g. a key or mouse button input).
 * <br>
 * Interupting the attract service will send a signal to all attract services
 * to indicate that the front-end is active.  The front-end will remain active
 * until a time-out is reached where no input has been given, at which point
 * a signal will be sent to all attract services to indicate that the front-end
 * is no longer active.
 */
class Engine:public IEngine {
  private:
  Project* cProject;
  
  void loadProject(DOMNodeWrapper*);
  void loadProject(DOMNodeWrapper*, const std::string&);
  
  public:
  Engine();

  /**********************\
   * Implements IEngine *
  \**********************/
  void run();
};

#endif
